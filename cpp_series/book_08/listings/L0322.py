action={'id':'INC-17-A1','owner':'service-team',
        'failure_fixture':'dependency_slow_200ms',
        'acceptance':'returns deadline_exceeded within configured transport deadline',
        'test':'deadline_regression','status':'open'}
required=('owner','failure_fixture','acceptance','test')
assert all(action.get(k) for k in required)
assert action['status']!='closed'
results={'deadline_regression':'passed'}
if results.get(action['test'])=='passed':action['status']='ready_for_review'
print('INC-17-A1: ready_for_review, not automatically closed')
