import hashlib
config=b'workers=4\ntimeout_ms=250\n'
first=hashlib.sha256(config).hexdigest()
changed=hashlib.sha256(config.replace(b'workers=4',b'workers=8')).hexdigest()
assert first!=changed
print('configuration change produces a different rollout fingerprint')
