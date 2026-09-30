candidates=[{'name':'small','monthly':90,'p99_ms':180},
            {'name':'medium','monthly':140,'p99_ms':85},
            {'name':'large','monthly':220,'p99_ms':60}]
qualified=[c for c in candidates if c['p99_ms']<=100]
choice=min(qualified,key=lambda c:c['monthly'])
assert choice['name']=='medium'
print('illustrative qualified choice=medium; cheapest small fails p99<=100ms')
