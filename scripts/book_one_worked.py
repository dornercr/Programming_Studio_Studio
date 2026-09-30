"""Source-linked runnable Book I examples and honest single-file adapters."""
import json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
CHAPTER_EXPERIMENTS={
1:'Change one printed value or compile-time check, predict the result, then run. Identify which C++ expression produced it.',
2:'Print one more compiler or type property. Decide whether the value depends on the platform before you run.',
3:'Change a printed value or supply a boundary input. Predict stdout, stderr, and the process exit code.',
4:'Change one initializer without narrowing. Predict the type and result before running.',
5:'Change one operand in the expression, predict its result, and explain which operation runs first.',
6:'Choose values that take a different decision branch. Trace exactly which branch runs.',
7:'Change the loop limit or sentinel. Predict the last value visited and the final sum.',
8:'Call the function with another value. Explain whether its parameters can change the caller.',
9:'Change an inner-scope value or add one function call. Predict what still exists afterward.',
10:'Change one array element. Trace each index and the computed total.',
11:'Change a character in the source string, then predict the new text or parsing result.',
12:'Change a stored element. If this is a vector, append one and compare its size with its capacity, which may depend on the library.',
13:'Change the caller value through a reference. Explain which name refers to the same object.',
14:'Change a pointed-to value, or try nullptr only if the code checks for it. Never dereference null.',
15:'Change an owned value and observe when cleanup runs. Do not use a pointer after its owner ends.',
16:'Change a record field or enum state and predict the output. Explain which fields have distinct types.',
17:'Change one call to the class and predict the new state. If a method can reject work, check that failure preserves its private state.',
18:'Create another object in a nested scope. Predict when its constructor and destructor run.',
19:'Trace declaration, definition, and main. Compare this online single-file arrangement with the original multi-file source.',
20:'Change one input or acceptance assertion. Trace how the program decides whether a requirement still holds.',
22:'Change the offset near an int boundary. A rejected update must leave every vector element unchanged.',
23:'Trigger a different constructor failure. Predict which already-constructed objects still get destructors.'}

def project_adapter(entry):
 d=ROOT/'cpp_series/book_01/projects'/entry
 if not d.exists():return None
 header=list(sorted(d.glob('*.hpp')))
 source=list(sorted(d.glob('*.cpp')))
 def clean(p):
  return '\n'.join(line for line in p.read_text().splitlines() if not re.match(r'\s*#include\s*"[^"]+"',line) and line.strip()!='#pragma once')
 return '\n\n'.join('// Online single-file adaptation of '+p.relative_to(ROOT).as_posix()+'\n'+clean(p) for p in header+source)+'\n'

def source_block_adapter(entry,original,source):
 parts={'B01-L0120':('B01-L0118','B01-L0119'),
        'B01-L0129':('B01-L0127','B01-L0128'),
        'B01-L0135':('B01-L0133','B01-L0134')}
 if entry not in parts:return None,[]
 header_id,impl_id=parts[entry]
 def clean(x):return '\n'.join(line for line in x.splitlines() if not re.match(r'\s*#include\s*"[^"]+"',line))
 sections=[f'// Original book listing {header_id}\n'+source[header_id],
           f'// Original book listing {impl_id}\n'+clean(source[impl_id]),
           f'// Original book listing {entry}\n'+clean(original)]
 return '\n\n'.join(sections)+'\n',[header_id,impl_id]

def excerpt_adapter(entry,original):
 setup={
  'B01-L0024':('#include <iostream>\n','',''),
  'B01-L0031':('#include <iostream>\n','','std::cout<<devices<<" "<<racks<<" "<<spares<<"\\n";'),
  'B01-L0037':('#include <iostream>\n','int items=23,capacity=5;','std::cout<<full<<" "<<remainder<<" "<<needed<<"\\n";'),
  'B01-L0039':('#include <iostream>\n','int count=5,maximum=10;','std::cout<<std::boolalpha<<valid<<"\\n";'),
  'B01-L0041':('#include <iostream>\n','int count=4;','std::cout<<before<<" "<<after<<"\\n";'),
  'B01-L0060':('#include <iostream>\n','',''),
  'B01-L0071':('#include <iostream>\n#include <string>\n','','std::cout<<label<<"\\n";'),
  'B01-L0077':('#include <iostream>\n#include <array>\n','','for(int value:calibration)std::cout<<value<<" ";std::cout<<"\\n";'),
 }
 if entry not in setup:return None
 headers,before,after=setup[entry]
 return headers+'int main(){\n'+before+'\n'+original+'\n'+after+'\n}\n'

