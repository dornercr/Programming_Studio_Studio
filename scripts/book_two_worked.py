"""Build source-linked Book II workshops from its actual C++ listings."""
from pathlib import Path
import json
import re

ROOT = Path(__file__).resolve().parents[1]
BASELINES = ROOT / 'scripts/book_two_baselines.json'
JOIN = {
    'B02-L0092': ['B02-L0090', 'B02-L0091'],
    'B02-L0094': ['B02-L0090', 'B02-L0091'],
    'B02-L0102': ['B02-L0101'],
    'B02-L0106': ['B02-L0104', 'B02-L0105'],
    'B02-L0109': ['B02-L0104', 'B02-L0105'],
}
PARTS = set(sum(JOIN.values(), [])) | {'B02-L0110'}
SPECIAL_INPUTS = {
    'B02-L0092': [('Valid and completed rows', '7|1|0|Keep this\n9|2|1|Done\n'),
                  ('Duplicate identifier', '7|1|0|Keep this\n7|2|0|Again\n')],
    'B02-L0106': [('Valid priority report', '7|2|0|Review\n3|1|0|Fix\n9|3|1|Done\n'),
                  ('Invalid priority', '1|4|0|Bad\n'),
                  ('Duplicate identifier', '1|1|0|First\n1|2|0|Second\n')],
}
EXPERIMENTS = {
    1: 'Change a value in a copied object. Predict which copy changes.',
    2: 'Try a boundary at construction. Explain which class invariant holds.',
    3: 'Move the file or wrapper into a smaller scope. Predict when it closes.',
    4: 'Change one copy. Trace which storage belongs to each object.',
    5: 'Move one temporary. Predict which operation runs and what may be read afterward.',
    6: 'Change an owner. Predict which observer can still lock the object.',
    7: 'Change one operand. Check whether the result behaves like an independent value.',
    8: 'Add one base and derived print. Predict construction and destruction order.',
    9: 'Change the concrete object. Predict which override a base reference calls.',
    10: 'Replace one component. Trace calls through the small interface.',
    11: 'Call the template with another supported type. Predict the selected code.',
    12: 'Change the type or capacity argument. Trace what the generic class stores.',
    13: 'Change a type argument. Predict whether the constraint accepts it at compile time.',
    14: 'Change a captured value. Explain whether the lambda owns or borrows it.',
    15: 'Add a container item. Predict which old iterators remain valid.',
    16: 'Add an item or lookup. Check the container operation’s ordering promise.',
    17: 'Change the input order. Decide which algorithm precondition matters.',
    18: 'Change one source value. Trace when the view reads it.',
    19: 'Try a missing or invalid input. Trace the result alternative.',
    20: 'Change the file data or random seed. Predict stable values and platform-dependent values separately.',
    21: 'Try a bad record after good ones. Predict the transaction result and exit status.',
    23: 'Change one injected failure. Check whether acquired resources are released.',
    24: 'Try an arithmetic boundary and trace the rejected operation without signed overflow.',
    25: 'Try duplicate identifiers and verify the last accepted report is unchanged.',
    26: 'Change a test case and observe the failure report and exit code.',
}
SPECIFIC_EXPERIMENTS = {
    'B02-L0002': 'Change b from 7 to 8. Predict the UserId equality result, then explain why two different wrapper values compare false.',
    'B02-L0003': 'Change the copied Point, then print the original and copy to show that their fields are independent.',
    'B02-L0004': 'Transfer the unique_ptr to another owner. Predict which pointer is empty and when the owned int is released.',
    'B02-L0085': 'Change the random seed and predict the total. The timing check only says whether the end follows the start.',
    'B02-L0098': 'Read the value checks after vector reserve. The original listing prints library-dependent copy and move counts.',
    'B02-L0106': 'Try duplicate identifiers and verify the report rejects input with the documented error and exit code.',
}
FILE_LABS={'B02-L0012','B02-L0013','B02-L0083'}

def clean_local_includes(code):
    return '\n'.join(line for line in code.splitlines()
                     if not re.match(r'\s*#include\s*"[^\"]+"', line)) + '\n'

