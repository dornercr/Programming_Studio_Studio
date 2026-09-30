import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
p=docs[0]['spec'];c=p['containers'][0]
assert c['env'][0]=={'name':'LOG_DESTINATION','value':'stdout'}
assert c['securityContext']['readOnlyRootFilesystem'] is True
assert p['volumes'][0]['emptyDir']['sizeLimit']=='32Mi'
print('offline log destination stdout; bounded scratch volume separate')
