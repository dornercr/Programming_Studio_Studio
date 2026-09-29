import pathlib,yaml
docs=list(yaml.safe_load_all(pathlib.Path("example.yaml").read_text()))
s=docs[0]['spec'];pods=[{'app':'harbor-front','release':'blue'},
                       {'app':'harbor-front','release':'green'}]
selected=[p for p in pods if all(p.get(k)==v for k,v in s['selector'].items())]
assert selected==[pods[1]]
print('offline selector chooses green; blue remains outside the Service')