def build_worked(content):
    course = next(c for c in content['courses'] if c['id'] == 'cpp-book-02')
    blocks = {b['id']: b for t in course['topics'] for b in t.get('blocks', [])}
    listings = [e for e in course['series']['listings'] if e['language'] == 'cpp']
    sources = {e['id']: blocks[e['blockId']]['code'] for e in listings}
    baselines = json.loads(BASELINES.read_text()) if BASELINES.exists() else {}
    entries = []
    for e in listings:
        source_id = e['id']
        if source_id in PARTS:
            continue
        original = sources[source_id]
        joined = JOIN.get(source_id, [])
        test_header = 'course_test.hpp' in original
        parts = (["B02-L0110"] if test_header else []) + joined
        if joined or test_header:
            sections = []
            for part_id in parts:
                sections.append('// Exact original book listing ' + part_id + '\n' + clean_local_includes(sources[part_id]))
            sections.append('// Original book listing ' + source_id + '\n' + clean_local_includes(original))
            code = '\n'.join(sections)
        else:
            code = original
        adaptation = []
        if joined: adaptation.append('The book header, implementation, and program/test bodies are joined for a single-file build; local #include lines are replaced by the corresponding inline source.')
        if test_header: adaptation.append('The book course_test.hpp body is placed before the program so its CHECK macro is defined; its local #include line is removed.')
        if source_id == 'B02-L0085':
            # Clock tick counts differ on every execution and across systems.
            old = "std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()"
            assert old in code
            code = code.replace(old, '(end >= start)')
            adaptation.append('The measured clock count is displayed as a stable nonnegative-time check; original timing code remains linked.')
        if source_id == 'B02-L0098':
            # Vector relocation counts vary with library implementations.
            old = '<< " copies=" << metrics.copies\n              << " moves=" << metrics.moves << \'\\n\';'
            assert old in code
            code = code.replace(old, '<< " values preserved\\n";')
            adaptation.append('Library-specific copy/move counts are omitted from the display; the program still checks the relocated values.')
        assert 'int main(' in code or 'int main()' in code, source_id
        adapted = code != original
        labels = SPECIAL_INPUTS.get(source_id) or [('Original program behavior', '')]
        recorded = baselines.get(source_id)
        checks = [dict(id='case'+str(i+1), label=label, input=stdin,
                       stdout=(recorded[i]['stdout'] if recorded else ''),
                       stderr=(recorded[i]['stderr'] if recorded else ''),
                       exitCode=(recorded[i]['exitCode'] if recorded else 0),
                       hint='Compare the first different output line, exit code, or CHECK invariant.')
                  for i, (label, stdin) in enumerate(labels)]
        assert not recorded or len(recorded) == len(labels), source_id
        x = dict(id='worked-'+source_id,sourceId=source_id,sourcePartIds=parts,
                 sourceKind='program',courseId='cpp-book-02',chapter=e['chapter'],
                 chapterTitle=next(ch['title'] for ch in course['chapters'] if ch['number']==e['chapter']),
                 title=e['title'],sourceFilename=e['filename'],
                 environmentNote=('This program creates a local file. If online execution blocks filesystem access, download the source or use the offline Book II companion.' if source_id in FILE_LABS else ''),
                 sourceStatus=(e.get('validation') or {}).get('status') or 'project component',
                 source=code,originalSource=original,adapted=adapted,
                 adaptationNote=' '.join(adaptation),
                 experiment=SPECIFIC_EXPERIMENTS.get(source_id,EXPERIMENTS.get(e['chapter'],'Change one value, predict output, and explain the invariant.')),
                 sampleInput=checks[0]['input'],sampleOutput=checks[0]['stdout'],checks=checks)
        folder=ROOT/'coding_lab/book_02_worked'/source_id
        folder.mkdir(parents=True,exist_ok=True)
        (folder/'main.cpp').write_text(code if code.endswith('\n') else code+'\n')
        (folder/'original.cpp').write_text(original if original.endswith('\n') else original+'\n')
        (folder/'workshop.json').write_text(json.dumps({k:v for k,v in x.items() if k not in ['source','originalSource']},indent=2,ensure_ascii=False)+'\n')
        entries.append(x)
    covered={i for x in entries for i in [x['sourceId'],*x['sourcePartIds']]}
    assert covered==set(sources), (sorted(set(sources)-covered),sorted(covered-set(sources)))
    return entries
