"""Runnable Book III listings; preserve original bytes and label every adapter."""
from pathlib import Path
import json
import re

ROOT = Path(__file__).resolve().parents[1]
JOIN = {'B03-L0093': ['B03-L0091', 'B03-L0092'],
        'B03-L0094': ['B03-L0091', 'B03-L0092']}
PARTS = {'B03-L0091', 'B03-L0092'}
HEADER = ROOT / 'cpp_series/book_02/listings/L0110.cpp'
BASELINES = ROOT / 'scripts/book_three_baselines.json'
GUIDES = {
 1: ('Find an answer without inventing an input value.', 'A scan keeps a small fact about the visited prefix. A library algorithm is shorter when its contract already matches.', 'The stored answer describes only the items already visited.', 'An initial maximum of zero gives the wrong answer for all-negative input. Replacing < with <= changes first-maximum tie behavior.', 'For [3,9,9,2], the first maximum index moves from 0 to 1, then stays 1.', 'Keep empty-input and tie rules explicit so a caller can rely on them.'),
 2: ('Estimate work before choosing an implementation.', 'Count operations and compare equivalent results. A wall clock is useful later, but noise makes it a weak first correctness check.', 'Equivalent implementations must compute the same result.', 'A nested loop may do n*n visits even when its body is only one line. Big-O alone does not predict the faster implementation.', 'Doubling n doubles a linear count, but multiplies a full pair count by four.', 'Measure representative data and do not replace correctness tests with a benchmark.'),
 3: ('Store growing data in adjacent positions.', 'A vector or owning array offers direct index access. A linked structure is useful for some insertions but loses adjacency.', 'Logical size never exceeds capacity; live elements remain owned until removed or the container ends.', 'Keeping a pointer across reallocation can leave it dangling. Reading at size() is outside the live range.', 'Capacity doubling can progress 1,2,4,8 while logical sizes progress 1,2,3,4,5.', 'Separate logical bounds, storage allocation, and observer lifetime in reviews.'),
 4: ('Insert or remove nodes without losing the rest of a chain.', 'Links make local rewiring possible. A vector is simpler and often faster for traversal-heavy work.', 'Every owned node is released once; next/previous links agree where both exist.', 'Overwriting next before saving its old value loses the remainder. A shallow owning copy may double-delete nodes.', 'Save the successor, reconnect the neighbor, then release the removed node.', 'Test empty, singleton, middle removal, copy/move, and destruction paths.'),
 5: ('Remember unfinished work in last-in, first-out order.', 'A stack models nested brackets directly. Counting bracket types cannot prove nesting order.', 'The top is the most recent unmatched opening bracket.', '([)] has matching counts but wrong nesting. Reading top() when empty is invalid.', 'In ([ ]), push (, push [, close ] removes [, close ) removes (.', 'Make empty-stack behavior part of the API, not a hidden caller assumption.'),
 6: ('Process arrivals fairly or retain a moving window.', 'A queue is first-in, first-out; a deque allows edits at both ends. A vector front erase shifts remaining elements.', 'Removal order follows the chosen queue contract and stored count matches contents.', 'Using pop_back for a FIFO queue reverses service order. A full ring buffer must not silently overwrite live data.', 'Push 7,9; pop returns 7; push 11; later pops return 9 then 11.', 'Test wraparound and reuse after a full-to-not-full transition.'),
 7: ('Find keyed data despite hash collisions.', 'A hash narrows candidate buckets, then equality identifies the key. A sorted map trades expected constant-time lookup for ordered traversal.', 'Equal keys have equal hashes; unequal colliding keys remain distinguishable.', 'Treating one bucket as one key overwrites colliding entries. Open-address deletion without a tombstone can hide later entries.', 'Two keys in one bucket still need two equality checks; removing one must leave the other findable.', 'Use forced-collision tests rather than hoping a random test happens to collide.'),
 8: ('Keep unique values or counts with clear ordering.', 'A set keeps keys once; a map associates each key with a value. An unordered container is better when sorted traversal is not required.', 'Ordering must be a consistent strict comparison; frequency totals equal the input count.', 'A comparison that says a<a is true breaks ordering. Iterating an unordered container cannot promise sorted output.', 'For 3,1,3, a sorted set visits 1,3 and a count map records 1:1,3:2.', 'State whether order is part of the external contract before changing containers.'),
 9: ('Visit a hierarchy and measure its shape.', 'Recursive traversal follows each subtree. An explicit work stack is safer when input depth can exceed the call stack.', 'Every reachable tree node is visited according to the selected order; child ownership has no cycle.', 'A missing null base case dereferences no object. Accidental cycles make a tree traversal fail to terminate.', 'Preorder visits root before children; inorder places root between left and right.', 'Document whether height counts nodes or edges, especially for an empty tree.'),
 10: ('Search ordered nodes while allowing updates.', 'A binary search tree narrows a search by key. A sorted vector is simpler for mostly-read data and can be cache-friendly.', 'All keys on the left are smaller and all on the right larger under the duplicate policy.', 'Deleting a two-child node without reconnecting its successor loses ordering or a subtree.', 'After deleting 5 from keys 3,5,7, inorder must still produce 3,7.', 'Verify ordering after every update, not only whether one search succeeded.'),
 11: ('Keep a search tree from becoming a long chain.', 'Rotations preserve key order while reducing height imbalance. A standard ordered container avoids hand-maintained balancing.', 'Ordering and stored heights agree after each rotation; balance stays within the stated bound.', 'Updating heights before reconnecting children leaves stale metadata. Sorted insertion into an unbalanced tree can produce linear height.', 'A right rotation makes the old left child the root and preserves the middle subtree.', 'Test all rotation shapes and verify both values and structure.'),
 12: ('Choose the next most important item efficiently.', 'A heap keeps the best candidate at the top. Sorting is better when all results are needed once in order.', 'Parent/child priority order holds after insertion and removal.', 'Assuming every position is globally sorted misreads a heap. Mutating a queued priority without repair breaks the top guarantee.', 'Insert 4,9,2 into a max-heap; top is 9, then 4 after removing 9.', 'State tie rules when equal-priority tasks must have a stable service order.'),
 13: ('Represent a network with clear edge meaning.', 'Adjacency lists store neighbors; a matrix stores an entry for each possible pair. A matrix can be useful for dense graphs.', 'Vertex indices are valid; edge direction and duplicate rules match the model.', 'Adding only one half of an intended undirected edge makes reachability asymmetric.', 'A directed edge 0->1 permits travel from 0 to 1, not automatically back.', 'Validate graph input before traversal and keep vertex identity separate from display labels.'),
 14: ('Find reachable vertices and shortest unweighted routes.', 'Breadth-first search uses arrival layers; depth-first search explores a branch. DFS is not a shortest-edge-path substitute.', 'BFS marks a vertex when enqueued so it enters the queue once.', 'Marking only on removal adds duplicate work. Following one DFS branch does not establish the shortest route.', 'With 0->1,0->2,2->3, BFS distances from 0 are 0,1,1,2.', 'Test disconnected graphs, self-loops, cycles, and the returned path witness.'),
 15: ('Find a value or insertion boundary without off-by-one errors.', 'A half-open binary-search interval shrinks on every step. Linear search is sufficient for small or unsorted data.', '0 <= low <= high <= size; the insertion point remains between the boundaries.', 'Using low=mid on a smaller middle value can stop progress. Returning any duplicate differs from returning the first.', 'For [1,3,3,7] and target 3, the first valid index is 1, not 2.', 'Record sortedness as a precondition and test empty, absent, and repeated targets.'),
 16: ('Order data without losing records or tie behavior.', 'Insertion sort suits small/nearly sorted data; merge sort offers predictable splitting and can preserve ties.', 'The result is sorted and contains exactly the original records; stability is an extra promise.', 'Choosing the right equal item first during merging reverses tie order. A sorted result alone does not prove nothing was lost.', 'For (2,A),(1,B),(2,C), stable order is (1,B),(2,A),(2,C).', 'Check ordering, permutation, and stability independently when the API promises all three.'),
 17: ('Explore choices with a clear stopping rule.', 'Recursion reduces a problem; backtracking restores mutable choice state before the next branch. Iteration may avoid deep call stacks.', 'Each recursive step moves toward a base case; branch-local changes are undone.', 'Forgetting to pop a chosen character leaks one branch into the next. A base case that never becomes reachable exhausts the stack.', 'Length-two binary strings follow 00,01,10,11 when 0 is explored first.', 'Bound depth and input size, and separate search order from correctness.'),
 18: ('Make local choices only when their assumptions are justified.', 'Earliest-finish interval selection works for maximum count; Dijkstra uses nonnegative weights. Other objectives need other proofs.', 'Accepted intervals do not overlap; a shortest-path queue entry is checked against the current distance.', 'Earliest-start selection may keep one long interval instead of several short ones. Negative edges invalidate Dijkstra’s guarantee.', 'A route of costs 1,1,1 beats a direct edge of 8; stale queue entries must be ignored.', 'Write the weight and objective assumptions next to the interface and validation.'),
 19: ('Reuse answers to repeated subproblems.', 'Memoization stores results on demand; a bottom-up table follows dependency order. Direct recursion is simpler for tiny inputs without much overlap.', 'Each stored state means the declared subproblem and reads only valid predecessor states.', 'An ascending one-dimensional capacity loop can reuse one item in a 0/1 knapsack.', 'For coins 1,3,4, target 6 needs two coins (3+3), not greedy 4+1+1.', 'Test the state definition against an independent tiny exhaustive solver.'),
 20: ('Keep algorithm answers correct while studying memory access cost.', 'Contiguous traversal can use caches well. A benchmark explores cost but cannot establish correctness or guarantee a speedup.', 'Alternative traversals compute the same checksum; timing units and workload are clear.', 'One noisy run is not a stable speed claim. Removing a checksum can let optimization discard the work.', 'A 2x2 row-major buffer [1,2,3,4] visits 1,2,3,4 by rows and 1,3,2,4 by columns; both sum to 10.', 'Use repeated measurements, documented hardware/compiler options, and independent result checks.'),
 22: ('Make structure and path guarantees observable.', 'Collision histories, record identities, graph witnesses, and independent reference solvers expose errors that one answer check misses.', 'Every returned witness explains the answer; record identity and key reachability are preserved.', 'Copying the production recurrence into the oracle repeats the same bug. Testing random hashes may miss collision handling.', 'Delete one forced-collision key, then search another key that lies beyond its slot.', 'Prefer small adversarial fixtures plus independent oracles to large opaque random tests.'),
 23: ('Turn graph algorithms into a bounded command-line application.', 'Separate parse, validation, route search, rendering, and the process boundary. Floyd-Warshall is a simpler tiny-graph oracle but computes more than one query needs.', 'Rejected input leaves an accepted request unchanged; every route witness follows real edges and its costs sum to the reported distance.', 'Accepting a numeric prefix allows trailing junk. Writing output is not transactional: a failed stream can retain a prefix.', 'Input 4 4 0 3 with edges 0->1:8,0->2:1,2->1:1,1->3:1 yields distance 3 and path 0,2,1,3.', 'Keep stable exit codes, bounded inputs, deterministic ties, and failure-injection tests.'),
}

