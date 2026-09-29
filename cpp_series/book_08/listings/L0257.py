def accept_release(status, probes):
    controller_ok=(status['observedGeneration']==status['generation']
                   and status['updated']==status['desired']
                   and status['available']==status['desired'])
    application_ok=bool(probes) and all(p['status']==200 and p['result']==49 for p in probes)
    return controller_ok and application_ok
state=dict(generation=8,observedGeneration=8,updated=3,available=3,desired=3)
assert not accept_release(state,[{'status':200,'result':48}])
assert accept_release(state,[{'status':200,'result':49}])
print('controller complete + incorrect result: reject; correct result: accept review')
