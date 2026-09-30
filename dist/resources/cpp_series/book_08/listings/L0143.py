import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
s=docs[0]['spec']
assert s['clusterIP']=='None' and s['publishNotReadyAddresses'] is False
assert s['selector']=={'app':'harbor-worker'}
print('offline headless service: client resolves worker endpoints')
