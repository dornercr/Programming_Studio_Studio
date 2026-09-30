import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
d=docs[0]
selector=d['spec']['selector']['matchLabels'];labels=d['spec']['template']['metadata']['labels']
assert all(labels.get(k)==v for k,v in selector.items())
assert d['spec']['replicas']==3
print('offline Deployment: selector matches template, desired replicas3')
