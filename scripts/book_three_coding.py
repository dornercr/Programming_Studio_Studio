"""Independent C++20 exercises for the remaining Book III chapters and labs."""

def add_book_three(q):
    def Q(ch, slug, title, level, requirement, signature, starter, solution,
          driver, sample, cases, hints, theory, includes):
        q('cpp-book-03', 'b3-'+slug, title, level, requirement, signature,
          starter, solution, driver, sample, cases, hints, theory,
          chapter=ch, includes=includes)

    Q(1,'first-maximum','Keep the first maximum, not the last','Debug',
      'Return the index of the first largest integer, or std::nullopt for empty input. The vector is borrowed and must not change. Equal later values must not replace the first answer.',
      'std::optional<std::size_t> firstMaximum(const std::vector<int>& values)',
      'std::optional<std::size_t> firstMaximum(const std::vector<int>& v){if(v.empty())return std::nullopt;std::size_t best=0;for(std::size_t i=1;i<v.size();++i)if(v[i]>=v[best])best=i;return best;}',
      '''std::optional<std::size_t> firstMaximum(const std::vector<int>& v){
    if(v.empty()) return std::nullopt;
    std::size_t best=0;
    for(std::size_t i=1;i<v.size();++i)
        if(v[i]>v[best]) best=i; // Equal later items do not replace the first.
    return best;
}''',
      'int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);auto p=firstMaximum(v);if(p)std::cout<<*p<<"\\n";else std::cout<<"empty\\n";}',
      ('3 9 9 2\n','1\n'),
      [('First tie','firstMaximum({3,9,9,2})==1','true','Use >, not >=.'),('All equal','firstMaximum({7,7,7})==0','true','The first equal maximum is index zero.'),('All negative','firstMaximum({-8,-2,-5})==1','true','Initialize from a real item.'),('Empty','!firstMaximum({})','true','No index names an element.'),('One item','firstMaximum({-4})==0','true','The only item is first and largest.')],
      ['Trace best through two equal maxima.','optional distinguishes no result from a valid index zero.'],
      'After each iteration, best names the first largest item seen so far. The starter outputs 2 for the sample instead of 1 because >= replaces an equal answer. std::max_element is a shorter alternative with the same first-maximum rule; this loop makes the invariant visible. Keep the tie rule in tests when changing the search.',
      ['iostream','vector','optional','cstddef'])

    Q(2,'pair-count','Count unordered pairs without running a quadratic loop','Guided',
      'Return how many pairs of distinct positions exist in n items when order does not matter. Valid n is 0..1000000; reject larger n with std::invalid_argument. Return std::uint64_t. Do not enumerate pairs; tests check behavior, so also inspect complexity.',
      'std::uint64_t pairCount(std::uint64_t n)',
      'std::uint64_t pairCount(std::uint64_t n){if(n>1000000)throw std::invalid_argument("too many items");return n*n;}',
      'std::uint64_t pairCount(std::uint64_t n){if(n>1000000)throw std::invalid_argument("too many items");if(n<2)return 0;return n*(n-1)/2;}',
      'int main(){std::uint64_t n;if(!(std::cin>>n))return 2;try{std::cout<<pairCount(n)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"too many items\\n";}}',
      ('4\n','6\n'),
      [('Four items','pairCount(4)','6','Each pair must appear only once.'),('Zero','pairCount(0)','0','Avoid unsigned n-1 at zero.'),('Singleton','pairCount(1)','0','There is no other position.'),('Two','pairCount(2)','1','One pair, not two ordered pairs.'),('Maximum accepted','pairCount(1000000)','499999500000','Use a wide result.'),('Reject outside contract','([]{try{pairCount(1000001);return false;}catch(const std::invalid_argument&){return true;}})()','true','Check the input limit first.')],
      ['There are n*(n-1) ordered choices; divide by two.','Handle n<2 before subtracting one from an unsigned number.'],
      'A nested loop with j=i+1 counts the same pairs but does quadratic work. The formula does fixed arithmetic. The stated bound keeps multiplication inside uint64_t; a wider unrestricted input needs an overflow check. The starter outputs 16 for n=4 instead of 6 because it includes self-pairs and order. Preserve the input bound if this formula enters production code.',
      ['iostream','cstdint','stdexcept'])

    Q(3,'erase-index','Erase one array position and preserve order','Implementation',
      'Erase index from a vector<int>, shift later values left, and return true. If index is outside the live range, return false without changing the vector. Do not erase every occurrence of that value.',
      'bool eraseIndex(std::vector<int>& values, std::size_t index)',
      'bool eraseIndex(std::vector<int>& v,std::size_t i){if(i>=v.size())return false;v.pop_back();return true;}',
      '''bool eraseIndex(std::vector<int>& v,std::size_t i){
    if(i>=v.size()) return false;
    for(std::size_t j=i+1;j<v.size();++j) v[j-1]=v[j];
    v.pop_back();
    return true;
}''',
      'int main(){std::size_t i;if(!(std::cin>>i))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);std::cout<<std::boolalpha<<eraseIndex(v,i)<<"\\n";for(int n:v)std::cout<<n<<" ";std::cout<<"\\n";}',
      ('1 7 8 9\n','true\n7 9 \n'),
      [('Middle shift','([]{std::vector<int> v{7,8,9};return eraseIndex(v,1)&&v==std::vector<int>({7,9});})()','true','Move the suffix left.'),('First','([]{std::vector<int> v{1,2};eraseIndex(v,0);return v==std::vector<int>({2});})()','true','The first removal shifts all later items.'),('Last','([]{std::vector<int> v{1,2};return eraseIndex(v,1)&&v==std::vector<int>({1});})()','true','No shifting is needed at the last index.'),('Rejected is unchanged','([]{std::vector<int> v{1};return !eraseIndex(v,1)&&v==std::vector<int>({1});})()','true','Validate before editing.'),('Empty','([]{std::vector<int> v;return !eraseIndex(v,0)&&v.empty();})()','true','Index zero is invalid for an empty vector.')],
      ['The loop begins one item after the removed position.','Only shorten the vector after shifting.'],
      'The invariant is that the kept prefix stays in its old order and each suffix item moves left once. vector::erase is the simpler real-world alternative. Swapping with the last item is faster but changes order. The starter removes 9 rather than 8 in the sample and outputs 7 8. State whether order matters before choosing a faster removal.',
      ['iostream','vector','cstddef'])

    Q(4,'reverse-chain','Reverse an owned linked chain without copying nodes','Ownership',
      'Node owns its successor through unique_ptr. Reverse an acyclic chain and return its new owning head. Empty input is valid. Do not allocate replacement nodes; tests check values and node identity. Use at most 100 nodes in this exercise.',
      'struct Node; std::unique_ptr<Node> reverseChain(std::unique_ptr<Node> head)',
      '''struct Node{int value;std::unique_ptr<Node> next;explicit Node(int v):value(v){}};
std::unique_ptr<Node> reverseChain(std::unique_ptr<Node> head){return head;}''',
      '''struct Node{int value;std::unique_ptr<Node> next;explicit Node(int v):value(v){}};
std::unique_ptr<Node> reverseChain(std::unique_ptr<Node> head){
    std::unique_ptr<Node> reversed;
    while(head){
        auto rest=std::move(head->next); // Save ownership of the unvisited suffix.
        head->next=std::move(reversed);
        reversed=std::move(head);
        head=std::move(rest);
    }
    return reversed;
}''',
      'int main(){auto a=std::make_unique<Node>(1);a->next=std::make_unique<Node>(2);a->next->next=std::make_unique<Node>(3);auto r=reverseChain(std::move(a));for(auto p=r.get();p;p=p->next.get())std::cout<<p->value<<" ";std::cout<<"\\n";}',
      ('','3 2 1 \n'),
      [('Reverse two','([]{auto a=std::make_unique<Node>(1);a->next=std::make_unique<Node>(2);auto r=reverseChain(std::move(a));return !a&&r->value==2&&r->next->value==1&&!r->next->next;})()','true','Move each owning link.'),('Same node identity','([]{auto a=std::make_unique<Node>(7);a->next=std::make_unique<Node>(8);auto first=a.get();auto last=a->next.get();auto r=reverseChain(std::move(a));return r.get()==last&&r->next.get()==first;})()','true','Reuse nodes rather than copying values.'),('Empty','!reverseChain(nullptr)','true','The loop need not run.'),('Singleton','([]{auto a=std::make_unique<Node>(4);auto p=a.get();auto r=reverseChain(std::move(a));return r.get()==p&&!r->next;})()','true','A one-node chain keeps its identity.')],
      ['Save head->next before changing that link.','Moving a unique_ptr transfers responsibility for releasing its node.'],
      'Each iteration moves one node from the unvisited suffix to the reversed prefix. Every node still has one owner. The starter prints 1 2 3 instead of 3 2 1. A vector plus std::reverse is simpler when linked-node identity is not required. A raw-pointer version can work, but must state who deletes each node; unique ownership makes that obligation visible.',
      ['iostream','memory','utility'])

    Q(6,'ring-queue','Reuse slots in a bounded circular FIFO queue','Debug',
      'RingQueue stores up to its capacity integers, removes in arrival order, rejects full pushes without change, and returns nullopt when empty. Zero capacity is allowed. Reuse released slots. Single-threaded; capacity <=100.',
      'class RingQueue: RingQueue(size_t), bool push(int), optional<int> pop(), size() const',
      '''class RingQueue{std::vector<int> data_;std::size_t head_=0,count_=0;public:explicit RingQueue(std::size_t n):data_(n){}bool push(int v){if(count_==data_.size())return false;data_[(head_+count_)%data_.size()]=v;++count_;return true;}std::optional<int> pop(){if(!count_)return std::nullopt;int v=data_[head_];--count_;return v;}std::size_t size()const{return count_;}};''',
      '''class RingQueue{
    std::vector<int> data_;std::size_t head_=0,count_=0;
public:
    explicit RingQueue(std::size_t n):data_(n){}
    bool push(int v){
        if(count_==data_.size()) return false;
        data_[(head_+count_)%data_.size()]=v;++count_;return true;
    }
    std::optional<int> pop(){
        if(!count_) return std::nullopt;
        int v=data_[head_];head_=(head_+1)%data_.size();--count_;return v;
    }
    std::size_t size()const{return count_;}
};''',
      'int main(){RingQueue q(2);q.push(7);q.push(9);std::cout<<*q.pop()<<" ";q.push(11);std::cout<<*q.pop()<<" "<<*q.pop()<<"\\n";}',
      ('','7 9 11\n'),
      [('FIFO','([]{RingQueue q(2);q.push(7);q.push(9);return q.pop()==7&&q.pop()==9&&!q.pop();})()','true','Advance head after removal.'),('Wraparound','([]{RingQueue q(2);q.push(7);q.push(9);q.pop();q.push(11);return q.pop()==9&&q.pop()==11;})()','true','The released slot becomes the next tail slot.'),('Full unchanged','([]{RingQueue q(1);return q.push(3)&&!q.push(4)&&q.size()==1&&q.pop()==3;})()','true','Reject before overwriting.'),('Zero capacity','([]{RingQueue q(0);return !q.push(1)&&!q.pop()&&q.size()==0;})()','true','Avoid modulo zero.'),('Repeated reuse','([]{RingQueue q(1);for(int i=0;i<10;++i)if(!q.push(i)||q.pop()!=i)return false;return true;})()','true','Head and count must remain valid after each cycle.')],
      ['The live elements begin at head_ and continue count_ slots around the ring.','Only take a remainder after ruling out an empty/full zero-capacity queue.'],
      'count_ stays between zero and capacity. The index calculation wraps storage, not logical arrival order. The starter forgets to advance head_ and prints 7 7 7 for the sample. A deque is simpler for an unbounded FIFO; a ring gives a fixed allocation. This exercise is not thread-safe: synchronization and blocking behavior need separate design.',
      ['iostream','vector','optional','cstddef'])

    Q(8,'ordered-frequency','Keep an ordered frequency table','Implementation',
      'Return a map from each integer to its occurrence count. Empty input gives an empty map. Negative and zero keys are allowed. Iterating the map must visit keys in increasing order. Counts use size_t.',
      'std::map<int,std::size_t> frequencies(const std::vector<int>& values)',
      'std::map<int,std::size_t> frequencies(const std::vector<int>& v){std::map<int,std::size_t> m;for(int x:v)m[x]=1;return m;}',
      'std::map<int,std::size_t> frequencies(const std::vector<int>& v){std::map<int,std::size_t> m;for(int x:v)++m[x];return m;}',
      'int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(auto [key,n]:frequencies(v))std::cout<<key<<":"<<n<<"\\n";}',
      ('3 1 3 -2\n','-2:1\n1:1\n3:2\n'),
      [('Duplicates counted','frequencies({3,1,3}).at(3)','2','Increment rather than assign one.'),('Empty','frequencies({}).empty()','true','No keys are inserted.'),('Sorted first key','frequencies({5,-1,0}).begin()->first','-1','Use an ordered map.'),('Count total','([]{auto m=frequencies({2,2,0,-1});std::size_t n=0;for(auto [k,c]:m)n+=c;return n;})()','4','Each input adds one occurrence.'),('Zero key','frequencies({0,0}).at(0)','2','Zero is a normal key.')],
      ['operator[] creates a missing count initialized to zero.','The colon in the range loop means for each entry in the map.'],
      'The table describes the visited input prefix; its counts sum to that prefix length. An unordered_map can make lookup cheaper on average, but cannot promise sorted traversal. The starter assigns 1 repeatedly, so the sample prints 3:1 rather than 3:2. Keep order and count type in the public contract.',
      ['iostream','vector','map','cstddef'])

    Q(9,'tree-height','Measure tree height with an explicit empty case','Tracing',
      'Return height in nodes: an empty tree has height 0, a leaf 1. Node uniquely owns left and right children. height only borrows a const Node pointer. Input is an acyclic tree with at most 100 levels.',
      'struct Node; std::size_t height(const Node* root)',
      'struct Node{std::unique_ptr<Node> left,right;};std::size_t height(const Node* p){return p?1:0;}',
      '''struct Node{std::unique_ptr<Node> left,right;};
std::size_t height(const Node* p){
    if(!p) return 0;
    return 1+std::max(height(p->left.get()),height(p->right.get()));
}''',
      'int main(){Node root;root.left=std::make_unique<Node>();root.left->right=std::make_unique<Node>();std::cout<<height(&root)<<"\\n";}',
      ('','3\n'),
      [('Empty','height(nullptr)','0','Stop recursion at null.'),('Leaf','([]{Node n;return height(&n);})()','1','Node-count height includes the root.'),('Left chain','([]{Node n;n.left=std::make_unique<Node>();n.left->left=std::make_unique<Node>();return height(&n);})()','3','Use the deeper subtree.'),('Right branch','([]{Node n;n.right=std::make_unique<Node>();return height(&n);})()','2','Inspect both sides.'),('Wide is not tall','([]{Node n;n.left=std::make_unique<Node>();n.right=std::make_unique<Node>();return height(&n);})()','2','Take maximum depth, not number of nodes.')],
      ['Ask each child for its height.','The parent adds one to the larger child result.'],
      'The null base case ends recursion; each result becomes the answer for one subtree. const prevents changes through the borrowed pointer, while unique_ptr keeps ownership in the tree. The starter returns 1 for every nonempty tree, including the sample of height 3. An iterative breadth-first count of levels is better when extreme depth makes recursion unsafe.',
      ['iostream','memory','algorithm','cstddef'])

    Q(10,'bst-insert','Insert a unique key into a search tree','Implementation',
      'insert owns a root reference so it can create the root or a child. Insert an int only once; return true if new and false if duplicate. Keys smaller than a node go left, larger go right. Exercise trees contain at most 100 nodes.',
      'struct Node; bool insert(std::unique_ptr<Node>& root,int key)',
      '''struct Node{int key;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
bool insert(std::unique_ptr<Node>& root,int k){if(!root){root=std::make_unique<Node>(k);return true;}return false;}''',
      '''struct Node{int key;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
bool insert(std::unique_ptr<Node>& root,int k){
    if(!root){root=std::make_unique<Node>(k);return true;}
    if(k<root->key) return insert(root->left,k);
    if(k>root->key) return insert(root->right,k);
    return false;
}''',
      'void print(const Node* p){if(!p)return;print(p->left.get());std::cout<<p->key<<" ";print(p->right.get());}int main(){std::unique_ptr<Node> root;int x;while(std::cin>>x)insert(root,x);print(root.get());std::cout<<"\\n";}',
      ('5 3 7 3\n','3 5 7 \n'),
      [('Create root','([]{std::unique_ptr<Node> p;return insert(p,5)&&p->key==5;})()','true','Create when the owner is empty.'),('Left child','([]{auto p=std::make_unique<Node>(5);return insert(p,3)&&p->left&&p->left->key==3;})()','true','Follow the smaller-key link.'),('Right child','([]{auto p=std::make_unique<Node>(5);return insert(p,7)&&p->right&&p->right->key==7;})()','true','Follow the larger-key link.'),('Reject duplicate','([]{auto p=std::make_unique<Node>(5);return !insert(p,5)&&!p->left&&!p->right;})()','true','Equal keys do not create nodes.'),('Deep descendant','([]{std::unique_ptr<Node> p;insert(p,5);insert(p,3);insert(p,4);return p&&p->left&&p->left->right&&p->left->right->key==4;})()','true','Continue into the matching subtree.')],
      ['A reference to unique_ptr lets the recursive call replace that owning link.','Handle equality separately.'],
      'Each call preserves ordering by editing only the subtree selected by comparison. The starter stores only the root and prints 5 for the sample instead of 3 5 7. An unbalanced tree can become a chain; std::set is a better default when ordered unique data is needed without implementing a tree. Production updates also need deletion, exception, and depth policies.',
      ['iostream','memory'])

    Q(11,'right-rotation','Rotate a tree and repair its stored heights','Ownership',
      'Rotate a nonempty subtree right when it has a left child. Return the new owning root. Preserve keys and the middle subtree, and recompute heights (empty=0, leaf=1). Reject missing root/left with invalid_argument. Input subtree heights are initially correct.',
      'struct Node; std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> root)',
      '''struct Node{int key,height=1;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
int h(const std::unique_ptr<Node>& p){return p?p->height:0;}
void update(Node& p){p.height=1+std::max(h(p.left),h(p.right));}
std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> root){if(!root||!root->left)throw std::invalid_argument("rotation");return root;}''',
      '''struct Node{int key,height=1;std::unique_ptr<Node> left,right;explicit Node(int k):key(k){}};
int h(const std::unique_ptr<Node>& p){return p?p->height:0;}
void update(Node& p){p.height=1+std::max(h(p.left),h(p.right));}
std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> root){
    if(!root||!root->left) throw std::invalid_argument("rotation");
    auto pivot=std::move(root->left);
    root->left=std::move(pivot->right); // Keep the middle subtree.
    update(*root);
    pivot->right=std::move(root);
    update(*pivot);
    return pivot;
}''',
      'int main(){auto p=std::make_unique<Node>(3);p->left=std::make_unique<Node>(2);p->left->left=std::make_unique<Node>(1);update(*p->left);update(*p);auto r=rotateRight(std::move(p));std::cout<<r->key<<" "<<r->height<<"\\n";}',
      ('','2 2\n'),
      [('New root','([]{auto p=std::make_unique<Node>(3);p->left=std::make_unique<Node>(2);update(*p);auto r=rotateRight(std::move(p));return r&&r->key==2&&r->right&&r->right->key==3;})()','true','The old left child becomes the root.'),('Middle survives','([]{auto p=std::make_unique<Node>(8);p->left=std::make_unique<Node>(4);p->left->right=std::make_unique<Node>(6);update(*p->left);update(*p);auto r=rotateRight(std::move(p));return r&&r->right&&r->right->left&&r->right->left->key==6;})()','true','Move pivot->right into old root->left.'),('Heights repaired','([]{auto p=std::make_unique<Node>(3);p->left=std::make_unique<Node>(2);p->left->left=std::make_unique<Node>(1);update(*p->left);update(*p);auto r=rotateRight(std::move(p));return r&&r->height==2&&r->right&&r->right->height==1;})()','true','Update the lower old root first.'),('Missing root rejected','([]{try{rotateRight(nullptr);return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate the required shape.'),('No left child rejected','([]{try{rotateRight(std::make_unique<Node>(1));return false;}catch(const std::invalid_argument&){return true;}})()','true','A right rotation needs a pivot.')],
      ['Save the owning left link as the pivot.','Reconnect children before updating their parent height.'],
      'The keys still appear in the same inorder sequence, but the subtree root changes. Each move leaves one owner for every node. The starter sample remains rooted at 3 with height 3 instead of 2 with height 2. A full AVL insertion needs balance tests and other rotation shapes; this function alone is not a balanced container. Prefer std::set unless maintaining tree shape is itself the requirement.',
      ['iostream','memory','algorithm','stdexcept','utility'])

    Q(12,'max-heap','Remove the largest queued value','Debug',
      'MaxHeap accepts integers including duplicates. pop returns and removes the largest value, or nullopt when empty. Maintain the heap after every change. The exercise is single-threaded.',
      'class MaxHeap: void push(int), std::optional<int> pop()',
      'class MaxHeap{std::vector<int> v_;public:void push(int x){v_.push_back(x);std::push_heap(v_.begin(),v_.end());}std::optional<int> pop(){if(v_.empty())return std::nullopt;int x=v_.back();v_.pop_back();return x;}};',
      '''class MaxHeap{std::vector<int> v_;
public:
    void push(int x){v_.push_back(x);std::push_heap(v_.begin(),v_.end());}
    std::optional<int> pop(){
        if(v_.empty()) return std::nullopt;
        std::pop_heap(v_.begin(),v_.end());
        int x=v_.back();v_.pop_back();return x;
    }
};''',
      'int main(){MaxHeap q;int x;while(std::cin>>x)q.push(x);while(auto v=q.pop())std::cout<<*v<<" ";std::cout<<"\\n";}',
      ('4 9 2\n','9 4 2 \n'),
      [('Largest first','([]{MaxHeap q;q.push(4);q.push(9);q.push(2);return q.pop()==9&&q.pop()==4&&q.pop()==2&&!q.pop();})()','true','pop_heap moves the largest item to the end.'),('Duplicates kept','([]{MaxHeap q;q.push(5);q.push(5);return q.pop()==5&&q.pop()==5&&!q.pop();})()','true','Equal values are separate entries.'),('Negative','([]{MaxHeap q;q.push(-8);q.push(-1);return q.pop()==-1;})()','true','Largest may still be negative.'),('Empty','([]{MaxHeap q;return !q.pop();})()','true','Do not read an empty vector.'),('Reuse','([]{MaxHeap q;q.push(2);q.pop();q.push(7);return q.pop()==7;})()','true','An emptied heap can receive new work.')],
      ['A heap is not a completely sorted vector.','pop_heap rearranges but does not reduce size; pop_back does that.'],
      'The best value is at the front. pop_heap moves it to the back while repairing the remaining prefix. The starter sample removes the unsorted tail and produces 2 4 9, not 9 4 2. std::priority_queue is a simpler complete abstraction; direct heap algorithms are useful when existing vector storage must be reused. Test the full removal sequence, not just one top value.',
      ['iostream','vector','algorithm','optional'])

    Q(13,'adjacency','Build a validated directed adjacency list','Implementation',
      'Given n<=100 and directed (from,to) pairs, return n neighbor vectors in insertion order. Keep duplicate edges. Reject n>100 or any endpoint outside [0,n) with invalid_argument. n=0 is valid only without edges.',
      'std::vector<std::vector<std::size_t>> adjacency(size_t n,const vector<pair<size_t,size_t>>& edges)',
      'std::vector<std::vector<std::size_t>> adjacency(std::size_t n,const std::vector<std::pair<std::size_t,std::size_t>>& edges){if(n>100)throw std::invalid_argument("graph");std::vector<std::vector<std::size_t>> g(n);for(auto [a,b]:edges){if(a>=n||b>=n)throw std::invalid_argument("edge");g[b].push_back(a);}return g;}',
      '''std::vector<std::vector<std::size_t>> adjacency(std::size_t n,const std::vector<std::pair<std::size_t,std::size_t>>& edges){
    if(n>100) throw std::invalid_argument("graph");
    std::vector<std::vector<std::size_t>> g(n);
    for(auto [a,b]:edges){
        if(a>=n||b>=n) throw std::invalid_argument("edge");
        g[a].push_back(b);
    }
    return g;
}''',
      'int main(){auto g=adjacency(3,{{0,1},{0,2},{2,1}});for(std::size_t i=0;i<g.size();++i){std::cout<<i<<":";for(auto j:g[i])std::cout<<j<<" ";std::cout<<"\\n";}}',
      ('','0:1 2 \n1:\n2:1 \n'),
      [('Direction','adjacency(2,{{0,1}})==std::vector<std::vector<std::size_t>>({{1},{}})','true','Append to the source list.'),('Duplicates','adjacency(2,{{0,1},{0,1}})[0]==std::vector<std::size_t>({1,1})','true','Do not silently deduplicate.'),('Empty graph','adjacency(0,{}).empty()','true','Zero vertices need zero lists.'),('Reject endpoint','([]{try{adjacency(2,{{0,2}});return false;}catch(const std::invalid_argument&){return true;}})()','true','Index n is outside the range.'),('Isolated vertex','adjacency(3,{}).size()','3','Keep vertices even with no outgoing edges.')],
      ['The pair names source first, destination second.','Validate before indexing the vector.'],
      'One list owns the outgoing neighbors of one vertex. The starter reverses every arrow: the sample begins with 0: instead of 0:1 2. A matrix makes edge tests direct but uses n*n cells; lists suit sparse networks. The new graph is built locally, so a rejected edge never mutates a caller’s existing graph. Keep directedness explicit in source and diagrams.',
      ['iostream','vector','utility','cstddef','stdexcept'])

    Q(14,'bfs-distance','Find shortest unweighted distances by arrival layers','Algorithm',
      'Return an int distance for each vertex from source, using -1 for unreachable vertices. Directed adjacency lists have <=100 vertices. Reject an invalid source or any invalid neighbor with invalid_argument, even in an unreachable component. Do not mutate the graph.',
      'std::vector<int> distances(const std::vector<std::vector<int>>& graph,int source)',
      'std::vector<int> distances(const std::vector<std::vector<int>>& g,int s){if(s<0||std::size_t(s)>=g.size())throw std::invalid_argument("source");for(auto& row:g)for(int v:row)if(v<0||std::size_t(v)>=g.size())throw std::invalid_argument("edge");std::vector<int> d(g.size(),-1);d[s]=0;return d;}',
      '''std::vector<int> distances(const std::vector<std::vector<int>>& g,int s){
    if(s<0||std::size_t(s)>=g.size()) throw std::invalid_argument("source");
    for(const auto& row:g) for(int v:row)
        if(v<0||std::size_t(v)>=g.size()) throw std::invalid_argument("edge");
    std::vector<int> d(g.size(),-1);std::queue<int> pending;
    d[s]=0;pending.push(s);
    while(!pending.empty()){
        int u=pending.front();pending.pop();
        for(int v:g[u]) if(d[v]==-1){d[v]=d[u]+1;pending.push(v);}
    }
    return d;
}''',
      'int main(){for(int d:distances({{1,2},{3},{3},{}},0))std::cout<<d<<" ";std::cout<<"\\n";}',
      ('','0 1 1 2 \n'),
      [('Layers','distances({{1,2},{3},{3},{}},0)','[0, 1, 1, 2]','A new neighbor is one layer farther.'),('Cycle','distances({{1},{0}},0)','[0, 1]','Mark on enqueue to avoid revisiting.'),('Unreachable','distances({{},{}},1)','[-1, 0]','Keep unreachable as -1.'),('Invalid source','([]{try{distances({{}},1);return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate before setting d[s].'),('Bad unreachable edge','([]{try{distances({{}, {9}},0);return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate the entire graph, not only reached edges.'),('Self loop','distances({{0}},0)','[0]','The source already has a distance.')],
      ['A FIFO queue preserves layers of increasing distance.','Assign the distance before enqueueing.'],
      'The first visit is shortest because every edge adds one step and the queue handles earlier layers first. The starter sample prints 0 -1 -1 -1 because it never explores neighbors. DFS finds reachability but not these shortest distances; weighted edges need a different algorithm. Keep validation separate from traversal so malformed hidden components cannot enter accepted state.',
      ['iostream','vector','queue','cstddef','stdexcept'])

    Q(16,'stable-sort','Preserve record identity while sorting equal keys','Debug',
      'Sort Record values by key in increasing order without changing the relative order of equal keys. Each Record has an int key and a char identity. Use insertion sort; tests check stability, ordering and record preservation, not the chosen algorithm.',
      'struct Record; std::vector<Record> stableSort(std::vector<Record> values)',
      '''struct Record{int key;char identity;bool operator==(const Record&)const=default;};
std::vector<Record> stableSort(std::vector<Record> v){for(std::size_t i=1;i<v.size();++i){auto item=v[i];auto j=i;while(j>0&&v[j-1].key>=item.key){v[j]=v[j-1];--j;}v[j]=item;}return v;}''',
      '''struct Record{int key;char identity;bool operator==(const Record&)const=default;};
std::vector<Record> stableSort(std::vector<Record> v){
    for(std::size_t i=1;i<v.size();++i){
        auto item=v[i];auto j=i;
        while(j>0&&v[j-1].key>item.key){v[j]=v[j-1];--j;}
        v[j]=item;
    }
    return v;
}''',
      'int main(){for(auto x:stableSort({{2,\'A\'},{1,\'B\'},{2,\'C\'}}))std::cout<<x.key<<":"<<x.identity<<" ";std::cout<<"\\n";}',
      ('','1:B 2:A 2:C \n'),
      [('Tie identity','stableSort({{2,\'A\'},{1,\'B\'},{2,\'C\'}})==std::vector<Record>({{1,\'B\'},{2,\'A\'},{2,\'C\'}})','true','Do not shift equal predecessors.'),('All equal','stableSort({{1,\'A\'},{1,\'B\'},{1,\'C\'}})==std::vector<Record>({{1,\'A\'},{1,\'B\'},{1,\'C\'}})','true','Equal records keep arrival order.'),('Empty','stableSort({}).empty()','true','No insertion is needed.'),('Reverse keys','stableSort({{3,\'A\'},{2,\'B\'},{1,\'C\'}})==std::vector<Record>({{1,\'C\'},{2,\'B\'},{3,\'A\'}})','true','Shift larger keys right.'),('Input is copied','([]{std::vector<Record> v{{2,\'A\'},{1,\'B\'}};auto out=stableSort(v);return v[0].key==2&&out[0].key==1;})()','true','The parameter owns a copy.')],
      ['Stability is a promise about identities, not just equal numeric values.','A strict > comparison leaves equal predecessors before the new item.'],
      'The visited prefix is sorted and stable after each insertion. The starter uses >=, so its sample ends with 2:C 2:A instead of 2:A 2:C. std::stable_sort is better for large general inputs; insertion sort is small and useful for short or nearly sorted ranges. Preserve a test with labeled records so a future change cannot silently weaken stability.',
      ['iostream','vector','cstddef'])

    Q(17,'binary-choices','Generate every binary choice without leaking branch state','Guided',
      'Return all binary strings of length n, in lexicographic order. n=0 returns one empty string. Reject n>10 with invalid_argument to bound exponential work. Use a backtracking helper; tests verify values and order.',
      'std::vector<std::string> binaries(std::size_t n)',
      'std::vector<std::string> binaries(std::size_t n){if(n>10)throw std::invalid_argument("too long");return {std::string(n,\'0\')};}',
      '''void visit(std::size_t n,std::string& path,std::vector<std::string>& out){
    if(path.size()==n){out.push_back(path);return;}
    for(char choice:{'0','1'}){
        path.push_back(choice);visit(n,path,out);path.pop_back();
    }
}
std::vector<std::string> binaries(std::size_t n){
    if(n>10) throw std::invalid_argument("too long");
    std::string path;std::vector<std::string> out;visit(n,path,out);return out;
}''',
      'int main(){std::size_t n;if(!(std::cin>>n))return 2;try{for(auto& s:binaries(n))std::cout<<"["<<s<<"]\\n";}catch(const std::invalid_argument&){std::cout<<"too long\\n";}}',
      ('2\n','[00]\n[01]\n[10]\n[11]\n'),
      [('Two positions','binaries(2)==std::vector<std::string>({"00","01","10","11"})','true','Explore zero before one at each level.'),('Empty choice','binaries(0)==std::vector<std::string>({""})','true','There is one way to choose nothing.'),('One position','binaries(1)==std::vector<std::string>({"0","1"})','true','Visit both branches.'),('Bounded count','binaries(10).size()','1024','There are 2^n complete choices.'),('Reject larger','([]{try{binaries(11);return false;}catch(const std::invalid_argument&){return true;}})()','true','Reject before constructing the tree of choices.')],
      ['Undo push_back with pop_back after the recursive call.','A completed path must be copied into the output.'],
      'The path belongs to the current recursive branch and is restored before the next choice. Output strings own copies; they do not borrow the mutable path. The starter prints only [00] for n=2, missing three choices. Bit-mask enumeration is an iterative alternative; backtracking extends naturally to pruning. State the input bound because the number of answers grows exponentially.',
      ['iostream','vector','string','cstddef','stdexcept'])

    Q(18,'intervals','Choose the largest count of nonoverlapping intervals','Design',
      'Return the largest number of compatible intervals (begin,end), where begin<end and touching endpoints are allowed. Reject zero-length or reversed intervals with invalid_argument. Select by earliest finish. At most 100 intervals; do not change the caller’s vector.',
      'std::size_t selectCount(std::vector<std::pair<int,int>> intervals)',
      '''std::size_t selectCount(std::vector<std::pair<int,int>> v){for(auto [a,b]:v)if(a>=b)throw std::invalid_argument("interval");std::sort(v.begin(),v.end());std::optional<int> end;std::size_t n=0;for(auto [a,b]:v)if(!end||a>=*end){++n;end=b;}return n;}''',
      '''std::size_t selectCount(std::vector<std::pair<int,int>> v){
    for(auto [a,b]:v) if(a>=b) throw std::invalid_argument("interval");
    std::sort(v.begin(),v.end(),[](auto a,auto b){return std::tie(a.second,a.first)<std::tie(b.second,b.first);});
    std::optional<int> end;std::size_t n=0;
    for(auto [a,b]:v) if(!end||a>=*end){++n;end=b;}
    return n;
}''',
      'int main(){std::cout<<selectCount({{0,10},{1,2},{2,3},{3,4}})<<"\\n";}',
      ('','3\n'),
      [('Early finish beats early start','selectCount({{0,10},{1,2},{2,3},{3,4}})','3','Do not let a long first-starting interval block three short ones.'),('Touch allowed','selectCount({{0,1},{1,2}})','2','begin==old end is compatible.'),('All overlap','selectCount({{1,4},{2,5},{3,6}})','1','At most one can be accepted.'),('Empty','selectCount({})','0','No intervals means zero.'),('Negative times','selectCount({{-4,-3},{-3,-2}})','2','Do not invent a starting end of zero.'),('Invalid interval','([]{try{selectCount({{2,2}});return false;}catch(const std::invalid_argument&){return true;}})()','true','Strict begin<end is required.')],
      ['Compare end first, then begin for a deterministic tie.','An optional prior end handles negative times without a sentinel.'],
      'After each accepted interval, its end leaves as much remaining time as possible. An exchange argument can replace an optimal solution’s first interval by the earliest-finishing one without lowering its count. The starter selects the long interval and prints 1 instead of 3. Weighted rewards need dynamic programming, not this proof. Record the exact objective before reusing a greedy choice.',
      ['iostream','vector','utility','tuple','algorithm','optional','cstddef','stdexcept'])

    Q(19,'minimum-coins','Solve repeated coin subproblems instead of trusting greedy','Algorithm',
      'Return the minimum number of coins making target, or -1 if impossible. Coins may be reused and must all be positive, even if target is zero. Valid target is 0..1000; reject invalid inputs with invalid_argument. Duplicate denominations are allowed.',
      'int minimumCoins(const std::vector<int>& coins,int target)',
      'int minimumCoins(const std::vector<int>& coins,int target){if(target<0||target>1000)throw std::invalid_argument("target");for(int c:coins)if(c<=0)throw std::invalid_argument("coin");return target==0?0:-1;}',
      '''int minimumCoins(const std::vector<int>& coins,int target){
    if(target<0||target>1000) throw std::invalid_argument("target");
    for(int c:coins) if(c<=0) throw std::invalid_argument("coin");
    const int missing=target+1;std::vector<int> dp(target+1,missing);dp[0]=0;
    for(int amount=1;amount<=target;++amount)
        for(int c:coins) if(c<=amount&&dp[amount-c]!=missing)
            dp[amount]=std::min(dp[amount],dp[amount-c]+1);
    return dp[target]==missing?-1:dp[target];
}''',
      'int main(){int target;if(!(std::cin>>target))return 2;std::vector<int> coins;int c;while(std::cin>>c)coins.push_back(c);try{std::cout<<minimumCoins(coins,target)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"invalid input\\n";}}',
      ('6 1 3 4\n','2\n'),
      [('Greedy counterexample','minimumCoins({1,3,4},6)','2','3+3 beats 4+1+1.'),('Unreachable','minimumCoins({4,6},5)','-1','Keep unreachable distinct from a valid count.'),('Zero target','minimumCoins({},0)','0','Use no coins.'),('No denominations','minimumCoins({},7)','-1','A positive amount cannot be made.'),('Duplicate coin','minimumCoins({2,2},4)','2','Duplicates do not change the optimum.'),('Reject zero coin at zero target','([]{try{minimumCoins({0},0);return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate coins before returning a base answer.')],
      ['dp[a] means the best count for exactly amount a.','Only read smaller valid amounts, then add one coin.'],
      'Each positive coin reduces the remaining amount, so increasing amounts follow the dependency order. The sentinel is larger than any possible answer because every coin is at least one. The starter sample outputs -1 instead of 2. Greedy is shorter but fails for denominations 1,3,4; breadth-first search on amounts is a useful independent small-input oracle. Keep the reuse rule separate from 0/1 item problems.',
      ['iostream','vector','algorithm','stdexcept'])

    Q(20,'matrix-layout','Validate a square row-major buffer before summing','Contracts',
      'Return the long long sum of a flat square matrix with side width. width=0 requires empty data. Reject any size mismatch with invalid_argument without multiplying width*width, which may overflow. Tests use at most 10000 values; their sum fits long long.',
      'long long matrixSum(const std::vector<int>& data,std::size_t width)',
      'long long matrixSum(const std::vector<int>& data,std::size_t width){(void)width;long long sum=0;for(int x:data)sum+=x;return sum;}',
      '''long long matrixSum(const std::vector<int>& data,std::size_t width){
    if(width==0){if(!data.empty())throw std::invalid_argument("shape");return 0;}
    if(data.size()/width!=width||data.size()%width!=0) throw std::invalid_argument("shape");
    long long sum=0;
    for(std::size_t row=0;row<width;++row)
        for(std::size_t col=0;col<width;++col) sum+=data[row*width+col];
    return sum;
}''',
      'int main(){std::size_t w;if(!(std::cin>>w))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);try{std::cout<<matrixSum(v,w)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"shape mismatch\\n";}}',
      ('2 1 2 3\n','shape mismatch\n'),
      [('Square','matrixSum({1,2,3,4},2)','10','Visit the row-major offsets.'),('Empty square','matrixSum({},0)','0','Handle zero before division.'),('Missing element','([]{try{matrixSum({1,2,3},2);return false;}catch(const std::invalid_argument&){return true;}})()','true','A partial row is not a square.'),('Huge width','([]{try{matrixSum({},std::numeric_limits<std::size_t>::max());return false;}catch(const std::invalid_argument&){return true;}})()','true','Use division and remainder, not unchecked multiplication.'),('Negative sum','matrixSum({-3},1)','-3','Do not use an unsigned accumulator.'),('Data with zero width','([]{try{matrixSum({1},0);return false;}catch(const std::invalid_argument&){return true;}})()','true','A zero-side matrix contains no elements.')],
      ['After ruling out width==0, divide the actual size by width.','Only index rows after validating the entire shape.'],
      'For a valid square, row*width+col is within the actual buffer. Checking quotient and remainder avoids computing a potentially overflowing square. The starter silently sums malformed input and outputs 6 for the sample instead of shape mismatch. A plain accumulate is simpler if callers already establish shape; this boundary owns validation. Row-major traversal is a layout choice, not a promised measured speedup.',
      ['iostream','vector','cstddef','limits','stdexcept'])

    Q(22,'path-witness','Check a graph route independently of its search','Verification',
      'Return true only when path is a simple directed source-to-target path with exactly distance edges. Validate all vertex and neighbor indices; a malformed graph returns false. Vertices <=100. A singleton path is valid only for equal source and target at distance zero. This verifies a witness, not shortestness.',
      'bool validPath(const vector<vector<int>>& graph,const vector<int>& path,int source,int target,size_t distance)',
      'bool validPath(const std::vector<std::vector<int>>& g,const std::vector<int>& p,int s,int t,std::size_t d){(void)g;(void)d;return !p.empty()&&p.front()==s&&p.back()==t;}',
      '''bool validPath(const std::vector<std::vector<int>>& g,const std::vector<int>& p,int s,int t,std::size_t d){
    auto index=[&](int v){return v>=0&&std::size_t(v)<g.size();};
    if(!index(s)||!index(t)||p.empty()||p.front()!=s||p.back()!=t||p.size()-1!=d) return false;
    for(const auto& row:g) for(int v:row) if(!index(v)) return false;
    std::vector<bool> seen(g.size());
    for(std::size_t i=0;i<p.size();++i){
        int v=p[i];if(!index(v)||seen[v]) return false;seen[v]=true;
        if(i&&std::find(g[p[i-1]].begin(),g[p[i-1]].end(),v)==g[p[i-1]].end()) return false;
    }
    return true;
}''',
      'int main(){std::cout<<std::boolalpha<<validPath({{1},{2},{}},{0,2},0,2,1)<<"\\n";}',
      ('','false\n'),
      [('Real path','validPath({{1},{2},{}},{0,1,2},0,2,2)','true','Check each consecutive directed edge.'),('Invented shortcut','validPath({{1},{2},{}},{0,2},0,2,1)','false','Endpoints alone do not establish an edge.'),('Wrong length','validPath({{1},{}},{0,1},0,1,2)','false','Edge count is path.size()-1.'),('Repeated vertex','validPath({{1},{0,2},{}},{0,1,0,1,2},0,2,4)','false','The witness contract requires a simple path.'),('Same vertex','validPath({{}},{0},0,0,0)','true','A zero-edge route is valid.'),('Invalid hidden neighbor','validPath({{}, {9}},{0},0,0,0)','false','Validate the whole graph.')],
      ['Validate an index before using it in either seen or graph.','A valid witness is evidence of a route, not evidence that no shorter route exists.'],
      'This checker follows actual edges without sharing a BFS queue implementation. It independently checks endpoints, length and no repeated vertices. The starter outputs true for a nonexistent 0->2 shortcut. To prove shortestness, compare distance with an independent search as well; a witness alone cannot do that. Independent checks are useful at API boundaries where a plausible numeric answer can hide a broken path.',
      ['iostream','vector','algorithm','cstddef'])

    Q(23,'transactional-routes','Reject a bad request without losing the last accepted graph','Integration',
      'loadRequest reads whitespace-separated unsigned fields: vertices edges source target, then exactly edges triples from to weight. Bounds: 1..128 vertices, <=4096 edges, endpoints <vertices, weights <=1000000. Digits only: reject signs, suffixes, missing/trailing fields with invalid_argument. Commit only after the entire request validates. Duplicate directed edges are allowed.',
      'struct Request; void loadRequest(std::string_view text,Request& accepted)',
      '''struct Request{std::size_t vertices=1,source=0,target=0;std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> edges;bool operator==(const Request&)const=default;};
void loadRequest(std::string_view text,Request& accepted){(void)text;accepted=Request{};}''',
      '''struct Request{std::size_t vertices=1,source=0,target=0;std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> edges;bool operator==(const Request&)const=default;};
void loadRequest(std::string_view text,Request& accepted){
    std::istringstream input{std::string(text)};
    auto field=[&](std::size_t limit){
        std::string token;if(!(input>>token)) throw std::invalid_argument("missing field");
        std::size_t value=0;
        for(char c:token){
            if(c<'0'||c>'9') throw std::invalid_argument("digits required");
            auto digit=std::size_t(c-'0');
            if(digit>limit||value>(limit-digit)/10) throw std::invalid_argument("field limit");
            value=value*10+digit;
        }
        return value;
    };
    Request candidate;candidate.vertices=field(128);
    if(!candidate.vertices) throw std::invalid_argument("no vertices");
    auto count=field(4096);candidate.source=field(candidate.vertices-1);candidate.target=field(candidate.vertices-1);
    for(std::size_t i=0;i<count;++i){
        auto from=field(candidate.vertices-1);auto to=field(candidate.vertices-1);auto weight=field(1000000);
        candidate.edges.emplace_back(from,to,weight);
    }
    std::string extra;if(input>>extra) throw std::invalid_argument("trailing field");
    static_assert(std::is_nothrow_move_assignable_v<Request>);
    accepted=std::move(candidate);
}''',
      'int main(){std::string text{std::istreambuf_iterator<char>(std::cin),{}};Request r;try{loadRequest(text,r);std::cout<<r.vertices<<" "<<r.source<<" "<<r.target<<" "<<r.edges.size()<<"\\n";}catch(const std::invalid_argument&){std::cout<<"invalid request\\n";}}',
      ('3 2 0 2\n0 1 5\n1 2 4\n','3 0 2 2\n'),
      [('Complete request','([]{Request r;loadRequest("3 2 0 2 0 1 5 1 2 4",r);return r.vertices==3&&r.source==0&&r.target==2&&r.edges.size()==2;})()','true','Read all triples before committing.'),('Failure preserves accepted','([]{Request r;r.vertices=2;r.target=1;auto old=r;try{loadRequest("3 1 0 2 0 1",r);return false;}catch(const std::invalid_argument&){return r==old;}})()','true','Parse into a separate candidate.'),('Trailing junk','([]{Request r;try{loadRequest("1 0 0 0 x",r);return false;}catch(const std::invalid_argument&){return true;}})()','true','Reject extra tokens.'),('Signed field','([]{Request r;try{loadRequest("2 1 0 1 0 1 -1",r);return false;}catch(const std::invalid_argument&){return true;}})()','true','Digits only, not a signed parser.'),('Out-of-range vertex','([]{Request r;try{loadRequest("2 1 0 1 0 2 4",r);return false;}catch(const std::invalid_argument&){return true;}})()','true','Index vertices is invalid.'),('Duplicate edges allowed','([]{Request r;loadRequest("2 2 0 1 0 1 5 0 1 5",r);return r.edges.size();})()','2','Do not drop an input edge.'),('Weight limit','([]{Request r;try{loadRequest("2 1 0 1 0 1 1000001",r);return false;}catch(const std::invalid_argument&){return true;}})()','true','Check before accumulating beyond the bound.')],
      ['A candidate object protects the old accepted state on parsing failure.','The digit>limit check must happen before limit-digit, because size_t is unsigned.'],
      'Parsing, validation, and commit are separate phases. A bounded digit loop rejects a numeric prefix such as 5x and prevents overflow. The final move is nonthrowing, so rejection leaves the old request unchanged. The starter overwrites accepted immediately and reports 1 0 0 0 for the sample instead of 3 0 2 2. Streaming directly into accepted is shorter but breaks rollback. The full HarborRoutes workshop adds stream failures, route search, witness checks, and output handling; external output cannot be rolled back merely by preserving in-memory state.',
      ['iostream','vector','tuple','string','string_view','sstream','iterator','utility','cstddef','stdexcept','type_traits'])
