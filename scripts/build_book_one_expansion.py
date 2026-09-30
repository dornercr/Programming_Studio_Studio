"""Regenerate the additive Book I teaching pack. Never replaces textbook blocks."""
import hashlib,json,sys,textwrap
from pathlib import Path
sys.path.insert(0,str(Path(__file__).parent/'book_one_expansion'))
from labs import LABS
ROOT=Path(__file__).resolve().parents[1]
def write(p,text):
 p=ROOT/p;p.parent.mkdir(parents=True,exist_ok=True);p.write_text(text)
def dump(p,x):write(p,json.dumps(x,ensure_ascii=False,indent=2)+'\n')
def explain(line):
 s=line.strip()
 if s.startswith('//'):return s[2:].strip()
 if not s:return 'A blank line separates the surrounding steps for reading.'
 if s in ('}', '};'):return 'This closing brace ends the block or type begun above; a type definition also ends with a semicolon.'
 if s.startswith('#include'):return 'Bring declarations from this standard-library header into the translation unit.'
 if 'std::move' in s:return 'std::move permits ownership or value transfer here; it does not by itself extend the moved-from object’s lifetime.'
 if 'for (' in s or 'for(' in s:
  if ':' in s:return 'The colon separates the element declaration from its range. A plain value copies each element; & borrows it, and const prevents writes through that borrow.'
  return 'The traditional for loop has initialization, a condition, and an update separated by semicolons. It visits only indices below the stated count.'
 if 'if (' in s or 'if(' in s:return 'Evaluate this condition before continuing. A true guard takes its return or throw path; && requires both conditions, while || accepts either.'
 if s.startswith('return'):return 'Return this result to the caller and stop the current function call. The permitted result must satisfy the contract beside this block.'
 if 'push_back' in s:return 'Append one live element to the owning vector. This changes logical size and can reallocate its storage.'
 if 'swap' in s:return 'Exchange the two vector states here, committing the fully validated candidate without applying partial original-state writes.'
 if 'make_unique' in s:return 'Allocate the array and attach its release responsibility to one unique_ptr owner. The [] form uses array deletion.'
 if 'from_chars' in s:return 'Parse the character range. The result records both an error code and where parsing stopped; check both before committing.'
 if 'catch' in s:return 'This handler runs for the named exception type after cleanup of completed objects on the throwing path.'
 if 'try' in s:return 'Run this block with an exception handler available; constructed local owners still follow scope-exit cleanup rules.'
 if s.startswith('~'):return 'This destructor body runs when the fully constructed object is destroyed. It does not run for an owner whose constructor never completed.'
 if 'const {' in s or 'const{' in s:return 'This const member function observes state without modifying ordinary members through this access path.'
 if s=='public:':return 'Following members can be called by code outside this class; private storage above is protected from direct access.'
 if 'enum class' in s:return 'Define named scoped states. Use the enum type’s :: scope to name an enumerator; it is not an untyped integer status.'
 if s.startswith('struct ') or s.startswith('class '):return 'Define a record or class type. Members belong to each object; struct is public by default and class is private by default.'
 if ') :' in s:return 'The colon introduces member initialization before the body. Members initialize in declaration order, not the order written in this list.'
 if 'namespace' in s:return 'Group this declaration in a named namespace. The qualified definition must match this public interface.'
 if 'output =' in s:return 'Commit the candidate into the caller’s reference only after all preceding validation succeeds.'
 if '++' in s:return 'Increase this counter by one after the relevant checks. Its stored meaning must remain consistent with the processed data.'
 if ' + ' in s or '+=' in s or '-=' in s:return 'Calculate or update this value under the bounded inputs in the contract. Validate before arithmetic or writes that could violate its rules.'
 if 'std::' in s:return 'Use the named standard-library type or operation. :: selects its namespace; keep ownership, bounds, and lifetime promises in view.'
 return 'This declaration or statement supplies part of the shown interface or transformation. Follow the same named object through the surrounding checks and return path.'
