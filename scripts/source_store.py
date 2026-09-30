"""Read the modular source for the original Python verification/packaging tools."""
import base64,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]/'content'
def _read(name): return json.loads((ROOT/name).read_text())
def _assets(x):
    if isinstance(x,dict):
        if '$asset' in x:return x['prefix']+base64.b64encode((ROOT/x['$asset']).read_bytes()).decode()
        return {k:_assets(v) for k,v in x.items()}
    if isinstance(x,list):return [_assets(v) for v in x]
    return x
def load_content():
    m=_read('manifest.json');data={**m['root'],'courses':[]}
    for e in m['courses']:
        c=_read(e['base']);c['topics']=[]
        for name in e['chunks']:
            ch=_read(name);c['topics']+=ch['topics']
            for key in ['teaching','examples','diagrams','lectures']:
                if key in ch:c[key].update(ch[key])
            if 'listings' in ch:c['series']['listings']+=ch['listings']
        order={id:i for i,id in enumerate(e['topicOrder'])};c['topics'].sort(key=lambda t:order[t['id']])
        if 'series' in c:
            order={id:i for i,id in enumerate(e['listingOrder'])};c['series']['listings'].sort(key=lambda t:order[t['id']])
        data['courses'].append(c)
    return _assets(data)
def load_coding():
    m=_read('manifest.json');data={**m['codingRoot'],'questions':[],'workedPrograms':[]}
    for name in m['coding']:
        ch=_read(name)
        for key in ['questions','workedPrograms']:data[key]+=ch[key]
    for key in ['questions','workedPrograms']:
        order={id:i for i,id in enumerate(m['codingOrder'][key])};data[key].sort(key=lambda q:order[q['id']])
    return data
