desired={'workers':4,'queue_limit':64,'log_level':'info'}
observed={'workers':4,'queue_limit':256,'log_level':'info','uptime_seconds':900}
drift={k:(observed.get(k),v) for k,v in desired.items() if observed.get(k)!=v}
assert drift=={'queue_limit':(256,64)}
print('queue_limit: observed=256 desired=64; uptime is not managed configuration')
