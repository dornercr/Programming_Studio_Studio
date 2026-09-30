"""Original Book I exercises. Each contract includes an intentionally wrong starter."""
HEADERS='''#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <charconv>
#include <memory>
#include <utility>
'''
LABS=[]
def lab(ch,slug,title,signature,prompt,solution,starter,driver,sample,tests,design,invariant,alternative,bug):
 LABS.append(dict(chapter=ch,id='b1x-'+slug,title=title,signature=signature,prompt=prompt,solution=HEADERS+'\n'+solution+'\n',starter=HEADERS+'\n'+starter+'\n',driver=driver+'\n',sampleInput=sample[0],sampleOutput=sample[1],tests=[dict(id='case'+str(i+1),label=t[0],expression=t[1],expected=t[2],hint=t[3]) for i,t in enumerate(tests)],design=design,invariant=invariant,alternative=alternative,bug=bug,body=solution))
def case(label,expr,expected,hint='Trace the inputs against the stated contract.'):
 return (label,expr,expected,hint)
lab(1,'pipeline','Name a build-pipeline stage','std::string stageName(int stage)',
 'Map 0 to preprocess, 1 to compile, 2 to assemble, and 3 to link. Return invalid for any other integer. This models stage labels; it does not run a compiler.',
 '''std::string stageName(int stage) {
    // Reject a label outside the four modeled stages.
    if (stage < 0 || stage > 3) return "invalid";
    if (stage == 0) return "preprocess";
    if (stage == 1) return "compile";
    if (stage == 2) return "assemble";
    return "link";
}''',
 'std::string stageName(int stage) {\n    // BUG: every stage gets the same name.\n    return stage == 0 ? "preprocess" : "compile";\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;std::cout<<stageName(n)<<"\\n";}',('3\n','link\n'),
 [case('First stage','stageName(0)','preprocess'),case('Compilation','stageName(1)','compile'),case('Assembly','stageName(2)','assemble'),case('Final stage','stageName(3)','link'),case('Negative stage','stageName(-1)','invalid'),case('Past final stage','stageName(4)','invalid')],
 'Explicit comparisons expose the four-stage mapping without introducing a new type. The driver prints the returned label.',
 'Each valid stage has exactly one label; every other integer is rejected with invalid.',
 'A switch is equally reasonable. A lookup table is useful for larger mappings but still needs a bounds check.',
 'For stage 3 the starter returns compile instead of link. It also mislabels every invalid nonzero input.')
lab(2,'environment','Check a measured environment report','bool environmentReady(long tag, bool compiler, bool library)',
 'Return true only if compiler and library are both available and the measured language tag is at least 202002. The function evaluates reported facts; it does not detect installed tools.',
 '''bool environmentReady(long tag, bool compiler, bool library) {
    // All three requirements must be satisfied.
    return compiler && library && tag >= 202002;
}''',
 'bool environmentReady(long tag, bool compiler, bool library) {\n    // BUG: accepts a compiler alone.\n    return compiler || (library && tag >= 202002);\n}',
 'int main(){long tag;int c,l;if(!(std::cin>>tag>>c>>l))return 2;std::cout<<std::boolalpha<<environmentReady(tag,c!=0,l!=0)<<"\\n";}',('201703 1 1\n','false\n'),
 [case('C++20 boundary','environmentReady(202002,true,true)','true'),case('Older mode','environmentReady(201703,true,true)','false'),case('No compiler','environmentReady(202002,false,true)','false'),case('No library','environmentReady(202002,true,false)','false'),case('Later mode','environmentReady(202302,true,true)','true')],
 'A Boolean conjunction directly expresses the three independent requirements. Passing the facts makes boundary tests independent of this machine.',
 'Success requires every stated fact, including the language-version boundary.',
 'An environment probe can gather facts, but mixing detection and evaluation makes controlled unit tests harder.',
 'With an available compiler and tag 201703, the starter returns true even though the required language mode is absent.')
lab(3,'strict-input','Read one whole bounded count','bool readCount(const std::string& text, int& output)',
 'Accept exactly one integer from 0 through 20, with optional surrounding stream whitespace. Reject empty, malformed, out-of-range, and trailing non-whitespace text. On rejection leave output unchanged.',
 '''bool readCount(const std::string& text, int& output) {
    std::istringstream input(text);
    int candidate{};
    // Read a candidate, then check domain and complete consumption.
    if (!(input >> candidate) || candidate < 0 || candidate > 20) return false;
    input >> std::ws;
    if (!input.eof()) return false;
    output = candidate; // Commit only after every check succeeds.
    return true;
}''',
 '''bool readCount(const std::string& text, int& output) {
    std::istringstream input(text);
    // BUG: a successful numeric prefix is accepted.
    return static_cast<bool>(input >> output);
}''',
 'int main(){std::string text;std::getline(std::cin,text);int n=7;if(readCount(text,n))std::cout<<n<<"\\n";else std::cout<<"invalid "<<n<<"\\n";}',('12x\n','invalid 7\n'),
 [case('Valid count','([]{int n=7;return readCount("12",n)&&n==12;})()','true'),case('Trailing text','([]{int n=7;return !readCount("12x",n)&&n==7;})()','true'),case('Lower boundary','([]{int n=7;return readCount("0",n)&&n==0;})()','true'),case('Upper boundary','([]{int n=7;return readCount("20",n)&&n==20;})()','true'),case('Range rejected','([]{int n=7;return !readCount("21",n)&&n==7;})()','true'),case('Empty rejected','([]{int n=7;return !readCount("",n)&&n==7;})()','true'),case('Whitespace allowed','([]{int n=7;return readCount(" 3 ",n)&&n==3;})()','true')],
 'A temporary separates parsing from committing caller state. A stream matches the whitespace rule and the beginner input lesson.',
 'A rejected field preserves the old output; an accepted field consumes the whole permitted field and satisfies 0 through 20.',
 'from_chars is useful for strict byte-range parsing, but it has different whitespace rules that the adapter must handle explicitly.',
 'For 12x the starter reports success and changes output to 12. It neither validates the whole input nor preserves state on all failures.')
