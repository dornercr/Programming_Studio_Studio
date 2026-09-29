"""Chapter-specific extension labs. Independent complete C++17 reference programs."""
import json
from pathlib import Path
from teaching_examples import ROWS,headers
ROOT=Path(__file__).resolve().parents[1]
LABS={}
def lab(n,title,task,checks,definitions,main,output,reason,bug,repair,terms,extra=''):
 code=headers(definitions+main)+''.join('#include <'+h+'>\n' for h in extra.split() if h)+ '\n'+definitions.strip()+'\n\nint main() {\n'+main.strip('\n')+'\n}\n'
 LABS[n]=dict(title=title,task=task,checks=checks,code=code,output=output,reason=reason,bug=bug,repair=repair,terms=[dict(term=a,definition=b) for a,b in terms])
lab(1,'Separate calculation from presentation','Extract a reusable sum operation and a formatter. Changing the label must not change the numeric result. Inputs are small integers whose sum fits in int.',
 ['sum(7,5) is 12; sum(-2,5) is 3.','The formatter accepts a caller-supplied label.','Both formatted results include the computed value.'],r'''
int sum(int first, int second) { return first + second; }
std::string format(const std::string& label, int value) {
    return label + "=" + std::to_string(value);
}
''',r'''
    assert(sum(7,5) == 12 && sum(-2,5) == 3);
    assert(format("sum",3) == "sum=3");
    std::cout << format("total",sum(7,5)) << '\n';
    std::cout << format("sum",sum(-2,5)) << '\n';
''','total=12\nsum=3\n',
 'The calculator returns a value and knows nothing about streams. The formatter turns that value into owned text. Tests can check each boundary independently; a different output destination need not alter addition. The compiler may inline both functions, so this source-level separation does not promise extra machine calls.',
 'Put the label into the arithmetic function and return only formatted text. A numeric caller then has to parse the text again.',
 'Return an int from calculation and create text at the presentation boundary. Keep the input range explicit; these tests do not prove signed addition cannot overflow for arbitrary inputs.',
 [('Contract','The inputs, results, and rules that callers and implementations agree to.'),('Layer','A part of a system that performs one kind of work through a defined boundary.')])
lab(2,'Make copying and borrowing visible','Write one function that returns a changed copy and one that changes the caller’s vector. Reject an empty vector before accessing its first element.',
 ['copy_changed leaves the caller unchanged.','change_in_place updates the caller.','Both functions reject an empty vector.'],r'''
std::vector<int> copy_changed(std::vector<int> values, int replacement) {
    if(values.empty()) throw std::invalid_argument("empty");
    values[0] = replacement;
    return values;
}
void change_in_place(std::vector<int>& values, int replacement) {
    if(values.empty()) throw std::invalid_argument("empty");
    values[0] = replacement;
}
''',r'''
    std::vector<int> source{2,4};
    const auto copy = copy_changed(source,9);
    assert(source[0] == 2 && copy[0] == 9);
    change_in_place(source,7);
    std::vector<int> empty;
    int rejected = 0;
    try { copy_changed(empty,1); } catch(const std::invalid_argument&) { ++rejected; }
    try { change_in_place(empty,1); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2 && source[0] == 7);
    std::cout << "source=" << source[0] << " copy=" << copy[0] << " rejected=" << rejected << '\n';
''','source=7 copy=9 rejected=2\n',
 'A value parameter creates a separate vector object. A reference parameter, marked &, names the existing vector. The empty check is a precondition check performed before indexing; throwing leaves the original vector unchanged. Return-by-value lets the returned vector own its storage without a dangling reference.',
 'Remove & from change_in_place. The function changes only its local copy, and the caller still sees 2.',
 'Use a reference for the in-place operation and a value parameter for the copy operation. Name the operations so a caller can see the difference.',
 [('Value parameter','A parameter object initialized from the argument; this vector has independent element storage.'),('Reference parameter','Another name for the caller’s object, with no new copy of that object.')],extra='stdexcept')
lab(3,'Validate a packed-field setter','Create a setter for bits 4–7 of an unsigned word. Reject values above 15 and preserve all bits outside that field.',
 ['0xA5 with field 3 becomes 0x35.','Writing 0 and 15 clears or fills the selected field.','16 is rejected and neighboring bits stay unchanged.'],r'''
unsigned set_field(unsigned word, unsigned value) {
    if(value > 15u) throw std::invalid_argument("four-bit field");
    const unsigned mask = 0xFu << 4;
    return (word & ~mask) | (value << 4);
}
''',r'''
    assert(set_field(0xA5u,3u) == 0x35u);
    assert(set_field(0xA5u,0u) == 0x05u);
    assert(set_field(0xA5u,15u) == 0xF5u);
    assert((set_field(0x1234u,9u) & ~(0xFu << 4)) == (0x1234u & ~(0xFu << 4)));
    bool rejected = false;
    try { set_field(0,16); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << std::hex << set_field(0xA5u,3u) << " rejected=" << rejected << '\n';
''','35 rejected=1\n',
 'The setter clears the old four-bit field before inserting the new one. The two groups do not overlap, so bitwise OR combines them without carrying. Unsigned operations avoid sign-extension questions. The check belongs inside the setter because every caller must obey the same width rule.',
 'Use word | (value << 4) without clearing. Setting 0xA5 to field 3 produces 0xB5.',
 'Clear with word & ~mask, then insert with OR. Test clearing to zero, which immediately exposes a setter that can only add one-bits.',
 [('Mask','A bit pattern selecting which bit positions an operation should affect.'),('Bit field','A fixed group of bits used to store one value within a larger word.')],extra='stdexcept')
lab(4,'Return a checked byte count','Write checked_bytes(count,width,limit). Return no value when multiplication would exceed limit; zero-width requests return zero.',
 ['8×12 with limit 100 returns 96.','9×12 is rejected.','Zero count and zero width are accepted as zero.','The maximum size_t count times two is rejected against the type maximum.'],r'''
std::optional<std::size_t> checked_bytes(std::size_t count, std::size_t width,
                                        std::size_t limit) {
    if(width != 0 && count > limit / width) return std::nullopt;
    return count * width;
}
''',r'''
    const auto accepted = checked_bytes(8,12,100);
    assert(accepted && *accepted == 96);
    assert(!checked_bytes(9,12,100));
    assert(checked_bytes(0,12,100) == 0 && checked_bytes(9,0,100) == 0);
    const auto maximum = std::numeric_limits<std::size_t>::max();
    assert(!checked_bytes(maximum,2,maximum));
    std::cout << "bytes=" << *accepted << " oversized=rejected\n";
''','bytes=96 oversized=rejected\n',
 'The check divides before multiplying, so the product is formed only when it is within the caller’s limit. optional distinguishes a successful zero from rejection. A checked byte count still does not promise memory is available: allocation failure is a separate boundary.',
 'Multiply first and compare the result with limit. An unsigned wrapped result can be small enough to pass.',
 'When width is nonzero, require count <= limit/width before forming the product. Use a separate state to represent rejection.',
 [('Unsigned wrap','Unsigned arithmetic reduces a result modulo one more than the type’s maximum value.'),('Precondition','A rule that must hold before an operation can safely perform its work.')])
lab(5,'Compare nearby floating-point results','Implement a relative-and-absolute closeness test for finite doubles. Make the tolerances explicit and reject negative tolerances. Exact equality handles equal infinities before the finite-only calculation.',
 ['0.1+0.2 and 0.3 are close under 1e-12 tolerance.','A small absolute tolerance works near zero.','Distinct large values beyond the selected relative tolerance are not close.','NaN is not close to any value; negative tolerances are rejected.'],r'''
bool close(double a, double b, double absolute, double relative) {
    if(!(absolute >= 0) || !(relative >= 0)) throw std::invalid_argument("tolerance");
    if(a == b) return true;
    if(!std::isfinite(a) || !std::isfinite(b)) return false;
    const double scale = std::max(std::abs(a),std::abs(b));
    return std::abs(a-b) <= std::max(absolute,relative*scale);
}
''',r'''
    assert(close(0.1+0.2,0.3,1e-12,1e-12));
    assert(close(0,1e-14,1e-12,0));
    assert(!close(1000000,1000010,0,1e-8));
    assert(!close(std::numeric_limits<double>::quiet_NaN(),0,1,1));
    bool rejected = false;
    try { close(1,1,-1,0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "decimal=close near-zero=close far=distinct\n";
''','decimal=close near-zero=close far=distinct\n',
 'An absolute tolerance gives a useful bound near zero; a relative tolerance scales with magnitude. The application chooses both from its error budget. Closeness is not a universal equivalence relation: it can fail transitivity, so it must not replace ordering in a sorted container. This lab covers modest finite magnitudes; a fully general comparator must also avoid overflow in difference and tolerance calculations.',
 'Use a fixed epsilon as a universal rule for all magnitudes. A bound useful near 1 can be too tight or too loose elsewhere.',
 'State an error budget and test both near-zero and scaled values. Keep NaN handling explicit rather than letting an accidental comparison decide policy.',
 [('Rounding error','The difference caused when an exact result must be stored as one of the available floating-point values.'),('Relative tolerance','An allowed error scaled to the magnitude of the compared values.')],extra='cmath stdexcept')
lab(6,'Compute a checked row-major index','Map a row and column into a flat vector with a known number of columns. Check dimensions and bounds before accessing the data.',
 ['The (1,2) element of a 2×3 array is 6.','Rows and columns outside the shape are rejected.','The data length must match the declared shape.'],r'''
int at_grid(const std::vector<int>& data, std::size_t rows, std::size_t cols,
            std::size_t row, std::size_t col) {
    if(cols != 0 && rows > data.size()/cols) throw std::invalid_argument("shape");
    if(rows*cols != data.size()) throw std::invalid_argument("shape");
    if(row >= rows || col >= cols) throw std::out_of_range("coordinate");
    return data[row*cols+col];
}
''',r'''
    const std::vector<int> data{1,2,3,4,5,6};
    assert(at_grid(data,2,3,1,2) == 6);
    int rejected = 0;
    try { at_grid(data,2,3,2,0); } catch(const std::out_of_range&) { ++rejected; }
    try { at_grid(data,2,3,0,3); } catch(const std::out_of_range&) { ++rejected; }
    try { at_grid(data,3,3,0,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 3);
    std::cout << "value=" << at_grid(data,2,3,1,2) << " rejected=" << rejected << '\n';
''','value=6 rejected=3\n',
 'The logical shape is part of the indexing contract; a flat allocation alone cannot prove coordinates are valid. Once the shape and coordinates are checked, row*cols+col selects the row-major element. Multiplication is bounded by data.size() before it is used. The vector owns the storage while the const reference only borrows it.',
 'Use row+col instead of row*cols+col. Coordinate (1,2) then reads 4 instead of 6.',
 'Count complete rows before adding the position inside the selected row. Test a non-square grid so swapped dimensions do not hide the error.',
 [('Row-major layout','Consecutive columns of one row occupy consecutive element positions.'),('Alignment','A rule about which addresses are valid starting points for an object type.')],extra='stdexcept')
lab(7,'Traverse empty and nonempty ranges','Implement a bounded sum using vector iterators. Use a wide accumulator for this small-int fixture and support an empty range without dereferencing its end.',
 ['{3,5,7} sums to 15.','The empty vector sums to zero.','{-4,4} sums to zero.'],r'''
long long sum_range(const std::vector<int>& values) {
    long long total = 0;
    for(auto cursor = values.begin(); cursor != values.end(); ++cursor)
        total += *cursor;
    return total;
}
''',r'''
    assert(sum_range({3,5,7}) == 15);
    assert(sum_range({}) == 0);
    assert(sum_range({-4,4}) == 0);
    std::cout << "nonempty=" << sum_range({3,5,7}) << " empty=" << sum_range({}) << '\n';
''','nonempty=15 empty=0\n',
 'A half-open range stops before its end iterator. For an empty vector, begin equals end, so no dereference occurs. The const reference prevents this function from resizing the vector. That matters because reallocation could invalidate its iterators. A wider accumulator reduces overflow risk for small fixtures but does not make an unbounded sum safe.',
 'Use <= instead of != on random-access iterators in this loop. It tries to dereference end.',
 'Stop at end, and test the empty range before trusting a traversal. There is no valid element to read from an empty range.',
 [('Half-open range','A range including its first position but excluding its end position.'),('Iterator','An object representing a position that a traversal can examine and advance.')])
lab(8,'Return an owner across a function boundary','Create a unique owner in a factory function, return it, move it to a second owner, and show the original owner is empty afterward.',
 ['make_value(42) returns a non-null owner.','The value survives the factory’s local scope.','After a move, only the destination owner retains the object.'],r'''
std::unique_ptr<int> make_value(int value) { return std::make_unique<int>(value); }
''',r'''
    auto first = make_value(42);
    assert(first && *first == 42);
    auto second = std::move(first);
    assert(!first && second && *second == 42);
    std::cout << "value=" << *second << " first-empty=" << !first << '\n';
''','value=42 first-empty=1\n',
 'The allocated int and the local unique_ptr are different objects. Returning the owner transfers responsibility without returning the address of a dying local int. std::move allows transfer; the unique_ptr operation performs it. The second owner releases the int when its scope ends, including during stack unwinding.',
 'Return the address of a local int from the factory. It points at an object whose lifetime ends when the function returns.',
 'Return the value itself when ownership is unnecessary, or return an owning handle for a separately allocated object. A raw address cannot extend lifetime.',
 [('Object lifetime','The interval during which a constructed object may be used according to its type’s rules.'),('Ownership transfer','Moving responsibility for releasing a resource from one owner to another.')])
