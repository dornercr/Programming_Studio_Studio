required={'gcc-debug','gcc-release','clang-asan'}
results={'gcc-debug':'passed','gcc-release':'passed'}
def promote(r):return all(r.get(k)=='passed' for k in required)
assert not promote(results)
results['clang-asan']='passed';assert promote(results)
print('incomplete matrix blocks promotion; complete passing matrix permits review')