lab(4,'increment','Increment without signed overflow','int checkedNext(int value)',
 'Return value plus one when representable as int. At the maximum int throw std::out_of_range("overflow") before arithmetic.',
 '''int checkedNext(int value) {
    // Check the boundary before the potentially overflowing expression.
    if (value == std::numeric_limits<int>::max()) throw std::out_of_range("overflow");
    return value + 1;
}''',
 'int checkedNext(int value) {\n    // BUG: valid inputs never increase.\n    if(value==std::numeric_limits<int>::max())throw std::out_of_range("overflow");\n    return value;\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;try{std::cout<<checkedNext(n)<<"\\n";}catch(const std::out_of_range&){std::cout<<"overflow\\n";}}',('9\n','10\n'),
 [case('Normal','checkedNext(9)','10'),case('Negative','checkedNext(-2)','-1'),case('Zero','checkedNext(0)','1'),case('Last valid input','checkedNext(std::numeric_limits<int>::max()-1)','', 'Use the type limit, not a hard-coded machine width.'),case('Maximum rejected','([]{try{checkedNext(std::numeric_limits<int>::max());return false;}catch(const std::out_of_range&){return true;}})()','true')],
 'Checking numeric_limits makes the representation boundary explicit before addition. The function returns a value and does not mutate caller state.',
 'No path evaluates overflowing signed addition; accepted values increase by exactly one.',
 'A wider type can help when its range is known to cover the calculation. It still needs a checked conversion to int afterward.',
 'The starter returns 9 for input 9 instead of 10. An unchecked value+1 at the maximum would have undefined behavior, not a promised wrapped output.')