lab(9,'Grow a collection under a hard limit','Append an element only when a vector is below a fixed maximum size. Rejection must leave the vector unchanged. Do not retain an element pointer while growing it.',
 ['A size-two vector accepts its third element with maximum three.','The next append is rejected without changing existing values.','A maximum of zero rejects even the first element.'],r'''
bool append_bounded(std::vector<int>& values, int value, std::size_t maximum) {
    if(values.size() >= maximum) return false;
    values.push_back(value);
    return true;
}
''',r'''
    std::vector<int> values{4,6};
    assert(append_bounded(values,8,3));
    const auto before = values;
    assert(!append_bounded(values,10,3) && values == before);
    std::vector<int> empty;
    assert(!append_bounded(empty,1,0));
    std::cout << "size=" << values.size() << " last=" << values.back() << " limit=preserved\n";
''','size=3 last=8 limit=preserved\n',
 'The limit is a bound on live elements, not on reserved capacity. A rejection occurs before push_back, so it cannot partially alter the sequence. The vector handles storage growth and release. This function may still throw if allocation fails; its Boolean reports only the explicit size-limit decision.',
 'Compare capacity() with maximum. Reserving spare storage can then reject a valid append or confuse storage with live elements.',
 'Use size() for an element-count rule. Keep allocation failures distinct from an intentional capacity policy.',
 [('Size','The number of live elements in a container.'),('Capacity','The number of elements that fit in currently reserved vector storage without reallocation.')])
lab(10,'Build a bounded aligned arena model','Implement reserve_bytes(used,capacity,request) for eight-byte-aligned starts. Return no start on failure and leave used unchanged. This models offsets, not actual object construction.',
 ['Requests 5 and 9 start at offsets 0 and 8.','A request that crosses the capacity is rejected.','A failed request leaves used unchanged.'],r'''
std::optional<std::size_t> reserve_bytes(std::size_t& used, std::size_t capacity,
                                        std::size_t request) {
    if(used > capacity) return std::nullopt;
    const auto padding = (8 - used%8)%8;
    if(padding > capacity-used) return std::nullopt;
    const auto start = used+padding;
    if(request > capacity-start) return std::nullopt;
    used = start+request;
    return start;
}
''',r'''
    std::size_t used = 0;
    const auto first = reserve_bytes(used,32,5);
    const auto second = reserve_bytes(used,32,9);
    assert(first == 0 && second == 8 && used == 17);
    const auto before = used;
    assert(!reserve_bytes(used,32,20) && used == before);
    std::cout << "starts=" << *first << ',' << *second << " used=" << used << '\n';
''','starts=0,8 used=17\n',
 'Padding is computed separately and checked against remaining space before addition. The state changes only after every bound passes, which makes failure leave the arena usable. Returning an optional offset distinguishes rejection from a valid start at zero. This bump model has no individual free operation and does not create C++ objects in raw storage.',
 'Update used before checking the request. A rejected allocation then consumes or corrupts the remaining arena state.',
 'Compute candidate offsets in local values and commit used only after the last bound check. Test failure after one successful allocation.',
 [('Fragmentation','Space that exists but cannot be used for the desired request under an allocator’s rules.'),('Commit point','The operation at which a prepared change becomes the visible new state.')])
lab(11,'Check a copy before touching the destination','Copy a source string into an existing character array only if the characters and trailing null fit. Return false without writing anything when the capacity is too small.',
 ['cat needs capacity four including its trailing null.','Capacity three rejects cat.','Rejection preserves every destination byte.','An empty string still needs one byte for the null.'],r'''
bool copy_text(char* destination, std::size_t capacity, const std::string& source) {
    if(capacity == 0 || source.size() >= capacity) return false;
    std::copy(source.begin(),source.end(),destination);
    destination[source.size()] = '\0';
    return true;
}
''',r'''
    std::array<char,4> destination{'x','x','x','x'};
    const auto before = destination;
    assert(!copy_text(destination.data(),3,"cat") && destination == before);
    assert(copy_text(destination.data(),4,"cat"));
    assert(std::string(destination.data()) == "cat");
    std::array<char,1> empty{'x'};
    assert(copy_text(empty.data(),1,"") && empty[0] == '\0');
    assert(!copy_text(nullptr,0,""));
    std::cout << destination.data() << " rejected=unchanged\n";
''','cat rejected=unchanged\n',
 'The guard avoids computing source.size()+1, which could overflow at the type boundary. The caller must supply a live writable range of capacity bytes when capacity is nonzero. A check on a number cannot prove an arbitrary pointer is valid. An owned string is simpler when the external interface does not require a caller-owned buffer.',
 'Accept source.size() <= capacity. An exactly full source then writes its null terminator one byte past the destination.',
 'Require source.size() < capacity and test the exact-fit boundary including the terminator.',
 [('Bounds error','An access outside the valid element range of an object.'),('Undefined behavior','A program operation for which the C++ standard imposes no behavior requirements.')])
lab(12,'Turn the instruction trace into a bounded interpreter','Interpret Load, Add, and Halt in a tiny model. Reject a program that runs past its instruction array without halting. Arithmetic inputs are limited to small values with representable results.',
 ['LOAD 4, ADD 3, HALT returns 7.','HALT alone returns the initial zero.','A program without HALT is rejected.'],r'''
enum class Op { Load, Add, Halt };
struct Instruction { Op op; int operand; };
int execute(const std::vector<Instruction>& program) {
    int accumulator = 0;
    for(const auto& instruction : program) {
        switch(instruction.op) {
            case Op::Load: accumulator = instruction.operand; break;
            case Op::Add: accumulator += instruction.operand; break;
            case Op::Halt: return accumulator;
        }
    }
    throw std::invalid_argument("missing halt");
}
''',r'''
    const int result = execute({{Op::Load,4},{Op::Add,3},{Op::Halt,0}});
    assert(result == 7 && execute({{Op::Halt,0}}) == 0);
    bool rejected = false;
    try { execute({{Op::Load,4}}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "acc=" << result << " missing-halt=rejected\n";
''','acc=7 missing-halt=rejected\n',
 'An enum gives each modeled operation a distinct name. A small struct now earns its place because an instruction consists of both an operation and an operand. The switch defines the instruction semantics while the vector traversal supplies the next instruction. The model is deliberately not a binary encoding or a real processor pipeline.',
 'Fall through from Load into Add by omitting break. LOAD 4 then leaves 8 rather than 4 in the accumulator.',
 'End each non-returning case explicitly and test a one-instruction effect before testing a whole program.',
 [('Architectural state','The registers and other state whose behavior an instruction set promises to software.'),('Instruction set','The operations and rules a machine exposes to programs.')],extra='stdexcept')
lab(13,'Make subregister width an explicit input','Model writes of 16, 32, or 64 bits to a 64-bit x86 register. A 32-bit write clears the high half; a 16-bit write preserves the other bits. Reject unsupported widths.',
 ['Writing 7 at width 32 produces 7.','Writing 7 at width 16 preserves the old high bits.','Width 64 replaces the entire value.','Width 8 is rejected by this deliberately limited model.'],r'''
std::uint64_t write_register(std::uint64_t old, std::uint64_t value, unsigned width) {
    if(width == 64) return value;
    if(width == 32) return static_cast<std::uint32_t>(value);
    if(width == 16) return (old & ~std::uint64_t{0xFFFF}) | (value & 0xFFFFu);
    throw std::invalid_argument("unsupported width");
}
''',r'''
    const std::uint64_t old = 0xFFFFFFFF00000000ULL;
    assert(write_register(old,7,32) == 7);
    assert(write_register(old,7,16) == 0xFFFFFFFF00000007ULL);
    assert(write_register(old,9,64) == 9);
    bool rejected = false;
    try { write_register(old,7,8); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << std::hex << write_register(old,7,32) << ' ' << write_register(old,7,16) << '\n';
''','7 ffffffff00000007\n',
 'The write width is part of the operation, so the function receives it explicitly instead of guessing from the numeric value. Masking the new low bits prevents them from affecting preserved bits. Rejecting widths the model does not implement is more honest than silently applying a different rule. This models ISA behavior; it does not force the compiler to use a specific register.',
 'Preserve the old high half for a 32-bit write. That models an AX-like partial update, not an EAX write.',
 'Treat width 32 as zero extension of the low 32-bit value. Test with nonzero old high bits so the distinction is observable.',
 [('Subregister','A named portion of a larger machine register.'),('Zero extension','Increasing a value’s width by filling the new high bits with zeros.')],extra='stdexcept')
lab(14,'Check an effective-address calculation','Compute base+index*scale+displacement under a caller-supplied upper bound. The result is a numeric model, not a dereferenceable C++ pointer.',
 ['1000 + 3×4 + 8 is 1020 under limit 2000.','Reject a scaled index or displacement that exceeds remaining range.','Zero index works without requiring a transfer of data.'],r'''
std::optional<std::size_t> effective(std::size_t base, std::size_t index,
    std::size_t scale, std::size_t displacement, std::size_t limit) {
    if(base > limit) return std::nullopt;
    if(scale != 0 && index > (limit-base)/scale) return std::nullopt;
    const auto partial = base+index*scale;
    if(displacement > limit-partial) return std::nullopt;
    return partial+displacement;
}
''',r'''
    const auto result = effective(1000,3,4,8,2000);
    assert(result == 1020);
    assert(!effective(1000,300,4,0,2000));
    assert(!effective(1999,0,4,2,2000));
    assert(effective(10,0,4,2,20) == 12);
    std::cout << "address=" << *result << " invalid=rejected\n";
''','address=1020 invalid=rejected\n',
 'The model separates computing a value from accessing memory. Every addition is preceded by a remaining-range check, and multiplication is bounded before it occurs. A real addressing mode has its own width and validity rules. Passing these numeric checks alone does not create a live C++ object at the resulting number.',
 'Dereference the computed integer by casting it to a pointer. Numeric arithmetic has not proved that such an object exists.',
 'Keep this model numeric. Use a valid array or OS mapping when the task actually requires reading storage.',
 [('Effective address','The address value formed from the components of a machine addressing mode.'),('Displacement','A fixed offset included in an address calculation.')])
lab(15,'Guard signed division boundaries','Return quotient and remainder for int inputs. Reject division by zero and the minimum int divided by -1, whose quotient cannot fit in int.',
 ['-17/5 returns quotient -3 and remainder -2.','17/-5 returns -3 and 2.','Zero divisor and the unrepresentable minimum/-1 case are rejected.'],r'''
std::optional<std::pair<int,int>> divide(int value, int divisor) {
    if(divisor == 0) return std::nullopt;
    if(value == std::numeric_limits<int>::min() && divisor == -1) return std::nullopt;
    return std::pair<int,int>{value/divisor,value%divisor};
}
''',r'''
    const auto first = divide(-17,5);
    const auto second = divide(17,-5);
    assert(first && first->first == -3 && first->second == -2);
    assert(second && second->first == -3 && second->second == 2);
    assert(!divide(1,0));
    assert(!divide(std::numeric_limits<int>::min(),-1));
    std::cout << first->first << ' ' << first->second << " invalid=rejected\n";
''','-3 -2 invalid=rejected\n',
 'The guards run before either / or %, since both need a valid divisor and representable quotient. optional marks rejection separately from a legitimate zero quotient. For valid cases, quotient*divisor+remainder equals the dividend; the remainder follows the dividend’s sign unless it is zero.',
 'Check only divisor != 0. The minimum-int/-1 case is still not representable.',
 'Check the exceptional signed boundary as well as zero. Perform the checks before evaluating either arithmetic expression.',
 [('Quotient','The integer result of division after truncation toward zero in this C++ operation.'),('Remainder','The part left after multiplying the integer quotient by the divisor.')])
lab(16,'Generalize a loop with a clear stopping rule','Sum integers in [0,limit) for limits up to 1000. Reject larger limits so the accumulator’s range is easy to justify.',
 ['Limit zero returns zero without entering the body.','Limits one and four return zero and six.','Limit 1000 returns 499500; 1001 is rejected.'],r'''
unsigned sum_before(unsigned limit) {
    if(limit > 1000) throw std::invalid_argument("limit");
    unsigned total = 0;
    for(unsigned i = 0; i < limit; ++i) total += i;
    return total;
}
''',r'''
    assert(sum_before(0) == 0 && sum_before(1) == 0);
    assert(sum_before(4) == 6 && sum_before(1000) == 499500);
    bool rejected = false;
    try { sum_before(1001); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "empty=" << sum_before(0) << " four=" << sum_before(4) << '\n';
''','empty=0 four=6\n',
 'The invariant before each loop test is that total contains the sum of all indexes less than i. The body adds i once, and the increment moves the boundary forward. At termination i equals limit. The stated cap keeps the result representable on the required implementation, which this lab checks with a static assertion in the generated program if needed.',
 'Use i <= limit. For limit four the result becomes ten instead of six.',
 'Use the half-open condition i < limit. Include limit zero in the tests to expose extra-iteration mistakes.',
 [('Loop invariant','A rule maintained before every repetition, used to explain the loop’s result.'),('Control flow','The order in which a program can execute its operations.')],extra='stdexcept')
lab(17,'Represent a call contract in C++','Write a helper that doubles a small integer and a caller that preserves a separate value across that call. Test positive, zero, and negative inputs.',
 ['caller(10,3) is 16.','caller(10,0) is 10.','caller(10,-3) is 4.','The caller’s saved value is never overwritten by the helper.'],r'''
int helper(int argument) { return argument*2; }
int caller(int saved, int argument) {
    const int returned = helper(argument);
    return saved+returned;
}
''',r'''
    assert(caller(10,3) == 16 && caller(10,0) == 10 && caller(10,-3) == 4);
    std::cout << caller(10,3) << ' ' << caller(10,0) << ' ' << caller(10,-3) << '\n';
''','16 10 4\n',
 'The caller relies on a returned value and its own preserved value. The C++ compiler chooses how to satisfy that rule under the target ABI, possibly by eliminating the machine call entirely. Studying callee-saved registers requires inspecting a concrete build; the source-level contract is still the same if the helper is inlined.',
 'In handwritten assembly, keep saved only in a caller-saved register and assume a nested call preserves it.',
 'Follow the actual ABI’s preservation duties or let ordinary C++ express the call. A source-variable name is not a register-preservation promise.',
 [('ABI','The binary-level agreement for calls, object layout, symbols, and other machine interfaces.'),('Callee-saved register','A register whose incoming value a callee must restore when required by the relevant ABI.')])
