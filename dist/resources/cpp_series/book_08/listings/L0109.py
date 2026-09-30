import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
c=docs[0]['spec']['containers'][0]
assert {c[k]['httpGet']['path'] for k in ('startupProbe','livenessProbe','readinessProbe')}=={'/startup','/live','/ready'}
assert c['startupProbe']['failureThreshold']*c['startupProbe']['periodSeconds']==60
print('offline probes: three meanings; approximate startup allowance60s')