# A portable expected expression is evaluated by the supplied harness.
LABS[-1]['tests'][3]['expectedExpression']='std::numeric_limits<int>::max()'
lab(5,'packing','Keep quotient and remainder consistent','std::pair<int,int> packing(int items, int group)',
 'items must be 0 through 1000 and group 1 through 100. Throw std::invalid_argument("range") otherwise. Return complete groups and leftover items.',
 '''std::pair<int,int> packing(int items, int group) {
    // Positive group size prevents division by zero.
    if (items < 0 || items > 1000 || group < 1 || group > 100) throw std::invalid_argument("range");
    return {items / group, items % group};
}''',
 'std::pair<int,int> packing(int items,int group){\n if(items<0||items>1000||group<1||group>100)throw std::invalid_argument("range");\n // BUG: swaps the two meanings.\n return {items%group,items/group};\n}',
 'int main(){int n,g;if(!(std::cin>>n>>g))return 2;try{auto p=packing(n,g);std::cout<<p.first<<" "<<p.second<<"\\n";}catch(const std::invalid_argument&){std::cout<<"range\\n";}}',('17 5\n','3 2\n'),
 [case('Leftover items','packing(17,5)','(3, 2)'),case('Exact groups','packing(20,5)','(4, 0)'),case('Empty inventory','packing(0,7)','(0, 0)'),case('Larger group','packing(3,5)','(0, 3)'),case('Zero divisor rejected','([]{try{packing(3,0);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'Integer quotient and remainder express complete groups without introducing floating-point rounding.',
 'items equals groups times group plus remainder, and remainder is smaller than group.',
 'Repeated subtraction can produce the same values but does unnecessary work and adds a loop invariant to maintain.',
 'The starter reports 2 groups and 3 leftovers for 17 items in groups of 5. Those results do not reconstruct the original 17 items.')
lab(6,'thresholds','Partition score boundaries','std::string band(int score)',
 'Scores 0 through 100 map to high at 80 or above, pass at 50 through 79, and retry below 50. Outside 0 through 100 return invalid.',
 '''std::string band(int score) {
    if (score < 0 || score > 100) return "invalid";
    // Most specific threshold is checked first.
    if (score >= 80) return "high";
    if (score >= 50) return "pass";
    return "retry";
}''',
 'std::string band(int score){\n if(score<0||score>100)return "invalid";\n // BUG: broader range catches high scores.\n if(score>=50)return "pass";\n if(score>=80)return "high";\n return "retry";\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;std::cout<<band(n)<<"\\n";}',('80\n','high\n'),
 [case('Below pass','band(49)','retry'),case('Pass boundary','band(50)','pass'),case('Below high','band(79)','pass'),case('High boundary','band(80)','high'),case('Top score','band(100)','high'),case('Outside domain','band(101)','invalid')],
 'An ordered exclusive chain gives each valid score exactly one label. The order reflects nested threshold ranges.',
 'Each boundary belongs to one band; invalid input never receives a successful score band.',
 'Explicit disjoint ranges are also valid, but repeating every upper bound creates more places for a later policy edit to drift.',
 'The starter returns pass for 80 because it returns from the broader >=50 branch before reaching the high test.')
lab(7,'bounded-loop','Sum accepted readings before a sentinel','std::pair<int,int> accepted(const std::vector<int>& readings)',
 'Read in order. Stop at -999 or after accepting three values. Ignore other negatives. Accepted values are 0 through 100; input obeys these value categories. Return accepted count and sum.',
 '''std::pair<int,int> accepted(const std::vector<int>& readings) {
    int count = 0, sum = 0;
    for (int value : readings) {
        if (value == -999) break; // Sentinel is not data.
        if (value < 0) continue;
        sum += value;
        ++count;
        if (count == 3) break; // Bound accepted items, not attempts.
    }
    return {count, sum};
}''',
 'std::pair<int,int> accepted(const std::vector<int>& readings){\n int count=0,sum=0;\n for(int value:readings){if(value==-999)break;if(value<0)continue;sum+=value;++count;}\n // BUG: misses the accepted-item limit.\n return {count,sum};\n}',
 'int main(){std::vector<int> v;int n;while(std::cin>>n)v.push_back(n);auto p=accepted(v);std::cout<<p.first<<" "<<p.second<<"\\n";}',('1 -2 3 4 5\n','3 8\n'),
 [case('Bound at three','accepted({1,-2,3,4,5})','(3, 8)'),case('Sentinel first','accepted({-999,4})','(0, 0)'),case('Sentinel later','accepted({2,-999,7})','(1, 2)'),case('All skipped','accepted({-1,-2})','(0, 0)'),case('Zero accepted','accepted({0,0,0,9})','(3, 0)'),case('Empty range','accepted({})','(0, 0)')],
 'A range loop advances safely even on continue. Separate count and sum track the same accepted prefix.',
 'The count is at most three, and the sum describes exactly those count accepted readings before the sentinel.',
 'An index loop is useful when positions matter. Here it adds a manual index without helping the acceptance rule.',
 'The starter accepts four values and returns (4,13) for the sample, violating the three-item boundary.')
lab(8,'pricing','Separate a price calculation from its caller','int priceTotal(int unit, int count)',
 'Return unit times count when unit is 0 through 1000 and count 0 through 100. Otherwise throw std::invalid_argument("range"). Do not print inside the function.',
 '''int priceTotal(int unit, int count) {
    // A long intermediate holds this bounded product.
    // Check the int range before narrowing the return value.
    if (unit < 0 || unit > 1000 || count < 0 || count > 100) throw std::invalid_argument("range");
    long result = static_cast<long>(unit) * count;
    if (result > std::numeric_limits<int>::max()) throw std::out_of_range("overflow");
    return static_cast<int>(result);
}''',
 'int priceTotal(int unit,int count){\n if(unit<0||unit>1000||count<0||count>100)throw std::invalid_argument("range");\n // BUG: adds instead of pricing the quantity.\n return unit+count;\n}',
 'int main(){int u,n;if(!(std::cin>>u>>n))return 2;try{std::cout<<priceTotal(u,n)<<"\\n";}catch(const std::exception& e){std::cout<<e.what()<<"\\n";}}',('12 3\n','36\n'),
 [case('Several items','priceTotal(12,3)','36'),case('Zero quantity','priceTotal(12,0)','0'),case('Free item','priceTotal(0,3)','0'),case('Single item','priceTotal(15,1)','15'),case('Reject negative count','([]{try{priceTotal(12,-1);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'A value-returning helper isolates the calculation. A long intermediate and range check make conversion to int deliberate.',
 'The helper prints nothing; accepted arguments produce their mathematical product if representable, and invalid inputs are rejected.',
 'Reading and printing inside the helper can work for a one-off demo, but couples pricing to a terminal and makes reuse harder.',
 'The starter returns 15 for unit 12 and count 3 instead of 36. Its calculation violates the pricing requirement.')
lab(9,'caller-counter','Keep counters independent','int nextTicket(int& counter)',
 'counter must be 0 through 999. Return its current value and increment the same caller-owned counter. On invalid input throw std::invalid_argument("range") without changing it.',
 '''int nextTicket(int& counter) {
    if (counter < 0 || counter > 999) throw std::invalid_argument("range");
    // Save the old value, then update the caller-owned state.
    int issued = counter;
    ++counter;
    return issued;
}''',
 'int nextTicket(int& counter){\n if(counter<0||counter>999)throw std::invalid_argument("range");\n // BUG: reads the state but never advances it.\n return counter;\n}',
 'int main(){int n;if(!(std::cin>>n))return 2;try{int issued=nextTicket(n);std::cout<<issued<<" "<<n<<"\\n";}catch(const std::invalid_argument&){std::cout<<"range\\n";}}',('7\n','7 8\n'),
 [case('Advance state','([]{int n=7;int issued=nextTicket(n);return issued==7&&n==8;})()','true'),case('Independent counters','([]{int a=1,b=20;nextTicket(a);return a==2&&b==20;})()','true'),case('Two calls','([]{int n=0;nextTicket(n);return nextTicket(n)==1&&n==2;})()','true'),case('Final valid ticket','([]{int n=999;return nextTicket(n)==999&&n==1000;})()','true'),case('Failure preserves state','([]{int n=1000;try{nextTicket(n);}catch(const std::invalid_argument&){return n==1000;}return false;})()','true')],
 'A reference makes the changing state explicit at the call boundary. Separate callers can maintain independent sequences.',
 'Success issues the old count and advances only that counter; rejection changes no state.',
 'A function-local static hides one persistent sequence shared by callers. It fits a deliberately global service, but weakens independent tests here.',
 'The starter leaves counter at 7 after issuing ticket 7. The next call would issue the same ticket again.')
lab(10,'matrix','Respect both array dimensions','std::vector<int> rowTotals(const std::array<std::array<int,3>,2>& grid)',
 'Return two totals, one per row of the fixed 2-by-3 grid. Each element is -100 through 100. Preserve the input and include every column.',
 '''std::vector<int> rowTotals(const std::array<std::array<int,3>,2>& grid) {
    std::vector<int> totals;
    for (const auto& row : grid) {
        int sum = 0; // Each row starts a new total.
        for (int value : row) sum += value;
        totals.push_back(sum);
    }
    return totals;
}''',
 'std::vector<int> rowTotals(const std::array<std::array<int,3>,2>& grid){\n std::vector<int> out;\n // BUG: visits only the first two columns.\n for(const auto& row:grid)out.push_back(row[0]+row[1]);\n return out;\n}',
 'int main(){std::array<std::array<int,3>,2> g{};for(auto& row:g)for(int& x:row)if(!(std::cin>>x))return 2;auto v=rowTotals(g);std::cout<<v[0]<<" "<<v[1]<<"\\n";}',('1 2 3 4 5 6\n','6 15\n'),
 [case('All columns','rowTotals({{{1,2,3},{4,5,6}}})','[6, 15]'),case('Zero grid','rowTotals({{{0,0,0},{0,0,0}}})','[0, 0]'),case('Mixed signs','rowTotals({{{-2,1,4},{3,-5,1}}})','[3, -1]'),case('Last column matters','rowTotals({{{0,0,9},{0,0,-4}}})','[9, -4]')],
 'Nested range loops follow the actual row and column extents. A local sum is reset for each row.',
 'Each result sums exactly three elements of its matching row, and the borrowed grid is unchanged.',
 'Index loops are useful when a computation needs coordinates. For plain summation, manual bounds add an avoidable maintenance risk.',
 'The starter returns [3,9] for the sample because it drops each third column.')
lab(11,'field-parser','Require complete numeric fields','bool parseField(const std::string& text, int& output)',
 'Use from_chars to parse one complete decimal int field, with no whitespace or leading plus. Allow a leading minus only for zero (such as -0); the value must be 0 through 100. Reject an empty or partial field and preserve output on failure.',
 '''bool parseField(const std::string& text, int& output) {
    int candidate{};
    auto result = std::from_chars(text.data(), text.data() + text.size(), candidate);
    // Both conversion status and complete consumption are required.
    if (result.ec != std::errc{} || result.ptr != text.data() + text.size()) return false;
    if (candidate < 0 || candidate > 100) return false;
    output = candidate;
    return true;
}''',
 'bool parseField(const std::string& text,int& output){\n int candidate{};auto r=std::from_chars(text.data(),text.data()+text.size(),candidate);\n // BUG: ignores unconsumed trailing bytes.\n if(r.ec!=std::errc{}||candidate<0||candidate>100)return false;\n output=candidate;return true;\n}',
 'int main(){std::string s;std::getline(std::cin,s);int n=8;if(parseField(s,n))std::cout<<n<<"\\n";else std::cout<<"invalid "<<n<<"\\n";}',('42x\n','invalid 8\n'),
 [case('Valid field','([]{int n=8;return parseField("42",n)&&n==42;})()','true'),case('Trailing byte','([]{int n=8;return !parseField("42x",n)&&n==8;})()','true'),case('Leading whitespace','([]{int n=8;return !parseField(" 42",n)&&n==8;})()','true'),case('Upper boundary','([]{int n=8;return parseField("100",n)&&n==100;})()','true'),case('Out of domain','([]{int n=8;return !parseField("101",n)&&n==8;})()','true'),case('Empty','([]{int n=8;return !parseField("",n)&&n==8;})()','true'),case('Leading plus rejected','([]{int n=8;return !parseField("+3",n)&&n==8;})()','true')],
 'from_chars exposes conversion status and the stopping position. A temporary protects the prior output from rejected text.',
 'Success consumes every byte and produces 0 through 100; failure preserves the previous output.',
 'Stream extraction supports a different whitespace policy. It still needs complete-field and domain checks to meet this stricter contract.',
 'The starter accepts 42x and commits 42. Conversion success alone proves only that a numeric prefix was read.')
lab(12,'bounded-vector','Append only inside a logical size limit','bool appendBounded(std::vector<int>& values, int value, std::size_t limit)',
 'Append value only when it is -100 through 100 and size is below limit. Otherwise return false unchanged. limit is at most 100. Allocation failures may propagate; they are not represented by false.',
 '''bool appendBounded(std::vector<int>& values, int value, std::size_t limit) {
    // Logical size is the number of live values, not capacity.
    if (value < -100 || value > 100 || values.size() >= limit) return false;
    values.push_back(value);
    return true;
}''',
 'bool appendBounded(std::vector<int>& values,int value,std::size_t limit){\n if(value<-100||value>100)return false;\n // BUG: size equal to limit is accepted.\n if(values.size()>limit)return false;\n values.push_back(value);return true;\n}',
 'int main(){std::vector<int> v{3,4};std::cout<<std::boolalpha<<appendBounded(v,5,2)<<" "<<v.size()<<"\\n";}',('','false 2\n'),
 [case('At limit unchanged','([]{std::vector<int> v{3,4};return !appendBounded(v,5,2)&&v==std::vector<int>{3,4};})()','true'),case('Below limit','([]{std::vector<int> v{3};return appendBounded(v,4,2)&&v==std::vector<int>{3,4};})()','true'),case('Zero limit','([]{std::vector<int> v;return !appendBounded(v,1,0)&&v.empty();})()','true'),case('Invalid value','([]{std::vector<int> v{1};return !appendBounded(v,101,2)&&v==std::vector<int>{1};})()','true'),case('Capacity is not size','([]{std::vector<int> v;v.reserve(10);return appendBounded(v,1,1)&&v.size()==1;})()','true')],
 'A size check enforces the user-visible bound before push_back. Capacity stays an implementation storage fact.',
 'A false return preserves values and size. Successful append adds exactly one allowed element while staying within limit.',
 'A fixed std::array is better for a truly fixed extent. Here the logical count varies, so a vector fits the requirement.',
 'The starter appends at size equal to limit. It reports true and size 3 for a limit of 2.')
lab(13,'transactional-shift','Shift a collection without partial writes','bool shiftAll(std::vector<int>& values, int delta)',
 'Input elements and delta are -100 through 100. Shift every value by delta only if every result stays in -100 through 100. Otherwise return false with the whole vector unchanged.',
 '''bool shiftAll(std::vector<int>& values, int delta) {
    // Validate the entire batch before the first write.
    for (int value : values) if (value + delta < -100 || value + delta > 100) return false;
    for (int& value : values) value += delta;
    return true;
}''',
 'bool shiftAll(std::vector<int>& values,int delta){\n // BUG: an early value changes before a later rejection.\n for(int& value:values){if(value+delta<-100||value+delta>100)return false;value+=delta;}\n return true;\n}',
 'int main(){std::vector<int> v{1,99};bool ok=shiftAll(v,2);std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\\n";}',('','false 1 99\n'),
 [case('Late failure unchanged','([]{std::vector<int> v{1,99};return !shiftAll(v,2)&&v==std::vector<int>{1,99};})()','true'),case('Successful shift','([]{std::vector<int> v{1,-3};return shiftAll(v,2)&&v==std::vector<int>{3,-1};})()','true'),case('Lower edge','([]{std::vector<int> v{-99};return shiftAll(v,-1)&&v[0]==-100;})()','true'),case('Lower rejection','([]{std::vector<int> v{-100};return !shiftAll(v,-1)&&v[0]==-100;})()','true'),case('Empty succeeds','([]{std::vector<int> v;return shiftAll(v,4)&&v.empty();})()','true')],
 'The first loop proves all integer results fit the domain. The second uses references to update actual elements. The stated small bounds keep addition representable.',
 'Either all elements shift, or no element changes. Validation must complete before mutation begins.',
 'A candidate copy is useful when mutation can fail after validation. Here int assignment does not throw, so two passes avoid the extra copy.',
 'The starter changes 1 to 3 before rejecting 99+2. It returns false with state {3,99}, violating all-or-nothing mutation.')
lab(14,'borrowed-selection','Return a nullable borrowed selection','int* firstAbove(std::vector<int>& values, int threshold)',
 'Return a pointer to the first element greater than threshold, or nullptr if none. Do not change or copy the vector. The pointer borrows its element and must not outlive or survive invalidation of that element.',
 '''int* firstAbove(std::vector<int>& values, int threshold) {
    for (int& value : values) {
        if (value > threshold) return &value; // Borrow the actual element.
    }
    return nullptr;
}''',
 'int* firstAbove(std::vector<int>& values,int threshold){\n // BUG: selects values equal to the threshold too.\n for(int& value:values)if(value>=threshold)return &value;\n return nullptr;\n}',
 'int main(){std::vector<int> v{4,4,9};int* p=firstAbove(v,4);if(p)std::cout<<*p<<"\\n";else std::cout<<"none\\n";}',('','9\n'),
 [case('Strict comparison','([]{std::vector<int> v{4,4,9};return firstAbove(v,4)==&v[2];})()','true'),case('First match','([]{std::vector<int> v{8,9};return firstAbove(v,4)==&v[0];})()','true'),case('No match','([]{std::vector<int> v{2,4};return firstAbove(v,4)==nullptr;})()','true'),case('Empty','([]{std::vector<int> v;return firstAbove(v,0)==nullptr;})()','true'),case('Mutate through borrow','([]{std::vector<int> v{8};int* p=firstAbove(v,4);*p=10;return v[0]==10;})()','true')],
 'A pointer expresses both a live borrowed result and absence. The reference loop obtains addresses of actual vector elements rather than temporary copies.',
 'A successful result points at the first strictly qualifying live element; nullptr represents no match.',
 'An optional index can avoid carrying an address across vector edits. It still requires a valid index and an unchanged intended selection meaning.',
 'The starter selects the first 4 at threshold 4 instead of the later 9. Returning the address of a loop value copy would be a separate dangling-pointer bug.')
lab(15,'owned-buffer','Build an automatically owned sequence','std::unique_ptr<int[]> makeSequence(std::size_t count)',
 'For count 1 through 8 return an owning int array with values 1 through count. For zero return an empty owner. Above 8 throw std::invalid_argument("range"). The caller keeps the count separately.',
 '''std::unique_ptr<int[]> makeSequence(std::size_t count) {
    if (count > 8) throw std::invalid_argument("range");
    if (count == 0) return {};
    auto result = std::make_unique<int[]>(count);
    for (std::size_t i = 0; i < count; ++i) result[i] = static_cast<int>(i) + 1;
    return result; // Transfer sole ownership to the caller.
}''',
 'std::unique_ptr<int[]> makeSequence(std::size_t count){\n if(count>8)throw std::invalid_argument("range");if(count==0)return {};\n auto out=std::make_unique<int[]>(count);\n // BUG: values remain zero-initialized.\n return out;\n}',
 'int main(){auto p=makeSequence(3);std::cout<<p[0]<<" "<<p[1]<<" "<<p[2]<<"\\n";}',('','1 2 3\n'),
 [case('One element','([]{auto p=makeSequence(1);return p&&p[0]==1;})()','true'),case('Every element','([]{auto p=makeSequence(8);for(int i=0;i<8;++i)if(p[i]!=i+1)return false;return true;})()','true'),case('Empty owner','([]{auto p=makeSequence(0);return !p;})()','true'),case('Ownership move','([]{auto p=makeSequence(2);auto q=std::move(p);return !p&&q&&q[1]==2;})()','true'),case('Bound rejected','([]{try{makeSequence(9);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'unique_ptr<int[]> makes sole array ownership explicit. Scope-based destruction releases the array even if later caller work throws.',
 'The returned owner manages exactly count elements; allowed indices are below count. No raw delete is required from the caller.',
 'vector<int> is usually simpler when storing size and collection operations is needed. This exercise isolates ownership transfer from the size policy.',
 'The starter returns an allocated array containing 0 0 0 for the sample. Owning storage correctly does not establish the required element values.')
lab(16,'parcel-state','Validate a record before a state transition','bool releaseParcel(Parcel& parcel)',
 'Define enum class ParcelState { pending, ready } and struct Parcel { int weight; ParcelState state; }. Permit pending to ready only for weight 1 through 100. Every rejection leaves all fields unchanged.',
 '''enum class ParcelState { pending, ready };
struct Parcel { int weight; ParcelState state; };
bool releaseParcel(Parcel& parcel) {
    if (parcel.state != ParcelState::pending || parcel.weight < 1 || parcel.weight > 100) return false;
    parcel.state = ParcelState::ready; // Commit the permitted transition.
    return true;
}''',
 'enum class ParcelState { pending, ready };\nstruct Parcel { int weight; ParcelState state; };\nbool releaseParcel(Parcel& parcel){\n // BUG: changes the state before validating the weight.\n parcel.state=ParcelState::ready;return parcel.weight>=1&&parcel.weight<=100;\n}',
 'int main(){Parcel p{0,ParcelState::pending};bool ok=releaseParcel(p);std::cout<<std::boolalpha<<ok<<" "<<(p.state==ParcelState::pending)<<"\\n";}',('','false true\n'),
 [case('Valid transition','([]{Parcel p{3,ParcelState::pending};return releaseParcel(p)&&p.state==ParcelState::ready;})()','true'),case('Bad weight unchanged','([]{Parcel p{0,ParcelState::pending};return !releaseParcel(p)&&p.weight==0&&p.state==ParcelState::pending;})()','true'),case('Already ready rejected','([]{Parcel p{3,ParcelState::ready};return !releaseParcel(p)&&p.state==ParcelState::ready;})()','true'),case('Upper edge','([]{Parcel p{100,ParcelState::pending};return releaseParcel(p);})()','true'),case('Above upper edge','([]{Parcel p{101,ParcelState::pending};return !releaseParcel(p)&&p.state==ParcelState::pending;})()','true')],
 'A named scoped state type exposes legal states. Validation precedes the one allowed field change.',
 'Only pending parcels with allowed weight become ready; rejected records retain all prior field values.',
 'A class can enforce this rule at every mutation boundary. A public struct is sufficient when callers follow this deliberately narrow operation contract.',
 'The starter rejects weight zero but still changes pending to ready. A false return does not undo the assignment.')
lab(17,'stock-class','Protect a stock invariant','class Stock',
 'Stock(int count) accepts 0 through 100 and otherwise throws std::invalid_argument("range"). count() const reports count. take(int amount) accepts 0 through available count, subtracts it, and returns true; rejection returns false unchanged.',
 '''class Stock {
    int count_;
public:
    explicit Stock(int count) : count_(count) {
        if (count < 0 || count > 100) throw std::invalid_argument("range");
    }
    int count() const { return count_; }
    bool take(int amount) {
        if (amount < 0 || amount > count_) return false;
        count_ -= amount; // Private state changes only after validation.
        return true;
    }
};''',
 'class Stock{int count_;public:\n explicit Stock(int n):count_(n){if(n<0||n>100)throw std::invalid_argument("range");}\n int count()const{return count_;}\n bool take(int amount){\n // BUG: rejects exact depletion.\n if(amount<0||amount>=count_)return false;count_-=amount;return true;\n }\n};',
 'int main(){Stock s(5);bool ok=s.take(5);std::cout<<std::boolalpha<<ok<<" "<<s.count()<<"\\n";}',('','true 0\n'),
 [case('Exact depletion','([]{Stock s(5);return s.take(5)&&s.count()==0;})()','true'),case('Partial take','([]{Stock s(5);return s.take(2)&&s.count()==3;})()','true'),case('Insufficient unchanged','([]{Stock s(5);return !s.take(6)&&s.count()==5;})()','true'),case('Negative unchanged','([]{Stock s(5);return !s.take(-1)&&s.count()==5;})()','true'),case('Zero allowed','([]{Stock s(0);return s.take(0)&&s.count()==0;})()','true'),case('Construction rejects','([]{try{Stock s(101);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'Private storage and validated operations place the count rule at one boundary. The const query lets callers inspect state without a write path.',
 'Every successfully constructed Stock keeps count in 0 through 100. Failed takes preserve the count.',
 'A public int is enough for a local calculation with one trusted owner. It becomes weaker when several callers must preserve the same stock rule.',
 'The starter rejects taking all five available units. It uses >= where the contract allows equality.')
lab(18,'construction','Establish a valid interval at construction','class Interval',
 'Interval(int low,int high) accepts -100 through 100 endpoints with low<=high. Reject invalid construction with std::invalid_argument("range"). low() const and high() const report the endpoints. A one-point interval is valid.',
 '''class Interval {
    int low_, high_;
public:
    Interval(int low, int high) : low_(low), high_(high) {
        if (low < -100 || high > 100 || low > high) throw std::invalid_argument("range");
    }
    int low() const { return low_; }
    int high() const { return high_; }
};''',
 'class Interval{int low_,high_;public:\n Interval(int low,int high):low_(low),high_(high){\n // BUG: rejects equal endpoints.\n if(low<-100||high>100||low>=high)throw std::invalid_argument("range");\n }\n int low()const{return low_;}int high()const{return high_;}\n};',
 'int main(){try{Interval r(3,3);std::cout<<r.low()<<" "<<r.high()<<"\\n";}catch(const std::invalid_argument&){std::cout<<"range\\n";}}',('','3 3\n'),
 [case('One-point interval','([]{Interval r(3,3);return r.low()==3&&r.high()==3;})()','true'),case('Normal interval','([]{Interval r(-2,5);return r.low()==-2&&r.high()==5;})()','true'),case('Full domain','([]{Interval r(-100,100);return r.low()==-100&&r.high()==100;})()','true'),case('Reversed rejected','([]{try{Interval r(5,2);return false;}catch(const std::invalid_argument&){return true;}})()','true'),case('Out of range rejected','([]{try{Interval r(-101,0);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'The initializer list supplies member values, and the constructor rejects a relationship that cannot establish the invariant. No successfully constructed invalid interval escapes.',
 'Every live successfully constructed Interval has -100<=low<=high<=100.',
 'A factory returning an explicit failure value is useful when rejected input is expected control flow. A public unvalidated pair leaves the invariant to every caller.',
 'The starter throws for (3,3), even though equality denotes a valid one-point interval. A constructor must match the stated domain, not a guessed one.')
lab(19,'namespace-contract','Keep a declaration and definition in agreement','int billing::subtotal(int unit, int count)',
 'Declare subtotal inside namespace billing, then supply its matching definition. unit and count are 0 through 100. Return their product. Invalid input throws std::invalid_argument("range"). Keep the driver outside that namespace.',
 '''namespace billing {
    int subtotal(int unit, int count); // Public declaration.
}
int billing::subtotal(int unit, int count) {
    if (unit < 0 || unit > 100 || count < 0 || count > 100) throw std::invalid_argument("range");
    return unit * count;
}''',
 'namespace billing{int subtotal(int unit,int count);}\nint billing::subtotal(int unit,int count){\n if(unit<0||unit>100||count<0||count>100)throw std::invalid_argument("range");\n // BUG: body disagrees with the documented contract.\n return unit+count;\n}',
 'int main(){int u,n;if(!(std::cin>>u>>n))return 2;try{std::cout<<billing::subtotal(u,n)<<"\\n";}catch(const std::invalid_argument&){std::cout<<"range\\n";}}',('7 4\n','28\n'),
 [case('Named interface','billing::subtotal(7,4)','28'),case('Zero count','billing::subtotal(7,0)','0'),case('Single item','billing::subtotal(7,1)','7'),case('Upper bounds','billing::subtotal(100,100)','10000'),case('Reject negative','([]{try{billing::subtotal(-1,2);return false;}catch(const std::invalid_argument&){return true;}})()','true')],
 'A namespace-qualified definition visibly matches the declaration. The supplied offline split files demonstrate separate compilation of this same interface.',
 'The public declaration and definition agree in namespace and parameter types; every accepted result equals unit times count.',
 'A header-only inline definition can fit small utilities. A separately compiled implementation is helpful when clients should depend only on the interface.',
 'The starter links successfully but returns 11 instead of 28. Link success proves a definition exists, not that it implements the contract.')
lab(20,'batch-capstone','Commit inventory batches only after full validation','bool applyBatch(std::vector<int>& stock, const std::vector<Change>& changes)',
 'Define struct Change { std::size_t index; int delta; }. Initial stock values are 0 through 1000; deltas are -1000 through 1000. Apply changes in order to a candidate. Reject any bad index or intermediate count outside 0 through 1000, preserving original stock. Empty batches succeed.',
 '''struct Change { std::size_t index; int delta; };
bool applyBatch(std::vector<int>& stock, const std::vector<Change>& changes) {
    auto candidate = stock; // Original remains intact during validation.
    for (const auto& change : changes) {
        if (change.index >= candidate.size()) return false;
        int next = candidate[change.index] + change.delta;
        if (next < 0 || next > 1000) return false;
        candidate[change.index] = next;
    }
    stock.swap(candidate); // Commit the fully checked batch.
    return true;
}''',
 'struct Change{std::size_t index;int delta;};\nbool applyBatch(std::vector<int>& stock,const std::vector<Change>& changes){\n // BUG: writes directly to original state before knowing the whole batch works.\n for(const auto& c:changes){if(c.index>=stock.size())return false;int n=stock[c.index]+c.delta;if(n<0||n>1000)return false;stock[c.index]=n;}\n return true;\n}',
 'int main(){std::vector<int> v{5,2};bool ok=applyBatch(v,{{0,-1},{1,-3}});std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\\n";}',('','false 5 2\n'),
 [case('Late failure rollback','([]{std::vector<int> v{5,2};return !applyBatch(v,{{0,-1},{1,-3}})&&v==std::vector<int>{5,2};})()','true'),case('Successful batch','([]{std::vector<int> v{5,2};return applyBatch(v,{{0,-1},{1,3}})&&v==std::vector<int>{4,5};})()','true'),case('Bad index rollback','([]{std::vector<int> v{5};return !applyBatch(v,{{0,-1},{1,1}})&&v==std::vector<int>{5};})()','true'),case('Repeated index sequence','([]{std::vector<int> v{5};return applyBatch(v,{{0,-3},{0,2}})&&v[0]==4;})()','true'),case('Intermediate rejection','([]{std::vector<int> v{1};return !applyBatch(v,{{0,-2},{0,2}})&&v[0]==1;})()','true'),case('Empty batch','([]{std::vector<int> v{5};return applyBatch(v,{})&&v[0]==5;})()','true')],
 'A candidate vector owns tentative state. The single commit separates validation from original-state mutation and handles interacting changes in order.',
 'Success commits all changes; any rejection preserves every original element. Every intermediate candidate count must meet the domain.',
 'A carefully proved prevalidation pass can avoid a copy, but repeated indices and intermediate limits make that proof more involved here.',
 'The starter changes stock[0] from 5 to 4 before rejecting the second change. It returns false with partial state {4,2}.')
lab(21,'diagnostic-stage','Identify the earliest failed boundary','std::string failedStage(int compileStatus, int linkStatus, int runStatus)',
 'Given three recorded exit statuses, report compile if compile is nonzero, otherwise link if link is nonzero, otherwise run if run is nonzero, otherwise none. Later statuses are ignored after an earlier failure.',
 '''std::string failedStage(int compileStatus, int linkStatus, int runStatus) {
    if (compileStatus != 0) return "compile";
    if (linkStatus != 0) return "link";
    if (runStatus != 0) return "run";
    return "none";
}''',
 'std::string failedStage(int compileStatus,int linkStatus,int runStatus){\n // BUG: chooses the last failure instead of the earliest.\n if(runStatus!=0)return "run";if(linkStatus!=0)return "link";if(compileStatus!=0)return "compile";return "none";\n}',
 'int main(){int c,l,r;if(!(std::cin>>c>>l>>r))return 2;std::cout<<failedStage(c,l,r)<<"\\n";}',('1 1 1\n','compile\n'),
 [case('Earliest wins','failedStage(1,1,1)','compile'),case('Link failure','failedStage(0,2,3)','link'),case('Run failure','failedStage(0,0,4)','run'),case('All success','failedStage(0,0,0)','none'),case('Negative failure status','failedStage(-1,0,0)','compile')],
 'An ordered guard chain encodes diagnostic priority. It uses recorded status facts rather than guessing from the final output.',
 'The first failed boundary owns the diagnosis; successful later-looking evidence cannot override it.',
 'A structured report retaining all statuses is useful for tooling. Its summary should still identify the earliest actionable failure.',
 'The starter reports run for three failed statuses. That skips the compile failure that must be resolved before later evidence is meaningful.')
lab(22,'transfer','Transfer units as one transaction','bool transfer(std::vector<int>& stock, std::size_t from, std::size_t to, int amount)',
 'Initial counts are 0 through 1000. Reject bad indices, negative amount, insufficient source, or a destination above 1000. A same-index transfer succeeds without change when the amount is available. Any rejection leaves stock unchanged.',
 '''bool transfer(std::vector<int>& stock, std::size_t from, std::size_t to, int amount) {
    if (from >= stock.size() || to >= stock.size() || amount < 0) return false;
    if (amount > stock[from]) return false;
    if (from == to) return true; // No net movement, after availability validation.
    if (amount > 1000 - stock[to]) return false;
    stock[from] -= amount;
    stock[to] += amount;
    return true;
}''',
 'bool transfer(std::vector<int>& stock,std::size_t from,std::size_t to,int amount){\n if(from>=stock.size()||to>=stock.size()||amount<0||amount>stock[from])return false;if(from==to)return true;\n // BUG: withdraws before destination validation.\n stock[from]-=amount;if(amount>1000-stock[to])return false;stock[to]+=amount;return true;\n}',
 'int main(){std::vector<int> v{10,999};bool ok=transfer(v,0,1,2);std::cout<<std::boolalpha<<ok<<" "<<v[0]<<" "<<v[1]<<"\\n";}',('','false 10 999\n'),
 [case('Destination rejection unchanged','([]{std::vector<int> v{10,999};return !transfer(v,0,1,2)&&v==std::vector<int>{10,999};})()','true'),case('Valid movement','([]{std::vector<int> v{10,2};return transfer(v,0,1,3)&&v==std::vector<int>{7,5};})()','true'),case('Same index no change','([]{std::vector<int> v{10};return transfer(v,0,0,3)&&v[0]==10;})()','true'),case('Same index insufficient','([]{std::vector<int> v{10};return !transfer(v,0,0,11)&&v[0]==10;})()','true'),case('Bad index','([]{std::vector<int> v{10};return !transfer(v,0,1,1)&&v[0]==10;})()','true'),case('Negative amount','([]{std::vector<int> v{10,0};return !transfer(v,0,1,-1)&&v==std::vector<int>{10,0};})()','true')],
 'Complete prevalidation works because the subsequent bounded int updates cannot fail. Same-object aliasing is handled before the two writes.',
 'A successful distinct-index transfer preserves total units. Rejection and same-index success preserve all counts.',
 'A candidate copy works for more complex transactions, but these two bounded, nonthrowing writes need only a complete precheck.',
 'The starter subtracts two from the source before discovering a full destination. It rejects with {8,999}, losing two units.')
lab(23,'cleanup-trace','Trace cleanup of incomplete construction','std::string cleanupTrace(bool fail)',
 'Record member construction A+, B+, then owner body. On success record owner-, B-, A-. If the owner body throws, record B-, A-, caught with no owner- entry. Return the entries joined by single spaces.',
 '''struct LoggedMember {
    std::string name;
    std::vector<std::string>& log;
    LoggedMember(std::string n, std::vector<std::string>& out) : name(std::move(n)), log(out) { log.push_back(name + "+"); }
    ~LoggedMember() { log.push_back(name + "-"); }
};
struct LoggedOwner {
    std::vector<std::string>& log;
    LoggedMember a, b;
    LoggedOwner(std::vector<std::string>& out, bool fail) : log(out), a("A", out), b("B", out) {
        log.push_back("body");
        if (fail) throw std::runtime_error("construction");
    }
    ~LoggedOwner() { log.push_back("owner-"); }
};
std::string cleanupTrace(bool fail) {
    // Reserve enough log entries before any destructor records its message.
    std::vector<std::string> log;
    log.reserve(8);
    try { LoggedOwner owner(log, fail); }
    catch (const std::runtime_error&) { log.push_back("caught"); }
    std::string joined;
    for (const auto& event : log) { if (!joined.empty()) joined += ' '; joined += event; }
    return joined;
}''',
 'std::string cleanupTrace(bool fail){\n // BUG: invents owner destruction for an incomplete object.\n return fail ? "A+ B+ body owner- B- A- caught" : "A+ B+ body owner- B- A-";\n}',
 'int main(){std::cout<<cleanupTrace(true)<<"\\n";}',('','A+ B+ body B- A- caught\n'),
 [case('Failure cleanup','cleanupTrace(true)','A+ B+ body B- A- caught'),case('Successful cleanup','cleanupTrace(false)','A+ B+ body owner- B- A-'),case('Repeated runs independent','cleanupTrace(true)==cleanupTrace(true)','true'),case('No completed owner on failure','cleanupTrace(true).find("owner-")==std::string::npos','true')],
 'The log outlives the owner and its members. Actual constructors and destructors produce the trace, so the evidence follows the language cleanup rules.',
 'Completed members are destroyed in reverse order. An owner whose constructor throws never runs its own destructor.',
 'A hard-coded trace can predict one case but does not demonstrate real cleanup. Production cleanup should use a nonthrowing logging policy; this bounded test assumes log allocation succeeds.',
 'The starter includes owner- on failure, claiming destruction of an object that never completed construction.')
lab(24,'acceptance','Do not accept an empty checklist','bool acceptanceComplete(const std::vector<bool>& checks)',
 'Return true only when at least one required check was supplied and every supplied result is true. An empty list is incomplete, not proof of acceptance. This models recorded checks; it does not execute the application tests.',
 '''bool acceptanceComplete(const std::vector<bool>& checks) {
    if (checks.empty()) return false; // Missing evidence is not acceptance.
    for (bool passed : checks) if (!passed) return false;
    return true;
}''',
 'bool acceptanceComplete(const std::vector<bool>& checks){\n // BUG: vacuous success accepts missing evidence.\n for(bool passed:checks)if(!passed)return false;return true;\n}',
 'int main(){std::cout<<std::boolalpha<<acceptanceComplete({})<<"\\n";}',('','false\n'),
 [case('Missing evidence','acceptanceComplete({})','false'),case('One pass','acceptanceComplete({true})','true'),case('All pass','acceptanceComplete({true,true,true})','true'),case('One fail','acceptanceComplete({true,false,true})','false'),case('Only fail','acceptanceComplete({false})','false')],
 'A nonempty check makes the evidence requirement explicit before a simple all-results scan.',
 'Success means evidence exists and every supplied required check passed. It does not assert that unlisted requirements were tested.',
 'A report with named requirements and missing-result states is stronger for a real release. This Boolean summary is a small exercise about the acceptance boundary.',
 'The starter returns true for an empty vector. The loop has no failing result, but the contract also requires actual evidence.')
