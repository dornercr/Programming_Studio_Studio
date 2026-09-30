#!/usr/bin/env python3
"""Loopback-only process tests. Each invocation owns a private temporary directory."""
from __future__ import annotations
import argparse
import pathlib
import selectors
import socket
import sqlite3
import struct
import subprocess
import sys
import tempfile
import time

if not __debug__:
    raise RuntimeError("Run the integration harness without -O or PYTHONOPTIMIZE")

class Process:
    def __init__(self, executable: pathlib.Path, *args: str):
        self.p = subprocess.Popen([str(executable), *map(str,args)], stdout=subprocess.PIPE,
                                  stderr=subprocess.PIPE, text=True)
        try:
            with selectors.DefaultSelector() as selector:
                selector.register(self.p.stdout, selectors.EVENT_READ)
                if not selector.select(8):
                    raise RuntimeError('server startup timeout')
                line = self.p.stdout.readline().strip()
            if not line.startswith('READY '):
                raise RuntimeError(f'server startup failed: {line}')
            self.port = int(line.split()[1])
        except BaseException:
            self.stop()
            raise
    def stop(self):
        if self.p.poll() is None:
            self.p.terminate()
            try: self.p.wait(timeout=5)
            except subprocess.TimeoutExpired: self.p.kill(); self.p.wait(timeout=2)
        self.out, self.err = self.p.communicate(timeout=2)
        if 'AddressSanitizer' in self.err or 'runtime error:' in self.err:
            raise AssertionError(self.err)

def exact(s: socket.socket, n: int) -> bytes:
    data = bytearray()
    while len(data)<n:
        chunk=s.recv(n-len(data))
        if not chunk: raise EOFError('incomplete reply')
        data.extend(chunk)
    return bytes(data)

def call(port: int, message: str, fragment=False) -> str:
    payload=message.encode('ascii')
    wire=struct.pack('!I',len(payload))+payload
    with socket.create_connection(('127.0.0.1',port),timeout=3) as s:
        s.settimeout(3)
        if fragment:
            for c in wire: s.sendall(bytes([c]))
        else: s.sendall(wire)
        length=struct.unpack('!I',exact(s,4))[0]
        if length>4096: raise AssertionError('unbounded response')
        return exact(s,length).decode('ascii')

def eventually(port: int, key: str, expected: int):
    end=time.monotonic()+8
    while time.monotonic()<end:
        status=call(port,'STATUS '+key)
        if status==f'DONE {key} {expected}': return
        if status not in {f'PENDING {key}', 'ERROR'}: raise AssertionError(status)
        time.sleep(0.03)
    raise AssertionError('job did not finish')

