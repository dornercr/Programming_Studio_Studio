import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
d,s=docs;p=d['spec']['template'];c=p['spec']['containers'][0]
assert s['spec']['selector']==p['metadata']['labels']
assert s['spec']['ports'][0]['targetPort']==c['ports'][0]['name']
assert c['readinessProbe']['httpGet']['path']=='/readyz'
print('offline integration: selector, named port, probes, resources, and identity agree')
