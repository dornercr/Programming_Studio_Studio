#!/usr/bin/env python3
"""Original, executable coding questions for all ten Study Studio books."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
questions=[]
def q(course,id,title,level,prompt,signature,starter,solution,driver,sample,tests,hints,explanation,chapter=1,includes=None):
 headers=includes or ['iostream','vector','string','string_view','array','cstdint','cstddef','algorithm','stdexcept','limits','functional','memory','optional','deque','unordered_set','future','numeric','utility']
 pre=''.join('#include <'+h+'>\n' for h in headers)+'\n'
 questions.append(dict(id=id,courseId=course,chapter=chapter,title=title,level=level,prompt=prompt,signature=signature,starter=pre+starter.strip()+'\n',solution=pre+solution.strip()+'\n',driver=driver.strip()+'\n',sampleInput=sample[0],sampleOutput=sample[1],tests=[dict(id='case'+str(i+1),label=t[0],expression=t[1],expected=t[2],hint=t[3]) for i,t in enumerate(tests)],hints=hints,explanation=explanation))
# Fundamentals: every challenge states its input domain and preserves the supplied main driver.
q('cpp-book-01','b1-add','Write a function that adds two values','Guided',
 'Write add(a, b). Return their sum. Inputs and their sum are in the int range. Return the value; the supplied driver prints it.',
 'int add(int a, int b)', 'int add(int a, int b) {\n    // TODO: return the sum.\n    return 0;\n}', 'int add(int a, int b) { return a + b; }',
 'int main(){ int a,b; if(!(std::cin>>a>>b)) return 2; std::cout<<add(a,b)<<"\\n"; }',('7 5\n','12\n'),
 [('Two positive values','add(7,5)','12','Use both parameters in the returned expression.'),('Negative and positive','add(-8,3)','-5','Addition must keep the sign of the result.'),('Both negative','add(-4,-6)','-10','Do not replace addition with an absolute value.'),('Zero','add(0,0)','0','Zero is a valid input.')],
 ['The + operator adds two values.','A return statement sends a value back to the caller.'],
 'return a + b computes and returns one int. The driver owns input and output, so the function can be tested without reading a terminal.',includes=['iostream'])
q('cpp-book-01','b1-positive','Count positive readings','Guided',
 'Return how many values are strictly greater than zero. Zero is not positive. Do not change the vector. An empty vector has a count of zero. The vector length fits in int.',
 'int countPositive(const std::vector<int>& values)',
 'int countPositive(const std::vector<int>& values) {\n    // TODO: inspect every value.\n    return 0;\n}',
 'int countPositive(const std::vector<int>& values) {\n    int count=0;\n    for (int value : values) if (value>0) ++count;\n    return count;\n}',
 'int main(){ std::vector<int> values; int x; while(std::cin>>x) values.push_back(x); std::cout<<countPositive(values)<<"\\n"; }',('-2 0 4 7 -1\n','2\n'),
 [('Mixed signs','countPositive({-2,0,4,7,-1})','2','Use > 0 rather than >= 0.'),('Empty input','countPositive({})','0','Start the count at zero.'),('Only negatives','countPositive({-3,-1})','0','Negative values do not increase the count.'),('Every item counts','countPositive({2,2,2})','3','Count occurrences, not distinct values.')],
 ['Keep a count and increase it only for a matching value.','The colon in for (int value : values) means “for each value in values.”'],
 'The loop visits each element once. The const reference borrows the vector for reading. The count is a separate local value.',includes=['iostream','vector'])
q('cpp-book-01','b1-maximum','Repair a maximum function','Debug',
 'Return the largest value in a nonempty vector. Throw std::invalid_argument for an empty vector. The starter incorrectly assumes the answer cannot be negative.',
 'int maximum(const std::vector<int>& values)',
 'int maximum(const std::vector<int>& values) {\n    if(values.empty()) throw std::invalid_argument("empty input");\n    int best=0; // BUG: what if every value is negative?\n    for(int x : values) if(x>best) best=x;\n    return best;\n}',
 'int maximum(const std::vector<int>& values) {\n    if(values.empty()) throw std::invalid_argument("empty input");\n    int best=values.front();\n    for(int x : values) if(x>best) best=x;\n    return best;\n}',
 'int main(){ std::vector<int> v; int x; while(std::cin>>x)v.push_back(x); try{std::cout<<maximum(v)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"empty input\\n";} }',('-8 -3 -10\n','-3\n'),
 [('All negative','maximum({-8,-3,-10})','-3','Initialize the candidate from an actual input value.'),('Mixed values','maximum({4,9,2,7})','9','Update the candidate only when a larger value appears.'),('One value','maximum({-5})','-5','A single element is its own maximum.'),('Empty is rejected','([]{try{maximum({});return false;}catch(const std::invalid_argument&){return true;}})()','true','Check emptiness before reading the first element.')],
 ['Which line invents a value not present in the input?','Use the first element only after checking empty().'],
 'Starting at the first item works for both positive and negative data. After every step, best is the greatest item visited so far.',includes=['iostream','vector','stdexcept'])
q('cpp-book-02','b2-owner','Return an owned reading','Ownership',
 'Return a std::unique_ptr<int> that owns a newly created int with the requested value. Each call must create its own object. Do not return an address of a local variable.',
 'std::unique_ptr<int> makeReading(int value)',
 'std::unique_ptr<int> makeReading(int value) {\n    // TODO: create an owned int.\n    return nullptr;\n}',
 'std::unique_ptr<int> makeReading(int value) {\n    return std::make_unique<int>(value);\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;auto p=makeReading(n);if(!p){std::cout<<"no reading\\n";return 0;}std::cout<<*p<<"\\n";}',('42\n','42\n'),
 [('Stores requested value','([]{auto p=makeReading(42);return p?*p:-999;})()','42','Return a non-null owner initialized with the argument.'),('Preserves negative values','([]{auto p=makeReading(-7);return p?*p:-999;})()','-7','The pointed-to value should equal the input.'),('Separate objects','([]{auto a=makeReading(3),b=makeReading(3);return a&&b&&a.get()!=b.get();})()','true','Each call needs a new owned object.'),('Ownership moves','([]{auto a=makeReading(8);auto b=std::move(a);return !a&&b&&*b==8;})()','true','unique_ptr transfers ownership when moved.')],
 ['std::make_unique creates an object and returns its owner.','The owner releases the object when the owner is destroyed.'],
 'make_unique<int>(value) constructs an int in owned dynamic storage. Returning its unique_ptr transfers that responsibility safely to the caller.',includes=['iostream','memory','utility'])
q('cpp-book-02','b2-trim','Turn a borrowed view into owned text','Implementation',
 'Remove leading and trailing ASCII space characters from the view and return an owning std::string. Keep internal spaces. The view may cover only part of a larger buffer; do not assume a terminating zero byte.',
 'std::string trimSpaces(std::string_view text)',
 'std::string trimSpaces(std::string_view text) {\n    // TODO: trim only the two ends.\n    return std::string(text);\n}',
 'std::string trimSpaces(std::string_view text) {\n    auto first=text.find_first_not_of(\' \');\n    if(first==std::string_view::npos) return {};\n    auto last=text.find_last_not_of(\' \');\n    return std::string(text.substr(first,last-first+1));\n}',
 'int main(){std::string line;std::getline(std::cin,line);std::cout<<trimSpaces(line)<<"\\n";}',('  hello world  \n','hello world\n'),
 [('Both ends','trimSpaces("  hello world  ")','hello world','Keep the inner space while trimming the ends.'),('All spaces','trimSpaces("   ")','','Handle a view with no non-space character.'),('Empty view','trimSpaces("")','','Do not subtract indices before checking npos.'),('View length matters','trimSpaces(std::string_view(" abXYZ",3))','ab','Respect the view length instead of treating data() as a C string.')],
 ['find_first_not_of and find_last_not_of can locate the kept range.','A returned string copies the selected characters into its own storage.'],
 'The view borrows existing bytes. Constructing a string from its subview creates an owning result whose lifetime does not depend on the original buffer.',includes=['iostream','string','string_view'])
q('cpp-book-02','b2-pipeline','Apply a list of operations in order','Composition',
 'Start with the supplied integer and apply each operation once, from left to right. Return the final value. An empty operation list returns the starting value. Test inputs stay within int range.',
 'int applyAll(int start, const std::vector<std::function<int(int)>>& operations)',
 'int applyAll(int start, const std::vector<std::function<int(int)>>& operations) {\n    // TODO: feed each result into the next operation.\n    return start;\n}',
 'int applyAll(int start, const std::vector<std::function<int(int)>>& operations) {\n    for(const auto& operation : operations) start=operation(start);\n    return start;\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;std::cout<<applyAll(n,{[](int x){return x+2;},[](int x){return x*3;}})<<"\\n";}',('4\n','18\n'),
 [('Order matters','applyAll(4,{[](int x){return x+2;},[](int x){return x*3;}})','18','Use the previous result as the next input.'),('Empty list','applyAll(-7,{})','-7','No operations means no change.'),('One operation','applyAll(5,{[](int x){return -x;}})','-5','Invoke a supplied function rather than returning it.'),('Called once each','([]{int calls=0;int value=applyAll(1,{[&](int x){++calls;return x+1;},[&](int x){++calls;return x*2;}});return value==4&&calls==2;})()','true','Do not repeat a callback to inspect its result.')],
 ['An operation can be called like a normal function: operation(current).','const auto& borrows each stored function object.'],
 'This is a small pipeline: each stage receives the output of the stage before it. The loop separates the order of work from the details of each operation.',includes=['iostream','vector','functional'])
q('cpp-book-03','b3-lower-bound','Find the first valid insertion position','Algorithm',
 'For a sorted vector, return the first index whose value is at least target. Return size() if all values are smaller. Implement a binary search with a half-open [low, high) interval. Tests check results; inspect your loop to confirm logarithmic search.',
 'std::size_t lowerBound(const std::vector<int>& values, int target)',
 'std::size_t lowerBound(const std::vector<int>& values, int target) {\n    // TODO: shrink a half-open interval.\n    return values.size();\n}',
 'std::size_t lowerBound(const std::vector<int>& values, int target) {\n    std::size_t low=0,high=values.size();\n    while(low<high){\n        auto mid=low+(high-low)/2;\n        if(values[mid]<target) low=mid+1;\n        else high=mid;\n    }\n    return low;\n}',
 'int main(){int target;if(!(std::cin>>target))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);std::cout<<lowerBound(v,target)<<"\\n";}',('3 1 3 3 7\n','1\n'),
 [('First duplicate','lowerBound({1,3,3,7},3)','1','Equality moves high toward the first matching position.'),('Between values','lowerBound({1,3,7},5)','2','The answer may be an insertion point with no equal value.'),('Before all','lowerBound({1,3,7},-1)','0','Index zero is valid.'),('After all','lowerBound({1,3,7},9)','3','The end position equals size().'),('Empty','lowerBound({},5)','0','An empty interval finishes without reading an element.')],
 ['Keep high exclusive; it starts at size().','When the middle value is smaller, discard it with low=mid+1.'],
 'Every iteration reduces the possible interval while keeping the desired insertion position inside its boundaries. low+(high-low)/2 avoids overflowing the sum of the endpoints.',includes=['iostream','vector','cstddef'])
q('cpp-book-03','b3-brackets','Check nested delimiters','Data structures',
 'Return true if (), [], and {} are properly matched and nested. Ignore all other characters. Empty input is balanced. A closing bracket must match the most recent unmatched opening bracket.',
 'bool balanced(std::string_view text)',
 'bool balanced(std::string_view text) {\n    // TODO: track the unmatched opening brackets.\n    return true;\n}',
 '''bool balanced(std::string_view text) {
    std::vector<char> stack;
    for(char ch:text){
        if(ch=='('||ch=='['||ch=='{') stack.push_back(ch);
        else if(ch==')'||ch==']'||ch=='}'){
            if(stack.empty()) return false;
            char wanted=ch==')'?'(':ch==']'?'[':'{';
            if(stack.back()!=wanted) return false;
            stack.pop_back();
        }
    }
    return stack.empty();
}''',
 'int main(){std::string line;std::getline(std::cin,line);std::cout<<std::boolalpha<<balanced(line)<<"\\n";}',('a{b[c(d)]}\n','true\n'),
 [('Nested types','balanced("a{b[c(d)]}")','true','Treat the vector as a last-in, first-out stack.'),('Wrong nesting','balanced("([)]")','false','Matching counts alone cannot prove nesting.'),('Early closing','balanced(")(")','false','Check empty() before reading back().'),('Unclosed opening','balanced("(()")','false','The stack must be empty after the loop.'),('No brackets','balanced("hello")','true','Ignore ordinary characters.')],
 ['A vector can act as a stack with push_back, back, and pop_back.','Each closing bracket removes exactly one matching opening bracket.'],
 'The stack represents unfinished nesting. Its top is the only opening bracket that a new closing bracket is allowed to match.',includes=['iostream','string','string_view','vector'])
q('cpp-book-03','b3-unique','Remove duplicates without reordering','Design',
 'Return each distinct integer once, keeping the order of its first appearance. Do not change the input. Use a set to track seen values and a vector to retain order.',
 'std::vector<int> stableUnique(const std::vector<int>& values)',
 'std::vector<int> stableUnique(const std::vector<int>& values) {\n    // TODO: remember which values already appeared.\n    return values;\n}',
 'std::vector<int> stableUnique(const std::vector<int>& values) {\n    std::unordered_set<int> seen;\n    std::vector<int> result;\n    for(int x:values) if(seen.insert(x).second) result.push_back(x);\n    return result;\n}',
 'int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(int x:stableUnique(v))std::cout<<x<<" ";std::cout<<"\\n";}',('3 1 3 2 1\n','3 1 2 \n'),
 [('Preserve order','stableUnique({3,1,3,2,1})','[3, 1, 2]','Do not sort the output.'),('Empty','stableUnique({})','[]','Empty input produces an empty result.'),('One repeated value','stableUnique({4,4,4})','[4]','Append only when insert reports a new value.'),('Signs and zero','stableUnique({0,-2,0,5,-2})','[0, -2, 5]','Track the actual integer value, including its sign.')],
 ['unordered_set::insert returns a pair. Its second member says whether insertion was new.','The set handles membership; the vector handles order.'],
 'One container answers “have I seen it?” and another records the required sequence. Iterating the unordered_set itself would lose the specified order.',includes=['iostream','vector','unordered_set'])
q('cpp-book-04','b4-align','Round a size up safely','Implementation',
 'Round bytes up to the next multiple of alignment. alignment must be positive; throw std::invalid_argument for zero. Throw std::overflow_error if the rounded result cannot fit in uint64_t. Alignment need not be a power of two.',
 'std::uint64_t alignedSize(std::uint64_t bytes, std::uint64_t alignment)',
 'std::uint64_t alignedSize(std::uint64_t bytes, std::uint64_t alignment) {\n    // TODO: validate and round without overflow.\n    return bytes;\n}',
 'std::uint64_t alignedSize(std::uint64_t bytes, std::uint64_t alignment) {\n    if(alignment==0) throw std::invalid_argument("zero alignment");\n    auto remainder=bytes%alignment;\n    if(remainder==0) return bytes;\n    auto extra=alignment-remainder;\n    if(bytes>std::numeric_limits<std::uint64_t>::max()-extra) throw std::overflow_error("size overflow");\n    return bytes+extra;\n}',
 'int main(){std::uint64_t n,a;if(!(std::cin>>n>>a))return 2;try{std::cout<<alignedSize(n,a)<<"\\n";}catch(const std::exception& e){std::cout<<e.what()<<"\\n";}}',('13 8\n','16\n'),
 [('Round up','alignedSize(13,8)','16','Add only the missing remainder.'),('Already aligned','alignedSize(24,8)','24','Do not add a full alignment when the remainder is zero.'),('Non-power-of-two','alignedSize(10,6)','12','Bit masks are insufficient for arbitrary alignments.'),('Zero alignment','([]{try{alignedSize(1,0);return false;}catch(const std::invalid_argument&){return true;}})()','true','Reject zero before using the remainder operator.'),('Overflow','([]{try{alignedSize(std::numeric_limits<std::uint64_t>::max(),2);return false;}catch(const std::overflow_error&){return true;}})()','true','Check the available range before adding.')],
 ['Find bytes % alignment first.','Compare bytes with max - extra before computing bytes + extra.'],
 'Checking before addition avoids unsigned wraparound. This function calculates a size; it does not itself allocate or align a memory address.',includes=['iostream','cstdint','limits','stdexcept'])
q('cpp-book-04','b4-bounds','Repair an overflow-prone bounds check','Debug',
 'Return whether length bytes starting at offset fit in a buffer of size bytes. An empty range at size is allowed. Avoid addition that can wrap around size_t.',
 'bool fits(std::size_t offset, std::size_t length, std::size_t size)',
 'bool fits(std::size_t offset, std::size_t length, std::size_t size) {\n    return offset + length <= size; // BUG: addition can wrap.\n}',
 'bool fits(std::size_t offset, std::size_t length, std::size_t size) {\n    return offset<=size && length<=size-offset;\n}',
 'int main(){std::size_t o,n,s;if(!(std::cin>>o>>n>>s))return 2;std::cout<<std::boolalpha<<fits(o,n,s)<<"\\n";}',('8 2 10\n','true\n'),
 [('Exact end','fits(8,2,10)','true','A range may end exactly at size.'),('Too long','fits(8,3,10)','false','The remaining space is only two bytes.'),('Empty at end','fits(10,0,10)','true','An empty range at the end is allowed.'),('Offset outside','fits(11,0,10)','false','Validate the offset even when length is zero.'),('Wraparound attack','fits(std::numeric_limits<std::size_t>::max(),2,10)','false','Do not let offset+length wrap into a small value.')],
 ['First check that offset is no greater than size.','Only then is size-offset safe to compute.'],
 'The && operator evaluates its right side only after the left side is true. That makes the subtraction safe and avoids overflow-prone addition.',includes=['iostream','cstddef','limits'])
q('cpp-book-04','b4-progress','Advance a partial transfer','Contracts',
 'Track a transfer of requested bytes. completed is the old count and reported is the new chunk length. Return the new count. Throw std::invalid_argument if the old count is too large or the chunk exceeds the remaining space.',
 'std::size_t advance(std::size_t completed, std::size_t reported, std::size_t requested)',
 'std::size_t advance(std::size_t completed, std::size_t reported, std::size_t requested) {\n    // TODO: check the old state and the new chunk.\n    return completed;\n}',
 'std::size_t advance(std::size_t completed, std::size_t reported, std::size_t requested) {\n    if(completed>requested || reported>requested-completed) throw std::invalid_argument("invalid progress");\n    return completed+reported;\n}',
 'int main(){std::size_t c,n,r;if(!(std::cin>>c>>n>>r))return 2;try{std::cout<<advance(c,n,r)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"invalid progress\\n";}}',('4 3 10\n','7\n'),
 [('Partial chunk','advance(4,3,10)','7','Add the new chunk to the completed count.'),('Finish exactly','advance(7,3,10)','10','Completion at the requested count is valid.'),('No progress','advance(4,0,10)','4','A zero-sized chunk leaves the count unchanged.'),('Oversized chunk','([]{try{advance(8,3,10);return false;}catch(const std::invalid_argument&){return true;}})()','true','A chunk cannot exceed requested-completed.'),('Invalid old state','([]{try{advance(11,0,10);return false;}catch(const std::invalid_argument&){return true;}})()','true','Check the old count before subtracting it.')],
 ['The invariant is completed <= requested.','Check the chunk against the remaining amount before addition.'],
 'This is the accounting rule used around partial reads or writes. It does not perform I/O; it gives a small, testable boundary for real I/O code.',includes=['iostream','cstddef','stdexcept'])
q('cpp-book-05','b5-port','Parse a strict port configuration','Validation',
 'Accept a nonempty string containing ASCII digits only and a number in 1..65535. Leading zeroes are allowed. Reject spaces, signs, trailing text, zero, and out-of-range numbers with std::invalid_argument.',
 'int parsePort(std::string_view text)',
 'int parsePort(std::string_view text) {\n    // TODO: validate the entire input.\n    return 80;\n}',
 '''int parsePort(std::string_view text) {
    if(text.empty()) throw std::invalid_argument("invalid port");
    int port=0;
    for(char ch:text){
        if(ch<'0'||ch>'9') throw std::invalid_argument("invalid port");
        port=port*10+(ch-'0');
        if(port>65535) throw std::invalid_argument("invalid port");
    }
    if(port==0) throw std::invalid_argument("invalid port");
    return port;
}''',
 'int main(){std::string s;std::getline(std::cin,s);try{std::cout<<parsePort(s)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"invalid port\\n";}}',('0080\n','80\n'),
 [('Leading zeros','parsePort("0080")','80','Accumulate digits instead of rejecting leading zeros.'),('Upper bound','parsePort("65535")','65535','The upper bound is included.'),('Trailing text','([]{try{parsePort("80x");return false;}catch(const std::invalid_argument&){return true;}})()','true','Consume and validate every character.'),('Above range','([]{try{parsePort("65536");return false;}catch(const std::invalid_argument&){return true;}})()','true','Reject a value as soon as it exceeds the limit.'),('Whitespace rejected','([]{try{parsePort(" 80");return false;}catch(const std::invalid_argument&){return true;}})()','true','This contract allows digits only.'),('Zero and empty','([]{int n=0;for(auto s:{"0",""})try{parsePort(s);}catch(const std::invalid_argument&){++n;}return n;})()','2','Reject both zero and an empty input.')],
 ['Check each character before turning it into a digit.','Checking the 65535 bound after every digit keeps the next int multiplication small.'],
 'The loop validates the entire representation. Incremental range checks prevent accumulation from reaching int overflow. The rules are explicit rather than relying on a parser that accepts prefixes.',includes=['iostream','string','string_view','stdexcept'])
q('cpp-book-05','b5-topk','Return the largest k measurements','Implementation',
 'Return up to k values in descending order. Keep duplicate values. Do not change the input. If k exceeds the input size, return all values. Tests check results, not performance.',
 'std::vector<int> topK(const std::vector<int>& values, std::size_t k)',
 'std::vector<int> topK(const std::vector<int>& values, std::size_t k) {\n    // TODO: return the largest values in descending order.\n    return {};\n}',
 'std::vector<int> topK(const std::vector<int>& values, std::size_t k) {\n    auto result=values;\n    k=std::min(k,result.size());\n    std::partial_sort(result.begin(),result.begin()+k,result.end(),std::greater<int>{});\n    result.resize(k);\n    return result;\n}',
 'int main(){std::size_t k;if(!(std::cin>>k))return 2;std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);for(int x:topK(v,k))std::cout<<x<<" ";std::cout<<"\\n";}',('3 8 2 8 5 1\n','8 8 5 \n'),
 [('Keep duplicates','topK({8,2,8,5,1},3)','[8, 8, 5]','Duplicate measurements occupy separate result slots.'),('Zero requested','topK({3,2},0)','[]','k=0 requests no values.'),('Larger than input','topK({2,7},8)','[7, 2]','Clamp k to the input size.'),('Negative values','topK({-8,-1,-3},2)','[-1, -3]','Descending order still applies to negative numbers.'),('Empty','topK({},2)','[]','Avoid indexing an empty vector.')],
 ['Copy the input before rearranging it.','partial_sort can order just the requested prefix; a full sort is also correct but may do more work.'],
 'partial_sort places the largest k values at the front when used with greater<int>. Resizing removes the unused suffix. Tests validate behavior; benchmarking is a separate task.',includes=['iostream','vector','algorithm','functional','cstddef'])
q('cpp-book-05','b5-runs','Count adjacent runs without extra storage','Algorithm',
 'A run is a consecutive group of equal values. Return the number of runs. The values 1,1,2,1 contain three runs. Empty input has zero. Use a single pass; tests check answers rather than allocation behavior.',
 'std::size_t countRuns(const std::vector<int>& values)',
 'std::size_t countRuns(const std::vector<int>& values) {\n    // TODO: count changes between adjacent values.\n    return values.size();\n}',
 'std::size_t countRuns(const std::vector<int>& values) {\n    if(values.empty()) return 0;\n    std::size_t runs=1;\n    for(std::size_t i=1;i<values.size();++i) if(values[i]!=values[i-1]) ++runs;\n    return runs;\n}',
 'int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);std::cout<<countRuns(v)<<"\\n";}',('1 1 2 1\n','3\n'),
 [('Value can return','countRuns({1,1,2,1})','3','Count boundaries, not distinct values.'),('One run','countRuns({5,5,5})','1','Equal neighbors stay in the same run.'),('Empty','countRuns({})','0','Handle empty input before starting at one.'),('Every item differs','countRuns({1,2,3,4})','4','Each changed neighbor starts a new run.')],
 ['A nonempty vector starts with one run.','Each later item starts a new run only when it differs from its predecessor.'],
 'Only the previous element matters. No auxiliary container is required, so this is a useful example of keeping a scan simple.',includes=['iostream','vector','cstddef'])
q('cpp-book-06','b6-partitions','Split work into balanced ranges','Parallel design',
 'Return exactly workers half-open index ranges covering [0,n), with no overlap. Earlier ranges receive any extra elements. Their lengths differ by at most one. Empty ranges are allowed. Throw std::invalid_argument when workers is zero. Tests use at most eight workers.',
 'std::vector<std::pair<std::size_t,std::size_t>> partition(std::size_t n, std::size_t workers)',
 'std::vector<std::pair<std::size_t,std::size_t>> partition(std::size_t n, std::size_t workers) {\n    // TODO: divide the count and distribute the remainder.\n    return {};\n}',
 'std::vector<std::pair<std::size_t,std::size_t>> partition(std::size_t n, std::size_t workers) {\n    if(workers==0) throw std::invalid_argument("zero workers");\n    std::vector<std::pair<std::size_t,std::size_t>> ranges;\n    std::size_t begin=0;\n    for(std::size_t i=0;i<workers;++i){\n        auto length=n/workers+(i<n%workers?1:0);\n        ranges.push_back({begin,begin+length});\n        begin+=length;\n    }\n    return ranges;\n}',
 'int main(){std::size_t n,w;if(!(std::cin>>n>>w))return 2;try{for(auto [a,b]:partition(n,w))std::cout<<a<<" "<<b<<"\\n";}catch(const std::invalid_argument&){std::cout<<"zero workers\\n";}}',('8 3\n','0 3\n3 6\n6 8\n'),
 [('Remainder first','partition(8,3)','[(0, 3), (3, 6), (6, 8)]','Give one extra item to each of the first n%workers ranges.'),('More workers than items','partition(2,4)','[(0, 1), (1, 2), (2, 2), (2, 2)]','Keep all requested ranges, including empty ones.'),('No items','partition(0,2)','[(0, 0), (0, 0)]','Empty work still has two empty ranges.'),('Reject zero workers','([]{try{partition(3,0);return false;}catch(const std::invalid_argument&){return true;}})()','true','Check the divisor before division.')],
 ['Use n/workers for the shared base length.','The end of one range is the beginning of the next.'],
 'Balanced, nonoverlapping half-open ranges are a useful starting contract for parallel writes. This exercise calculates the ranges; it does not launch GPU or CPU work.',includes=['iostream','vector','utility','cstddef','stdexcept'])
q('cpp-book-06','b6-sum','Join two CPU tasks before returning','Concurrency',
 'Split the vector at its middle. Launch two std::async tasks with std::launch::async, each returning its own long long subtotal. Wait for both and return their sum. Keep the borrowed vector alive until both finish. Tests check totals; review your code to verify that it uses two tasks and disjoint ranges.',
 'long long parallelSum(const std::vector<int>& values)',
 'long long parallelSum(const std::vector<int>& values) {\n    // TODO: compute two independent subtotals and join them.\n    return 0;\n}',
 'long long parallelSum(const std::vector<int>& values) {\n    auto sum=[&](std::size_t begin,std::size_t end){\n        long long total=0;\n        for(auto i=begin;i<end;++i) total+=values[i];\n        return total;\n    };\n    auto mid=values.size()/2;\n    auto first=std::async(std::launch::async,sum,0,mid);\n    auto second=std::async(std::launch::async,sum,mid,values.size());\n    return first.get()+second.get();\n}',
 'int main(){std::vector<int> v;int x;while(std::cin>>x)v.push_back(x);std::cout<<parallelSum(v)<<"\\n";}',('1 2 3 4 5\n','15\n'),
 [('Odd length','parallelSum({1,2,3,4,5})','15','The second range includes the remaining item.'),('Empty','parallelSum({})','0','Both empty subtotals are zero.'),('One value','parallelSum({-6})','-6','One range may be empty.'),('Wider accumulator','parallelSum({2000000000,2000000000})','4000000000','Accumulate in long long, not int.'),('Mixed signs','parallelSum({4,-8,2})','-2','Preserve signed values.')],
 ['Each task should own its subtotal instead of sharing one writable total.','future.get waits for completion and retrieves the returned subtotal.'],
 'The tasks share only read access to the vector. Their totals are separate local values, and get establishes completion before the results are combined. Passing these fixtures does not prove a speedup or every concurrency property.',includes=['iostream','vector','future','cstddef'])
q('cpp-book-06','b6-deadlines','Count missed deadlines','Real-time model',
 'Compare completion times with matching deadlines. A completion exactly at its deadline is on time. Return the number with completion > deadline. Both vectors contain nonnegative integer times. Throw std::invalid_argument if their lengths differ.',
 'std::size_t deadlineMisses(const std::vector<int>& completed, const std::vector<int>& deadlines)',
 'std::size_t deadlineMisses(const std::vector<int>& completed, const std::vector<int>& deadlines) {\n    // TODO: validate sizes and count late completions.\n    return 0;\n}',
 'std::size_t deadlineMisses(const std::vector<int>& completed, const std::vector<int>& deadlines) {\n    if(completed.size()!=deadlines.size()) throw std::invalid_argument("size mismatch");\n    std::size_t misses=0;\n    for(std::size_t i=0;i<completed.size();++i) if(completed[i]>deadlines[i]) ++misses;\n    return misses;\n}',
 'int main(){std::vector<int> c,d;int a,b;while(std::cin>>a>>b){c.push_back(a);d.push_back(b);}std::cout<<deadlineMisses(c,d)<<"\\n";}',('8 10\n10 10\n12 10\n','1\n'),
 [('Before, equal, after','deadlineMisses({8,10,12},{10,10,10})','1','Equality is not a miss.'),('All late','deadlineMisses({5,7},{4,6})','2','Count every late completion.'),('Empty','deadlineMisses({},{})','0','Empty matching vectors are valid.'),('Mismatched sizes','([]{try{deadlineMisses({1},{});return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate lengths before indexed access.')],
 ['Check equal sizes before the loop.','Use > rather than >= for the late test.'],
 'This checks recorded timing data. It models deadline accounting and makes no claim about real-time scheduling guarantees of the machine running it.',includes=['iostream','vector','cstddef','stdexcept'])
q('cpp-book-07','b7-admission','Preserve a bounded admission contract','Stateful design',
 'Complete Admission. submit accepts values from 0 through 100 while count is below its capacity. Return false for invalid values or a full queue without changing count. The model is single-threaded. size() reports accepted submissions.',
 'class Admission: bool submit(int value); std::size_t size() const',
 'class Admission {\n    std::size_t limit_,count_=0;\npublic:\n    explicit Admission(std::size_t limit):limit_(limit){}\n    bool submit(int value) {\n        // TODO: validate before changing state.\n        return false;\n    }\n    std::size_t size() const { return count_; }\n};',
 'class Admission {\n    std::size_t limit_,count_=0;\npublic:\n    explicit Admission(std::size_t limit):limit_(limit){}\n    bool submit(int value) {\n        if(value<0||value>100||count_>=limit_) return false;\n        ++count_; return true;\n    }\n    std::size_t size() const { return count_; }\n};',
 'int main(){std::size_t cap;if(!(std::cin>>cap))return 2;Admission a(cap);int x;while(std::cin>>x)std::cout<<std::boolalpha<<a.submit(x)<<" ";std::cout<<"count="<<a.size()<<"\\n";}',('2 7 101 8 9\n','true false true false count=2\n'),
 [('Valid submission','([]{Admission a(2);return a.submit(7)&&a.size()==1;})()','true','Increase the count only after acceptance.'),('Invalid leaves state','([]{Admission a(2);return !a.submit(101)&&!a.submit(-1)&&a.size()==0;})()','true','Reject values outside 0..100 before mutation.'),('Capacity is enforced','([]{Admission a(1);return a.submit(0)&&!a.submit(100)&&a.size()==1;})()','true','Full means count is already at the limit.'),('Zero capacity','([]{Admission a(0);return !a.submit(1)&&a.size()==0;})()','true','A zero-capacity model accepts nothing.')],
 ['A failed call must leave count_ unchanged.','Keep all rejection checks before ++count_.'],
 'Admission is a small failure boundary: a caller sees either acceptance with a state change, or rejection with the old state preserved. Concurrent callers would need additional synchronization.',includes=['iostream','cstddef'])
q('cpp-book-07','b7-retry','Retry only eligible failures','Policy',
 'attempt is the one-based number of the attempt that just failed. Return true only if the failure is transient and another attempt is available below maxAttempts. Return false for attempt < 1 or maxAttempts < 1.',
 'bool shouldRetry(int attempt, bool transient, int maxAttempts)',
 'bool shouldRetry(int attempt, bool transient, int maxAttempts) {\n    // TODO: combine eligibility and the attempt limit.\n    return transient;\n}',
 'bool shouldRetry(int attempt, bool transient, int maxAttempts) {\n    return attempt>=1 && maxAttempts>=1 && transient && attempt<maxAttempts;\n}',
 'int main(){int a,t,m;if(!(std::cin>>a>>t>>m))return 2;std::cout<<std::boolalpha<<shouldRetry(a,t!=0,m)<<"\\n";}',('2 1 3\n','true\n'),
 [('Another attempt remains','shouldRetry(2,true,3)','true','There is one remaining attempt after attempt two.'),('Last allowed attempt','shouldRetry(3,true,3)','false','Use a strict comparison at the limit.'),('Permanent failure','shouldRetry(1,false,3)','false','A retry budget does not make every failure transient.'),('Invalid numbering','shouldRetry(0,true,3)','false','The first valid attempt number is one.'),('No attempts allowed','shouldRetry(1,true,0)','false','Reject an invalid maximum.')],
 ['Write the conditions as independent boolean rules.','A retry is available only when attempt < maxAttempts.'],
 'This policy separates “may another attempt help?” from “is the retry budget exhausted?” It does not itself repeat a request or make an operation idempotent.',includes=['iostream'])
q('cpp-book-07','b7-idempotent','Apply a request key once','Distributed-systems model',
 'Complete IdempotentCounter. apply(key, delta) changes the total only the first time a nonempty key appears and returns true. Reusing a key returns false without changing total. Reject an empty key with std::invalid_argument. Inputs keep every total in int range. This is an in-memory, single-threaded model, not durable distributed storage.',
 'class IdempotentCounter: bool apply(const std::string& key, int delta); int total() const',
 'class IdempotentCounter {\n    std::unordered_set<std::string> seen_;\n    int total_=0;\npublic:\n    bool apply(const std::string& key,int delta) {\n        // TODO: reject empty keys, then apply each key once.\n        total_+=delta; return true;\n    }\n    int total() const { return total_; }\n};',
 'class IdempotentCounter {\n    std::unordered_set<std::string> seen_;\n    int total_=0;\npublic:\n    bool apply(const std::string& key,int delta) {\n        if(key.empty()) throw std::invalid_argument("empty key");\n        if(!seen_.insert(key).second) return false;\n        total_+=delta; return true;\n    }\n    int total() const { return total_; }\n};',
 'int main(){IdempotentCounter c;std::string key;int delta;while(std::cin>>key>>delta)c.apply(key,delta);std::cout<<c.total()<<"\\n";}',('a 5\na 5\nb 3\n','8\n'),
 [('First request applies','([]{IdempotentCounter c;return c.apply("a",5)&&c.total()==5;})()','true','Store both the key and the new total.'),('Duplicate does not apply','([]{IdempotentCounter c;c.apply("a",5);return !c.apply("a",999)&&c.total()==5;})()','true','The key determines duplication, even if its delta changes.'),('Different keys','([]{IdempotentCounter c;c.apply("a",5);c.apply("b",-2);return c.total();})()','3','Distinct keys may both update the total.'),('Empty key rejected','([]{IdempotentCounter c;try{c.apply("",4);return false;}catch(const std::invalid_argument&){return c.total()==0;}})()','true','Reject an empty key before touching state.')],
 ['insert(...).second tells you whether the key was new.','Return false before adding the delta when the key already exists.'],
 'Remembering completed keys prevents repeated effects in this process. A production design also needs atomic persistence, retention rules, and concurrency control.',includes=['iostream','string','unordered_set','stdexcept'])
q('cpp-book-08','b8-health','Allow only valid service transitions','State machine',
 'Keep the provided Health enum. transition handles starting + "ready" -> ready, ready + "stop" -> draining, and draining + "drained" -> stopped. Every other event leaves the state unchanged. No sockets or services are started by this model.',
 'Health transition(Health current, std::string_view event)',
 'enum class Health { starting, ready, draining, stopped };\nHealth transition(Health current, std::string_view event) {\n    // TODO: allow only the three stated transitions.\n    return current;\n}',
 'enum class Health { starting, ready, draining, stopped };\nHealth transition(Health current, std::string_view event) {\n    if(current==Health::starting&&event=="ready") return Health::ready;\n    if(current==Health::ready&&event=="stop") return Health::draining;\n    if(current==Health::draining&&event=="drained") return Health::stopped;\n    return current;\n}',
 'int main(){Health h=Health::starting;std::string event;while(std::cin>>event)h=transition(h,event);std::cout<<static_cast<int>(h)<<"\\n";}',('ready stop drained\n','3\n'),
 [('Ready after start','static_cast<int>(transition(Health::starting,"ready"))','1','Match both the current state and the event.'),('Stop begins draining','static_cast<int>(transition(Health::ready,"stop"))','2','Stopping a ready service begins its drain phase.'),('Drain completes shutdown','static_cast<int>(transition(Health::draining,"drained"))','3','Only drained completes this transition.'),('Cannot skip startup','static_cast<int>(transition(Health::starting,"drained"))','0','Reject an event that does not fit the current state.'),('Stopped stays stopped','static_cast<int>(transition(Health::stopped,"ready"))','3','Do not invent a restart transition.')],
 ['Each accepted transition needs a state condition and an event condition.','The default result is the current state.'],
 'Explicit transitions keep callers from skipping required phases. The integer results in tests correspond to the enum order: starting=0, ready=1, draining=2, stopped=3.',includes=['iostream','string','string_view'])
q('cpp-book-08','b8-redact','Redact token values in a simple log format','Text processing',
 'Replace the value after every literal "token=" with "***". A value ends at the next ASCII space or the end of the string. Preserve all other characters. This is a deliberately narrow log format, not a general secret detector.',
 'std::string redactLog(std::string text)',
 'std::string redactLog(std::string text) {\n    // TODO: redact each token= value.\n    return text;\n}',
 'std::string redactLog(std::string text) {\n    std::size_t pos=0;\n    while((pos=text.find("token=",pos))!=std::string::npos){\n        auto start=pos+6;\n        auto end=text.find(\' \',start);\n        if(end==std::string::npos) end=text.size();\n        text.replace(start,end-start,"***");\n        pos=start+3;\n    }\n    return text;\n}',
 'int main(){std::string s;std::getline(std::cin,s);std::cout<<redactLog(s)<<"\\n";}',('user=pat token=sample123 ok=true\n','user=pat token=*** ok=true\n'),
 [('Middle field','redactLog("user=pat token=sample123 ok=true")','user=pat token=*** ok=true','Preserve fields around the redacted value.'),('At end','redactLog("token=demo")','token=***','The end of the string also ends a value.'),('Multiple fields','redactLog("token=one token=two")','token=*** token=***','Continue searching after replacement.'),('No token','redactLog("status=ready")','status=ready','Leave unrelated text alone.'),('Empty value','redactLog("token= ok=true")','token=*** ok=true','Even an empty value gets the replacement marker.')],
 ['Find the marker, then find the next space after it.','Replacement changes the string length, so advance using the replacement length.'],
 'The scan follows the stated field boundary rather than trying to guess secrets. Real logging should avoid emitting sensitive fields in the first place and use a structured redaction policy.',includes=['iostream','string','cstddef'])
q('cpp-book-08','b8-backoff','Cap exponential retry delay without overflow','Reliability',
 'Start at min(base, cap). Double once per failure, stopping at cap. Return zero when base or cap is zero. Avoid unsigned overflow, even for a very large failure count. This deterministic calculation omits jitter on purpose.',
 'std::size_t retryDelay(unsigned failures, std::size_t base, std::size_t cap)',
 'std::size_t retryDelay(unsigned failures, std::size_t base, std::size_t cap) {\n    // TODO: grow the delay, but never wrap or exceed cap.\n    return base;\n}',
 'std::size_t retryDelay(unsigned failures, std::size_t base, std::size_t cap) {\n    std::size_t delay=std::min(base,cap);\n    if(delay==0) return 0;\n    while(failures-- && delay<cap){\n        if(delay>cap/2) return cap;\n        delay*=2;\n    }\n    return delay;\n}',
 'int main(){unsigned n;std::size_t b,c;if(!(std::cin>>n>>b>>c))return 2;std::cout<<retryDelay(n,b,c)<<"\\n";}',('3 100 500\n','500\n'),
 [('No failures','retryDelay(0,100,1000)','100','The initial delay is not doubled yet.'),('Two doublings','retryDelay(2,100,1000)','400','Double once per failure.'),('Cap reached','retryDelay(3,100,500)','500','Saturate rather than exceeding the cap.'),('Huge count','retryDelay(100000,1,9)','9','Stop growing once the cap is reached.'),('Overflow boundary','retryDelay(2,std::numeric_limits<std::size_t>::max()/2+1,std::numeric_limits<std::size_t>::max())','SIZE_MAX','Check against cap/2 before doubling.'),('Disabled delay','retryDelay(8,0,100)','0','A zero base remains zero.')],
 ['Before doubling, compare delay with cap/2.','Once delay reaches cap, later failures cannot change the answer.'],
 'Saturating growth prevents a large unsigned value from wrapping into a short delay. The early stop also avoids looping through a huge failure count.',includes=['iostream','cstddef','algorithm','limits'])
# Design-pattern and systems courses receive their own original coding sets.
q('design-patterns-cpp','dp-strategy','Call the chosen pricing strategy','Strategy',
 'checkout receives a base price and a pricing function. Call that function exactly once with the base price and return its result. Inputs stay in int range. The caller chooses the policy.',
 'int checkout(int base, const std::function<int(int)>& policy)',
 'int checkout(int base, const std::function<int(int)>& policy) {\n    // TODO: delegate this decision to the selected policy.\n    return base;\n}',
 'int checkout(int base, const std::function<int(int)>& policy) { return policy(base); }',
 'int main(){int base;if(!(std::cin>>base))return 2;std::cout<<checkout(base,[](int x){return x-10;})<<"\\n";}',('100\n','90\n'),
 [('Fixed discount','checkout(100,[](int x){return x-10;})','90','Use the supplied policy instead of hard-coding a price.'),('Different policy','checkout(100,[](int x){return x/2;})','50','The same checkout function must support another policy.'),('Free policy','checkout(100,[](int){return 0;})','0','Zero is a legitimate policy result.'),('Exactly one call','([]{int calls=0;int result=checkout(12,[&](int x){++calls;return x+3;});return result==15&&calls==1;})()','true','Calling a policy twice can repeat its side effects.')],
 ['The policy is callable with one integer argument.','Strategy separates the choice of a rule from the code that uses it.'],
 'The caller supplies behavior. A callable is enough for this small variation point; a class hierarchy is useful only when the policy needs a richer interface.',chapter=20,includes=['iostream','functional'])
q('design-patterns-cpp','dp-decorator','Wrap a renderer with brackets','Decorator',
 'Keep the Renderer alias. bracket(next) returns a new renderer that calls next once and surrounds its result with [ and ]. Store the wrapped callable by value so the returned callable stays valid. Nested wrappers must compose.',
 'Renderer bracket(Renderer next)',
 'using Renderer=std::function<std::string(std::string)>;\nRenderer bracket(Renderer next) {\n    // TODO: return a callable that adds behavior around next.\n    return next;\n}',
 'using Renderer=std::function<std::string(std::string)>;\nRenderer bracket(Renderer next) {\n    return [next=std::move(next)](std::string text){\n        return "["+next(std::move(text))+"]";\n    };\n}',
 'int main(){std::string s;std::getline(std::cin,s);auto render=bracket([](std::string text){return text;});std::cout<<render(s)<<"\\n";}',('hello\n','[hello]\n'),
 [('One wrapper','bracket([](std::string x){return x;})("hello")','[hello]','Wrap the returned text, not the original input only.'),('Nested wrappers','bracket(bracket([](std::string x){return x;}))("x")','[[x]]','Every wrapper adds one layer.'),('Preserve inner behavior','bracket([](std::string x){return x+"!";})("yes")','[yes!]','Call the wrapped renderer before adding brackets.'),('Empty text','bracket([](std::string x){return x;})("")','[]','Empty output still gets its wrapper.')],
 ['Return a lambda that captures next by value.','Capturing the local parameter by reference would dangle after bracket returns.'],
 'Each wrapper keeps the same calling interface and adds one behavior around the next component. Value capture preserves the wrapped callable’s lifetime.',chapter=9,includes=['iostream','string','functional','utility'])
q('design-patterns-cpp','dp-observer','Publish to a stable listener snapshot','Observer',
 'Complete Signal::publish. Call subscribers in registration order. Subscribers added during a publication must start receiving events only on the next publication. No unsubscribe or concurrent access is required.',
 'class Signal: void subscribe(std::function<void(int)>); void publish(int)',
 'class Signal {\n    std::vector<std::function<void(int)>> listeners_;\npublic:\n    void subscribe(std::function<void(int)> fn){listeners_.push_back(std::move(fn));}\n    void publish(int value){\n        // TODO: notify the listeners present when this call begins.\n    }\n};',
 'class Signal {\n    std::vector<std::function<void(int)>> listeners_;\npublic:\n    void subscribe(std::function<void(int)> fn){listeners_.push_back(std::move(fn));}\n    void publish(int value){\n        auto snapshot=listeners_;\n        for(const auto& fn:snapshot) fn(value);\n    }\n};',
 'int main(){int n;if(!(std::cin>>n))return 2;Signal s;s.subscribe([](int x){std::cout<<"first="<<x<<"\\n";});s.subscribe([](int x){std::cout<<"second="<<x<<"\\n";});s.publish(n);}',('7\n','first=7\nsecond=7\n'),
 [('Registration order','([]{Signal s;std::string log;s.subscribe([&](int){log+="A";});s.subscribe([&](int){log+="B";});s.publish(1);return log;})()','AB','Visit the snapshot in its stored order.'),('Value delivered','([]{Signal s;int seen=0;s.subscribe([&](int x){seen=x;});s.publish(9);return seen;})()','9','Pass the published value to the listener.'),('New listener waits','([]{Signal s;int early=0,late=0;s.subscribe([&](int){++early;if(early==1)s.subscribe([&](int){++late;});});s.publish(1);bool first=late==0;s.publish(2);return first&&early==2&&late==1;})()','true','Iterate over a snapshot, not the vector being changed.'),('No listeners','([]{Signal s;s.publish(3);return true;})()','true','An empty subscriber list is valid.')],
 ['Adding to the original vector while iterating it may invalidate iteration.','Copy the listener list before invoking callbacks.'],
 'The snapshot makes the notification boundary explicit and avoids iterator invalidation caused by new subscribers. It does not make the signal thread-safe or define recursive publication policies.',chapter=18,includes=['iostream','vector','functional','string','utility'])
q('systems-programming','sys-cache','Calculate a cache set index','Address model',
 'Return (address / blockSize) % sets. blockSize and sets must both be positive; throw std::invalid_argument for zero. This is a direct-mapped indexing calculation, not a real hardware measurement.',
 'std::uint64_t cacheSet(std::uint64_t address, std::uint64_t blockSize, std::uint64_t sets)',
 'std::uint64_t cacheSet(std::uint64_t address, std::uint64_t blockSize, std::uint64_t sets) {\n    // TODO: remove the byte offset, then choose a set.\n    return 0;\n}',
 'std::uint64_t cacheSet(std::uint64_t address, std::uint64_t blockSize, std::uint64_t sets) {\n    if(blockSize==0||sets==0) throw std::invalid_argument("zero dimension");\n    return (address/blockSize)%sets;\n}',
 'int main(){std::uint64_t a,b,s;if(!(std::cin>>a>>b>>s))return 2;try{std::cout<<cacheSet(a,b,s)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"zero dimension\\n";}}',('192 64 8\n','3\n'),
 [('Block offset ignored','cacheSet(63,64,8)','0','Divide by block size before taking the set remainder.'),('Next block','cacheSet(64,64,8)','1','A new block advances the index.'),('Index wraps','cacheSet(512,64,8)','0','The set index wraps after eight blocks.'),('Another offset','cacheSet(255,64,8)','3','Bytes within the same block share a set.'),('Reject zero','([]{try{cacheSet(1,0,8);return false;}catch(const std::invalid_argument&){return true;}})()','true','Validate both divisors before arithmetic.')],
 ['First convert a byte address into a block number.','Then reduce the block number modulo the set count.'],
 'This calculation cleanly separates the byte offset from the set choice. It is a small deterministic model suitable for tests around address decomposition.',chapter=30,includes=['iostream','cstdint','stdexcept'])
q('systems-programming','sys-endian','Decode four little-endian bytes','Representation',
 'Interpret byte 0 as the least significant byte and byte 3 as the most significant. Return a uint32_t. Convert each byte to uint32_t before shifting, so signed promotion cannot corrupt the high bit.',
 'std::uint32_t readLE32(const std::array<unsigned char,4>& bytes)',
 'std::uint32_t readLE32(const std::array<unsigned char,4>& bytes) {\n    // TODO: combine all four bytes in little-endian order.\n    return bytes[0];\n}',
 'std::uint32_t readLE32(const std::array<unsigned char,4>& bytes) {\n    return std::uint32_t(bytes[0]) | (std::uint32_t(bytes[1])<<8) |\n           (std::uint32_t(bytes[2])<<16) | (std::uint32_t(bytes[3])<<24);\n}',
 'int main(){std::array<unsigned char,4> b{};for(auto& x:b){unsigned n;if(!(std::cin>>n)||n>255)return 2;x=static_cast<unsigned char>(n);}std::cout<<readLE32(b)<<"\\n";}',('120 86 52 18\n','305419896\n'),
 [('Known byte order','readLE32({0x78,0x56,0x34,0x12})','305419896','The first input byte is the low byte.'),('All zero','readLE32({0,0,0,0})','0','Zero bytes should contribute no bits.'),('High bit set','readLE32({0,0,0,128})','2147483648','Shift an unsigned 32-bit value.'),('All bits set','readLE32({255,255,255,255})','4294967295','Keep the result unsigned.')],
 ['Use shifts of 0, 8, 16, and 24 bits.','Bitwise OR combines nonoverlapping bit fields.'],
 'The explicit shifts define the serialized byte order independently of the host machine. No pointer cast or unaligned memory access is required.',chapter=4,includes=['iostream','array','cstdint'])
q('systems-programming','sys-queue','Implement a bounded FIFO model','Integration',
 'Complete BoundedQueue. push returns false when full and otherwise adds a value. pop removes and returns the oldest value, or std::nullopt if empty. Zero capacity is valid. This exercise is single-threaded; it does not claim to implement a synchronized queue.',
 'class BoundedQueue: bool push(int); std::optional<int> pop(); std::size_t size() const',
 'class BoundedQueue {\n    std::deque<int> values_;\n    std::size_t capacity_;\npublic:\n    explicit BoundedQueue(std::size_t capacity):capacity_(capacity){}\n    bool push(int value){ /* TODO */ return false; }\n    std::optional<int> pop(){ /* TODO */ return std::nullopt; }\n    std::size_t size() const { return values_.size(); }\n};',
 'class BoundedQueue {\n    std::deque<int> values_;\n    std::size_t capacity_;\npublic:\n    explicit BoundedQueue(std::size_t capacity):capacity_(capacity){}\n    bool push(int value){\n        if(values_.size()>=capacity_) return false;\n        values_.push_back(value); return true;\n    }\n    std::optional<int> pop(){\n        if(values_.empty()) return std::nullopt;\n        int value=values_.front(); values_.pop_front(); return value;\n    }\n    std::size_t size() const { return values_.size(); }\n};',
 'int main(){std::size_t cap;if(!(std::cin>>cap))return 2;BoundedQueue q(cap);int x;while(std::cin>>x)q.push(x);while(auto value=q.pop())std::cout<<*value<<" ";std::cout<<"\\n";}',('2 7 9 11\n','7 9 \n'),
 [('FIFO order','([]{BoundedQueue q(2);q.push(7);q.push(9);return q.pop()==7&&q.pop()==9&&!q.pop();})()','true','Remove from the front, not the back.'),('Full leaves state','([]{BoundedQueue q(1);return q.push(3)&&!q.push(4)&&q.size()==1&&q.pop()==3;})()','true','Reject before inserting when at capacity.'),('Reclaim a slot','([]{BoundedQueue q(1);q.push(1);q.pop();return q.push(2)&&q.pop()==2;})()','true','pop must remove the returned value.'),('Zero capacity','([]{BoundedQueue q(0);return !q.push(1)&&!q.pop()&&q.size()==0;})()','true','Handle empty and zero-capacity cases.')],
 ['deque supports push_back and pop_front for FIFO order.','optional distinguishes “no value” from a valid integer such as zero.'],
 'The queue’s contract is small enough to test directly. Synchronization, blocking waits, cancellation, and shutdown are separate concerns for a later concurrent implementation.',chapter=61,includes=['iostream','deque','optional','cstddef'])
from book_one_coding import add_book_one
add_book_one(q)
from book_two_coding import add_book_two
add_book_two(q)
from book_three_coding import add_book_three
add_book_three(q)
# Adapt the numeric max fixture to the declared compilation target without losing portability.
for item in questions:
 for test in item['tests']:
  if test['expected']=='SIZE_MAX':test['expectedExpression']='std::numeric_limits<std::size_t>::max()';test['expectedLabel']='maximum size_t value';test['expected']='18446744073709551615'
# Match each exercise to an existing chapter based on its subject where a close title exists.
content=json.loads((ROOT/'src/content.json').read_text());courses={c['id']:c for c in content['courses']}
terms={'b1-add':['functions'],'b1-positive':['loops','control'],'b1-maximum':['vector','standard'],'b2-owner':['ownership','resource'],'b2-trim':['string','view'],'b2-pipeline':['lambda','callable'],'b3-lower-bound':['efficient search'],'b3-brackets':['last-in'],'b3-unique':['hash'],'b4-align':['memory'],'b4-bounds':['memory'],'b4-progress':['file','input'],'b5-port':['test','configuration'],'b5-topk':['algorithm','performance'],'b5-runs':['profil'],'b6-partitions':['parallel'],'b6-sum':['parallel'],'b6-deadlines':['real-time'],'b7-admission':['admission control'],'b7-retry':['retries'],'b7-idempotent':['idempot','distributed'],'b8-health':['lifecycle'],'b8-redact':['observability','logging'],'b8-backoff':['failure injection']}
for item in questions:
 c=courses[item['courseId']]
 if item['id'] in terms:
  candidates=[ch for ch in c['chapters'] if ch.get('role')=='chapter' and any(term in ch['title'].lower() for term in terms[item['id']])]
  if candidates:item['chapter']=candidates[0]['number']
 item['chapterTitle']=next(ch['title'] for ch in c['chapters'] if ch['number']==item['chapter'])
 d=ROOT/'coding_lab'/item['id'];d.mkdir(parents=True,exist_ok=True)
 for name,code in [('starter.cpp',item['starter']),('solution.cpp',item['solution']),('driver.cpp',item['driver'])]:(d/name).write_text(code)
 (d/'question.json').write_text(json.dumps(item,ensure_ascii=False,indent=2)+'\n')
from book_one_worked import build_worked
from book_two_worked import build_worked as build_book_two_worked
from book_three_worked import build_worked as build_book_three_worked
worked=build_worked(content)+build_book_two_worked(content)+build_book_three_worked(content)
(ROOT/'src/coding-content.json').write_text(json.dumps(dict(version=2,standard='c++20',questions=questions,workedPrograms=worked),ensure_ascii=False,indent=2)+'\n')
print('Authored',len(questions),'questions across',len(set(q['courseId'] for q in questions)),'courses; tests:',sum(len(q['tests']) for q in questions),'; Book I/II/III worked programs:',len(worked))
