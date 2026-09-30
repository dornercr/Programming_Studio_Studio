import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
pod=docs[0];c=pod['spec']['containers'][0]
assert pod['spec']['automountServiceAccountToken'] is False
assert c['securityContext']['readOnlyRootFilesystem'] is True
assert c['resources']['limits']['memory']=='128Mi'
print('offline contract: non-root, no API token, read-only root, 128Mi limit')