BASELINES={
 'B01-L0120':'groups=5\nPASS\n',
 'B01-L0129':'1 "bench scope" 2\n2 "meter" 3\ntotal=5\nPASS\n',
 'B01-L0135':'checked offset: valid, overflow, underflow, extreme offset, and empty cases PASS\n',
 'B01-L0024':'HarborWorks\ndevices=4\n',
 'B01-L0031':'5 2 3\n',
 'B01-L0037':'4 3 5\n',
 'B01-L0039':'true\n',
 'B01-L0041':'4 5\n',
 'B01-L0060':'7\n0\n',
 'B01-L0071':'sensor=42\n',
 'B01-L0077':'2 4 6 \n',
}
SPECIAL={
 'B01-L0026':[dict(label='Valid age',input='29\n',stdout='Age: next year=30\n',stderr='',exitCode=0,hint='Read the number, then print next year.'),dict(label='Negative rejected',input='-1\n',stdout='Age: ',stderr='invalid age\n',exitCode=1,hint='Validate before calculating.'),dict(label='Missing number',input='',stdout='Age: ',stderr='invalid age\n',exitCode=1,hint='Extraction can fail.')],
 'B01-L0027':[dict(label='Valid count',input='4\n',stdout='devices=4\nwatts=10.00\n',stderr='',exitCode=0,hint='Each device contributes 2.5 watts.'),dict(label='Too many rejected',input='1001\n',stdout='',stderr='error: count must be in [0,1000]\n',exitCode=3,hint='Check both count boundaries.'),dict(label='Missing integer',input='',stdout='',stderr='error: expected an integer\n',exitCode=2,hint='The extraction itself can fail.')],
}

EXPERIMENTS_BY_ID={
 'B01-L0003':'Change the sensor with id 1 to id 4. Trace the sort comparison: it uses id, so predict order and printed values before running.',
 'B01-L0079':'Push a ninth vector value. Predict size first; capacity growth depends on the standard library implementation.',
 'B01-L0110':'Change the report text and run. The check observes process output and exit; inspecting the created file also requires a local filesystem.',
 'B01-L0129':'Change the duplicate-id self-test. Predict whether add rejects it and which self-test return code would report a broken contract.',
}

def build_worked(content):
 c=next(c for c in content['courses'] if c['id']=='cpp-book-01')
 blocks={b['id']:b for t in c['topics'] for b in t.get('blocks',[])}
 source={e['id']:blocks[e['blockId']]['code'] for e in c['series']['listings'] if e['language']=='cpp'}
 entries=[]
 for e in c['series']['listings']:
  if e['language']!='cpp':continue
  original=source[e['id']]
  joined,part_ids=source_block_adapter(e['id'],original,source)
  excerpt=excerpt_adapter(e['id'],original)
  code=project_adapter(e['id']) or joined or excerpt or (original if e.get('completeCandidate') else None)
  if code is None:continue # Header/implementation parts belong to a joined book program.
  adapted=code!=original
  existing=e.get('validation') or {}
  baseline=existing.get('stdout','') if existing.get('status')=='ran' else BASELINES.get(e['id'],'')
  cases=SPECIAL.get(e['id']) or [dict(label='Original program behavior',input='',stdout=baseline,stderr='',exitCode=0,hint='Read the first divergent line or assertion. Compare the original source and trace the changed branch.')]
  note=('Placed the book excerpt inside main() with a small setup/print statement. The original excerpt remains in Code & labs.' if excerpt else 'Joined the book’s exact header, implementation, and main listing for an online single-file build; the original separate listings remain in Code & labs.' if joined else 'Joined the packaged multi-file project into one online translation unit. The separated project files remain in the ZIP.')
  entry=dict(id='worked-'+e['id'],sourceId=e['id'],sourcePartIds=part_ids,sourceKind='excerpt' if excerpt else 'program',courseId='cpp-book-01',chapter=e['chapter'],chapterTitle=next(ch['title'] for ch in c['chapters'] if ch['number']==e['chapter']),title=e['title'],sourceFilename=e['filename'],sourceStatus=existing.get('status') or 'not verified',source=code,originalSource=original,adapted=adapted,adaptationNote=note if adapted else '',experiment=EXPERIMENTS_BY_ID.get(e['id'],CHAPTER_EXPERIMENTS.get(e['chapter'],'Change one value, predict the output, and run again.')),sampleInput=cases[0]['input'],sampleOutput=cases[0]['stdout'],checks=[dict(id='case'+str(i+1),**case) for i,case in enumerate(cases)])
  folder=ROOT/'coding_lab/book_01_worked'/e['id'];folder.mkdir(parents=True,exist_ok=True)
  (folder/'main.cpp').write_text(code if code.endswith('\n') else code+'\n')
  (folder/'original.cpp').write_text(original if original.endswith('\n') else original+'\n')
  (folder/'workshop.json').write_text(json.dumps({k:v for k,v in entry.items() if k not in ['source','originalSource']},indent=2,ensure_ascii=False)+'\n')
  entries.append(entry)
 return entries
