import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
p=docs[0]['spec'];c=p['containers'][0]
assert c['args'][1]=='/run/tls/tls.crt' and c['args'][3]=='/run/tls/tls.key'
assert c['volumeMounts'][0]['readOnly'] is True
assert p['volumes'][0]['secret']['secretName']=='harbor-server-tls'
print('offline TLS mount: certificate/key paths, no embedded private key')
