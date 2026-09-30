import subprocess,selectors
subprocess.run(['g++','-std=c++20','-pthread','main.cpp','-o','app'],check=True)
p=subprocess.Popen(['./app'],stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
try:
    with selectors.DefaultSelector() as s:
        s.register(p.stdout,selectors.EVENT_READ)
        assert s.select(2),'startup timeout'
        assert p.stdout.readline()=='ready=1\n'
    p.terminate();out,err=p.communicate(timeout=2)
    assert p.returncode==0 and out=='ready=0\ndrained\n' and err==''
    print('SIGTERM observed: readiness removed and process exited0')
finally:
    if p.poll() is None:p.kill();p.communicate()