lab(18,'Test an alias-sensitive rewrite','Implement write_then_read(a,b) and verify both separate objects and two references to the same object. Do not move the read before the write.',
 ['Separate a=4,b=6 returns 6 and changes a to 9.','When a and b alias x=4, the return is 9.','A cached-before-write variant returns 4 for the aliasing case and is not equivalent.'],r'''
int write_then_read(int& a, const int& b) { a = 9; return b; }
int cached_before(int& a, const int& b) { const int old = b; a = 9; return old; }
''',r'''
    int a = 4, b = 6;
    assert(write_then_read(a,b) == 6 && a == 9);
    int x = 4;
    const int correct = write_then_read(x,x);
    x = 4;
    const int changed = cached_before(x,x);
    assert(correct == 9 && changed == 4);
    std::cout << "ordered=" << correct << " reordered=" << changed << '\n';
''','ordered=9 reordered=4\n',
 'A const reference prevents mutation through that reference; it does not prove the underlying object cannot change through another name. The write through a can therefore affect the later read through b. Optimization must preserve this allowed input case unless an additional valid non-aliasing contract excludes it.',
 'Cache b before assigning a and assume distinct parameter names mean distinct objects.',
 'Preserve the order or supply and enforce a stronger non-overlap contract. Test both aliasing and non-aliasing arguments.',
 [('Aliasing','Two access paths referring to the same underlying object or storage.'),('Observable behavior','The effects that a valid optimization must preserve under the language’s rules.')])
lab(19,'Validate a privileged-style range request','Model a protected service that accepts a length only when a numeric offset and extent fit inside a 16-byte allowed region. Reject both oversized lengths and invalid starting offsets.',
 ['Offset 4,length 8 is allowed.','Offset 16,length 0 is allowed as an empty end range.','Offset 17 is rejected; offset 15,length 2 is rejected.','A maximum-size request cannot bypass the check by wrapping.'],r'''
bool allowed(std::size_t offset, std::size_t length, std::size_t extent) {
    return offset <= extent && length <= extent-offset;
}
''',r'''
    assert(allowed(4,8,16));
    assert(allowed(16,0,16));
    assert(!allowed(17,0,16) && !allowed(15,2,16));
    assert(!allowed(1,std::numeric_limits<std::size_t>::max(),16));
    std::cout << "valid=accepted outside=rejected\n";
''','valid=accepted outside=rejected\n',
 'The trusted boundary checks the whole requested range, not only its length. The offset check comes first so extent-offset cannot underflow. This numeric model still cannot verify a real caller pointer or prevent a concurrent change to a mapping; an actual kernel copy operation must use the OS’s access rules and failure handling.',
 'Check offset+length <= extent without checking overflow. A very large length can wrap the sum.',
 'Check offset <= extent, then compare length with extent-offset. State what an empty range at the end means.',
 [('Privilege boundary','A boundary across which code must have different authority to access resources.'),('Validation','Checking that a request satisfies its required shape, range, and permissions before acting on it.')])
lab(20,'Distinguish recoverable and fatal faults','Model a load with separate permitted and present flags. A permitted absent page may be resolved and retried; a forbidden access leaves the program counter unchanged and returns failure.',
 ['Present,permitted load completes without a fault.','Absent,permitted load records one fault then advances once.','Forbidden load fails and never advances the program counter.'],r'''
bool load(bool permitted, bool& present, int& pc, int& faults) {
    if(!permitted) { ++faults; return false; }
    if(!present) { ++faults; present = true; }
    ++pc;
    return true;
}
''',r'''
    bool present = false; int pc = 8, faults = 0;
    assert(load(true,present,pc,faults) && pc == 9 && faults == 1);
    assert(load(true,present,pc,faults) && pc == 10 && faults == 1);
    bool absent = false; int stopped = 8, denied = 0;
    assert(!load(false,absent,stopped,denied) && stopped == 8 && !absent);
    std::cout << "completed-pc=" << pc << " denied-pc=" << stopped << '\n';
''','completed-pc=10 denied-pc=8\n',
 'The model separates permission from presence. Missing backing is recoverable only when the mapping policy allows the access. The instruction advances after success, preserving the restart point across a repair. This is a model of architectural control transfer, not a real exception handler or a promise that every page fault is repaired.',
 'Advance pc before checking the load. On denial, the model silently skips an instruction that never completed.',
 'Commit the new program counter only on the successful path. Test both denied and recoverable cases from the same starting pc.',
 [('Fault','An exception associated with an instruction that could not complete under the current conditions.'),('Restart point','The saved execution position used to retry an operation after a recoverable condition is repaired.')])
lab(21,'Enforce scheduler state transitions','Use an enum to model Ready, Running, and Waiting. Permit dispatch, blocking, and wakeup only from their valid starting states.',
 ['Ready→Running dispatch succeeds.','Running→Waiting block succeeds.','Waiting→Ready wakeup succeeds.','Dispatching a waiting process is rejected without changing it.'],r'''
enum class State { Ready, Running, Waiting };
bool transition(State& state, State from, State to) {
    if(state != from) return false;
    state = to;
    return true;
}
''',r'''
    State process = State::Ready;
    assert(transition(process,State::Ready,State::Running));
    assert(transition(process,State::Running,State::Waiting));
    assert(!transition(process,State::Ready,State::Running) && process == State::Waiting);
    assert(transition(process,State::Waiting,State::Ready));
    std::cout << "ready -> running -> waiting -> ready\n";
''','ready -> running -> waiting -> ready\n',
 'A state machine makes the permitted change explicit at each call. Checking the source state prevents an event from pretending a blocked process can run. This generic transition helper trusts its caller to supply the allowed edge; a production scheduler would expose named operations or validate the edge set centrally.',
 'Wake a waiting process directly into Running without considering which process owns the CPU.',
 'Wakeup makes work eligible by moving it to Ready. Dispatch is a separate scheduling decision.',
 [('Runnable','Eligible to receive CPU time, though not necessarily executing now.'),('State transition','A permitted change from one named state to another after an event.')])
lab(22,'Represent private and shared mappings','Use two maps whose entries own backing integers. Private mappings must point to distinct objects; one intentionally shared mapping must point to the same object in both maps.',
 ['Writes to A’s private address leave B’s private address unchanged.','Writes to the shared address are visible through both maps.','The two shared handles identify the same backing object.'],r'''
using Space = std::map<unsigned,std::shared_ptr<int>>;
''',r'''
    Space a{{0x1000,std::make_shared<int>(7)}};
    Space b{{0x1000,std::make_shared<int>(9)}};
    const auto shared = std::make_shared<int>(2);
    a[0x2000] = shared; b[0x2000] = shared;
    *a.at(0x1000) = 11;
    *a.at(0x2000) = 5;
    assert(*b.at(0x1000) == 9 && *b.at(0x2000) == 5);
    assert(a.at(0x2000) == b.at(0x2000));
    std::cout << "private=" << *a.at(0x1000) << ',' << *b.at(0x1000)
              << " shared=" << *b.at(0x2000) << '\n';
''','private=11,9 shared=5\n',
 'The maps model address spaces and the owned integers model backing storage. Equal address keys do not imply equal objects. Sharing is represented by two entries holding handles to one backing object. The smart pointers operate within this demonstration process; they are not a cross-process shared-memory mechanism.',
 'Create a fresh integer for each supposed shared mapping. The initial values match, but later writes do not propagate.',
 'Share the backing object when the contract calls for sharing. Do not infer sharing from equal values or equal numeric virtual addresses.',
 [('Address space','The mapping context in which a program interprets its virtual addresses.'),('Backing storage','The underlying storage reached through a mapping or handle.')])
lab(23,'Translate through a checked page map','Translate byte addresses using 4096-byte pages and a virtual-page-to-frame map. Reject an unmapped virtual page and guard the frame multiplication.',
 ['0x1234 maps through virtual page 1 to physical 29236 when frame is 7.','The next byte maps to 29237.','An unmapped page returns no result.','An oversized frame number is rejected.'],r'''
std::optional<std::size_t> translate(std::size_t address,
    const std::map<std::size_t,std::size_t>& pages) {
    const std::size_t size = 4096;
    const auto entry = pages.find(address/size);
    if(entry == pages.end()) return std::nullopt;
    const auto offset = address%size;
    const auto maximum = std::numeric_limits<std::size_t>::max();
    if(entry->second > (maximum-offset)/size) return std::nullopt;
    return entry->second*size+offset;
}
''',r'''
    std::map<std::size_t,std::size_t> pages{{1,7}};
    assert(translate(0x1234,pages) == 29236 && translate(0x1235,pages) == 29237);
    assert(!translate(0x2000,pages));
    pages[2] = std::numeric_limits<std::size_t>::max();
    assert(!translate(0x2000,pages));
    std::cout << "physical=" << *translate(0x1234,pages) << " unmapped=rejected\n";
''','physical=29236 unmapped=rejected\n',
 'The quotient selects a mapping and the remainder stays unchanged. A failed lookup is a defined outcome, not an accidental insertion of frame zero. This lab checks numeric mapping and overflow; permissions are added in the next chapter. A real OS may resolve some missing entries rather than simply reject them.',
 'Use pages[page] when only looking up a map. That operation can insert a default entry and make an unmapped page appear to map to frame zero.',
 'Use find for a read-only lookup and handle end explicitly. Keep absence distinct from a valid mapping to frame zero.',
 [('Page','A fixed-size block in a virtual address space.'),('Frame','A fixed-size physical-memory block that can back a virtual page.')])
lab(24,'Make page permissions part of a lookup','Represent a page entry with present, readable, and writable flags. A read and write request must use the corresponding permission, and an absent page denies both.',
 ['A present read-only entry accepts reads and rejects writes.','A present read-write entry accepts both.','An absent entry rejects both regardless of permission bits.'],r'''
struct Entry { bool present; bool readable; bool writable; };
enum class Access { Read, Write };
bool allows(const Entry& entry, Access access) {
    return entry.present && (access == Access::Write ? entry.writable : entry.readable);
}
''',r'''
    const Entry read_only{true,true,false}, read_write{true,true,true}, absent{false,true,true};
    assert(allows(read_only,Access::Read) && !allows(read_only,Access::Write));
    assert(allows(read_write,Access::Read) && allows(read_write,Access::Write));
    assert(!allows(absent,Access::Read) && !allows(absent,Access::Write));
    std::cout << "read-only: read=yes write=no; absent: denied\n";
''','read-only: read=yes write=no; absent: denied\n',
 'The entry groups flags that describe one mapping. Presence and permission answer different questions, so neither can replace the other. The pure allows function is easy to test and has no state changes. Architecture-specific user/supervisor and execute permissions are outside this limited model and would need explicit fields and checks.',
 'Return entry.present for every access. A present read-only mapping then accepts a write.',
 'Pass the requested access type and check its matching permission after presence. Test the same entry with two different operations.',
 [('Page-table entry','A record containing mapping information and access-control state for a page.'),('Protection fault','An exception caused when an access violates the mapping’s permission rules.')])
lab(25,'Count TLB walks separately from data hits','Model a translation cache over a fixed page table. The first lookup of a mapped page performs a walk; the second reuses the translation. An unmapped page must not enter the TLB.',
 ['Two lookups of page 1 produce one successful walk.','Page 2 is unmapped and is not cached.','Clearing the TLB forces another walk for page 1.'],r'''
std::optional<unsigned> lookup(unsigned page, const std::map<unsigned,unsigned>& table,
    std::map<unsigned,unsigned>& tlb, unsigned& walks) {
    const auto hit = tlb.find(page);
    if(hit != tlb.end()) return hit->second;
    ++walks;
    const auto entry = table.find(page);
    if(entry == table.end()) return std::nullopt;
    tlb.emplace(page,entry->second);
    return entry->second;
}
''',r'''
    const std::map<unsigned,unsigned> table{{1,7}};
    std::map<unsigned,unsigned> tlb; unsigned walks = 0;
    assert(lookup(1,table,tlb,walks) == 7 && lookup(1,table,tlb,walks) == 7 && walks == 1);
    assert(!lookup(2,table,tlb,walks) && tlb.count(2) == 0 && walks == 2);
    tlb.clear();
    assert(lookup(1,table,tlb,walks) == 7 && walks == 3);
    std::cout << "walks=" << walks << " cached=" << tlb.size() << '\n';
''','walks=3 cached=1\n',
 'The two maps represent different levels of information: the page table is the model’s mapping authority, and the TLB is a derived cache. Invalidation is necessary when an authoritative mapping changes. Walk count includes the failed unmapped lookup, while data-cache behavior is intentionally not modeled by this function.',
 'Update the page table while continuing to trust an old TLB entry. The next hit returns stale frame information.',
 'Invalidate or update cached translations under the platform’s rules when mappings change. This lab uses clear to make that dependency observable.',
 [('TLB','A cache of recently used virtual-to-physical translations and related access information.'),('Invalidation','Removing a cached fact when the authoritative state no longer supports it.')])
