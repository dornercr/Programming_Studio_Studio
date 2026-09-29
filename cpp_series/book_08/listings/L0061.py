import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
j=docs[0]
assert j['kind']=='Job' and j['spec']['backoffLimit']==2
assert j['spec']['template']['spec']['restartPolicy']=='Never'
assert j['spec']['activeDeadlineSeconds']==120
print('offline Job: finite deadline120s and retry limit2')
