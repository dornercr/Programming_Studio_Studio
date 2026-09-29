compute=120.0;storage=30.0;egress=10.0  # invented monthly dollars for this exercise
completed=2_000_000
cost_per_million=(compute+storage+egress)/completed*1_000_000
print(f'illustrative cost per million successful requests=${cost_per_million:.2f}')
assert cost_per_million==80
