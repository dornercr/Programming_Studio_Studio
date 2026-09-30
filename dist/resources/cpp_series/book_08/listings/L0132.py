import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
h=docs[0]['spec']
assert h['minReplicas']==2 and h['maxReplicas']==8
assert h['metrics'][0]['resource']['target']['averageUtilization']==60
assert h['behavior']['scaleDown']['stabilizationWindowSeconds']==300
print('offline HPA: 2..8 replicas, CPU target60%, downscale window300s')