lab(26,'Detach a shared value before a private write','Implement a single-threaded copy-on-write update for shared_ptr<int>. Reject a null handle, copy only when another owner shares the object, and preserve the other owner’s value.',
 ['The first private update detaches a shared object.','Updating an already-unique object keeps the same address.','A null handle is rejected.'],r'''
void private_write(std::shared_ptr<int>& owner, int value) {
    if(!owner) throw std::invalid_argument("null owner");
    if(!owner.unique()) owner = std::make_shared<int>(*owner);
    *owner = value;
}
''',r'''
    auto parent = std::make_shared<int>(7);
    auto child = parent;
    private_write(child,9);
    assert(*parent == 7 && *child == 9 && parent != child);
    const int* address = child.get();
    private_write(child,11);
    assert(child.get() == address && *child == 11);
    std::shared_ptr<int> empty; bool rejected = false;
    try { private_write(empty,1); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "parent=" << *parent << " child=" << *child << " detached=yes\n";
''','parent=7 child=11 detached=yes\n',
 'Copying the old value before assignment preserves the private-write contract. If allocation throws, the assignment to owner has not yet committed and the original shared value remains intact. unique() is suitable only under this lab’s single-threaded access rule; it does not establish a safe concurrent mutation protocol. OS copy-on-write applies similar intent at a different boundary.',
 'Write through child before making the private copy. The parent observes the mutation before separation occurs.',
 'Prepare private backing first, then write it. Keep the thread-safety and lifetime assumptions explicit.',
 [('Copy on write','Sharing unchanged backing and making a private copy before a permitted private mutation.'),('Demand paging','Providing backing for a permitted page when an access first requires it.')],extra='stdexcept')
lab(27,'Compare batching with separate transfers','Under a serial transfer model, compare one 10,000-byte batch with 100 transfers of 100 bytes. Latency is 10 microseconds and bandwidth is 100 bytes per microsecond; reject nonpositive bandwidth.',
 ['The batch costs 110 microseconds.','Separate requests cost 1100 microseconds.','Zero and negative bandwidth are rejected.'],r'''
double transfer(double bytes, double latency, double bandwidth) {
    if(!(bytes >= 0) || !(latency >= 0) || !(bandwidth > 0))
        throw std::invalid_argument("model parameters");
    return latency+bytes/bandwidth;
}
''',r'''
    const double batch = transfer(10000,10,100);
    const double separate = 100*transfer(100,10,100);
    assert(batch == 110 && separate == 1100);
    int rejected = 0;
    try { transfer(1,10,0); } catch(const std::invalid_argument&) { ++rejected; }
    try { transfer(1,10,-1); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "batch=" << batch << "us separate=" << separate << "us\n";
''','batch=110us separate=1100us\n',
 'Each separate transfer pays startup latency again. A larger batch pays it once, so batching can help even without faster hardware. The result relies on serialization, fixed latency, fixed bandwidth, and finite modest inputs. Real queueing, overlap, contention, and protocol limits can change the result.',
 'Use bytes*bandwidth for the transfer time. The units become bytes squared per time rather than time.',
 'Divide byte count by bytes per microsecond. Carry units through the arithmetic before trusting the number.',
 [('Latency','The elapsed delay for a particular operation or request.'),('Bandwidth','The amount of data that can be transferred per unit time under stated conditions.')],extra='stdexcept')
lab(28,'Generalize the traversal trace','Count misses for a square row-major array in a one-line cache. The dimension and elements per line are inputs from 1 through 64; reject zero. Compare both traversal orders.',
 ['A 4×4 array with four elements per line gives 4 row-first and 16 column-first misses.','A 1×1 array gives one miss for either order.','Zero dimension or line length is rejected.'],r'''
unsigned misses(unsigned size, unsigned elements_per_line, bool rows_first) {
    if(size == 0 || size > 64 || elements_per_line == 0 || elements_per_line > 64)
        throw std::invalid_argument("model dimensions");
    int resident = -1; unsigned count = 0;
    for(unsigned outer=0; outer<size; ++outer)
        for(unsigned inner=0; inner<size; ++inner) {
            const unsigned index = rows_first ? outer*size+inner : inner*size+outer;
            const int block = static_cast<int>(index/elements_per_line);
            if(block != resident) { ++count; resident = block; }
        }
    return count;
}
''',r'''
    assert(misses(4,4,true) == 4 && misses(4,4,false) == 16);
    assert(misses(1,1,true) == 1 && misses(1,1,false) == 1);
    int rejected = 0;
    try { misses(0,4,true); } catch(const std::invalid_argument&) { ++rejected; }
    try { misses(4,0,true); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "rows=" << misses(4,4,true) << " columns=" << misses(4,4,false) << '\n';
''','rows=4 columns=16\n',
 'Both traversals touch exactly the same indexes but in different orders. A line contains consecutive element positions, so row traversal uses nearby values before replacing the resident line. The input bounds also keep index arithmetic small. This miss count is a model result, not a timing benchmark or a prediction for all real cache sizes.',
 'Swap the loop order but keep the same index formula using outer as the row. The supposed column traversal is still row traversal.',
 'Define what outer and inner mean under each order, then calculate the resulting flat index. Inspect the first four indexes before counting misses.',
 [('Spatial locality','Using nearby locations within a short interval.'),('Temporal locality','Using the same location again before its useful cached state is displaced.')],extra='stdexcept')
lab(29,'Decompose a cache address for variable geometry','Return block, set, and tag for nonzero line size and set count. Accept byte addresses as unsigned integers; reject zero geometry.',
 ['Address 64 with 16-byte lines and four sets gives block 4, set 0, tag 1.','Addresses within the same line have the same block, set, and tag.','A zero line size or set count is rejected.'],r'''
struct Parts { unsigned block; unsigned set; unsigned tag; };
Parts decompose(unsigned address, unsigned line_size, unsigned sets) {
    if(line_size == 0 || sets == 0) throw std::invalid_argument("cache geometry");
    const unsigned block = address/line_size;
    return {block,block%sets,block/sets};
}
''',r'''
    const auto first = decompose(64,16,4), last = decompose(79,16,4);
    assert(first.block == 4 && first.set == 0 && first.tag == 1);
    assert(first.block == last.block && first.set == last.set && first.tag == last.tag);
    int rejected = 0;
    try { decompose(0,0,4); } catch(const std::invalid_argument&) { ++rejected; }
    try { decompose(0,16,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "block=" << first.block << " set=" << first.set << " tag=" << first.tag << '\n';
''','block=4 set=0 tag=1\n',
 'Division removes the byte offset, modulo selects a set, and the remaining quotient distinguishes competing blocks. Powers of two allow bit-field extraction, but this numeric model also supports other positive sizes. Decomposition does not test whether the cache line is valid or resident; lookup needs that additional state.',
 'Use address%sets directly for the set number. Byte offsets inside one cache line then appear to choose different sets.',
 'Remove the line offset first by dividing by line_size, then compute the set from the block number.',
 [('Set associativity','The number of entries in one cache set that can hold competing blocks.'),('Tag','The part of a block’s identity used to distinguish blocks selecting the same set.')],extra='stdexcept')
lab(30,'Make cold-cache behavior a reusable function','Simulate reads for a configurable direct-mapped cache. Each call begins cold; use explicit empty slots and return the hit count. Reject zero line size or zero slot count.',
 ['The chapter trace has two hits with four 16-byte slots.','An empty trace has zero hits.','Reading the same address three times has two hits.','Zero geometry is rejected.'],r'''
unsigned hits(const std::vector<unsigned>& addresses, unsigned line_size, unsigned slots) {
    if(line_size == 0 || slots == 0) throw std::invalid_argument("cache geometry");
    std::vector<std::optional<unsigned>> resident(slots);
    unsigned count = 0;
    for(unsigned address : addresses) {
        const unsigned block = address/line_size, slot = block%slots;
        if(resident[slot] && *resident[slot] == block) ++count;
        resident[slot] = block;
    }
    return count;
}
''',r'''
    assert(hits({0,16,0,64,0,16},16,4) == 2);
    assert(hits({},16,4) == 0 && hits({0,0,0},16,4) == 2);
    bool rejected = false;
    try { hits({0},0,4); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "trace-hits=" << hits({0,16,0,64,0,16},16,4)
              << " repeat-hits=" << hits({0,0,0},16,4) << '\n';
''','trace-hits=2 repeat-hits=2\n',
 'optional gives each slot an empty state separate from every unsigned block number, including zero. The vector is local to the simulation call, so repeated tests start cold rather than sharing hidden history. On a miss, the selected slot is replaced; the other slots keep their state. The function reports events, not elapsed time.',
 'Initialize unsigned slots to zero and compare only the stored value. The first access to block zero is incorrectly called a hit.',
 'Represent validity explicitly, with optional or a valid bit, and include block zero as the first cold access in a test.',
 [('Cache hit','An access whose required block is already present and usable at the modeled cache level.'),('Conflict miss','A repeated miss caused by restricted placement relative to the chosen fully associative reference model.')],extra='stdexcept')
lab(31,'Compare FIFO and LRU with the same trace','Simulate a two-entry cache with either first-in-first-out or least-recently-used replacement. Return the final contents from oldest to newest under the selected policy.',
 ['Trace A,B,A,C leaves B,C under FIFO.','The same trace leaves A,C under LRU.','Repeated hits do not create duplicate entries.'],r'''
std::vector<char> simulate(const std::vector<char>& trace, bool lru) {
    std::vector<char> resident;
    for(char block : trace) {
        const auto found = std::find(resident.begin(),resident.end(),block);
        if(found != resident.end()) {
            if(lru) { resident.erase(found); resident.push_back(block); }
            continue;
        }
        if(resident.size() == 2) resident.erase(resident.begin());
        resident.push_back(block);
    }
    return resident;
}
''',r'''
    const auto fifo = simulate({'A','B','A','C'},false);
    const auto lru = simulate({'A','B','A','C'},true);
    assert((fifo == std::vector<char>{'B','C'}));
    assert((lru == std::vector<char>{'A','C'}));
    assert(simulate({'A','A','A'},true).size() == 1);
    std::cout << "FIFO=" << fifo[0] << fifo[1] << " LRU=" << lru[0] << lru[1] << '\n';
''','FIFO=BC LRU=AC\n',
 'The container order is the policy state. FIFO leaves it unchanged on a hit; LRU moves a hit to the recent end. Both insert a new block at the end and evict from the front when full. Using the same trace isolates the policy difference; it does not prove one policy wins on every workload.',
 'Move a hit to the end in the FIFO branch. The implementation no longer preserves arrival order.',
 'Update recency only for LRU. Use a hit between insertion and eviction to distinguish the policies.',
 [('Replacement policy','The rule selecting which resident item to remove when space is needed.'),('Dirty line','A cached block containing changes that must be preserved before the block is discarded.')],extra='algorithm')
lab(32,'Use local miss rates in a two-level cost model','Compute h1 + m1×(h2 + m2×memory). Miss rates must lie in [0,1]; times must be nonnegative. Assume serial costs and incremental penalties.',
 ['h1=1,m1=.1,h2=5,m2=.2,memory=50 gives 2.5.','No L1 misses gives h1 alone.','Rates outside [0,1] are rejected.'],r'''
double amat(double h1, double m1, double h2, double m2, double memory) {
    if(!(h1 >= 0 && h2 >= 0 && memory >= 0 && m1 >= 0 && m1 <= 1 && m2 >= 0 && m2 <= 1))
        throw std::invalid_argument("cost model");
    return h1+m1*(h2+m2*memory);
}
''',r'''
    const double result = amat(1,.1,5,.2,50);
    assert(std::abs(result-2.5) < 1e-12);
    assert(amat(1,0,5,.2,50) == 1);
    bool rejected = false;
    try { amat(1,.1,5,2,50); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "AMAT=" << result << "ns\n";
''','AMAT=2.5ns\n',
 'Only L1 misses reach L2, and only the fraction m2 of those pay the memory penalty. Multiplying the rates preserves their denominators. The h1 and h2 terms are incremental lookup costs in this model; adding a miss penalty that already contains them would count time twice. Real overlapping accesses require a different performance interpretation.',
 'Use h1+m1*h2+m2*memory. It treats the L2 local miss rate as a fraction of all original accesses.',
 'Nest the L2 cost inside the L1 miss probability. Write the denominator of every rate before substituting numbers.',
 [('Local miss rate','Misses divided by requests reaching a particular cache level.'),('AMAT','Average memory-access time calculated under a stated cost model.')],extra='cmath stdexcept')
lab(33,'Read a bounded record slice','Return a substring only when offset and length fit within the file model. An empty slice at end is valid; an offset past end is not.',
 ['ABCDE at offset two,length three gives CDE.','Offset five,length zero gives an empty string.','Past-end offset and oversized length are rejected.'],r'''
std::optional<std::string> read_slice(const std::string& file, std::size_t offset,
                                     std::size_t length) {
    if(offset > file.size() || length > file.size()-offset) return std::nullopt;
    return file.substr(offset,length);
}
''',r'''
    assert(read_slice("ABCDE",2,3) == "CDE");
    assert(read_slice("ABCDE",5,0) == "");
    assert(!read_slice("ABCDE",6,0));
    assert(!read_slice("ABCDE",2,std::numeric_limits<std::size_t>::max()));
    std::cout << *read_slice("ABCDE",2,3) << " end-empty=valid\n";
''','CDE end-empty=valid\n',
 'The slice owns its returned characters, so it does not depend on the original string’s later lifetime. The checks use the valid input extent, not its allocation capacity. An optional empty string is a success, while an empty optional is rejection. Real file reads can also fail or return fewer bytes; this memory model isolates the bounds contract.',
 'Treat an empty returned string as failure. A valid zero-length slice then becomes indistinguishable from rejection.',
 'Represent the success state separately from its data. Test the boundary at exactly end and just beyond end.',
 [('File offset','A byte position used to locate data in a file.'),('Record boundary','A division between logical items imposed by a format above the byte stream.')])
