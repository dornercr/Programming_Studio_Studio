import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
p=docs[0]['spec']['template'];c=p['spec']['topologySpreadConstraints'][0]
assert c['labelSelector']['matchLabels']==p['metadata']['labels']
assert c['whenUnsatisfiable']=='DoNotSchedule' and c['maxSkew']==1
print('offline zone-spread constraint matches the workload labels')
