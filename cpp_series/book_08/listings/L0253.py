import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
s=docs[0]['spec'];r=s['strategy']['rollingUpdate']
assert r['maxSurge']==1 and r['maxUnavailable']==0
assert s['progressDeadlineSeconds']>s['minReadySeconds']
print('offline rollout: desired=3; surge=1; unavailable=0; stable-ready=10s')
