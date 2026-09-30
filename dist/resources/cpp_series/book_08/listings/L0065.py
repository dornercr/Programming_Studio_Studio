import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
service,stateful=docs
assert service['spec']['clusterIP']=='None'
assert stateful['spec']['serviceName']==service['metadata']['name']
assert stateful['spec']['volumeClaimTemplates'][0]['metadata']['name']=='data'
print('offline StatefulSet: stable service identity and per-replica data claim')
