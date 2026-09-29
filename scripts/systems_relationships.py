"""Focused relationship views for the examples where straight-line flow is insufficient."""
def add_views(c):
 def code(n,needle,count=1,solution=False):
  lines=c['examples'][str(n)]['solutions' if solution else 'examples']['code'].splitlines()
  start=next(i for i,s in enumerate(lines) if needle in s)
  return [s.strip() for s in lines[start:start+count]]
 def node(id,label,lines,comment):return dict(id=id,label=label,code=['// '+comment]+lines)
 def edge(a,b,kind,label):return {'from':a,'to':b,'kind':kind,'label':label}
 def view(n,title,kind,nodes,edges,explanation):c['diagrams'][str(n)]['extra_overviews'].append(dict(title=title,kind=kind,nodes=nodes,edges=edges,explanation=explanation))
 view(21,'The demonstrated process-state cycle','state',[
  node('ready','Ready',code(21,'State process',solution=True),'Eligible to be dispatched.'),
  node('running','Running',code(21,'assert(transition(process,State::Ready',solution=True),'Dispatch follows Ready.'),
  node('waiting','Waiting',code(21,'assert(transition(process,State::Running',solution=True),'A wait event blocks execution.')],
  [edge('ready','running','flow','dispatch'),edge('running','waiting','flow','block for an event'),edge('waiting','ready','flow','event completes: wake up')],
  'Solid arrows are the three state transitions exercised by systems_labs/ch21/solution.cpp. They are not ownership. This selected cycle omits other scheduler events such as preemption. The helper checks the current source state but trusts its caller to select a permitted edge; the diagram does not claim it enforces a complete scheduler policy.')
 view(26,'Ownership after a private write detaches the child','ownership',[
  node('parent','Parent handle',code(26,'auto parent'),'Owns the original integer.'),
  node('original','Original backing value',code(26,'assert(*parent'),'Parent still reads seven.'),
  node('child','Child handle',code(26,'if (!child.unique())'),'Detach before writing.'),
  node('private','Private backing value',code(26,'*child ='),'Child now reads nine.')],
  [edge('parent','original','shared','shared_ptr owning handle'),edge('child','private','shared','separate shared_ptr owning handle')],
  'Green dashed arrows denote shared_ptr ownership as labeled, not inheritance. This is the state after detachment: each handle reaches a different integer. Before detachment, child copied parent and both reached the original integer. The demonstration is single-threaded; a shared_ptr count is not a general concurrent write lock.')
 view(46,'One-shot publication between two threads','sequence',[
  node('producer','Producer',code(46,'payload = 42'),'Write before publication.'),
  node('ready','Atomic readiness',code(46,'std::atomic<bool> ready'),'Initially false.'),
  node('consumer','Consumer',code(46,'observed = payload'),'Read after acquire succeeds.')],
  [edge('producer','ready','flow','release-store true after writing payload'),edge('ready','consumer','flow','acquire-load observes that true'),edge('producer','consumer','dependency','prior payload write happens-before payload read')],
  'Time runs down the page. The first two arrows are the release/acquire communication; the final arrow states the resulting happens-before relation, not a direct function call or ownership. Unsuccessful polling loads are omitted. Main joins both threads before reading observed.')
 view(49,'The queue owns its synchronization and state','class',[
  node('queue','Queue',code(49,'class Queue',1,True),'push, pop, and close share one contract.'),
  node('storage','Owned item storage',code(49,'std::deque<int> items_',1,True),'Accepted integers in FIFO order.'),
  node('sync','Owned synchronization',code(49,'std::mutex mutex_',2,True),'One mutex and two condition variables.')],
  [edge('queue','storage','ownership','contains 0 to capacity items'),edge('queue','sync','ownership','members live with Queue')],
  'A filled diamond at Queue marks composition: these members belong to the Queue object. There is no inheritance. A consumer can still pop accepted items after close; only a closed and empty queue ends the stream. Code is from systems_labs/ch49/solution.cpp.')
 view(61,'Capstone: work ownership and result storage','ownership',[
  node('pipeline','pipeline function',code(61,'WorkQueue queue(2)',2,True),'Build shared state before workers start.'),
  node('queue','WorkQueue',code(61,'std::deque<Work> work_',1,True),'Own queued Work objects.'),
  node('work','Work',code(61,'struct Work',1,True),'Each job owns its byte string.'),
  node('results','Indexed result slots',code(61,'std::vector<std::optional<Result>> results',1,True),'One writer per preallocated slot.')],
  [edge('pipeline','queue','dependency','constructs, pushes, closes'),edge('queue','work','ownership','0 to 2 queued jobs'),edge('pipeline','results','dependency','allocates before launch; returns after join'),edge('work','results','dependency','worker writes result at work.index')],
  'The filled diamond marks owned queued work. Dashed arrows mark use or data dependency, not inheritance or an execution schedule. Workers borrow the queue and result vector until their futures are collected. Completed results keep input order because each job receives one unique index. Source: systems_labs/ch61/solution.cpp.')
 def focus(n,title,needle,explanations,what,where,why,invariant,risk):
  ex=c['examples'][str(n)]['solutions'];lines=ex['code'].splitlines();start=next(i for i,s in enumerate(lines) if needle in s)
  c['diagrams'][str(n)]['focus'].append(dict(title=title,file=ex['filename'],lineStart=start+1,lineEnd=start+len(explanations),code='\n'.join(lines[start:start+len(explanations)]),explain=explanations,what=what,where=where,why=why,invariant=invariant,risk=risk))
 focus(49,'Wait, drain, then end','readable_.wait',[
  'wait temporarily releases the unique_lock while sleeping, then reacquires it. The lambda [&] borrows the queue state. It returns true if closure or an available item permits progress; the predicate is rechecked after wakeups.',
  'The empty check occurs while holding the mutex. Only emptiness now returns nullopt. Closed but nonempty must continue so accepted items can drain.',
  'Copy the oldest integer before removing it. Both operations happen within the same critical section.',
  'Notify a blocked producer that capacity may be available. The producer must still reacquire the mutex and recheck its own predicate.',
  'Return the removed value by value. The caller owns its copy after this function releases the lock.'
 ],'This block is the blocking consumer operation.','Inside Queue::pop, after acquiring the queue mutex.','The persistent state decides whether to continue; a notification is only a reason to recheck that state.','Every items_ and closed_ access uses the queue mutex; accepted buffered items drain after close.','Returning immediately on closed_ loses buffered work. Waiting without the predicate can stall after an early notification.')
 focus(61,'Publish owned work into the bounded queue','writable_.wait',[
  'The producer sleeps until closure or space. wait releases and reacquires the same mutex, and tests this predicate after each wakeup.',
  'Closure rejects new work before changing the container.',
  'Move the owned Work, including its string, into the queue. The capacity check and insertion occur under the same lock.',
  'Wake one consumer to check for available work.',
  'Report acceptance only after insertion completes. The caller’s shutdown path handles exceptions from allocation.'
 ],'push enforces admission and transfers ownership.','Inside WorkQueue::push in the capstone reference solution.','A count-only check outside the lock would let multiple producers overfill the bound. Moving owned strings prevents queued dangling views.','Queued size never exceeds capacity; closure never accepts new work.','A reference to a temporary input string could outlive the source. A separate check and insertion would race.')
 focus(61,'Collect workers before shared objects die','for(auto& future : workers) future.get();',[
  'On the normal path, get waits for each worker and propagates its exception. The queue was closed immediately above this line.',
  'catch handles any exception from launching, submitting work, or collecting a worker.',
  'Close the queue on failure so workers blocked in pop or push can make progress toward exit.',
  'A future already consumed by get is invalid. Only remaining valid futures need collection.',
  'Collect each remaining worker. Cleanup exceptions are suppressed so the original failure remains the one reported.',
  'The cleanup loop ends only after each still-valid future was handled.',
  'Rethrow the original exception after shared-state users have finished.'
 ],'This is the completion and failure boundary of the pipeline.','At the end of pipeline, after submission and before returning its result vector.','Workers borrow local queue and result state. reserve(2) is performed before launch so storing a new future cannot fail by growing the vector after a worker has begun waiting.','Every started worker finishes before queue and results are destroyed. Results are read only after all workers complete.','Destroying borrowed state too early causes dangling references; forgetting close on a failure can leave a future waiting forever.')
