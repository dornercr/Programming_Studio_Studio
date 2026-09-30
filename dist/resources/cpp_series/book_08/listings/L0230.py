import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
c=docs[0]
assert c['route']['group_by']==['alertname','service']
assert c['route']['routes'][0]['receiver']=='oncall'
assert {r['name'] for r in c['receivers']}=={'warnings','oncall'}
print('offline routing: severity=page -> oncall; delivery integrations not configured')
