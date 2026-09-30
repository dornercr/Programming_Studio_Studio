import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
s=docs[0]['spec'];j=s['jobTemplate']['spec']
assert s['concurrencyPolicy']=='Forbid' and s['timeZone']=='Etc/UTC'
assert j['activeDeadlineSeconds']==600 and j['backoffLimit']==1
print('offline backup schedule: UTC 02:00; no overlap; 600s job deadline')
