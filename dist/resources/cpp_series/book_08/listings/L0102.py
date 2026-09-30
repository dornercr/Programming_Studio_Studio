slices=[{'endpoints':[{'addresses':['10.1.0.2'],'conditions':{'ready':False}},
                      {'addresses':['10.1.0.3'],'conditions':{'ready':True}}]}]
ready=[a for s in slices for e in s['endpoints'] if e['conditions'].get('ready') is True for a in e['addresses']]
assert ready==['10.1.0.3']
print('ready endpoints1: inspect readiness before DNS when this count becomes0')
