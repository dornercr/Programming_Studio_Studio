def assess(base,canary):
    if min(base['requests'],canary['requests'])<1000:return 'insufficient'
    b=base['errors']/base['requests'];c=canary['errors']/canary['requests']
    return 'hold' if c>b+0.005 else 'candidate'
assert assess({'requests':10000,'errors':20},{'requests':1000,'errors':12})=='hold'
assert assess({'requests':10000,'errors':20},{'requests':100,'errors':0})=='insufficient'
print('canary 1.2% vs baseline 0.2%: hold; 100 requests: insufficient')
