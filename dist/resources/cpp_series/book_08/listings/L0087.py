import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
claim,pod=docs
assert pod['spec']['volumes'][0]['persistentVolumeClaim']['claimName']==claim['metadata']['name']
assert pod['spec']['containers'][0]['volumeMounts'][0]['mountPath']=='/var/lib/harbor'
print('offline PVC mount: harbor-data -> /var/lib/harbor')
