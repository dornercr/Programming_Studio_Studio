from pathlib import Path
import hashlib,shutil
Path('state.bin').write_bytes(b'harbor-state-generation7')
expected=hashlib.sha256(Path('state.bin').read_bytes()).hexdigest()
shutil.copyfile('state.bin','backup.bin');shutil.copyfile('backup.bin','restored.bin')
assert hashlib.sha256(Path('restored.bin').read_bytes()).hexdigest()==expected
Path('restored.bin').write_bytes(b'corrupt')
assert hashlib.sha256(Path('restored.bin').read_bytes()).hexdigest()!=expected
print('restore verified; corrupted restore rejected')
