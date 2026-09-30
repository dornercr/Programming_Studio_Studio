arrival_per_s=200
mean_residence_s=0.05
mean_in_system=arrival_per_s*mean_residence_s
print(f'assumed stable workload: mean in-system={mean_in_system:.1f} requests')
assert mean_in_system==10
