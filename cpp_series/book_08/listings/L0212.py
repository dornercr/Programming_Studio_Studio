records=[{'seq':1,'event':'start'},{'seq':2,'event':'accepted'},{'seq':5,'event':'done'}]
gaps=[]
for a,b in zip(records,records[1:]):
    if b['seq']!=a['seq']+1:gaps.append((a['seq']+1,b['seq']-1))
assert gaps==[(3,4)]
print('missing diagnostic sequence3..4; event completeness not established')
