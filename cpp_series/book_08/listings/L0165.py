import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
p=docs[0]
assert p['apiVersion']=='security.istio.io/v1'
assert p['spec']['mtls']['mode']=='STRICT'
assert p['spec']['selector']['matchLabels']=={'app':'harbor'}
print('offline Istio policy: selected harbor workloads require STRICT mTLS')
