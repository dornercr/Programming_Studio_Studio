import math
samples=[10]*95+[200]*5
ordered=sorted(samples)
p99=ordered[math.ceil(.99*len(ordered))-1]
mean=sum(ordered)/len(ordered)
assert mean<25 and p99>100
print(f'mean={mean:.1f}ms p99={p99}ms: hold release')
