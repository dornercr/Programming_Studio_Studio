import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
service,pod=docs
assert all(pod['metadata']['labels'].get(k)==v for k,v in service['spec']['selector'].items())
target=service['spec']['ports'][0]['targetPort']
ports=pod['spec']['containers'][0]['ports']
assert any(p['name']==target and p['containerPort']==8080 for p in ports)
print('offline route: Service80 -> named http -> container8080')
