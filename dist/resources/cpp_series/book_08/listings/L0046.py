import json
from pathlib import Path
manifest={'artifact':'harbor-lab','components':[{'name':'libstdc++','version':'record-from-build'},{'name':'libc','version':'record-from-build'}]}
Path('inventory.json').write_text(json.dumps(manifest,indent=2))
names={c['name'] for c in manifest['components']}
assert {'libstdc++','libc'}<=names
broken={'components':[manifest['components'][1]]}
assert not {'libstdc++','libc'}<={c['name'] for c in broken['components']}
print('complete inventory accepted; missing C++ runtime rejected')
