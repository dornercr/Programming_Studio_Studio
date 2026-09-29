plan={'resource_changes':[
 {'address':'terraform_data.service','change':{'actions':['update']}},
 {'address':'terraform_data.volume_contract','change':{'actions':['delete','create']}}]}
destructive=[r['address'] for r in plan['resource_changes']
             if 'delete' in r['change']['actions']]
assert destructive==['terraform_data.volume_contract']
print('manual review required: terraform_data.volume_contract')
