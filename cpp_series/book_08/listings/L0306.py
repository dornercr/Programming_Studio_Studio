experiment={'target':'lab-worker','fault':'delay_ms','value':200,
            'duration_s':30,'max_error_rate':0.02,'max_targets':1}
assert experiment['duration_s']<=60 and experiment['max_targets']==1
observations=[(1000,5),(1000,25)]
for requests,errors in observations:
    abort=errors/requests>experiment['max_error_rate']
    print(f'errors={errors}/{requests} abort={str(abort).lower()}')
