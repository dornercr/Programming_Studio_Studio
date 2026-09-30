import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
config,pod=docs
assert config['data']['workers']=='4'
volume=pod['spec']['volumes'][0]
assert volume['secret']['secretName']=='harbor-credentials'
assert 'data' not in pod
print('offline wiring: ConfigMap workers and read-only Secret volume reference')
