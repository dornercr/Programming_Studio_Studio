import pathlib,subprocess,tempfile
with tempfile.TemporaryDirectory() as td:
 p=pathlib.Path(td)
 def git(*args):return subprocess.run(['git','-C',td,*args],check=True,capture_output=True,text=True).stdout.strip()
 git('init','-q');git('config','user.name','Lab');git('config','user.email','lab@example.invalid')
 (p/'service.conf').write_text('workers=4\n');git('add','service.conf');git('commit','-qm','baseline')
 (p/'service.conf').write_text('workers=8\n');git('commit','-qam','temporary emergency capacity')
 git('revert','--no-edit','HEAD')
 assert (p/'service.conf').read_text()=='workers=4\n'
 assert len(git('rev-list','HEAD').splitlines())==3
 print('baseline restored; emergency change and its revert remain in history')