book=json.loads((ROOT/'content/cpp-book-01/source.json').read_text());chapters={c['number']:c['title'] for c in book['chapters']}
chunks={n:json.loads((ROOT/f'content/cpp-book-01/ch{n}.json').read_text()) for n in range(1,25)}
topic={n:next((t['id'] for t in reversed(x['topics']) if 'Review and solution' in t['title']),x['topics'][0]['id']) for n,x in chunks.items()}
cards=[];per={}
for line in (ROOT/'scripts/book_one_expansion/recall.txt').read_text().splitlines():
 if not line or line.startswith('#'):continue
 n,q,a=line.split('|',2);n=int(n);per[n]=per.get(n,0)+1
 cards.append(dict(id=f'B01X.C{n:02}.R{per[n]:02}',chapter=n,topicId=topic[n],question=q,answer=a,category='Recall and reasoning'))
assert len(cards)==256
questions=[];studies=[]
for item in LABS:
 n=item['chapter'];slug=item['id'];folder=f'coding_lab/book_one_expansion/ch{n:02}'
 body=item.pop('body');item['driver']=item['driver'].replace('int main(){','int main() {\n    ').replace(';',';\n    ').rstrip()+'\n';q={k:v for k,v in item.items() if k not in ['design','invariant','alternative','bug']}
 q.update(courseId='cpp-book-01',chapterTitle=chapters[n],level='Debug / extend',hints=['Start with the contract and predict the sample before running.',item['invariant'],'The failed check names the boundary your implementation missed.'],explanation=item['design']+' '+item['invariant']+' Alternative: '+item['alternative'])
 q['guide']=dict(problem=item['prompt'],design=item['design'],invariant=item['invariant'],trace='Sample input: '+repr(item['sampleInput'])+'; expected output: '+repr(item['sampleOutput']),failure=item['bug'],maintenance='Preserve the contract and rerun every boundary check after an edit. '+item['alternative'])
 questions.append(q)
 for name in ['starter','solution']:write(f'{folder}/{name}.cpp',item[name]+'\n'+item['driver'])
 write(f'{folder}/README.md',f'''# Chapter {n}: {item['title']}

{item['prompt']}

## Design and rules
{item['design']}

{item['invariant']}

Alternative: {item['alternative']}

Starter defect: {item['bug']}

## Build and run (from the project root)
```sh
g++ -std=c++20 -Wall -Wextra -Wpedantic {folder}/solution.cpp -o /tmp/book-one-ch{n:02}
printf '%s' '{item['sampleInput']}' | /tmp/book-one-ch{n:02}
```
Expected standard output:
```text
{item['sampleOutput'].rstrip()}
```
Replace solution.cpp with starter.cpp to investigate the deliberate defect. The starter compiles but violates at least one check. The browser editor uses only the function/class portion; its driver is supplied separately. Run `node scripts/test-book-one-expansion.mjs` for all checks, not just this sample.
''')
 # Six concrete lab cards per chapter, each tied to this chapter's review lesson.
 additions=[
  ('Contract',f'What must {item["signature"]} accept and reject?',item['prompt'],None),
  ('Code tracing',f'Predict the output: {item["title"]}. Assume the required standard-library headers; the function and sample main are shown.',item['sampleOutput']+'Why: '+item['design'],body+'\n\n'+item['driver'].strip()),
  ('Bug diagnosis',f'What is wrong with this starter for {item["title"]}?',item['bug']+' Repair: '+item['invariant'],item['starter'].split('\n\n',1)[-1].strip()),
  ('Invariant',f'What must stay true while implementing {item["title"]}?',item['invariant'],None),
  ('Design choice',f'Why was this design selected for {item["title"]}?',item['design'],None),
  ('Alternative',f'Which competing design could solve {item["title"]}, and when is it useful?',item['alternative'],None)]
 for i,(kind,prompt,answer,code) in enumerate(additions,1):
  card=dict(id=f'B01X.C{n:02}.L{i:02}',chapter=n,topicId=topic[n],question=prompt,answer=answer,category=kind,labId=slug)
  if code:card['code']=code
  if i==2:card['sampleInput']=item['sampleInput']
  cards.append(card)
 # Actual line references into the delivered runnable solution.
 file=f'{folder}/solution.cpp';lines=(ROOT/file).read_text().splitlines();start=len(HEADERS_LINES:=item['solution'].split('\n\n',1)[0].splitlines())+2
 code_lines=body.splitlines();end=start+len(code_lines)-1
 assert '\n'.join(lines[start-1:end])==body
 focus=dict(expansionId=slug,title=item['title'],file=file,lineStart=start,lineEnd=end,code=body,explain=[explain(s) for s in code_lines],what=item['prompt'],where='The supplied main driver calls this operation. It belongs to '+chapters[n]+'.',why=item['design'],invariant=item['invariant'],risk=item['bug']+' '+item['alternative'])
 meaningful=[s.strip() for s in code_lines if s.strip() and s.strip() not in ('}', '};') and not s.strip().startswith('//')]
 caller=item['driver'].strip()
 # The flowchart makes call/result relationships explicit without pretending to be inheritance.
 view=dict(expansionId=slug,title=item['title']+' — call and contract',kind='activity',source=file,nodes=[dict(id='caller',label='Caller supplies input',code=[caller,'// main owns this sample session.']),dict(id='operation',label='Check and transform',code=meaningful[:4]+['// Excerpt: validate before committing state.']),dict(id='result',label='Return to the caller',code=[next((s for s in reversed(meaningful) if 'return ' in s),meaningful[-1]),'// Result and state must satisfy the contract.'])],edges=[dict(from_='caller',to='operation',kind='flow',label='call with the stated inputs'),dict(from_='operation',to='result',kind='flow',label='return or reported rejection; see full code')],explanation='Solid arrowheads show execution flow. The code boxes are excerpts, not complete programs. Purple theory labels are teaching annotations, not inheritance, ownership, or dependency arrows.')
 for e in view['edges']:e['from']=e.pop('from_')
 studies.append(dict(chapter=n,expansionId=slug,source=file,overview=view,focus=[focus],extra_overviews=[]))