lab(34,'Copy across short reads and writes','Build a deterministic transfer model that limits each read to two bytes and each write to one byte. Advance by actual counts and reject a zero-progress configuration.',
 ['ABCDE arrives intact even though every write is partial.','Empty input produces empty output.','A zero read or write limit is rejected rather than looping forever.'],r'''
std::string copy_chunks(const std::string& input, std::size_t read_limit, std::size_t write_limit) {
    if(read_limit == 0 || write_limit == 0) throw std::invalid_argument("no progress");
    std::string output;
    for(std::size_t pos=0; pos<input.size();) {
        const auto count = std::min(read_limit,input.size()-pos);
        const auto buffer = input.substr(pos,count);
        for(std::size_t sent=0; sent<buffer.size();) {
            const auto written = std::min(write_limit,buffer.size()-sent);
            output.append(buffer,sent,written);
            sent += written;
        }
        pos += count;
    }
    return output;
}
''',r'''
    assert(copy_chunks("ABCDE",2,1) == "ABCDE");
    assert(copy_chunks("",2,1).empty());
    bool rejected = false;
    try { copy_chunks("A",2,0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << copy_chunks("ABCDE",2,1) << " short-writes=handled\n";
''','ABCDE short-writes=handled\n',
 'Input progress and output progress are separate counters. A buffer must be fully written before it can be discarded or overwritten by another read. The inner loop advances by the actual write count. Real descriptor code must distinguish EOF, interruption, would-block, and permanent errors; this model supplies positive bounded transfers to isolate the progress rule.',
 'Advance sent by buffer.size() after the first one-byte write. Only A, C, and E reach the destination for this trace.',
 'Advance by written and loop over the untransferred suffix. Test read and write limits that differ so a one-call assumption cannot hide.',
 [('File descriptor','A process-local handle used to request operations on an open resource.'),('Short transfer','A successful operation that moves fewer bytes than requested.')],extra='stdexcept')
lab(35,'Flush the final partial buffer','Combine characters into batches of four, then forward any remaining suffix at the end. Do not count an empty final flush as a data transfer.',
 ['ABCDE produces two forwarded batches and the complete text.','ABCD produces exactly one batch.','Empty input produces no batch.'],r'''
std::pair<std::string,unsigned> buffered(const std::string& input) {
    std::string pending, output; unsigned flushes = 0;
    const auto flush = [&] {
        if(pending.empty()) return;
        output += pending; pending.clear(); ++flushes;
    };
    for(char ch : input) { pending += ch; if(pending.size() == 4) flush(); }
    flush();
    return {output,flushes};
}
''',r'''
    const auto result = buffered("ABCDE");
    assert(result.first == "ABCDE" && result.second == 2);
    assert(buffered("ABCD").second == 1 && buffered("").second == 0);
    std::cout << result.first << " batches=" << result.second << '\n';
''','ABCDE batches=2\n',
 'A partial final buffer still contains real data. The explicit final flush closes that gap, while the empty check prevents a misleading transfer count. This model counts forwarding to another memory string. Library flush, kernel acceptance, and durable storage are different boundaries in a real output path.',
 'Flush only when pending.size()==4. The final E never reaches output.',
 'Flush a nonempty suffix when the operation finishes. Test a size not divisible by the buffer capacity.',
 [('Buffering','Temporarily collecting data so it can be passed onward in useful batches.'),('Flush','Requesting that buffered data move to the next layer defined by the API.')])
lab(36,'Return an owned mapping snapshot','Contrast an independent snapshot with a shared reference to backing bytes. Then destroy the original handle and verify a copied owner still keeps the backing alive.',
 ['Private snapshot edits do not alter shared backing.','Shared edits are visible through both owners.','Resetting one owner does not destroy backing still owned by another.'],r'''
''',r'''
    auto backing = std::make_shared<std::string>("cat");
    auto shared = backing;
    std::string snapshot = *backing;
    snapshot[0] = 'b';
    (*shared)[2] = 'r';
    assert(*backing == "car" && snapshot == "bat");
    backing.reset();
    assert(shared && *shared == "car");
    std::cout << "shared=" << *shared << " snapshot=" << snapshot << '\n';
''','shared=car snapshot=bat\n',
 'Shared ownership and shared data are related but different facts. A shared_ptr keeps this object alive; a separate string snapshot copies its characters. A real memory mapping is governed by OS mapping and file-lifetime rules rather than shared_ptr counts. The model makes the intended aliasing and cleanup relationships inspectable without pretending to implement mmap.',
 'Store only a raw pointer from backing.get(), reset the last owner, then dereference that pointer.',
 'Retain ownership for as long as the object must live, or ensure a borrowed view ends before its owner. A borrowed address cannot keep backing alive.',
 [('Shared mapping','A mapping whose writes affect the shared backing according to the mapping API’s rules.'),('Private mapping','A mapping whose private modifications do not become shared file changes under its contract.')])
lab(37,'Separate a descriptor handle from its open-file state','Model two inherited handles referring to one open-file description. Closing the child’s handle must not close the parent’s handle or reset their shared offset.',
 ['A child read advances the parent-visible offset.','Resetting the child handle leaves the parent usable.','A separately opened description has an independent offset.'],r'''
struct OpenFile { unsigned offset = 0; };
''',r'''
    auto parent = std::make_shared<OpenFile>();
    auto child = parent;
    auto independent = std::make_shared<OpenFile>();
    child->offset += 2;
    assert(parent->offset == 2 && independent->offset == 0);
    child.reset();
    assert(parent && parent->offset == 2 && parent.use_count() == 1);
    std::cout << "parent-offset=" << parent->offset << " independent=" << independent->offset << '\n';
''','parent-offset=2 independent=0\n',
 'The handle is not the open-file description. Each process can close its own inherited descriptor while the kernel object remains referenced by another descriptor. The description holds the shared offset in this model. These smart pointers exist in one C++ process and illustrate the relationship; they are not the implementation of fork.',
 'Give the child a fresh OpenFile copy and assume its offset is shared just because its initial value matches.',
 'Represent shared open-file state with one backing object. Keep independent opens distinct from duplicated or inherited references.',
 [('Open-file description','Kernel-maintained state, such as a file offset, that multiple descriptors may reference.'),('Fork','A process-creation operation that creates a child with defined copies and shared references from its parent.')])
lab(38,'Keep failed replacement from destroying the current image','Model exec as a prepared replacement. Validate the new program name before committing it; preserve process identity on success and the old image on failure.',
 ['Replacing shell with worker keeps pid 42.','An empty name is rejected without changing the image.','Success and failure are distinct results.'],r'''
bool replace_image(std::string& image, const std::string& candidate) {
    if(candidate.empty()) return false;
    std::string prepared = candidate;
    image.swap(prepared);
    return true;
}
''',r'''
    const int pid = 42; std::string image = "shell";
    assert(!replace_image(image,"") && image == "shell");
    assert(replace_image(image,"worker") && image == "worker" && pid == 42);
    std::cout << "pid=" << pid << " image=" << image << '\n';
''','pid=42 image=worker\n',
 'Preparation can fail before the new state becomes visible. Swapping commits the prepared string while preserving the model’s process identifier. A real successful exec does not return into the old program; this returning function is only a state model. In actual code, the path after exec is the failure path.',
 'Clear the current image before validating the replacement. An invalid request then destroys valid existing state.',
 'Validate and prepare first, then commit. In real exec code, keep failure reporting on the path that returns from the call.',
 [('Program image','The executable code and associated user-space state installed in a process.'),('Exec','An operation that replaces the calling process’s program image rather than creating a second process.')])
lab(39,'Classify termination before reading its value','Use a tagged status model for normal exit and signal termination. Do not interpret a signal number as an exit code.',
 ['A normal exit value 7 formats as exit=7.','A signal termination value 15 formats as signal=15.','The tag controls the interpretation of the numeric field.'],r'''
enum class Termination { Exited, Signaled };
struct Status { Termination kind; int value; };
std::string describe(Status status) {
    return (status.kind == Termination::Exited ? "exit=" : "signal=") + std::to_string(status.value);
}
''',r'''
    assert(describe({Termination::Exited,7}) == "exit=7");
    assert(describe({Termination::Signaled,15}) == "signal=15");
    std::cout << describe({Termination::Exited,7}) << ' ' << describe({Termination::Signaled,15}) << '\n';
''','exit=7 signal=15\n',
 'The enum says which interpretation of value is valid. A real wait status uses OS-defined macros to discover this category; its packed integer is not directly the exit code. Separating classification from formatting makes the result clear to callers and prevents accidental bit guesses.',
 'Always print status.value as an exit code. A signal-killed child is then reported as if it returned normally.',
 'Classify the termination kind first, then extract the corresponding value using the platform API. Do not invent a universal bit layout.',
 [('Reaping','Collecting a terminated child’s status so its retained process record can be released.'),('Exit status','A result describing how a process terminated; its interpretation depends on the termination category.')])
lab(40,'Distinguish data, waiting, and EOF','Return a pipe-read state from buffered-byte count and open-writer count. Buffered data must be delivered even after the last writer closes.',
 ['Buffered data takes priority over EOF.','An empty buffer with an open writer means wait in this blocking model.','An empty buffer with no writers means EOF.'],r'''
enum class ReadState { Data, Wait, End };
ReadState read_state(unsigned bytes, unsigned writers) {
    if(bytes != 0) return ReadState::Data;
    return writers == 0 ? ReadState::End : ReadState::Wait;
}
''',r'''
    assert(read_state(2,0) == ReadState::Data);
    assert(read_state(0,1) == ReadState::Wait);
    assert(read_state(0,0) == ReadState::End);
    std::cout << "buffered=data empty-with-writer=wait drained-and-closed=EOF\n";
''','buffered=data empty-with-writer=wait drained-and-closed=EOF\n',
 'EOF describes a drained stream whose producer ends are closed. Closing the last writer does not erase already-buffered bytes. The three-state result prevents an empty buffer from being confused with a finished stream. A nonblocking real pipe would report would-block rather than waiting when writers still exist but no data is available.',
 'Return EOF as soon as writer count reaches zero. This discards the final buffered bytes.',
 'Check for available data before checking whether further data can arrive. Test a closed pipe that still has buffered bytes.',
 [('EOF','End of stream: no more bytes will be delivered under the stream’s contract.'),('Backpressure','A limit that makes a producer wait or reduce output when a downstream consumer cannot keep up.')])
lab(41,'Separate pending state from an event count','Model both a Boolean notification and a counted event queue. Deliver two events before consuming anything, then explain why the two representations produce different amounts of information.',
 ['Two Boolean notifications leave one true flag.','Two counted events leave count two.','Consuming the flag once clears it; consuming one count leaves one event.'],r'''
''',r'''
    bool pending = false; unsigned events = 0;
    for(unsigned i=0; i<2; ++i) { pending = true; ++events; }
    unsigned flag_actions = 0;
    if(pending) { pending = false; ++flag_actions; }
    --events;
    assert(flag_actions == 1 && !pending && events == 1);
    std::cout << "flag-actions=" << flag_actions << " counted-events-left=" << events << '\n';
''','flag-actions=1 counted-events-left=1\n',
 'A flag answers whether attention is needed; a counter can preserve multiplicity within its range. Neither this ordinary bool nor unsigned is a real concurrent signal-handling protocol. A real handler must obey async-signal-safety rules, and the main loop must close the check-then-sleep race using the OS’s waiting and mask operations.',
 'Expect two standard signal notifications to behave like two items in a reliable work queue.',
 'Treat a signal as a prompt to inspect authoritative state unless the API explicitly queues the required information. Use a suitable counted mechanism when every event matters.',
 [('Signal','An asynchronous process notification with delivery and handling rules defined by the OS.'),('Async-signal-safe','Permitted for use in an asynchronous signal handler under the applicable API’s rules.')])
lab(42,'Reject an incomplete pipeline before launching it','Parse a deliberately small grammar: word, pipe, word, separated by spaces. Reject missing stages, extra words, and multiple pipes. This parser has no quotes, expansions, or redirection.',
 ['emit | count returns the two stage names.','emit | and | count are rejected.','emit | count extra and a | b | c are rejected.'],r'''
std::optional<std::pair<std::string,std::string>> parse(const std::string& line) {
    std::istringstream input(line);
    std::string left, pipe, right, extra;
    if(!(input >> left >> pipe >> right) || input >> extra) return std::nullopt;
    if(pipe != "|" || left == "|" || right == "|") return std::nullopt;
    return std::pair<std::string,std::string>{left,right};
}
''',r'''
    const auto good = parse("emit | count");
    assert(good && good->first == "emit" && good->second == "count");
    assert(!parse("emit |") && !parse("| count"));
    assert(!parse("emit | count extra") && !parse("a | b | c"));
    std::cout << "left=" << good->first << " right=" << good->second << " invalid=rejected\n";
''','left=emit right=count invalid=rejected\n',
 'Parsing produces an execution plan before any process is launched. The result owns its stage names, so it survives destruction of the input stream. Rejecting unsupported forms makes the small grammar explicit; it would be incorrect to advertise this whitespace parser as a general shell parser.',
 'Launch the first stage as soon as the first word is read. Later syntax failure then occurs after side effects have already begun.',
 'Separate parsing from execution. Validate the complete structure and only then allocate pipes or launch children.',
 [('Token','A meaningful input unit such as a word or operator produced during parsing.'),('Execution plan','A validated description of the operations to perform before those operations are started.')])
