import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
r=docs[0]['groups'][0]['rules'][0]
assert '/ 0.001 > 10' in r['expr']
assert r['for']=='5m' and r['labels']['severity']=='page'
print('offline alert rule: error fraction / 0.001 exceeds10 for5m')