assert len(cards)==400 and len(questions)==24 and len(studies)==24
# A genuine three-file companion for the namespace lesson.
q=next(q for q in questions if q['chapter']==19);folder='coding_lab/book_one_expansion/ch19'
write(folder+'/billing.hpp','#ifndef BOOK_ONE_BILLING_HPP\n#define BOOK_ONE_BILLING_HPP\nnamespace billing { int subtotal(int unit, int count); }\n#endif\n')
write(folder+'/billing.cpp','#include "billing.hpp"\n#include <stdexcept>\n'+q['solution'].split('int billing::subtotal',1)[1].join(['int billing::subtotal','']))
write(folder+'/main.cpp','#include "billing.hpp"\n#include <iostream>\n#include <stdexcept>\n'+q['driver'])
write(folder+'/BUILD.md','From the project root:\n\n```sh\ng++ -std=c++20 -Wall -Wextra -Wpedantic coding_lab/book_one_expansion/ch19/main.cpp coding_lab/book_one_expansion/ch19/billing.cpp -o /tmp/billing\nprintf \'7 4\\n\' | /tmp/billing\n```\nExpected output: `28` followed by a newline. The header introduces the contract; billing.cpp defines it; main.cpp calls it. Omitting billing.cpp produces an undefined-reference link error.\n')
dump('content/book-one-expansion.json',dict(format=1,courseId='cpp-book-01',cards=cards,questions=questions,diagrams=studies))
print('Authored 400 cards, 24 challenges, and 24 diagram studies. Run node scripts/render-book-one-expansion.mjs, then node scripts/book-one-expansion.mjs --apply.')
