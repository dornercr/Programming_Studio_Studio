import re
refs=['registry.example/harbor:latest','registry.example/harbor@sha256:'+'a'*64]
def immutable_name(s):return re.fullmatch(r'[^\s@]+@sha256:[0-9a-f]{64}',s) is not None
assert not immutable_name(refs[0]) and immutable_name(refs[1])
print('tag rejected; digest-shaped reference accepted for further verification')
