import hashlib,json
approved={'replicas':3,'memory_mib':128,'revision':'release-a'}
def fingerprint(x):return hashlib.sha256(json.dumps(x,sort_keys=True,separators=(',',':')).encode()).hexdigest()
recorded=fingerprint(approved)
actual=dict(approved);actual['memory_mib']=64
assert fingerprint(actual)!=recorded
assert fingerprint(dict(reversed(list(approved.items()))))==recorded
print('key order does not matter; changed memory request invalidates review')
