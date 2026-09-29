import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
c=docs[0];pipeline=c['service']['pipelines']['traces']
assert pipeline['processors']==['memory_limiter','batch']
for kind in ('receivers','processors','exporters'):
    assert all(name in c[kind] for name in pipeline[kind])
print('offline Collector: OTLP -> memory_limiter -> batch -> debug')