lab(43,'Enumerate defined interleavings','Enumerate every order of two read/write steps from A and two from B while preserving each participant’s read-before-write order. Count final values in this sequential model.',
 ['There are six distinct schedules.','Four lose an update and finish at one.','Two serialize the updates and finish at two.','No actual unsynchronized C++ threads are used to model the failure.'],r'''
std::map<int,unsigned> outcomes() {
    std::array<int,4> order{0,0,1,1};
    std::map<int,unsigned> counts;
    do {
        int counter = 0;
        std::array<int,2> local{0,0}, step{0,0};
        for(int who : order) {
            if(step[who]++ == 0) local[who] = counter;
            else counter = local[who]+1;
        }
        ++counts[counter];
    } while(std::next_permutation(order.begin(),order.end()));
    return counts;
}
''',r'''
    const auto counts = outcomes();
    assert(counts.at(1) == 4 && counts.at(2) == 2);
    std::cout << "lost=" << counts.at(1) << " serialized=" << counts.at(2) << '\n';
''','lost=4 serialized=2\n',
 'Each participant’s first event reads and its second writes. Permuting the actor labels enumerates only schedules that keep this internal order. The sequential simulator has defined behavior, so its outcomes can be counted and inspected. Those counts do not assign probabilities to a real scheduler and do not describe the full set of behaviors of a C++ data race.',
 'Run ordinary shared counter++ from two threads and claim the observed results enumerate all valid outcomes.',
 'Use a defined model for interleaving reasoning, or synchronize the real program. Undefined behavior is not a scheduling model.',
 [('Interleaving','One ordering of steps from activities whose lifetimes overlap.'),('Concurrency','Multiple activities in progress whose steps may be interleaved or run at the same time.')],extra='algorithm')
lab(44,'Partition an uneven range safely','Sum a vector with two asynchronous workers, splitting at size/2. Keep the input alive until both results are obtained and support empty and odd-length inputs.',
 ['{1,2,3,4,5} totals 15 with no lost element.','An empty vector totals zero.','One element is assigned to exactly one partition.'],r'''
long long parallel_sum(const std::vector<int>& values) {
    const auto middle = values.size()/2;
    const auto part = [&](std::size_t begin, std::size_t end) {
        return std::accumulate(values.begin()+begin,values.begin()+end,0LL);
    };
    auto first = std::async(std::launch::async,part,0,middle);
    auto second = std::async(std::launch::async,part,middle,values.size());
    return first.get()+second.get();
}
''',r'''
    assert(parallel_sum({1,2,3,4,5}) == 15);
    assert(parallel_sum({}) == 0 && parallel_sum({7}) == 7);
    std::cout << "odd=" << parallel_sum({1,2,3,4,5}) << " empty=" << parallel_sum({}) << '\n';
''','odd=15 empty=0\n',
 'The half-open ranges [0,middle) and [middle,size) meet without overlap or a gap. Futures return values, so workers need no shared mutable accumulator. get waits for completion and propagates worker exceptions. The input remains borrowed until both asynchronous tasks finish; this lab assumes the small input sums fit in long long. Parallelism is an implementation exercise, not a speed claim.',
 'Give each worker size/2 elements starting at worker*(size/2). For five elements, the fifth is omitted.',
 'Use the actual size as the second range’s end. Check empty, one-element, even, and odd sizes.',
 [('Join','Waiting for a thread’s completion and establishing the completion relationship needed to use its results.'),('Partition','A division of work into ranges or tasks with explicit coverage and overlap rules.')],extra='future')
lab(45,'Compare split updates with atomic read-modify-write','Keep a deterministic split-load/store counterexample, then have two real workers perform 1000 atomic fetch_add operations each.',
 ['Two split loads followed by two stores finish at one.','Two workers using fetch_add finish at 2000.','The real counter is read for its final result only after both workers finish.'],r'''
int split_update() {
    std::atomic<int> counter{0};
    const int first = counter.load(), second = counter.load();
    counter.store(first+1); counter.store(second+1);
    return counter.load();
}
int combined_updates() {
    std::atomic<int> counter{0};
    const auto worker = [&] { for(int i=0;i<1000;++i) counter.fetch_add(1); };
    auto first = std::async(std::launch::async,worker);
    auto second = std::async(std::launch::async,worker);
    first.get(); second.get();
    return counter.load();
}
''',r'''
    const int split = split_update(), combined = combined_updates();
    assert(split == 1 && combined == 2000);
    std::cout << "split=" << split << " atomic=" << combined << '\n';
''','split=1 atomic=2000\n',
 'Every access to each shared atomic counter is atomic, but only fetch_add combines reading and updating into one indivisible operation. The split trace intentionally separates those steps and has a logical lost update without undefined behavior. Completion of both futures makes the final count meaningful; checking while workers run would observe a valid but incomplete total.',
 'Replace fetch_add with load followed by store in each worker. The C++ accesses remain atomic, but increments can be lost.',
 'Use one read-modify-write operation for the counter, or protect the entire logical update with a mutex when several objects must change together.',
 [('Data race','Conflicting concurrent C++ accesses, at least one a write, without the required atomicity or happens-before ordering.'),('Logical race','A result that depends on an undesired ordering of otherwise defined operations.')],extra='future')
lab(46,'Publish a caller-supplied payload','Build a one-shot publication function. A worker writes a non-atomic payload then releases a ready flag; the caller acquires that flag before reading the payload.',
 ['Publishing 42 returns 42.','Publishing a negative small integer preserves its value.','Each invocation uses a fresh flag and payload.'],r'''
int publish(int value) {
    int payload = 0;
    std::atomic<bool> ready{false};
    auto producer = std::async(std::launch::async,[&] {
        payload = value;
        ready.store(true,std::memory_order_release);
    });
    while(!ready.load(std::memory_order_acquire)) std::this_thread::yield();
    const int observed = payload;
    producer.get();
    return observed;
}
''',r'''
    assert(publish(42) == 42 && publish(-7) == -7);
    std::cout << "first=" << publish(42) << " second=" << publish(-7) << '\n';
''','first=42 second=-7\n',
 'The release store follows the payload write; the acquiring load that observes it orders the subsequent payload read. The local state outlives the producer because the future is completed before return. A fresh flag makes the protocol one-shot. Reusing one Boolean for several messages would need a richer protocol to prevent missed or overwritten publications.',
 'Use relaxed ordering on both flag operations and still rely on the flag to publish a non-atomic payload.',
 'Use the release/acquire handoff or a mutex-based protocol that supplies the required happens-before relation. Do not treat a successful test run as a memory-order proof.',
 [('Release/acquire','A pair of atomic ordering operations that can establish visibility of preceding work when the acquire observes the release.'),('Happens-before','An ordering relation used by C++ to determine whether effects are properly ordered between evaluations.')],extra='future')
lab(47,'Guard a conservation rule across real workers','Two accounts begin with 100 each. Two workers make 100 opposite-direction transfers of one unit. Protect each complete transfer with the same mutex.',
 ['Every completed transfer preserves total 200.','Both workers finish with balances 100 and 100.','No balance is read or changed concurrently outside the common lock.'],r'''
''',r'''
    int first = 100, second = 100;
    std::mutex mutex;
    const auto move = [&](bool forward) {
        for(int i=0;i<100;++i) {
            std::lock_guard<std::mutex> lock(mutex);
            int& from = forward ? first : second;
            int& to = forward ? second : first;
            assert(from > 0);
            --from; ++to;
            assert(first+second == 200);
        }
    };
    auto a = std::async(std::launch::async,move,true);
    auto b = std::async(std::launch::async,move,false);
    a.get(); b.get();
    assert(first == 100 && second == 100);
    std::cout << "balances=" << first << ',' << second << " total=" << first+second << '\n';
''','balances=100,100 total=200\n',
 'The invariant spans two variables, so the critical section spans both updates. The references from and to select direction while the lock is held. Each worker sends at most its account’s initial 100 units, so the positive-source assertion holds for every schedule in this fixture. Main reads the final state after both workers finish.',
 'Give each worker its own mutex. Both can then enter their critical sections at the same time and race on the accounts.',
 'Use the same synchronization object for every access governed by the invariant. Protect readers as well as writers if they can run concurrently.',
 [('Critical section','A region whose operations require coordinated access to shared state.'),('Mutex','A synchronization object that permits one cooperating owner at a time.')],extra='future')
lab(48,'Use a condition variable with a lasting predicate','Implement a one-shot handoff using a mutex, condition variable, ready flag, and payload. It must work whether the producer sets ready before or after the consumer starts waiting.',
 ['The consumer sees payload 42.','The wait uses a predicate while holding the associated mutex.','The producer changes the state under that same mutex.'],r'''
int handoff() {
    std::mutex mutex;
    std::condition_variable changed;
    bool ready = false; int payload = 0;
    auto producer = std::async(std::launch::async,[&] {
        { std::lock_guard<std::mutex> lock(mutex); payload = 42; ready = true; }
        changed.notify_one();
    });
    int observed;
    {
        std::unique_lock<std::mutex> lock(mutex);
        changed.wait(lock,[&] { return ready; });
        observed = payload;
    }
    producer.get();
    return observed;
}
''',r'''
    assert(handoff() == 42);
    std::cout << "payload=" << handoff() << '\n';
''','payload=42\n',
 'The predicate is durable state; a notification is only a reason to recheck it. wait releases the lock while waiting and reacquires it before evaluating readiness and returning. If ready was already true, the consumer does not sleep. The final get ensures the notification call and producer lifetime finish before local synchronization objects are destroyed.',
 'Call changed.wait(lock) once without a predicate, then read payload. A spurious wakeup or an earlier notification can break the intended protocol.',
 'Use the predicate overload or an explicit while loop. Keep state reads and writes under the same mutex.',
 [('Condition variable','A waiting mechanism used with a mutex and a predicate over shared state.'),('Spurious wakeup','A return from waiting that does not itself prove the desired condition is true.')],extra='condition_variable future')
QUEUE=r'''
class Queue {
    std::mutex mutex_;
    std::condition_variable readable_, writable_;
    std::deque<int> items_;
    std::size_t capacity_;
    bool closed_ = false;
public:
    explicit Queue(std::size_t capacity) : capacity_(capacity) {
        if(capacity == 0) throw std::invalid_argument("zero capacity");
    }
    bool push(int value) {
        std::unique_lock<std::mutex> lock(mutex_);
        writable_.wait(lock,[&] { return closed_ || items_.size() < capacity_; });
        if(closed_) return false;
        items_.push_back(value);
        readable_.notify_one();
        return true;
    }
    std::optional<int> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        readable_.wait(lock,[&] { return closed_ || !items_.empty(); });
        if(items_.empty()) return std::nullopt;
        const int value = items_.front(); items_.pop_front();
        writable_.notify_one();
        return value;
    }
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        readable_.notify_all(); writable_.notify_all();
    }
};
'''
lab(49,'Build a bounded, drainable queue','Implement a capacity-two queue with blocking push and pop. close rejects new pushes, wakes waiting participants, and permits consumers to drain existing items before reporting end.',
 ['Values 1 through 5 are delivered in FIFO order.','The consumer drains all five after producer closure.','A closed empty queue returns no value; a push after close is rejected.','Zero capacity is rejected.'],QUEUE,r'''
    Queue queue(2);
    std::vector<int> received;
    received.reserve(5); // Allocate before a producer can block.
    auto producer = std::async(std::launch::async,[&] {
        try {
            for(int i=1;i<=5;++i) assert(queue.push(i));
            queue.close();
        } catch(...) { queue.close(); throw; }
    });
    try {
        while(const auto item = queue.pop()) received.push_back(*item);
    } catch(...) {
        queue.close();
        try { producer.get(); } catch(...) {}
        throw;
    }
    producer.get();
    assert((received == std::vector<int>{1,2,3,4,5}));
    assert(!queue.push(6) && !queue.pop());
    bool rejected = false;
    try { Queue invalid(0); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "items=" << received.size() << " sum="
              << std::accumulate(received.begin(),received.end(),0) << " closed=yes\n";
''','items=5 sum=15 closed=yes\n',
 'A class is useful here because the mutex, predicates, queue, capacity, and closed flag must obey one shared rule across several operations. All shared state is accessed under the mutex. Separate conditions describe available items and available space. The producer closes on normal completion and on exceptions so the consumer cannot wait forever for more input. The queue must outlive every participant.',
 'In pop, return no value whenever closed_ is true, even if items_ is nonempty. Buffered work is lost during shutdown.',
 'After waiting, return no value only when the queue is empty. A closed queue can still contain items that the drain contract promises to deliver.',
 [('Drain','Consume already-accepted work before ending a closed queue or stream.'),('Shutdown protocol','The rules for stopping new work, waking waiters, finishing or discarding old work, and ending lifetimes.')],extra='condition_variable deque future stdexcept')
lab(50,'Avoid opposite lock-order deadlock','Transfer between accounts with two mutexes using scoped_lock. Treat transfer to the same account as a no-op, reject insufficient funds, and preserve the total.',
 ['Two workers transfer 100 units in opposite directions without opposite manual lock order.','Total remains 200.','Transfer to self changes nothing.','For distinct accounts, a request greater than the source balance returns false.'],r'''
struct Account { int balance = 100; std::mutex mutex; };
bool transfer(Account& from, Account& to, int amount) {
    if(amount < 0) return false;
    if(&from == &to) return true;
    std::scoped_lock lock(from.mutex,to.mutex);
    if(from.balance < amount) return false;
    from.balance -= amount; to.balance += amount;
    return true;
}
''',r'''
    Account first, second;
    const auto worker = [&](bool forward) {
        for(int i=0;i<100;++i)
            assert(forward ? transfer(first,second,1) : transfer(second,first,1));
    };
    auto a = std::async(std::launch::async,worker,true);
    auto b = std::async(std::launch::async,worker,false);
    a.get(); b.get();
    assert(first.balance == 100 && second.balance == 100);
    assert(transfer(first,first,10) && first.balance == 100);
    assert(!transfer(first,second,1000));
    std::cout << "total=" << first.balance+second.balance << " self-transfer=no-op\n";
''','total=200 self-transfer=no-op\n',
 'scoped_lock acquires the two distinct mutexes using a deadlock-avoidance strategy and releases both at scope exit. Handling the same-account case first avoids passing the same non-recursive mutex twice. The small fixed total bounds arithmetic in this fixture. Other locks, callbacks, or I/O added inside the critical section would introduce new waiting relationships that still need review.',
 'Lock the source mutex first and destination second in both transfer directions. Each thread can hold the lock the other needs.',
 'Use a consistent global ordering or an appropriate multi-mutex acquisition operation. Separately handle repeated references to the same resource.',
 [('Deadlock','A state in which participants cannot progress because each waits for conditions the waiting participants must supply.'),('Circular wait','A cycle of waiting dependencies, such as A waiting for B while B waits for A.')],extra='future')
