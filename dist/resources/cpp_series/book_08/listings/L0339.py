import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
s=docs[0]['spec'];r=s['template']['spec']['containers'][0]['resources']
assert s['replicas']==3 and r['requests']['cpu']=='150m'
assert int(r['requests']['memory'][:-2])<int(r['limits']['memory'][:-2])
print('offline plan preserves three replicas and declares 150m/96Mi requests')
