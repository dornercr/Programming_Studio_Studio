capacity={'zone-a':400,'zone-b':400,'zone-c':400}
demand=700
remaining={lost:sum(v for k,v in capacity.items() if k!=lost) for lost in capacity}
assert min(remaining.values())==800
headroom=min(remaining.values())-demand
print(f'worst surviving capacity=800 requests/s; headroom={headroom} requests/s')
assert demand<=min(remaining.values())