lab(51,'Parse complete frames without losing the next one','Parse a one-byte length followed by that many payload bytes, with maximum length eight. Report incomplete input separately from an invalid length, and return the number of consumed bytes.',
 ['A fragmented cat frame stays incomplete until all three payload bytes arrive.','A complete frame reports four consumed bytes.','A following frame remains available to parse.','A length above eight is rejected before payload allocation.'],r'''
struct Frame { std::string payload; std::size_t consumed; };
std::optional<Frame> frame(const std::vector<unsigned char>& bytes) {
    if(bytes.empty()) return std::nullopt;
    const std::size_t length = bytes[0];
    if(length > 8) throw std::invalid_argument("frame too large");
    if(bytes.size()-1 < length) return std::nullopt;
    return Frame{std::string(bytes.begin()+1,bytes.begin()+1+length),1+length};
}
''',r'''
    std::vector<unsigned char> bytes{3,'c'};
    assert(!frame(bytes));
    bytes.insert(bytes.end(),{'a','t',1,'x'});
    const auto first = frame(bytes);
    assert(first && first->payload == "cat" && first->consumed == 4);
    bytes.erase(bytes.begin(),bytes.begin()+first->consumed);
    assert(frame(bytes)->payload == "x");
    bool rejected = false;
    try { frame({9}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "first=" << first->payload << " next=" << frame(bytes)->payload << '\n';
''','first=cat next=x\n',
 'A byte stream can split or combine application frames arbitrarily. The parser therefore returns both owned payload data and a consumed count. It does not erase unconsumed bytes itself. Bounds are checked before forming payload iterators, and invalid size is distinct from incomplete arrival. Network errors and end-of-stream handling belong to the surrounding connection state machine.',
 'Clear the whole receive buffer after parsing the first frame. Any complete or partial second frame disappears.',
 'Remove only consumed bytes and retain the suffix. Test two frames arriving in one read as well as one frame arriving in several reads.',
 [('Framing','Rules that identify application message boundaries within a sequence of bytes.'),('Byte stream','An ordered sequence of bytes without a promise that transfer calls preserve message boundaries.')],extra='stdexcept')
lab(52,'Enforce an admission budget','Create a gate with a maximum active count. Admit only below the limit, release one existing permit at a time, and reject a release when none is held.',
 ['A limit-two gate admits twice and rejects a third request.','Releasing one permit allows another admission.','Active count never exceeds two or falls below zero.','An unmatched release is rejected.'],r'''
class Gate {
    unsigned active_ = 0;
    const unsigned limit_;
    std::mutex mutex_;
public:
    explicit Gate(unsigned limit) : limit_(limit) {}
    bool admit() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(active_ == limit_) return false;
        ++active_; return true;
    }
    bool release() {
        std::lock_guard<std::mutex> lock(mutex_);
        if(active_ == 0) return false;
        --active_; return true;
    }
};
''',r'''
    Gate gate(2);
    assert(gate.admit() && gate.admit() && !gate.admit());
    assert(gate.release() && gate.admit());
    assert(gate.release() && gate.release() && !gate.release());
    Gate closed(0); assert(!closed.admit());
    std::cout << "limit=2 third=rejected released=reusable\n";
''','limit=2 third=rejected released=reusable\n',
 'Checking the count and incrementing it form one critical section, otherwise two arrivals could both observe spare capacity. The gate bounds admitted work, not every byte the server owns. Callers must still pair successful admission with release on every completion and error path; a scope-based permit object is a useful next extension.',
 'Check the count without a lock, then lock only while incrementing. Two callers can both pass the same old count.',
 'Guard the check and state change together. Treat cleanup pairing as part of the admission contract.',
 [('Admission control','A rule deciding whether new work can enter the system given current resource limits.'),('Overload policy','The chosen behavior when incoming work exceeds capacity, such as rejection, bounded waiting, or dropping.')])
lab(53,'Model nonblocking drain outcomes','Consume a supplied trace of read results: positive counts add bytes, -1 means would-block in this model, and zero means EOF. Reject other negative values.',
 ['2,1,-1 buffers three bytes and reports blocked, not EOF.','2,0 buffers two bytes and reports EOF.','Results after would-block or EOF are not consumed in the same drain call.'],r'''
struct Drain { unsigned bytes = 0; bool blocked = false; bool eof = false; };
Drain drain(const std::vector<int>& results) {
    Drain result;
    for(int count : results) {
        if(count == -1) { result.blocked = true; break; }
        if(count == 0) { result.eof = true; break; }
        if(count < 0) throw std::invalid_argument("read error");
        result.bytes += static_cast<unsigned>(count);
    }
    return result;
}
''',r'''
    const auto blocked = drain({2,1,-1,9}), ended = drain({2,0,9});
    assert(blocked.bytes == 3 && blocked.blocked && !blocked.eof);
    assert(ended.bytes == 2 && ended.eof && !ended.blocked);
    bool rejected = false;
    try { drain({-2}); } catch(const std::invalid_argument&) { rejected = true; }
    assert(rejected);
    std::cout << "blocked-bytes=" << blocked.bytes << " eof-bytes=" << ended.bytes << '\n';
''','blocked-bytes=3 eof-bytes=2\n',
 'Would-block means the operation cannot make immediate progress; EOF means the stream has ended. Keeping them separate prevents a temporary lack of data from closing a connection. The model gives -1 one special meaning, while a real read returning -1 requires inspecting errno. The fixture uses small counts; production accumulation also needs an explicit buffer-size bound.',
 'Treat every nonpositive result as EOF. A live connection is closed when it merely has no bytes ready now.',
 'Distinguish success, EOF, would-block, interruption, and permanent errors according to the API. Return control to the event loop on would-block.',
 [('Readiness','A notification that an I/O operation may be able to make progress under the API’s conditions.'),('Would-block','A nonblocking result indicating that immediate progress is unavailable without waiting.')],extra='stdexcept')
lab(54,'Report a distribution as well as a mean','For a nonempty list of finite nonnegative millisecond durations with positive total time, report mean, nearest-rank 95th percentile, and serial throughput.',
 ['2,8,5 gives mean 5, p95 8, throughput 200 per second.','A single 10 ms request gives p95 10 and throughput 100.','Empty input, all-zero time, and negative durations are rejected.'],r'''
struct Stats { double mean; double p95; double per_second; };
Stats summarize(std::vector<double> times) {
    if(times.empty()) throw std::invalid_argument("empty sample");
    double total = 0;
    for(double time : times) {
        if(!std::isfinite(time) || time < 0) throw std::invalid_argument("duration");
        total += time;
    }
    if(!std::isfinite(total) || total <= 0) throw std::invalid_argument("total time");
    std::sort(times.begin(),times.end());
    const auto rank = static_cast<std::size_t>(std::ceil(.95*times.size()));
    return {total/times.size(),times[rank-1],times.size()/(total/1000)};
}
''',r'''
    const auto result = summarize({2,8,5});
    assert(result.mean == 5 && result.p95 == 8 && result.per_second == 200);
    assert(summarize({10}).p95 == 10 && summarize({10}).per_second == 100);
    int rejected = 0;
    for(const auto& values : std::vector<std::vector<double>>{{},{0,0},{-1,2}}) {
        try { summarize(values); } catch(const std::invalid_argument&) { ++rejected; }
    }
    assert(rejected == 3);
    std::cout << "mean=" << result.mean << " p95=" << result.p95 << " rate=" << result.per_second << "/s\n";
''','mean=5 p95=8 rate=200/s\n',
 'The mean describes total time divided by count; the selected percentile describes an order statistic. Sorting a copy preserves the caller’s sample order. Three observations make a very weak basis for real tail-latency inference: the example teaches the calculation, not statistical confidence. Throughput here uses a serial interval; concurrent request durations cannot simply be added to infer wall time.',
 'Report 3/15 as requests per second. The denominator is milliseconds, so the units are wrong by a factor of 1000.',
 'Convert the interval to seconds and state the percentile convention. Preserve raw measurements for later checks.',
 [('Throughput','Completed work divided by the elapsed measurement interval.'),('Percentile','A position in an ordered sample or distribution, interpreted using a stated convention.')],extra='cmath stdexcept')
lab(55,'Build reusable bounded range sums','Precompute prefix sums for at most one million int values in [-1000,1000]. Answer half-open range queries and reject reversed or out-of-bounds endpoints.',
 ['[1,4) over 2,4,6,8 returns 18.','An empty range returns zero.','[0,4) returns 20.','Reversed endpoints and endpoints beyond size are rejected.'],r'''
std::vector<long long> prefix(const std::vector<int>& values) {
    if(values.size() > 1000000) throw std::invalid_argument("count");
    std::vector<long long> result(values.size()+1,0);
    for(std::size_t i=0;i<values.size();++i) {
        if(values[i] < -1000 || values[i] > 1000) throw std::invalid_argument("value");
        result[i+1] = result[i]+values[i];
    }
    return result;
}
long long query(const std::vector<long long>& sums, std::size_t left, std::size_t right) {
    if(sums.empty() || left > right || right >= sums.size()) throw std::out_of_range("range");
    return sums[right]-sums[left];
}
''',r'''
    const auto sums = prefix({2,4,6,8});
    assert(query(sums,1,4) == 18 && query(sums,2,2) == 0 && query(sums,0,4) == 20);
    int rejected = 0;
    try { query(sums,3,2); } catch(const std::out_of_range&) { ++rejected; }
    try { query(sums,0,5); } catch(const std::out_of_range&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "range=" << query(sums,1,4) << " empty=" << query(sums,2,2) << '\n';
''','range=18 empty=0\n',
 'A leading zero makes the same subtraction work for ranges starting at zero and for empty ranges. The bounds on count and element value keep every prefix sum representable in long long. Queries borrow an already-built valid prefix vector; exposing an arbitrary editable vector is a deliberate simplification that a value type could later protect.',
 'Build a prefix array without the leading zero but keep the same query formula. Endpoint zero and the full-range query no longer follow the stated meaning.',
 'Define sums[k] as the total strictly before k and keep that meaning in construction, bounds, and queries.',
 [('Precomputation','Doing reusable work once so later operations require less repeated effort.'),('Complexity','How a program’s required work or storage grows with input size.')],extra='stdexcept')
lab(56,'Validate an Amdahl calculation','Compute whole-program speedup from a fraction in [0,1] and a finite positive local speedup. Reject invalid fractions and nonpositive factors.',
 ['Fraction .4 and local factor 2 gives 1.25.','Fraction zero gives one; fraction one gives the local factor.','A factor below one correctly predicts a slowdown.','Invalid fractions and factor zero are rejected.'],r'''
double speedup(double fraction, double local) {
    if(!std::isfinite(fraction) || fraction < 0 || fraction > 1 || !std::isfinite(local) || local <= 0)
        throw std::invalid_argument("speedup model");
    return 1/((1-fraction)+fraction/local);
}
''',r'''
    assert(std::abs(speedup(.4,2)-1.25) < 1e-12);
    assert(speedup(0,2) == 1 && speedup(1,2) == 2 && speedup(1,.5) == .5);
    int rejected = 0;
    try { speedup(1.1,2); } catch(const std::invalid_argument&) { ++rejected; }
    try { speedup(.4,0); } catch(const std::invalid_argument&) { ++rejected; }
    assert(rejected == 2);
    std::cout << "whole=" << speedup(.4,2) << " unchanged=" << speedup(0,2) << '\n';
''','whole=1.25 unchanged=1\n',
 'Normalize the old execution time to one, reduce only the affected fraction, and invert the new time to express a speed ratio. The model assumes the workload and unaffected costs stay fixed. Moving work across boundaries, changing input sizes, or adding coordination can invalidate the measured fraction even when the algebra is correct.',
 'Use 1 + fraction*(local-1). That averages speed factors instead of adding execution times.',
 'Combine time contributions first, then take their reciprocal. Check the fraction-zero and fraction-one boundaries.',
 [('Amdahl’s law','A time-based bound on whole-system speedup when only a stated fraction of work changes.'),('Bottleneck','A resource or phase that limits the performance of the current workload.')],extra='cmath stdexcept')
lab(57,'Validate a complete bounded frame extent','Return whether a header and claimed payload fit inside a received extent, with a separate maximum payload of eight. Check the header before subtracting.',
 ['Header 2,payload 3,received 5 is valid.','A missing header and a truncated payload are invalid.','A payload above eight is invalid even if the buffer is larger.','A maximum-size claim cannot wrap past the checks.'],r'''
bool valid_frame(std::size_t received, std::size_t header, std::size_t payload) {
    return header <= received && payload <= 8 && payload <= received-header;
}
''',r'''
    assert(valid_frame(5,2,3));
    assert(!valid_frame(1,2,0) && !valid_frame(4,2,3));
    assert(!valid_frame(100,2,9));
    assert(!valid_frame(10,2,std::numeric_limits<std::size_t>::max()));
    std::cout << "complete=valid truncated=rejected oversized=rejected\n";
''','complete=valid truncated=rejected oversized=rejected\n',
 'A parser needs both a received-extent check and an application resource limit. A large backing allocation does not imply that unread bytes are valid input, and a valid extent does not authorize an arbitrarily large message. Short-circuit evaluation ensures subtraction occurs only after the header is known to fit.',
 'Compare against buffer capacity rather than the number of bytes actually received. Uninitialized or stale bytes can be mistaken for input.',
 'Carry valid extent with the data and validate against it. Keep allocation capacity as a separate property.',
 [('Trust boundary','A point where data or actions cross between different levels of trust or authority.'),('Valid extent','The portion of a buffer that the current operation may treat as meaningful input.')])
