import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
c=docs[0];job=c['jobs']['build']
assert set(job['strategy']['matrix']['compiler'])=={'g++','clang++'}
assert job['strategy']['fail-fast'] is False
assert any('ctest' in s.get('run','') for s in job['steps'])
print('offline workflow: GCC and Clang build/test jobs; no remote CI execution')
