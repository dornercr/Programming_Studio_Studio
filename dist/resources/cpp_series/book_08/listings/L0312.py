def recovered(state):
    return (not state['fault_enabled'] and state['successes']==state['probes']
            and state['probes']>=20 and state['queue_depth']==0)
s={'fault_enabled':True,'successes':20,'probes':20,'queue_depth':0}
assert not recovered(s)
s['fault_enabled']=False;assert recovered(s)
s['queue_depth']=4;assert not recovered(s)
print('recovery requires fault removed, 20/20 probes, and drained queue')
