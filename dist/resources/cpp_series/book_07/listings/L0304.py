import pathlib,selectors,subprocess,tempfile,time
root=pathlib.Path('build').resolve()
processes=[]
def start(name,*args):
    p=subprocess.Popen([str(root/name),*map(str,args)],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
    processes.append(p)
    with selectors.DefaultSelector() as events:
        events.register(p.stdout,selectors.EVENT_READ)
        if not events.select(8):raise RuntimeError('startup timeout')
        line=p.stdout.readline().strip()
    if not line.startswith('READY '):raise RuntimeError('bad startup: '+line)
    return int(line.split()[1])
def call(port,*args):
    r=subprocess.run([str(root/'harbor_client'),str(port),*args],capture_output=True,text=True,timeout=4,check=True)
    return r.stdout.strip()
with tempfile.TemporaryDirectory(prefix='section28-') as directory:
    d=pathlib.Path(directory)
    try:
        worker=start('harbor_worker',d/'worker.db',0)
        gateway=start('harbor_gateway',d/'gateway.db',0,worker)
        assert call(gateway,'SUBMIT','lesson28','12')=='ACCEPTED lesson28'
        deadline=time.monotonic()+8
        while call(gateway,'STATUS','lesson28')!='DONE lesson28 144':
            if time.monotonic()>deadline:raise RuntimeError('completion timeout')
            time.sleep(.02)
        assert call(gateway,'SUBMIT','lesson28','12')=='ACCEPTED lesson28'
        assert call(gateway,'SUBMIT','lesson28','13')=='CONFLICT'
        assert call(worker,'COUNT')=='1'
        print('real processes: DONE144 duplicate accepted conflict rejected receipts1')
    finally:
        for p in reversed(processes):
            if p.poll() is None:
                p.terminate()
                try:p.wait(timeout=4)
                except subprocess.TimeoutExpired:p.kill();p.wait(timeout=2)
            p.communicate(timeout=2)