lab(58,'Show both detection and its limit','Model a record with payload and guard. Demonstrate that changing the guard is detected while a payload-only change can leave the guard check satisfied. Do not perform an invalid memory access.',
 ['The initial guard passes.','A deliberate guard-variable change fails the check.','Changing only the payload leaves the guard check passing.'],r'''
struct Guarded { int payload; unsigned guard; };
bool intact(const Guarded& value) { return value.guard == 0xA55Au; }
''',r'''
    Guarded record{7,0xA55Au};
    assert(intact(record));
    record.payload = 99;
    assert(intact(record));
    const bool payload_only = intact(record);
    record.guard ^= 1u;
    assert(!intact(record));
    std::cout << "payload-only=" << payload_only << " guard-change=" << intact(record) << '\n';
''','payload-only=1 guard-change=0\n',
 'A check proves only the fact it actually checks. This guard comparison can notice a changed guard value; it does not validate payload semantics or enforce bounds. The deliberate assignments are legal C++ operations, so the demonstration does not rely on undefined overflow behavior. Real mitigations add distinct defenses but do not replace a correct memory-access contract.',
 'Treat a passing guard check as proof that no memory-safety bug occurred anywhere in the operation.',
 'State the mitigation’s coverage and test its limitations. Keep length validation, lifetime rules, and build hardening as separate protections.',
 [('Mitigation','A measure that reduces certain consequences or detects certain failures without necessarily preventing the underlying bug.'),('Defense in depth','Using protections with different coverage so one failed assumption does not remove every defense.')])
lab(59,'Validate then commit a configuration update','Accept exactly version=N with decimal N in [1,99]. Parse into temporary state and change the live string only after complete validation.',
 ['version=2 replaces version=1.','A missing number, trailing junk, zero, or 100 is rejected.','Every rejected update preserves the previous live configuration.'],r'''
std::optional<int> parse_version(const std::string& candidate) {
    const std::string prefix = "version=";
    if(candidate.compare(0,prefix.size(),prefix) != 0) return std::nullopt;
    int value = 0;
    const char* first = candidate.data()+prefix.size();
    const char* last = candidate.data()+candidate.size();
    const auto parsed = std::from_chars(first,last,value);
    if(parsed.ec != std::errc{} || parsed.ptr != last || value < 1 || value > 99) return std::nullopt;
    return value;
}
bool update(std::string& live, const std::string& candidate) {
    const auto value = parse_version(candidate);
    if(!value) return false;
    std::string prepared = "version="+std::to_string(*value);
    live.swap(prepared);
    return true;
}
''',r'''
    std::string live = "version=1";
    for(const std::string candidate : {"version=","version=2x","version=0","version=100"}) {
        assert(!update(live,candidate) && live == "version=1");
    }
    assert(update(live,"version=2") && live == "version=2");
    std::cout << live << " rejected-updates=unchanged\n";
''','version=2 rejected-updates=unchanged\n',
 'from_chars reports both an error and the point where parsing stopped, so accepting a numeric prefix alone is not enough. The candidate must fit the entire grammar and range. A separately built string provides a commit point through swap; invalid input leaves the old value untouched. This is process-local exception-safe state replacement, not a durable file transaction.',
 'Accept version=2x because the numeric parser found an initial 2. The unconsumed suffix violates the format.',
 'Require successful conversion and parsed.ptr==last, then enforce the range before committing.',
 [('Strong exception guarantee','A promise that a failed operation leaves the relevant observable state unchanged.'),('Durability','A guarantee about state surviving the failures specified by a storage protocol, distinct from atomic visibility.')],extra='charconv system_error')
lab(60,'Build a byte-summary component with a clear contract','Summarize a string of up to one million bytes by its byte count and unsigned-byte sum. Treat embedded zero bytes as data and reject oversized input.',
 ['cat has three bytes and sum 312.','A two-byte string containing 0 and 255 has sum 255.','Empty input is valid.','The byte-count cap is enforced.'],r'''
struct Summary { std::size_t bytes; std::uint64_t sum; };
Summary summarize_bytes(const std::string& data) {
    if(data.size() > 1000000) throw std::length_error("input limit");
    std::uint64_t sum = 0;
    for(unsigned char byte : data) sum += byte;
    return {data.size(),sum};
}
''',r'''
    const auto word = summarize_bytes("cat");
    assert(word.bytes == 3 && word.sum == 312);
    const std::string binary{char(0),char(255)};
    assert(summarize_bytes(binary).bytes == 2 && summarize_bytes(binary).sum == 255);
    assert(summarize_bytes("").bytes == 0);
    bool rejected = false;
    try { summarize_bytes(std::string(1000001,'x')); } catch(const std::length_error&) { rejected = true; }
    assert(rejected);
    std::cout << "bytes=" << word.bytes << " sum=" << word.sum << " binary-sum=" << summarize_bytes(binary).sum << '\n';
''','bytes=3 sum=312 binary-sum=255\n',
 'A string stores a count as well as characters, so an embedded null does not end this range-based traversal. Converting each element to unsigned char makes the byte value nonnegative. The length cap bounds the sum well below uint64_t’s maximum. The lab isolates a computation that can later be used behind a file, pipe, or network input boundary.',
 'Use strlen on the binary input. It stops at the first zero byte and reports zero rather than two.',
 'Carry an explicit byte count and iterate the whole valid range. Keep text terminators separate from binary framing.',
 [('Translation unit','A source unit after preprocessing that the compiler processes under the language’s rules.'),('Linking','Combining compiled definitions and resolving references to build a program or library.')],extra='stdexcept')
CAPSTONE=r'''
struct Work { std::size_t index; std::string bytes; };
struct Result { bool accepted; std::size_t bytes; std::uint64_t sum; };
class WorkQueue {
    std::mutex mutex_;
    std::condition_variable readable_, writable_;
    std::deque<Work> work_;
    const std::size_t capacity_;
    bool closed_ = false;
public:
    explicit WorkQueue(std::size_t capacity) : capacity_(capacity) {
        if(capacity == 0) throw std::invalid_argument("capacity");
    }
    bool push(Work work) {
        std::unique_lock<std::mutex> lock(mutex_);
        writable_.wait(lock,[&] { return closed_ || work_.size() < capacity_; });
        if(closed_) return false;
        work_.push_back(std::move(work));
        readable_.notify_one();
        return true;
    }
    std::optional<Work> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        readable_.wait(lock,[&] { return closed_ || !work_.empty(); });
        if(work_.empty()) return std::nullopt;
        Work result = std::move(work_.front()); work_.pop_front();
        writable_.notify_one();
        return result;
    }
    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true; readable_.notify_all(); writable_.notify_all();
    }
};
Result process(const std::string& bytes) {
    if(bytes.size() > 4) return {false,0,0};
    std::uint64_t sum = 0;
    for(unsigned char byte : bytes) sum += byte;
    return {true,bytes.size(),sum};
}
std::vector<std::optional<Result>> pipeline(const std::vector<std::string>& inputs) {
    WorkQueue queue(2);
    std::vector<std::optional<Result>> results(inputs.size());
    std::vector<std::future<void>> workers;
    workers.reserve(2); // Allocate before any worker can wait on the queue.
    const auto worker = [&] {
        try {
            while(auto work = queue.pop()) results[work->index] = process(work->bytes);
        } catch(...) { queue.close(); throw; }
    };
    try {
        workers.push_back(std::async(std::launch::async,worker));
        workers.push_back(std::async(std::launch::async,worker));
        for(std::size_t i=0;i<inputs.size();++i)
            if(!queue.push({i,inputs[i]})) throw std::runtime_error("queue closed early");
        queue.close();
        for(auto& future : workers) future.get();
    } catch(...) {
        queue.close();
        for(auto& future : workers) if(future.valid()) {
            try { future.get(); } catch(...) {} // Preserve the original failure.
        }
        throw;
    }
    return results;
}
'''
lab(61,'Capstone: bounded concurrent byte processing','Integrate a capacity-two work queue, two workers, byte validation, owned payloads, ordered results, and shutdown. Accept payloads of at most four bytes; rejected items must receive a result without contributing to totals.',
 ['cat, A, empty, tools produce three accepted results and one rejection.','Accepted totals are four bytes and byte sum 377.','Results are printed in input order regardless of worker schedule.','An empty input list shuts down cleanly.','Workers finish before the queue and result storage leave scope.'],CAPSTONE,r'''
    const std::vector<std::string> inputs{"cat","A","","tools"};
    const auto results = pipeline(inputs);
    const std::array<Result,4> expected{{{true,3,312},{true,1,65},{true,0,0},{false,0,0}}};
    std::size_t accepted = 0, rejected = 0, bytes = 0;
    std::uint64_t sum = 0;
    for(std::size_t i=0;i<results.size();++i) {
        assert(results[i]);
        const auto result = *results[i];
        assert(result.accepted == expected[i].accepted && result.bytes == expected[i].bytes
               && result.sum == expected[i].sum);
        std::cout << i << " accepted=" << result.accepted << " bytes=" << result.bytes
                  << " sum=" << result.sum << '\n';
        if(result.accepted) { ++accepted; bytes += result.bytes; sum += result.sum; }
        else ++rejected;
    }
    assert(accepted == 3 && rejected == 1 && bytes == 4 && sum == 377);
    assert(pipeline({}).empty());
    std::cout << "accepted=" << accepted << " rejected=" << rejected
              << " bytes=" << bytes << " sum=" << sum << '\n';
''','0 accepted=1 bytes=3 sum=312\n1 accepted=1 bytes=1 sum=65\n2 accepted=1 bytes=0 sum=0\n3 accepted=0 bytes=0 sum=0\naccepted=3 rejected=1 bytes=4 sum=377\n',
 'Work items own their strings, so queue operations do not leave borrowed input pointers behind. Main assigns each index once; workers therefore write distinct result elements without resizing the vector. Queue predicates and close are protected by one mutex. Every normal and exceptional path closes the queue and collects workers before local shared state dies. Main prints after collection, so scheduling does not reorder the report. Queue capacity bounds queued jobs; it does not bound the already-loaded input vector’s total memory. A streaming input layer would need its own size and backpressure contract.',
 'Let workers push_back into the shared results vector without a lock, or let main return before collecting workers. The first can race on container internals; the second can leave dangling references.',
 'Allocate the result slots before starting workers, assign each slot to one input, and keep all shared objects alive through worker completion. Preserve close-and-collect cleanup if any operation fails.',
 [('Backpressure boundary','The point at which bounded downstream capacity makes an upstream producer wait or reject work.'),('End-to-end invariant','A rule spanning several components, such as every accepted input producing exactly one recorded result.')],extra='condition_variable deque future stdexcept')

def generate_labs():
    assert set(LABS) == set(range(1,62))
    for n,data in LABS.items():
        # Extra headers are explicit so examples do not rely on transitive declarations.
        needed=[]
        if n in (60,61):
            data['code']=data['code'].replace('int main() {', "static_assert('A' == 65 && 'c' == 99 && 'a' == 97 && 't' == 116);\nint main() {")
        if 'std::copy' in data['code'] or 'std::find' in data['code'] or 'std::next_permutation' in data['code']:needed.append('algorithm')
        if 'std::invalid_argument' in data['code'] or 'std::length_error' in data['code']:needed.append('stdexcept')
        for header in needed:
            if '#include <'+header+'>' not in data['code']:data['code']='#include <'+header+'>\n'+data['code']
        if n==16:data['code']='#include <limits>\n'+data['code'].replace('unsigned sum_before','static_assert(std::numeric_limits<unsigned>::max() >= 499500u, "32-bit unsigned required");\nunsigned sum_before')
        if n==60:data['code']='#include <climits>\n'+data['code'].replace('struct Summary','static_assert(CHAR_BIT == 8, "8-bit bytes required");\nstruct Summary')
        base=ROOT/f'systems_labs/ch{n:02}';base.mkdir(parents=True,exist_ok=True)
        starter='// LAB: '+data['title']+'\n// '+data['task']+'\n// This starter verifies the original example. Extend it to satisfy the lab checks.\n'+ROWS[n]['code']
        (base/'starter.cpp').write_text(starter);(base/'starter_expected.txt').write_text(ROWS[n]['output'])
        (base/'solution.cpp').write_text(data['code']);(base/'solution_expected.txt').write_text(data['output'])
        data['starter']=f'systems_labs/ch{n:02}/starter.cpp';data['solution']=f'systems_labs/ch{n:02}/solution.cpp'
        (base/'README.md').write_text('# '+data['title']+'\n\n'+data['task']+'\n\n## Acceptance checks\n\n'+'\n'.join('- '+x for x in data['checks'])+'\n\n## Build\n\nFrom the source ZIP root:\n\n```sh\ng++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -pthread '+data['solution']+' -o lab\n./lab\n```\n\nSubstitute `starter.cpp` to build the starting program. It runs the original example, not the completed lab. Compare its output to `starter_expected.txt`; the finished solution uses `solution_expected.txt`. Keep assertions enabled.\n\n## Explained solution\n\n'+data['reason']+'\n\n## Bug to diagnose\n\n'+data['bug']+'\n\n'+data['repair']+'\n')
    (ROOT/'teaching/systems-labs.json').write_text(json.dumps(LABS,ensure_ascii=False,indent=2)+'\n')
    print('Generated 61 lab starters and 61 complete reference solutions.')
if __name__=='__main__':generate_labs()
