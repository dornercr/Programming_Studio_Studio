import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
r=docs[0]['spec']['containers'][0]['resources']
assert r['requests']['cpu']=='250m' and str(r['limits']['cpu'])=='1'
assert r['requests']['memory']=='64Mi' and r['limits']['memory']=='128Mi'
print('offline resource policy: request0.25CPU/64Mi, limit1CPU/128Mi')
