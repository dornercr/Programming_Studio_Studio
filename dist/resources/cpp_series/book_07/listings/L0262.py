import hashlib,json,pathlib,subprocess
source=pathlib.Path('main.cpp').read_bytes()
record={'source_sha256':hashlib.sha256(source).hexdigest(),
        'compiler':subprocess.check_output(['g++','--version'],text=True).splitlines()[0],
        'flags':['-std=c++20','-O2']}
pathlib.Path('provenance.json').write_text(json.dumps(record,indent=2))
assert hashlib.sha256(pathlib.Path('main.cpp').read_bytes()).hexdigest()==record['source_sha256']
subprocess.run(['g++',*record['flags'],'main.cpp','-o','app'],check=True)
record['binary_sha256']=hashlib.sha256(pathlib.Path('app').read_bytes()).hexdigest()
pathlib.Path('provenance.json').write_text(json.dumps(record,indent=2))
print('source and binary identities recorded; authenticity not asserted')
