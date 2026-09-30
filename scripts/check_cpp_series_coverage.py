#!/usr/bin/env python3
"""Independent whole-spine text check against the supplied EPUB XML."""
from source_store import load_content, load_coding
import json,zipfile,re,posixpath,hashlib
from pathlib import Path
import xml.etree.ElementTree as ET
ROOT=Path(__file__).resolve().parents[1]
def local(e):return e.tag.rsplit('}',1)[-1]
def norm(s):return re.sub(r'\s+','',s)
data=load_content();results=[]
for c in data['courses']:
 if not c.get('series'):continue
 p=ROOT/'cpp_series/books'/c['series']['assets']['epub']['name'];z=zipfile.ZipFile(p)
 rootfile=next(e.get('full-path') for e in ET.fromstring(z.read('META-INF/container.xml')).iter() if local(e)=='rootfile')
 opf=ET.fromstring(z.read(rootfile));items={e.get('id'):e for e in opf.iter() if local(e)=='item'};chunks=[]
 for itemref in opf.iter():
  if local(itemref)!='itemref':continue
  item=items[itemref.get('idref')]
  if 'nav' in item.get('properties',''):continue
  file=posixpath.normpath(posixpath.join(posixpath.dirname(rootfile),item.get('href')))
  body=next(e for e in ET.fromstring(z.read(file)).iter() if local(e)=='body')
  chunks.append(''.join(body.itertext()))
 original=norm(''.join(chunks));reader=norm(''.join(b['text'] for t in c['topics'] for b in t['blocks']))
 assert original==reader,c['id']+' text coverage differs'
 results.append(dict(book=c['id'],normalizedCharacters=len(original),wholeSpineTextMatch=True,sha256=hashlib.sha256(original.encode()).hexdigest()))
(ROOT/'docs/cpp-series-text-coverage.json').write_text(json.dumps(results,indent=2)+'\n')
print('Whole-spine XML text matches reader text for all eight books.')
