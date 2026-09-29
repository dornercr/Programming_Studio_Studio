desired={'metadata':{'generation':8},'spec':{'replicas':3},'status':{'observedGeneration':7,'readyReplicas':3}}
def ready(d):return d['status'].get('observedGeneration',0)>=d['metadata']['generation'] and d['status'].get('readyReplicas',0)>=d['spec']['replicas']
assert not ready(desired)
desired['status']['observedGeneration']=8
assert ready(desired)
print('old generation rejected; observed generation8 with3 ready accepted')