def run(binary: pathlib.Path, case: str):
    processes=[]
    def start(exe, *args):
        p=Process(binary/exe,*args); processes.append(p); return p
    with tempfile.TemporaryDirectory(prefix='harbor-integration-') as work:
        folder=pathlib.Path(work)
        try:
            flags=['--drop-first-reply'] if case=='lost_reply' else []
            worker=start('harbor_worker',folder/'worker.db',0,*flags)
            gateway=start('harbor_gateway',folder/'gateway.db',0,worker.port)
            if case=='happy':
                for n in (0,7,1000000):
                    assert call(gateway.port,f'SUBMIT k{n} {n}')==f'ACCEPTED k{n}'
                    eventually(gateway.port,f'k{n}',n*n)
                assert call(worker.port,'COUNT')=='3'
                client=subprocess.run([str(binary/'harbor_client'),str(gateway.port),'PING'],
                                      capture_output=True,text=True,timeout=5,check=True)
                assert client.stdout.strip()=='PONG'
            elif case=='lost_reply':
                assert call(gateway.port,'SUBMIT lost 11')=='ACCEPTED lost'
                eventually(gateway.port,'lost',121)
                assert call(worker.port,'COUNT')=='1'
                metrics=call(gateway.port,'METRICS')
                errors=int([x for x in metrics.splitlines() if x.startswith('harbor_dispatch_errors_total')][0].split()[1])
                assert errors>=1
            elif case=='restart':
                port=worker.port; worker.stop()
                assert call(gateway.port,'SUBMIT pending 13')=='ACCEPTED pending'
                assert call(gateway.port,'STATUS pending')=='PENDING pending'
                gateway.stop()
                worker=start('harbor_worker',folder/'worker.db',port)
                gateway=start('harbor_gateway',folder/'gateway.db',0,worker.port)
                eventually(gateway.port,'pending',169)
                gateway.stop()
                gateway=start('harbor_gateway',folder/'gateway.db',0,worker.port)
                assert call(gateway.port,'STATUS pending')=='DONE pending 169'
            elif case=='worker_restart':
                assert call(worker.port,'EXEC direct 17')=='RESULT direct 289'
                port=worker.port; worker.stop()
                worker=start('harbor_worker',folder/'worker.db',port)
                assert call(worker.port,'EXEC direct 17')=='RESULT direct 289'
                assert call(worker.port,'COUNT')=='1'
            elif case=='partial':
                with socket.create_connection(('127.0.0.1',gateway.port),timeout=3) as stalled:
                    stalled.sendall(b'\0\0')
                    assert call(gateway.port,'PING',fragment=True)=='PONG'
                    assert call(gateway.port,'SUBMIT fragments 19',fragment=True)=='ACCEPTED fragments'
                eventually(gateway.port,'fragments',361)
            elif case=='invalid':
                for message in ('', 'SUBMIT a -1', 'SUBMIT a 2x', 'STATUS a extra', 'SUBMIT a 1000001', 'UNKNOWN a'):
                    assert call(gateway.port,message)=='INVALID'
                with socket.create_connection(('127.0.0.1',gateway.port),timeout=3) as s:
                    s.sendall(struct.pack('!I',4097))
                    try: assert s.recv(1)==b''
                    except ConnectionResetError: pass
                assert call(gateway.port,'PING')=='PONG'
                assert call(worker.port,'COUNT')=='0'
            elif case=='conflict':
                assert call(gateway.port,'SUBMIT repeat 23')=='ACCEPTED repeat'
                eventually(gateway.port,'repeat',529)
                assert call(gateway.port,'SUBMIT repeat 23')=='ACCEPTED repeat'
                assert call(gateway.port,'SUBMIT repeat 24')=='CONFLICT'
                assert call(worker.port,'EXEC repeat 24')=='ERROR'
                assert call(worker.port,'COUNT')=='1'
            elif case=='overload':
                worker.stop()
                for n in range(128): assert call(gateway.port,f'SUBMIT queued{n} {n}')==f'ACCEPTED queued{n}'
                assert call(gateway.port,'SUBMIT overflow 4')=='BUSY'
                assert call(gateway.port,'SUBMIT queued0 0')=='ACCEPTED queued0'
                with sqlite3.connect(folder/'gateway.db') as db:
                    assert db.execute('SELECT count(*) FROM jobs').fetchone()[0]==128
            elif case=='crash_rollback':
                # Interrupt a separate uncommitted transaction in a disposable database.
                dbfile=folder/'rollback.db'
                code='''import sqlite3,sys,time
c=sqlite3.connect(sys.argv[1]);c.execute("CREATE TABLE t(x INTEGER)");c.commit()
c.execute("BEGIN IMMEDIATE");c.execute("INSERT INTO t VALUES(9)")
print("READY 0",flush=True);time.sleep(30)
'''
                p=subprocess.Popen([sys.executable,'-c',code,str(dbfile)],stdout=subprocess.PIPE,text=True)
                try:
                    with selectors.DefaultSelector() as selector:
                        selector.register(p.stdout,selectors.EVENT_READ)
                        assert selector.select(5), 'rollback child startup'
                        assert p.stdout.readline().strip()=='READY 0'
                    p.kill();p.wait(timeout=3)
                    with sqlite3.connect(dbfile) as db: assert db.execute('SELECT count(*) FROM t').fetchone()[0]==0
                finally:
                    if p.poll() is None:p.kill();p.wait()
            else: raise ValueError(case)
            print(case+': passed')
        finally:
            failures=[]
            for p in reversed(processes):
                try:p.stop()
                except Exception as e:failures.append(str(e))
            if failures:raise AssertionError('\n'.join(failures))

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--bin',type=pathlib.Path,required=True);parser.add_argument('--case',required=True)
    args=parser.parse_args();run(args.bin.resolve(),args.case)
