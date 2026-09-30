import http.client,selectors,subprocess
p=subprocess.Popen(['./server'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
try:
 selector=selectors.DefaultSelector();selector.register(p.stdout,selectors.EVENT_READ)
 if not selector.select(5):raise RuntimeError('server never became ready')
 line=p.stdout.readline().strip();assert line.startswith('READY ')
 port=int(line.split()[1]);selector.close()
 def get(path):
  c=http.client.HTTPConnection('127.0.0.1',port,timeout=3)
  try:
   c.request('GET',path);r=c.getresponse();return r.status,r.read().decode()
  finally:c.close()
 assert get('/readyz')==(200,'ok\n')
 assert get('/healthz')==(200,'ok\n')
 assert get('/square?x=7')==(200,'49\n')
 assert get('/square?x=bad')[0]==400
 assert get('/missing')[0]==404
 p.terminate();out,err=p.communicate(timeout=3)
 assert p.returncode==0 and 'STOPPED' in out and not err
 print('loopback readiness, health, square=49, 400, 404, and SIGTERM exit verified')
finally:
 if p.poll() is None:p.kill();p.communicate()