def clean(code):
    return '\n'.join(line for line in code.splitlines()
                     if not re.match(r'\s*#include\s*"[^\"]+"', line)) + '\n'

def build_worked(content):
    course = next(c for c in content['courses'] if c['id'] == 'cpp-book-03')
    blocks = {b['id']: b for t in course['topics'] for b in t.get('blocks', [])}
    listings = [e for e in course['series']['listings'] if e['language'] == 'cpp']
    sources = {e['id']: blocks[e['blockId']]['code'] for e in listings}
    recorded = json.loads(BASELINES.read_text()) if BASELINES.exists() else {}
    result = []
    for e in listings:
        id = e['id']
        if id in PARTS: continue
        original = sources[id]
        parts = JOIN.get(id, [])
        uses_header = 'course_test.hpp' in original
        sections, notes = [], []
        if uses_header:
            sections.append('// Shared test support from Book II, B02-L0110.\n' + HEADER.read_text())
            notes.append('The unchanged shared course_test.hpp from Book II B02-L0110 is placed first; the local include is removed. CHECK stays active in optimized builds.')
        for part in parts:
            sections.append('// Book III source part ' + part + '\n' + clean(sources[part]))
        if parts:
            notes.append('The routes.hpp interface and routes.cpp implementation are joined before the program or test body; local include lines are replaced by that source.')
        code = '\n'.join(sections + [clean(original)]) if sections else original
        if id == 'B03-L0012':
            old = '(reinterpret_cast<std::uintptr_t>(&v[i])-reinterpret_cast<std::uintptr_t>(&v[0]))'
            assert old in code
            code = code.replace(old, '(&v[i]-&v[0])')
            notes.append('Display offsets in elements using pointer subtraction rather than implementation-specific integer addresses. The original byte-offset demonstration is preserved unchanged.')
        if id == 'B03-L0029':
            old = "std::cout<<hash_key({7,\"east\"})<<'\\n';"
            assert old in code
            code = code.replace(old, 'std::cout<<std::boolalpha<<(hash_key({7,"east"})==hash_key({7,"east"}))<<\'\\n\';')
            notes.append('The exact numeric std::hash result is library-specific. Display the equal-key hash guarantee instead; original numeric output remains in original.cpp.')
        if id == 'B03-L0083':
            code = code.replace('median_ns=', 'median_nonnegative=').replace('<<times[times.size()/2]', '<<(times[times.size()/2]>=0)')
            notes.append('The timing loop still runs, but its variable median is displayed as a nonnegative-duration check. Use original.cpp to inspect actual nanosecond measurements; this is not a speed assertion.')
        assert 'int main(' in code or 'int main()' in code, id
        cases = [('Original behavior', '')]
        if id == 'B03-L0093':
            cases = [('Cheaper indirect route', '4 4 0 3\n0 1 8\n0 2 1\n2 1 1\n1 3 1\n'),
                     ('Unreachable destination', '2 0 0 1\n'),
                     ('Same source and destination', '1 0 0 0\n'),
                     ('Negative edge rejected', '2 1 0 1\n0 1 -1\n'),
                     ('Trailing field rejected', '1 0 0 0 junk\n'),
                     ('Missing edge field rejected', '2 1 0 1\n0 1\n')]
        baseline = recorded.get(id)
        assert baseline is None or len(baseline) == len(cases), id
        checks = [dict(id='case'+str(i+1), label=label, input=stdin,
                       stdout=baseline[i]['stdout'] if baseline else '',
                       stderr=baseline[i]['stderr'] if baseline else '',
                       exitCode=baseline[i]['exitCode'] if baseline else 0,
                       hint='Compare the first changed output, assertion, or process exit. Preserve the stated invariant before changing the expected result.')
                  for i, (label, stdin) in enumerate(cases)]
        problem, design, invariant, failure, trace, maintenance = GUIDES[e['chapter']]
        guide = dict(problem=problem, design=design, invariant=invariant, failure=failure,
                     trace=trace, maintenance=maintenance,
                     scope='This panel connects this listing to its chapter’s problem. Short demonstrations illustrate one part; the chapter’s complete implementation and invariant labs test the broader contract.')
        chapter_title = next(ch['title'] for ch in course['chapters'] if ch['number'] == e['chapter'])
        x = dict(id='worked-'+id, sourceId=id, sourcePartIds=parts,
                 sourceSupportIds=['B02-L0110'] if uses_header else [],
                 sourceKind='program', courseId=course['id'], chapter=e['chapter'],
                 chapterTitle=chapter_title, title=e['title'], sourceFilename=e['filename'],
                 sourceStatus=(e.get('validation') or {}).get('status') or 'project component',
                 environmentNote='', source=code, originalSource=original,
                 adapted=code != original, adaptationNote=' '.join(notes), guide=guide,
                 experiment='Predict one boundary or tie case, then change this listing and trace the affected state. ' + trace,
                 sampleInput=checks[0]['input'], sampleOutput=checks[0]['stdout'], checks=checks)
        folder = ROOT / 'coding_lab/book_03_worked' / id
        folder.mkdir(parents=True, exist_ok=True)
        for name, text in [('main.cpp', code), ('original.cpp', original)]:
            (folder / name).write_text(text if text.endswith('\n') else text+'\n')
        (folder / 'workshop.json').write_text(json.dumps({k:v for k,v in x.items() if k not in ['source','originalSource']}, indent=2, ensure_ascii=False)+'\n')
        result.append(x)
    assert {i for w in result for i in [w['sourceId'], *w['sourcePartIds']]} == set(sources)
    return result
