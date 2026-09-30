from pathlib import Path
import yaml
b=yaml.safe_load(Path('base/deployment.yaml').read_text())
o=yaml.safe_load(Path('overlays/production/kustomization.yaml').read_text())
assert b['spec']['replicas']==1
assert o['replicas']==[{'name':'harbor-gitops','count':3}]
print('offline base=1, overlay=3; Kustomize rendering requires kubectl')
