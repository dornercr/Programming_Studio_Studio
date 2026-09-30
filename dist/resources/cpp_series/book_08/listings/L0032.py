from pathlib import Path
patterns=set(Path('.dockerignore').read_text().splitlines())
required={'.git','build/','.env','*.pem','secrets/','*.core'}
assert required<=patterns
print('context exclusions include VCS, builds, secrets, and core dumps')
