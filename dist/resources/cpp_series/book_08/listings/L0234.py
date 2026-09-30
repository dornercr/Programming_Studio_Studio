rules=[{'name':'HighErrorRate','severity':'page','owner':'service-oncall','runbook':'runbooks/errors.md'},
       {'name':'DiskTrend','severity':'warning','owner':'capacity-team'}]
def valid(r):return r['severity']!='page' or bool(r.get('owner') and r.get('runbook'))
assert all(map(valid,rules))
broken={'name':'Mystery','severity':'page'}
assert not valid(broken)
print('pages require owner and runbook; ownerless page rejected')
