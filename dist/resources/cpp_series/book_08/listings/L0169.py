statuses=[{'name':'app','ready':True},{'name':'proxy','ready':False}]
ready=all(c['ready'] for c in statuses)
assert not ready
print('application ready but proxy unready: combined readiness false')
