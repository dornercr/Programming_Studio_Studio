old={'name':'harbor-0','uid':'uid-old','restartCount':4}
new={'name':'harbor-0','uid':'uid-new','restartCount':0}
def same_instance(a,b):return a['uid']==b['uid']
assert old['name']==new['name'] and not same_instance(old,new)
print('same name, different UID: reset instance-local observations')
