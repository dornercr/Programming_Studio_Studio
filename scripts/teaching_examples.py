"""Editable, original chapter workshops. All programs are complete C++17.
Models explicitly simplify hardware/OS behavior; they are not implementations of an OS.
"""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def headers(code):
 includes=['cassert','iostream']
 symbols={'algorithm':['std::min','std::max','std::sort'],'array':['std::array'],'atomic':['std::atomic','std::memory_order'], 'cstdint':['std::uint'], 'cstddef':['std::size_t'], 'limits':['std::numeric_limits'], 'map':['std::map'], 'memory':['std::make_unique','std::unique_ptr','std::make_shared'], 'mutex':['std::mutex','std::lock_guard'], 'numeric':['std::accumulate'], 'optional':['std::optional'], 'sstream':['std::ostringstream','std::istringstream'], 'stdexcept':['std::out_of_range'], 'string':['std::string','std::stoi','std::to_string'], 'thread':['std::thread','std::this_thread'], 'utility':['std::move','std::swap','std::pair'], 'vector':['std::vector']}
 for header,terms in symbols.items():
  if any(term in code for term in terms):includes.append(header)
 return ''.join('#include <'+h+'>\n' for h in sorted(includes))
ROWS={}
def add(n,title,problem,code,output,steps,trap,question,answer,alternative,kind='C++ model'):
 ROWS[n]=dict(title=title,problem=problem,code=headers(code)+'\nint main() {\n'+code.strip('\n')+'\n}\n',output=output,steps=[dict(action=a,state=s,why=w) for a,s,w in steps],pitfall=trap,discussion=dict(question=question,answer=answer),alternative=alternative,kind=kind,filename=f'systems_examples/ch{n:02}/main.cpp')
add(1,'One request crosses several layers','A program must print the sum of two readings. Follow the value from a C++ expression to a stream without assuming that one source line equals one CPU instruction.',r'''
    const int first = 7;
    const int second = 5;
    const int total = first + second;
    std::ostringstream message;
    message << "total=" << total;
    std::cout << message.str() << '\n';
    assert(total == 12);
''','total=12\n',[
 ('Evaluate the expression','7 + 5 becomes 12','The C++ expression describes a value; an optimizer may compute it before execution.'),
 ('Format the value','message contains total=12','A stream converts the integer into text characters. This is separate from addition.'),
 ('Write the text','The output stream receives total=12 and a newline','The library and operating system handle output. Buffering means a newline is not a universal promise of an immediate system call.')],
 'Counting three source statements does not tell you the number of machine instructions or system calls.',
 'If output is slow, would replacing + with an assembly instruction necessarily help?',
 'No. The addition may already be folded to a constant. Measure the output path and separate computation from formatting and I/O before choosing a change.',
 'A direct std::cout expression is shorter. The intermediate stream is used here only to expose the boundary between a number and formatted text.','Complete C++ program')
add(2,'A value copy and a borrowed reference','Change one local copy and one borrowed object. Predict which updates the caller can see.',r'''
    std::vector<int> readings{2, 4};
    auto copy = readings;
    auto& borrowed = readings;
    copy[0] = 9;
    borrowed[1] = 7;
    for (const int value : readings) std::cout << value << ' ';
    std::cout << '\n';
    assert(readings[0] == 2 && readings[1] == 7);
''','2 7 \n',[
 ('Copy readings','copy has its own {2, 4}','auto without & creates a separate vector here.'),
 ('Bind borrowed','borrowed names readings','The & in this declaration means reference: no second vector is created.'),
 ('Change both','readings is {2, 7}; copy is {9, 4}','Only the assignment through borrowed reaches the original. In the for loop, : means take each element from the range on the right.')],
 'A reference does not extend the vector’s lifetime. Returning a reference to a local vector would leave a dangling reference.',
 'Would const auto& borrowed = readings still allow borrowed[1] = 7?',
 'No. const prevents this change through borrowed. Another non-const name may still change readings, so const on one reference does not freeze every alias.',
 'Copy when independent data is required; borrow when the caller retains ownership and the lifetime is clear.','Complete C++ program')
add(3,'Replace only one packed field','Bits 4 through 7 hold a four-bit setting. Replace that field in 0xA5 with 3 while preserving every other bit.',r'''
    const unsigned old_value = 0xA5u;
    const unsigned field = 3u;
    const unsigned mask = 0xFu << 4;
    const unsigned result = (old_value & ~mask) | (field << 4);
    std::cout << std::hex << result << '\n';
    assert(result == 0x35u);
''','35\n',[
 ('Make a mask','0xF << 4 selects the four target bits','F represents binary 1111; shifting moves that group to bit positions 4–7.'),
 ('Clear the old field','old_value & ~mask leaves the low 5','~ reverses mask bits; & keeps only bits allowed by both operands.'),
 ('Insert the replacement','0x05 | 0x30 = 0x35','| combines the two non-overlapping groups. field must fit in four bits.')],
 'Using old_value | (field << 4) leaves old one-bits in the field. It yields 0xB5, not 0x35.',
 'What extra check is needed if field comes from user input?',
 'Reject values above 15, or explicitly define truncation with field & 0xF. Silently spilling into nearby fields would violate the preservation requirement.',
 'Separate integer members are simpler when compact storage or a device format is not required.')
add(4,'Check multiplication before allocating','An artificial buffer limit is 100 bytes. Each record uses 12 bytes. Reject a request for 9 records without forming an overflowing product.',r'''
    const std::size_t limit = 100;
    const std::size_t count = 9;
    const std::size_t width = 12;
    const bool fits = width == 0 || count <= limit / width;
    std::cout << (fits ? "fits" : "reject") << '\n';
    assert(!fits);
''','reject\n',[
 ('State the bound','bytes must be at most 100','A capacity check and the integer type limit are separate bounds; real code must respect both.'),
 ('Divide before multiplying','100 / 12 = 8','Integer division discards the remainder. Nine records exceed the maximum safe count.'),
 ('Handle zero width','width == 0 short-circuits the ||','The right side is not evaluated when the left is true, preventing division by zero.')],
 'Checking count * width only after multiplication may observe a wrapped unsigned result. Signed overflow is undefined behavior.',
 'What replaces 100 when checking the representable range of std::size_t?',
 'Use std::numeric_limits<std::size_t>::max(), then separately compare with any allocation or application limit. Arithmetic safety does not guarantee allocation success.',
 'A fixed-size array avoids dynamic byte-count arithmetic when the bound is small and known.')
add(5,'A small value can disappear in a large sum','Model a machine using IEEE 754 binary64 for double. Add one to 2^53 and test whether the stored value changes.',r'''
    static_assert(std::numeric_limits<double>::is_iec559);
    static_assert(std::numeric_limits<double>::digits == 53);
    const double large = 9007199254740992.0;
    const double sum = large + 1.0;
    std::cout << std::boolalpha << (sum == large) << '\n';
    assert(sum == large);
''','true\n',[
 ('Inspect the format','double has 53 bits of precision in this program','The static assertions make the example’s platform assumption explicit.'),
 ('Look at neighboring values','At 2^53, the next larger representable number is 2 away','A fixed number of significant bits gives larger spacing at larger magnitudes.'),
 ('Round the exact sum','2^53 + 1 rounds to 2^53 under the default nearest-even mode','The addition is rounded; the stored result is not the exact mathematical integer.')],
 'Exact equality can be valid for this deliberate demonstration. It is usually a poor universal test for the results of different floating-point calculations.',
 'Would storing money as double fix this by printing only two decimal places?',
 'No. Formatting changes displayed text, not stored arithmetic. Integer minor units or a suitable decimal representation may be better when the contract requires exact decimal amounts.',
 'Use integers for exact bounded counts; use floating point for a wide range of approximate measurements. This example assumes the normal nearest-even rounding mode.')
add(6,'Array addresses count elements, not raw bytes','Three int objects form an array. Find the third element and the number of elements between its address and the first.',r'''
    const int values[]{10, 20, 30};
    const int* first = values;
    const int* third = first + 2;
    std::cout << *third << ' ' << (third - first) << '\n';
    assert(*third == 30 && third - first == 2);
''','30 2\n',[
 ('Begin at element zero','first points at 10','An array expression converts to a pointer to its first element in this initialization.'),
 ('Advance by two','third points at 30','Pointer addition scales by sizeof(int); it is not a two-byte move.'),
 ('Subtract related pointers','third - first is 2','The difference counts elements and is defined here because both pointers refer into the same array.')],
 'Assuming sizeof(int) is always four confuses one common platform with the language rule.',
 'Can the same subtraction be used for two separately allocated int objects?',
 'No. Pointer subtraction requires the proper same-array relationship. Similar numeric addresses do not create that relationship.',
 'Indexing values[2] is clearer when no pointer-based API is involved.')
add(7,'Stop at the one-past pointer','Sum a bounded range of three integers. The end pointer must stop the loop and must never be read.',r'''
    const int values[]{3, 5, 7};
    const int* cursor = values;
    const int* end = values + 3;
    int total = 0;
    while (cursor != end) {
        total += *cursor;
        ++cursor;
    }
    std::cout << total << '\n';
    assert(total == 15);
''','15\n',[
 ('Start','cursor points at 3; total is 0','The half-open range includes the first element and excludes end.'),
 ('Read and advance','totals become 3, then 8, then 15','*cursor reads the live element; ++cursor moves by one element.'),
 ('Stop','cursor equals end','Forming the one-past pointer is allowed. Dereferencing it is not.')],
 'Using cursor <= end performs one extra read and has undefined behavior. There is no reliable extra output to predict.',
 'Why is [begin, end) convenient for an empty range?',
 'begin equals end, so the loop executes zero times. The same rule works without a separate empty-range branch.',
 'A range-based for loop or std::accumulate is simpler when you already have a standard container.')
add(8,'Scope and object lifetime are different questions','Keep a heap-allocated integer alive after a temporary pointer variable leaves its block.',r'''
    std::unique_ptr<int> owner;
    {
        auto temporary = std::make_unique<int>(42);
        owner = std::move(temporary);
        assert(!temporary);
    }
    std::cout << *owner << '\n';
    assert(*owner == 42);
''','42\n',[
 ('Allocate','temporary owns an int containing 42','The pointer variable and the allocated int are different objects with different lifetimes.'),
 ('Transfer ownership','owner becomes responsible for the int','std::move permits moving the unique_ptr. The object itself stays allocated.'),
 ('Leave the inner block','temporary dies; the int remains','owner outlives the inner block and releases the int at the end of main.')],
 'Returning the address of an ordinary local int would not transfer its lifetime. That object would die on return.',
 'Does this demonstrate a fixed numeric heap or stack address?',
 'No. It demonstrates C++ storage duration and ownership. Actual process addresses and segment arrangements depend on the platform and execution.',
 'An ordinary local int is sufficient when no lifetime needs to cross a scope boundary.','Complete C++ program')
add(9,'Let a container own a growing sequence','Append three readings and keep their values correct as storage grows. Do not retain a pointer into storage across possible reallocation.',r'''
    std::vector<int> values;
    values.reserve(1);
    values.push_back(4);
    values.push_back(6);
    values.push_back(8);
    const int total = std::accumulate(values.begin(), values.end(), 0);
    std::cout << values.size() << ' ' << total << '\n';
    assert(values.size() == 3 && total == 18);
''','3 18\n',[
 ('Reserve room','capacity is at least 1, size remains 0','Capacity describes storage; size counts constructed elements.'),
 ('Append','size becomes 1, 2, then 3','The vector manages allocation and moves or copies elements when growth requires it.'),
 ('Read after growth','sum is 18','Obtaining iterators after the changes avoids using an invalidated pointer or iterator.')],
 'Writing values[0] after reserve(1) but before push_back is out of bounds: reserve does not create an element.',
 'Is calling reserve on every insertion a useful growth strategy?',
 'Usually not. Repeatedly requesting just one more element can defeat efficient growth. Reserve a credible final bound once, or let the vector manage growth.',
 'Use std::array for a fixed count; use vector when the count changes.','Complete C++ program')
add(10,'Account for allocator alignment','A model arena aligns every allocation to an eight-byte boundary. Allocate requests of 5 and 9 bytes, tracking the consumed space.',r'''
    const std::size_t capacity = 32;
    std::size_t used = 0;
    for (std::size_t request : {5u, 9u}) {
        const std::size_t start = (used + 7) / 8 * 8;
        assert(start <= capacity && request <= capacity - start);
        std::cout << start << ' ';
        used = start + request;
    }
    std::cout << "used=" << used << '\n';
''','0 8 used=17\n',[
 ('Allocate 5 bytes','start 0; used becomes 5','The first address is already aligned.'),
 ('Round the next start up','5 becomes 8','Three padding bytes satisfy the eight-byte alignment rule.'),
 ('Allocate 9 bytes','start 8; used becomes 17','Only 14 bytes were requested, but 17 bytes of the arena span have been consumed.')],
 'Advancing by the requested byte count alone can violate alignment. The rounding formula also needs overflow checks for unbounded inputs.',
 'Can this bump model release the first allocation while keeping the second and recover that hole?',
 'Not with just the used counter. It supports simple allocation and whole-arena reset. General freeing needs extra state, such as a free list, and different guarantees.',
 'Use normal containers or a standard allocator unless measured allocation costs or a shared lifetime justify a custom arena.')
add(11,'Turn an off-by-one read into a checked failure','A three-element vector has valid indexes 0, 1, and 2. Show the consequence of asking for index 3 using a checked operation.',r'''
    const std::vector<int> values{8, 9, 10};
    try {
        std::cout << values.at(3) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "out of range\n";
    }
    assert(values.size() == 3);
''','out of range\n',[
 ('State the range','0 <= index < 3','The count is not itself a valid element index.'),
 ('Check index 3','at throws std::out_of_range','No element value is produced by the failed expression.'),
 ('Handle the failure','the catch prints out of range','The handler makes the failure visible without performing an invalid access.')],
 'Replacing at(3) with operator[](3) removes the bounds check; it does not make the access valid. The resulting undefined behavior has no guaranteed output.',
 'Is catching every bounds exception enough to make a parser correct?',
 'No. Validate the expected format and length before using indexes. Exceptions catch this symptom, but the parser must still define what incomplete input means.',
 'A range-based loop is less error-prone when every element should be visited.','Complete C++ program')
add(12,'Trace an instruction-set model','An abstract machine has a program counter and one accumulator. Execute LOAD 4, ADD 3, then HALT.',r'''
    const std::array<int, 3> operands{4, 3, 0};
    int accumulator = 0;
    std::size_t pc = 0;
    accumulator = operands[pc++];
    accumulator += operands[pc++];
    std::cout << "pc=" << pc << " acc=" << accumulator << '\n';
    assert(pc == 2 && accumulator == 7);
''','pc=2 acc=7\n',[
 ('LOAD 4','accumulator 4; pc 1','The operation changes architectural state: the state promised by the instruction set.'),
 ('ADD 3','accumulator 7; pc 2','The next instruction sees the preceding result.'),
 ('HALT at position 2','No further state change in this model','How real hardware pipelines or caches these operations is a separate implementation question.')],
 'This C++ program models an instruction sequence. Its C++ statements are not an encoding of any real ISA.',
 'Could two CPUs execute the same ISA with different numbers of internal stages?',
 'Yes. They must preserve the required architectural behavior, but their internal scheduling and performance may differ.',
 'Use a real assembler and ISA manual when instruction encoding or exact machine effects matter.')
add(13,'Model a 32-bit register write','Under x86-64 rules, writing EAX clears the upper half of RAX. Contrast this with merely replacing the low 16 bits.',r'''
    const std::uint64_t old = 0xFFFFFFFF00000000ULL;
    const std::uint32_t eax = 7;
    const std::uint64_t after_eax = eax;
    const std::uint64_t after_ax = (old & ~0xFFFFULL) | 7ULL;
    std::cout << std::hex << after_eax << ' ' << after_ax << '\n';
    assert(after_eax == 7 && after_ax == 0xFFFFFFFF00000007ULL);
''','7 ffffffff00000007\n',[
 ('Initial RAX','ffffffff00000000','The upper half is deliberately nonzero.'),
 ('Write EAX = 7','RAX becomes 0000000000000007','A 32-bit general-register destination zero-extends on x86-64.'),
 ('Instead write AX = 7','RAX becomes ffffffff00000007','A 16-bit subregister write preserves the other bits.')],
 'Applying the EAX clearing rule to AX leads to an incorrect trace. Width is part of the instruction’s meaning.',
 'Does assigning a C++ short variable force an AX write?',
 'No. The compiler chooses registers and instructions. This is a numeric model of the ISA rule, not a guarantee about the generated code.',
 'Inspect generated assembly when the actual register operations are the question.')
add(14,'Decode base plus scaled index','An address model uses base 1000, index 3, scale 4, and displacement 8. Compute the effective address without reading memory.',r'''
    const std::uint64_t base = 1000;
    const std::uint64_t index = 3;
    const std::uint64_t scale = 4;
    const std::uint64_t displacement = 8;
    const auto address = base + index * scale + displacement;
    std::cout << address << '\n';
    assert(address == 1020);
''','1020\n',[
 ('Scale the index','3 * 4 = 12','The scale can represent an element width in an addressing mode.'),
 ('Add base and displacement','1000 + 12 + 8 = 1020','The calculation produces an address-shaped value.'),
 ('Separate address from load','No byte at 1020 is read','A real load uses the address to fetch data; LEA computes the effective address without that memory access.')],
 'Treating LEA as a load confuses the address with the value stored there. Dereferencing the integer 1020 in C++ would not be a valid portable demonstration.',
 'If memory at address 1020 contained 99, what would LEA return in this model?',
 '1020. A load from that address would return the stored data, subject to width and access validity.',
 'Use array indexing in C++; use the effective-address model to understand the assembly it becomes.')
add(15,'Signed division has a quotient and a remainder','Divide -17 by 5 and verify how C++17 relates the quotient and remainder.',r'''
    const int value = -17;
    const int divisor = 5;
    const int quotient = value / divisor;
    const int remainder = value % divisor;
    std::cout << quotient << ' ' << remainder << '\n';
    assert(quotient * divisor + remainder == value);
''','-3 -2\n',[
 ('Divide','quotient is -3','Integer division truncates toward zero when the quotient is representable.'),
 ('Find the remainder','remainder is -2','-3 * 5 + -2 reconstructs -17.'),
 ('Check prerequisites','divisor is nonzero; quotient fits','Division by zero and an unrepresentable signed quotient are not valid operations.')],
 'Assuming division rounds down would predict -4, which is wrong for this C++ expression. Assembly signed division also requires the correct dividend preparation.',
 'Can a signed right shift be substituted for division by 2 in every C++17 expression?',
 'No. Negative values expose different rounding behavior, and signed right shift of negative values is implementation-defined in C++17. Preserve the required semantics before optimizing.',
 'Write / and % first. Let the compiler select a valid machine sequence for known divisors.','Complete C++ program')
add(16,'A loop is a repeated branch','Sum indexes from zero up to, but excluding, 4. Track the test, body, and increment.',r'''
    int sum = 0;
    int i = 0;
    while (i < 4) {
        sum += i;
        ++i;
    }
    std::cout << "i=" << i << " sum=" << sum << '\n';
    assert(i == 4 && sum == 6);
''','i=4 sum=6\n',[
 ('Test i < 4','i = 0, 1, 2, 3 enter the body','The condition decides whether control reaches the addition.'),
 ('Accumulate and advance','sum = 0, 1, 3, 6','The update to i ensures progress toward the stopping condition.'),
 ('Test once more','i = 4 exits','The condition is evaluated five times, though the body runs only four times.')],
 'Changing < to <= adds 4 and yields sum 10. Omitting ++i leaves this loop running forever.',
 'Where would a conditional jump appear in a simple assembly translation?',
 'Near the loop test or back edge, depending on layout. A taken or untaken branch selects the next instruction address; the exact arrangement is a compiler choice.',
 'A for loop expresses initialization, test, and increment together; both forms can describe the same control flow.','Complete C++ program')
add(17,'Keep a needed value across a call','A caller has a saved value of 10 and asks a helper to double 3. The final result must be 16 even if temporary registers are reused.',r'''
    const auto helper = [](int argument) { return argument * 2; };
    const int saved = 10;
    const int returned = helper(3);
    std::cout << saved + returned << '\n';
    assert(saved + returned == 16);
''','16\n',[
 ('Before the call','saved = 10; argument = 3','The compiler must preserve any value needed after the call.'),
 ('In the callee','return value = 6','A calling convention defines how arguments and return values are passed at a machine boundary.'),
 ('After the call','10 + 6 = 16','The preservation may use a callee-saved register, a spill, or another valid optimization.')],
 'Handwritten assembly cannot assume every register survives a call. Caller-saved and callee-saved duties depend on the target ABI.',
 'Does this source guarantee that a stack frame or even a call instruction exists?',
 'No. The compiler may inline the helper and fold constants. Inspect a particular build when studying its ABI-level call sequence.',
 'Ordinary C++ functions let the compiler honor the platform ABI; assembly boundaries require explicit agreement.','Complete C++ program')
add(18,'Aliasing changes the result of a rewrite','Two references may name the same integer. Compare updating then reading through the second reference with remembering its old value first.',r'''
    int x = 4;
    int& a = x;
    int& b = x;
    const int remembered = b;
    a = 9;
    const int after = b;
    std::cout << remembered << ' ' << after << '\n';
    assert(remembered == 4 && after == 9);
''','4 9\n',[
 ('Bind both references','a and b name x','Different variable names do not imply different storage.'),
 ('Save the old read','remembered = 4','This value copy is independent of later changes.'),
 ('Write a, then read b','x = 9; after = 9','Moving the read before the write changes observable behavior when the references alias.')],
 'A hand optimization that caches b before every write through a is wrong unless the design proves the storage cannot overlap.',
 'Why can the compiler sometimes remove repeated loads anyway?',
 'It can use proven facts about aliasing, control flow, and the language rules. It must preserve observable behavior for all defined inputs, not just one test.',
 'Prefer clear code and measured compiler output before adding manual caches.','Complete C++ program')
add(19,'Model a protected request boundary','A kernel-like function must reject a request for more than 8 bytes. The caller cannot bypass the length check in this model.',r'''
    const auto kernel_copy = [](std::size_t length) {
        return length <= 8 ? "accepted" : "rejected";
    };
    std::cout << kernel_copy(4) << ' ' << kernel_copy(12) << '\n';
''','accepted rejected\n',[
 ('Request 4','accepted','The request meets the length contract.'),
 ('Request 12','rejected','The trusted side enforces its boundary instead of assuming its caller behaved.'),
 ('Separate model from protection','This is an ordinary function in one process','Real user/kernel isolation depends on hardware privilege and OS-controlled entry paths; a C++ access specifier is not that isolation.')],
 'Checking only in user code is insufficient when a privileged service receives requests from untrusted callers.',
 'What is missing before this could safely copy real user memory?',
 'Validating the address range, permissions, lifetime, partial-failure behavior, and concurrency are all required. A length check alone is only one part of the contract.',
 'An ordinary function boundary is enough for cooperating code inside one trust domain. Privilege separation solves a stronger problem.')
add(20,'A recoverable fault resumes the same instruction','Model a load that faults because a permitted page is absent. Resolve the page, retry the load, then advance the program counter.',r'''
    bool present = false;
    int pc = 8;
    int faults = 0;
    if (!present) { ++faults; present = true; }
    assert(present);
    const int loaded = 42;
    ++pc;
    std::cout << "faults=" << faults << " pc=" << pc << " value=" << loaded << '\n';
''','faults=1 pc=9 value=42\n',[
 ('Attempt instruction 8','page absent; one fault','The instruction cannot complete with the current translation state.'),
 ('Resolve the permitted page','page becomes present; resume point remains 8','A recoverable fault lets the OS repair a condition and retry.'),
 ('Complete the retried load','value 42; pc advances to 9','Advancing past the load before it succeeds would silently skip required work.')],
 'Not every fault is recoverable. An invalid access may terminate the process instead of resuming it.',
 'How is this different from a timer interrupt?',
 'The fault is caused by the current instruction’s attempt. A timer interrupt is an external event relative to that instruction stream. Both use controlled entry, but the reason and resumption rules differ.',
 'Use a state model for understanding control transfer; real behavior requires the target OS and ISA contracts.')
add(21,'Waiting is different from being runnable','Model process A issuing I/O while process B is ready. A must leave the run queue until its I/O completes.',r'''
    std::string a = "running";
    std::string b = "ready";
    a = "waiting";
    b = "running";
    std::cout << "A=" << a << " B=" << b << '\n';
    a = "ready";
    std::cout << "A=" << a << " B=" << b << '\n';
''','A=waiting B=running\nA=ready B=running\n',[
 ('A requests blocking I/O','A becomes waiting','A cannot make progress until an external completion occurs.'),
 ('Schedule B','B becomes running','A different runnable process can use the CPU while A waits.'),
 ('A receives completion','A becomes ready','Ready means eligible to run, not necessarily running at that instant.')],
 'Treating waiting and ready as the same state can make a scheduler repeatedly choose work that cannot progress.',
 'Does B running while A waits require two CPU cores?',
 'No. One core can switch from A to B. Parallel execution requires simultaneous execution resources; concurrency only requires overlapping work in progress.',
 'Sequential execution is simpler when tasks never wait and there is no need to overlap their lifetimes.')
add(22,'Equal virtual addresses need not share data','Two process models both use virtual address 0x1000. Their private maps must hold independent values.',r'''
    std::map<unsigned, int> process_a{{0x1000, 7}};
    std::map<unsigned, int> process_b{{0x1000, 9}};
    process_a.at(0x1000) = 11;
    std::cout << process_a.at(0x1000) << ' ' << process_b.at(0x1000) << '\n';
    assert(process_b.at(0x1000) == 9);
''','11 9\n',[
 ('Look up A:0x1000','value 7 in A’s map','A virtual address is interpreted in a particular address space.'),
 ('Write A:0x1000','A’s value becomes 11','Changing one private mapping does not change the other.'),
 ('Read B:0x1000','value remains 9','The same numeric address alone does not establish shared backing storage.')],
 'Logging only the numeric address loses the process context needed to identify memory.',
 'What would intentional shared memory change?',
 'Both address-space mappings would refer to the same backing object or physical storage. Their virtual addresses could be equal or different; shared backing is the key fact.',
 'Use ordinary references within one process. Cross-process sharing needs an OS-supported mapping and a synchronization contract.')
add(23,'Split a virtual address into page and offset','Pages are 4096 bytes. Translate virtual address 0x1234 when virtual page 1 maps to frame 7.',r'''
    const unsigned page_size = 4096;
    const unsigned address = 0x1234;
    const unsigned page = address / page_size;
    const unsigned offset = address % page_size;
    const unsigned frame = 7;
    const unsigned physical = frame * page_size + offset;
    std::cout << "page=" << page << " offset=" << offset
              << " physical=" << physical << '\n';
    assert(page == 1 && offset == 564 && physical == 29236);
''','page=1 offset=564 physical=29236\n',[
 ('Divide by page size','page number 1','Whole groups of 4096 bytes select the virtual page.'),
 ('Take the remainder','offset 564','Translation preserves the position inside the page.'),
 ('Substitute the frame','7 * 4096 + 564 = 29236','The page table supplies a frame; permission and presence checks must also succeed.')],
 'Multiplying the virtual page number by the page size reconstructs a virtual address, not the translated physical address.',
 'What changes if the address is 0x1235?',
 'The page number remains 1 and the offset becomes 565. The same page-table entry can translate it, subject to the same access checks.',
 'The arithmetic model is sufficient for decomposition; a real translation also needs page-table state and access type.')
add(24,'Check permissions as well as presence','A model page-table entry is present and readable but not writable. Test a read and a write against the same entry.',r'''
    const bool present = true;
    const bool readable = true;
    const bool writable = false;
    const auto allowed = [&](bool write) {
        return present && (write ? writable : readable);
    };
    std::cout << std::boolalpha << allowed(false) << ' ' << allowed(true) << '\n';
    assert(allowed(false) && !allowed(true));
''','true false\n',[
 ('Check presence','entry has backing in this model','Presence alone does not authorize every kind of access.'),
 ('Request a read','true','The read permission satisfies this access.'),
 ('Request a write','false','The access type requires a different permission. A real processor may raise a protection fault.')],
 'A translation that returns a frame without checking access permissions can break isolation.',
 'Could a denied write be part of normal copy-on-write behavior?',
 'Yes. The OS may deliberately mark a shared private page read-only, then handle the write fault by creating a private writable copy. Other denied writes are invalid; the mapping policy decides.',
 'A flat table is easy to model; real multilevel tables reduce storage for sparse address spaces at the cost of more involved walks.')
add(25,'A TLB miss can still lead to a cache hit','Model a valid mapping absent from the translation cache while the requested data is already in the data cache.',r'''
    bool tlb_has_mapping = false;
    const bool page_present = true;
    const bool data_cached = true;
    int walks = 0;
    if (!tlb_has_mapping) {
        ++walks;
        assert(page_present);
        tlb_has_mapping = true;
    }
    std::cout << "walks=" << walks << " data="
              << (data_cached ? "hit" : "miss") << '\n';
''','walks=1 data=hit\n',[
 ('Look in the TLB','mapping absent','The TLB caches translations, not the program’s requested data value.'),
 ('Walk the table','mapping present; one walk','A valid resident page may require a walk without a page fault.'),
 ('Look up the data','data cache hit','Translation lookup and data lookup are different events, though real hardware may overlap parts of them.')],
 'Equating every TLB miss with a page fault overcounts OS work. A page-table walk can succeed normally.',
 'Would a TLB hit prove that the data is in L1?',
 'No. It supplies a cached translation subject to permissions. The data-cache lookup may still miss.',
 'Keep separate counters for translation misses, page faults, and data-cache misses when diagnosing performance.')
add(26,'Copy on write separates values only when needed','Two private address-space models initially share a value of 7. A write in the child should leave the parent’s value unchanged.',r'''
    auto parent = std::make_shared<int>(7);
    auto child = parent;
    if (!child.unique()) child = std::make_shared<int>(*child);
    *child = 9;
    std::cout << *parent << ' ' << *child << '\n';
    assert(*parent == 7 && *child == 9);
''','7 9\n',[
 ('Share initial backing','parent and child both read 7','Sharing unchanged storage saves a copy.'),
 ('Prepare a child write','allocate a private copy','The model checks sharing before allowing mutation.'),
 ('Write 9','parent 7, child 9','The copy preserves the promise that private modifications are independent.')],
 'This single-threaded smart-pointer model is not an OS page-fault handler. Checking unique() is not a safe general concurrent copy-on-write protocol.',
 'Why might fork avoid copying every page immediately?',
 'A child may soon replace its program with exec, or many pages may remain unchanged. Copying only on a permitted private write avoids unnecessary copying.',
 'Eager copying is simpler when data is small or almost every element will change.')
add(27,'Latency and bandwidth answer different questions','A link has 10 microseconds of startup latency and transfers 100 bytes per microsecond. Estimate one 100-byte transfer and one 10,000-byte transfer.',r'''
    const double latency = 10.0;
    const double bytes_per_us = 100.0;
    const auto time = [&](double bytes) { return latency + bytes / bytes_per_us; };
    std::cout << time(100) << ' ' << time(10000) << '\n';
''','11 110\n',[
 ('Transfer 100 bytes','10 + 1 = 11 microseconds','Startup dominates the small transfer.'),
 ('Transfer 10,000 bytes','10 + 100 = 110 microseconds','Sustained transfer cost dominates the larger request.'),
 ('Compare with 100 small transfers','100 * 11 = 1100 microseconds if serialized','Batching amortizes startup in this deliberately simple model.')],
 'Doubling bandwidth does not halve the fixed latency. Real overlap and queueing require a richer model.',
 'Which change helps the small request more: halving latency or doubling bandwidth?',
 'Halving latency gives 6 microseconds; doubling bandwidth gives 10.5. The answer follows from the measured cost components, not the label fast memory.',
 'Use a latency model for dependent small accesses and a throughput model for large streaming work.')
add(28,'Traversal order changes reuse','Model a 4-by-4 row-major array and a one-line cache whose line holds four elements. Compare row-first and column-first traversal.',r'''
    const auto misses = [](bool by_row) {
        int resident = -1, count = 0;
        for (int outer = 0; outer < 4; ++outer)
            for (int inner = 0; inner < 4; ++inner) {
                const int index = by_row ? outer * 4 + inner : inner * 4 + outer;
                const int block = index / 4;
                if (block != resident) { ++count; resident = block; }
            }
        return count;
    };
    std::cout << misses(true) << ' ' << misses(false) << '\n';
    assert(misses(true) == 4 && misses(false) == 16);
''','4 16\n',[
 ('Visit a row','indexes 0, 1, 2, 3 share block 0','The first access misses; the next three reuse the fetched line.'),
 ('Visit a column','indexes 0, 4, 8, 12 visit four blocks','A one-line cache replaces its resident block on each step.'),
 ('Count all accesses','row-first 4 misses; column-first 16','Order changes locality without changing which elements are visited.')],
 'These counts belong to the stated tiny cache. A real cache with more lines, associativity, or prefetching can behave differently.',
 'Would the same order still be best for a column-major representation?',
 'The contiguous direction changes. Match traversal to the actual layout, then check other algorithm dependencies and measure.',
 'Loop interchange is useful only when it preserves the computation’s required order and results.')
add(29,'Find a cache line’s set and tag','A direct-mapped cache has four sets and 16-byte blocks. Decompose addresses 0, 16, and 64.',r'''
    for (unsigned address : {0u, 16u, 64u}) {
        const unsigned block = address / 16;
        const unsigned set = block % 4;
        const unsigned tag = block / 4;
        std::cout << address << ": set=" << set << " tag=" << tag << '\n';
    }
''','0: set=0 tag=0\n16: set=1 tag=0\n64: set=0 tag=1\n',[
 ('Remove the byte offset','divide address by 16','All 16 bytes in a block select the same block identity.'),
 ('Choose a set','block modulo 4','Addresses 0 and 64 both compete for set 0.'),
 ('Keep the tag','block divided by 4','Different tags distinguish blocks that use the same set.')],
 'Comparing only the set index would falsely call address 64 a hit after loading address 0.',
 'Why is a valid bit also needed even when the stored tag equals zero?',
 'A cold, unused entry can contain an arbitrary or zero tag. The valid bit says whether the entry actually represents cached data.',
 'More ways per set permit several competing tags, but require a choice when the set fills.')
add(30,'Trace hits before computing a hit rate','A cold direct-mapped cache has four lines of 16 bytes. Read addresses 0, 16, 0, 64, 0, 16.',r'''
    std::array<int, 4> tags{-1, -1, -1, -1};
    int hits = 0;
    for (int address : {0, 16, 0, 64, 0, 16}) {
        const int block = address / 16;
        const int slot = block % 4;
        const bool hit = tags[slot] == block;
        hits += hit;
        tags[slot] = block;
        std::cout << (hit ? 'H' : 'M') << ' ';
    }
    std::cout << "hits=" << hits << '\n';
    assert(hits == 2);
''','M M H M M H hits=2\n',[
 ('Read 0, 16, 0','miss, miss, hit','The first two reads bring in blocks 0 and 1; the third reuses block 0.'),
 ('Read 64, 0','miss, miss','Block 4 replaces block 0 in slot 0; returning to 0 must fetch it again.'),
 ('Read 16','hit; total 2 hits out of 6','Slot 1 still contains block 1. Hit rate is 2/6, about 33.3%.')],
 'Calling the second access to block 0 a compulsory miss is wrong. That block has been seen before; this eviction is a conflict relative to a four-line fully associative LRU reference.',
 'Would starting with a warm cache necessarily produce the same hit rate?',
 'No. Initial state changes which first accesses hit. State the initial contents and interval before comparing rates. The rate alone also does not determine elapsed time.',
 'A trace model explains causes; hardware counters measure the target workload under its real cache policies.')
add(31,'LRU and FIFO can evict different blocks','A two-entry cache sees A, B, A, C. Decide which block to evict when C arrives.',r'''
    std::vector<char> lru{'A', 'B'};
    lru.erase(lru.begin());
    lru.push_back('A'); // Touch A: it becomes most recently used.
    const char lru_victim = lru.front();
    const char fifo_victim = 'A'; // A arrived first; a hit does not move it.
    std::cout << "LRU=" << lru_victim << " FIFO=" << fifo_victim << '\n';
    assert(lru_victim == 'B');
''','LRU=B FIFO=A\n',[
 ('Insert A then B','oldest arrival A; least recent A','The two policies agree before any hit.'),
 ('Hit A','LRU order becomes B, A','Recency changes. Arrival order does not.'),
 ('Insert C','LRU evicts B; FIFO evicts A','The victim follows the policy’s state, not a universal oldest label.')],
 'Updating FIFO order on every hit silently turns it into a different policy.',
 'Would a dirty victim have the same work as a clean victim in a write-back cache?',
 'No. A dirty line needs its changes preserved at the next level before reuse. Replacement choice and write policy together determine the cost.',
 'Approximate recency policies can cost less to implement than exact LRU, especially with many ways.')
add(32,'Weight the miss penalty by how often it occurs','A model cache takes 1 ns for the hit lookup. Five percent of accesses incur an additional 40 ns miss penalty.',r'''
    const double hit_time = 1;
    const double miss_rate = 0.05;
    const double miss_penalty = 40;
    const double amat = hit_time + miss_rate * miss_penalty;
    std::cout << amat << " ns\n";
    assert(amat == 3);
''','3 ns\n',[
 ('Pay the lookup','1 ns for each access in this model','The base lookup cost is counted once.'),
 ('Weight extra work','0.05 * 40 = 2 ns on average','Only misses pay the additional penalty.'),
 ('Add the components','AMAT = 3 ns','AMAT means average memory-access time under the stated serial cost model.')],
 'Using 1 + 5 * 40 treats five percent as five. Counting the hit time inside the penalty and again outside also double-counts it.',
 'If miss rate falls to 2.5%, does the average time halve?',
 'No. The result becomes 1 + 0.025 * 40 = 2 ns. The fixed hit cost remains. Overlap in a real processor can further separate AMAT from whole-program time.',
 'Use this model to reason about tradeoffs, then measure end-to-end effects rather than assuming every saved miss has the same impact.')
add(33,'Files are byte sequences, not implicit records','A file model contains AB followed by CDE. Read three bytes beginning at byte offset 2.',r'''
    const std::string file = "ABCDE";
    const std::size_t offset = 2;
    const std::size_t count = 3;
    assert(offset <= file.size() && count <= file.size() - offset);
    std::cout << file.substr(offset, count) << '\n';
''','CDE\n',[
 ('Choose an offset','offset 2 identifies C','Offsets here count bytes starting from zero.'),
 ('Check the remaining extent','5 - 2 = 3 bytes remain','Subtraction after checking offset avoids an overflowing offset + count check.'),
 ('Read the slice','CDE','Any record boundaries come from a format layered over the bytes.')],
 'A file size does not say where a logical record begins or whether its contents are valid for the format.',
 'How would variable-length records change random access?',
 'You would need a way to find boundaries, such as an index or a scan of validated length fields. Multiplying a record number by one fixed size would no longer work.',
 'Line-based text is convenient for simple human-readable records; fixed-width or indexed formats support different access needs.')
add(34,'A successful read may be short','A mock reader returns at most two bytes per call. Copy all five input bytes while keeping a single advancing position.',r'''
    const std::string input = "ABCDE";
    std::string output;
    std::size_t position = 0;
    int calls = 0;
    while (position < input.size()) {
        const auto count = std::min<std::size_t>(2, input.size() - position);
        output.append(input, position, count);
        position += count;
        ++calls;
    }
    std::cout << output << " calls=" << calls << '\n';
    assert(output == input && calls == 3);
''','ABCDE calls=3\n',[
 ('First transfer','AB; position 2','A partial transfer can still be successful.'),
 ('Second transfer','CD; position 4','Advance by the actual count, not by the desired request size.'),
 ('Final transfer','E; position 5','The final chunk is smaller. Real read/write loops must also handle errors, interruption, and zero results according to each API.')],
 'Assuming one read fills the buffer loses data when the call returns fewer bytes. Treating every short result as EOF is also wrong for many streams.',
 'Does writing the five bytes require exactly one write call?',
 'No. A write can also be partial. Keep a separate output position and retry the untransferred suffix under the API’s error rules.',
 'A higher-level stream can hide some buffering mechanics, but its error and flushing contract still needs attention.')
add(35,'Buffering changes when output is forwarded','A model output buffer forwards data after four bytes or an explicit flush. Append AB and CD, then one final E.',r'''
    std::string pending, destination;
    int flushes = 0;
    const auto flush = [&] { destination += pending; pending.clear(); ++flushes; };
    pending += "AB";
    pending += "CD";
    if (pending.size() >= 4) flush();
    pending += "E";
    flush();
    std::cout << destination << " flushes=" << flushes << '\n';
    assert(destination == "ABCDE" && flushes == 2);
''','ABCDE flushes=2\n',[
 ('Append AB','pending AB; destination empty','The caller has supplied data but it has not yet reached the next layer.'),
 ('Append CD','one flush forwards ABCD','Combining small pieces reduces forwarding operations in this model.'),
 ('Append and flush E','destination ABCDE; two flushes','The explicit final flush prevents the last partial buffer from remaining pending.')],
 'Flushing a language-library buffer is not the same guarantee as durable storage on a device. Keep visibility and persistence separate.',
 'Why can flushing after every character reduce throughput?',
 'It removes opportunities to combine small writes and may pay fixed costs repeatedly. Interactive prompts may still justify an immediate flush for responsiveness.',
 'Use buffering for throughput; choose explicit flush points when another participant must see a boundary.')
add(36,'Private and shared mappings have different write effects','Model a file-backed byte sequence. A private view gets its own copy; a shared view refers to the backing bytes.',r'''
    std::string backing = "cat";
    auto private_view = backing;
    auto& shared_view = backing;
    private_view[0] = 'b';
    shared_view[2] = 'r';
    std::cout << backing << ' ' << private_view << '\n';
    assert(backing == "car" && private_view == "bat");
''','car bat\n',[
 ('Create views','both initially read cat','The model makes sharing policy explicit.'),
 ('Write the private view','private_view becomes bat','The backing bytes remain unchanged by this private write.'),
 ('Write the shared view','backing becomes car','The shared reference names the same bytes. Real mapped I/O also needs bounds, lifetime, synchronization, and persistence rules.')],
 'This string model does not implement mmap or prove that a write is durable. A real mapping can also become unsafe if the file is truncated underneath it.',
 'Would unmapping the private view make its changes appear in the file?',
 'No. A private mapping does not promise to write its modifications back as shared file changes. Use the correct mapping or explicit write path for that requirement.',
 'Explicit read/write calls can be clearer for streaming access; mapping can suit random access when lifetime and bounds are controlled.')
add(37,'Copied process state and shared file state coexist','Model the important fork distinction: ordinary private values are copied, while inherited descriptor references can share one open-file offset.',r'''
    int parent_value = 7;
    int child_value = parent_value;
    auto parent_offset = std::make_shared<int>(0);
    auto child_offset = parent_offset;
    child_value = 9;
    *child_offset += 2;
    std::cout << parent_value << ' ' << child_value << " offset=" << *parent_offset << '\n';
    assert(parent_value == 7 && *parent_offset == 2);
''','7 9 offset=2\n',[
 ('Copy private state','parent_value 7; child_value 7','Private memory changes are logically independent after fork.'),
 ('Change the child value','parent 7; child 9','The assignment does not update the parent’s private value.'),
 ('Advance a shared offset','parent observes offset 2','Inherited descriptors may refer to the same kernel open-file description, including its offset.')],
 'Neither everything is copied independently nor everything is shared is an accurate fork model.',
 'Why can buffered text be printed twice if buffered output exists before fork?',
 'The user-space buffer is copied with private memory. Both processes may later flush their copies. The underlying descriptor relationship does not remove the duplicate buffered data.',
 'A spawn API can be simpler than manually managing fork/exec in programs that only need to start another executable.')
add(38,'Exec replaces an image while keeping process identity','A state model starts process 42 running shell. A successful exec changes the program image to worker and keeps the process ID.',r'''
    const int pid = 42;
    std::string image = "shell";
    image = "worker";
    std::cout << "pid=" << pid << " image=" << image << '\n';
    assert(pid == 42 && image == "worker");
''','pid=42 image=worker\n',[
 ('Before exec','pid 42; image shell','Process identity and program image are different concepts.'),
 ('Successful replacement','new image worker','The old user-space image does not continue past a successful exec call.'),
 ('Preserve selected process state','pid remains 42','Descriptor inheritance depends on close-on-exec flags and the API contract.')],
 'The assignment is only a state model: unlike this C++ line, a successful real exec does not return into the old program.',
 'Where should error handling for exec failure be placed?',
 'Immediately after the exec call, since returning indicates failure. That path should report the error and choose a defined termination behavior.',
 'Launching a second process needs a creation operation as well; exec by itself is replacement.')
add(39,'A terminated child still needs collection','Model a parent observing a child’s normal exit code 7, then reaping its termination record.',r'''
    std::optional<int> child_status = 7;
    assert(child_status.has_value());
    const int exit_code = *child_status;
    child_status.reset();
    std::cout << "exit=" << exit_code << " reaped=" << std::boolalpha
              << !child_status.has_value() << '\n';
''','exit=7 reaped=true\n',[
 ('Child terminates','status record contains normal exit 7','Termination and collection by the parent are separate events.'),
 ('Parent inspects status','exit code is 7','Real wait status must first be classified with the platform’s macros; not every termination is a normal exit.'),
 ('Parent reaps','record is collected','The model removes the record only after saving the result needed by the parent.')],
 'Shifting raw wait-status bits without first checking the termination category can misreport a signal termination as an ordinary exit.',
 'Does a zombie child still execute user instructions?',
 'No. It has terminated; the remaining record lets the parent collect status. Failing to reap can still exhaust bookkeeping resources.',
 'A process wrapper can centralize waiting and status decoding when many call sites launch children.')
add(40,'EOF depends on every writer closing','A pipe model begins with two open write endpoints. Closing only one must not cause EOF after its buffered bytes are drained.',r'''
    int open_writers = 2;
    bool buffer_empty = true;
    const auto eof = [&] { return buffer_empty && open_writers == 0; };
    --open_writers;
    std::cout << std::boolalpha << eof() << ' ';
    --open_writers;
    std::cout << eof() << '\n';
    assert(eof());
''','false true\n',[
 ('One writer closes','one remains; empty buffer','An empty pipe with a writer still open can later receive more data.'),
 ('Reader tests EOF','false','In a blocking real pipe, an empty read can wait for data or closure instead.'),
 ('Last writer closes','zero writers; empty buffer','Now a reader can observe end of stream.')],
 'Leaving an unused inherited write descriptor open can make a pipeline reader wait forever for EOF.',
 'Why should a shell start both pipeline stages before waiting for the producer?',
 'A pipe has bounded capacity. A producer may block on a full pipe while its consumer has not started. Start the cooperating stages and close unused ends before waiting.',
 'A temporary file avoids live backpressure but changes latency, storage needs, and streaming behavior.')
add(41,'A signal notification is not a work queue','A model flag records that attention is needed. Two notifications arriving before the main loop checks the flag can collapse into one pending state.',r'''
    bool pending = false;
    pending = true;
    pending = true;
    int handled = 0;
    if (pending) { pending = false; ++handled; }
    std::cout << "handled=" << handled << '\n';
    assert(handled == 1);
''','handled=1\n',[
 ('First notification','pending true','The flag records a condition, not an event count.'),
 ('Second notification','pending still true','No extra information fits in one Boolean.'),
 ('Main loop reacts','one handling pass','That pass should inspect the underlying state if several events may have occurred.')],
 'This sequential model is not a valid signal handler implementation. Real handlers need only async-signal-safe operations and a correct signal-mask/wait protocol.',
 'Why is check flag, then sleep vulnerable without an atomic waiting protocol?',
 'A signal can arrive between the check and sleep. The process may then sleep after its wakeup event has already occurred. A signal-mask-based wait or suitable OS event mechanism closes the gap.',
 'Use a counted queue or appropriate queued notification mechanism when every event must be preserved.')
add(42,'Parse before you execute','A tiny shell language accepts words separated by spaces and the pipe symbol |. Tokenize a pipeline before deciding how to launch it.',r'''
    const std::string line = "emit | count";
    std::istringstream input(line);
    std::vector<std::string> tokens;
    for (std::string word; input >> word;) tokens.push_back(word);
    assert(tokens.size() == 3 && tokens[1] == "|");
    std::cout << "left=" << tokens[0] << " right=" << tokens[2] << '\n';
''','left=emit right=count\n',[
 ('Read input text','emit | count','The input is not yet a set of running processes.'),
 ('Tokenize under the stated grammar','emit, |, count','This intentionally tiny grammar requires spaces and has no quoting or expansion.'),
 ('Build the plan','left stage emit; right stage count','Validation must happen before allocating pipes and starting processes.')],
 'Splitting every real shell line on spaces breaks quoted arguments and operators without surrounding spaces. This example deliberately does not implement a full shell grammar.',
 'What should happen for the incomplete input emit | ?',
 'Reject it as a syntax error before launching emit. Starting part of an invalid pipeline creates side effects and cleanup work without a complete execution plan.',
 'For a fixed external command, a direct argument-vector API is safer and simpler than invoking a shell parser.')
add(43,'An interleaving can lose an update','Use a sequential model of two participants that both read a counter before either writes back its increment. The requirement is to count two events.',r'''
    int counter = 0;
    const int a_read = counter;
    const int b_read = counter;
    counter = a_read + 1;
    counter = b_read + 1;
    std::cout << counter << '\n';
    assert(counter == 1);
''','1\n',[
 ('A reads','A keeps 0','A has not written its new result yet.'),
 ('B reads','B also keeps 0','Both computations are now based on the same old state.'),
 ('A and B store','A stores 1; B stores 1','The final value counts only one event, even though two computations ran.')],
 'This is a defined sequential model of interleaving. Running unsynchronized writes to an ordinary shared C++ int would have a data race and undefined behavior, not a guaranteed final value of 1.',
 'Would putting the two participants on a single core automatically fix the design?',
 'No. Interleaving on one core can still split a read-modify-write sequence. The operation needs an appropriate atomic update or synchronization boundary.',
 'Keep state local to one participant when sharing is unnecessary; otherwise make the update contract explicit.')
add(44,'Join workers before consuming their results','Two threads sum disjoint halves of four readings. Each writes a separate result slot; main combines the results only after both joins.',r'''
    const std::array<int, 4> input{1, 2, 3, 4};
    std::array<int, 2> partial{0, 0};
    std::thread a([&] { partial[0] = input[0] + input[1]; });
    std::thread b([&] { partial[1] = input[2] + input[3]; });
    a.join();
    b.join();
    std::cout << partial[0] + partial[1] << '\n';
    assert(partial[0] + partial[1] == 10);
''','10\n',[
 ('Start the workers','a writes slot 0; b writes slot 1','The input is read-only and result slots are distinct int objects.'),
 ('Join both threads','both result writes are complete','A successful join synchronizes with completion, so main may read the results afterward.'),
 ('Combine','3 + 7 = 10','The array and its elements stay alive until after both workers finish.')],
 'Reading partial before the joins can race with a worker’s write. Letting a joinable std::thread be destroyed calls std::terminate.',
 'Is using two threads faster for these four integers?',
 'Almost certainly not; startup and coordination dominate such tiny work. The small input makes the lifetime and synchronization rules easy to see. Measure larger tasks before adding threads.',
 'A sequential std::accumulate is the practical choice for small ranges.','Complete C++ program')
add(45,'Atomic loads and stores do not form one increment','Use atomic accesses but deliberately read twice before storing. Then compare two fetch_add operations.',r'''
    std::atomic<int> counter{0};
    const int a = counter.load();
    const int b = counter.load();
    counter.store(a + 1);
    counter.store(b + 1);
    const int split_result = counter.load();
    counter.store(0);
    counter.fetch_add(1);
    counter.fetch_add(1);
    std::cout << split_result << ' ' << counter.load() << '\n';
    assert(split_result == 1 && counter.load() == 2);
''','1 2\n',[
 ('Separate loads','both read 0','Each operation is atomic, but the pair is not one indivisible increment.'),
 ('Separate stores','both store 1','The logical lost update occurs without any C++ data race in this trace.'),
 ('Use fetch_add','0 becomes 1, then 2','Each read-modify-write is one atomic operation with respect to the counter.')],
 'Stronger memory ordering on separate load and store calls does not combine them into a transaction.',
 'Why is this a better teaching trace than running two unsynchronized ordinary increments?',
 'It has defined behavior and deterministic output. It isolates the logical mistake without relying on undefined behavior to happen to look like a lost update.',
 'Use a mutex when one logical update must protect several related values; one atomic counter is enough only for the narrower contract.')
add(46,'Publish data with a release/acquire handoff','A producer writes a value, then publishes a ready flag. A consumer must not read the value until it observes readiness.',r'''
    int payload = 0;
    std::atomic<bool> ready{false};
    int observed = 0;
    std::thread producer([&] {
        payload = 42;
        ready.store(true, std::memory_order_release);
    });
    std::thread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) std::this_thread::yield();
        observed = payload;
    });
    producer.join();
    consumer.join();
    std::cout << observed << '\n';
    assert(observed == 42);
''','42\n',[
 ('Write payload','payload becomes 42','The write is sequenced before the release store.'),
 ('Publish and observe ready','the acquire load reads true from the release store','This handoff establishes the ordering needed to see the preceding payload write.'),
 ('Consume and join','observed becomes 42','The data remains alive; the one-shot producer performs no later writes to it.')],
 'Changing both operations to relaxed removes the required handoff for this non-atomic payload. This is a one-shot protocol, not a reusable queue.',
 'Why is an ordinary bool flag insufficient?',
 'Concurrent unsynchronized reads and writes of that flag would themselves be a data race. Even a flag that appears to change on your machine is not a C++ synchronization contract.',
 'A mutex and condition variable often make waiting and repeated exchanges easier to reason about than a custom atomic protocol.','Complete C++ program')
add(47,'Protect a whole invariant','Two balances total 100. Transfer 10 from one to the other while holding the same lock over both updates.',r'''
    int first = 60, second = 40;
    std::mutex guard;
    {
        std::lock_guard<std::mutex> lock(guard);
        assert(first >= 10);
        first -= 10;
        second += 10;
        assert(first + second == 100);
    }
    std::cout << first << ' ' << second << '\n';
''','50 50\n',[
 ('Lock before observing and changing','first 60; second 40','All concurrent accesses to these balances would need to follow the same locking rule.'),
 ('Apply both changes','first 50; second 50','The temporary half-complete update is hidden from other cooperating locked operations.'),
 ('Leave the block','lock_guard releases the mutex','Scope-based cleanup keeps release paired with acquisition.')],
 'Using a separate unrelated mutex at each call site provides no shared exclusion. Protecting only one balance also fails to protect the total.',
 'Would making both balances atomic automatically make the transfer atomic?',
 'No. Another participant could observe the state between the two atomic updates. A multi-object invariant needs a protocol covering the full logical transaction.',
 'For one independent count, an atomic operation can be simpler; for coupled state, a single clear mutex is often easier to audit.','Complete C++ program')
add(48,'Wait for a predicate, not a remembered notification','Model a consumer that arrives after an item was queued and a notification already happened. The queue state must still let it proceed.',r'''
    std::vector<int> queue;
    queue.push_back(7); // The producer acts before the consumer arrives.
    const bool should_wait = queue.empty();
    std::cout << "wait=" << std::boolalpha << should_wait
              << " value=" << queue.front() << '\n';
    assert(!should_wait);
''','wait=false value=7\n',[
 ('Producer changes state','queue contains 7','The queue is the lasting evidence that work exists.'),
 ('Consumer checks the predicate','queue.empty() is false','It should not wait merely because it missed an earlier notification.'),
 ('Generalize to real threads','check and wait under the associated mutex','Condition-variable wait atomically releases the lock while waiting and reacquires it; a loop rechecks after any wakeup.')],
 'A condition variable is not a stored event counter. Waiting without a predicate can sleep despite available work or act after a spurious wakeup.',
 'Why must the predicate be checked again after waking?',
 'Another consumer may have taken the item, or the wakeup may be spurious. The predicate under the lock decides whether proceeding is valid.',
 'A semaphore can naturally represent available permits, while a condition variable supports a general shared-state predicate.')
add(49,'A bounded queue needs space, work, and shutdown rules','Model a queue with capacity two. Reject a third item, consume one, then permit another insertion.',r'''
    std::vector<int> queue;
    const std::size_t capacity = 2;
    const auto push = [&](int value) {
        if (queue.size() == capacity) return false;
        queue.push_back(value);
        return true;
    };
    const bool first = push(1), second = push(2), full = push(3);
    assert(first && second && !full);
    queue.erase(queue.begin());
    const bool after_pop = push(3);
    std::cout << std::boolalpha << full << ' ' << after_pop
              << " size=" << queue.size() << '\n';
''','false true size=2\n',[
 ('Insert 1 and 2','size 2; full','The invariant is 0 <= size <= capacity.'),
 ('Attempt 3','reject; size stays 2','A real concurrent API must define whether full means wait, reject, or drop.'),
 ('Consume and insert','remove 1, insert 3; size 2','Making space changes the condition that blocked producers care about.')],
 'One wakeup rule is not enough: consumers need work; producers need space. Shutdown must also wake participants whose waits can no longer succeed.',
 'If shutdown occurs with two queued items, should consumers drop them?',
 'Either draining or discarding can be valid, but the contract must choose. A drainable queue rejects new pushes and lets consumers finish existing items before reporting closed-and-empty.',
 'An unbounded queue simplifies insertion but can turn overload into unbounded memory use.')
add(50,'Use a shared lock order','Two transfers need accounts 3 and 8. Sort the lock identities so opposite transfer directions request locks in the same order.',r'''
    const auto order = [](int from, int to) {
        if (from > to) std::swap(from, to);
        return std::pair<int, int>{from, to};
    };
    const auto forward = order(3, 8);
    const auto reverse = order(8, 3);
    std::cout << forward.first << ',' << forward.second << ' '
              << reverse.first << ',' << reverse.second << '\n';
    assert(forward == reverse);
''','3,8 3,8\n',[
 ('Forward transfer','request 3 then 8','A globally consistent order prevents one participant from taking the opposite order.'),
 ('Reverse transfer','still request 3 then 8','Business direction is separated from lock acquisition order.'),
 ('Rule out this cycle','no participant holds 8 while waiting for 3 under this rule','The rule removes circular waiting among locks covered by the same total order.')],
 'Ordering only some call sites is insufficient. Other locks, callbacks, or blocking operations can introduce different cycles.',
 'What if the transfer is from an account to itself?',
 'Handle that case explicitly. Attempting to lock the same non-recursive mutex twice can deadlock or violate a locking API’s requirements.',
 'std::scoped_lock can acquire multiple distinct mutexes with deadlock avoidance; a single mutex is simpler if contention is acceptable.')
add(51,'TCP does not preserve application messages','A byte-stream model delivers a length-prefixed message as two fragments. Assemble a complete frame before interpreting it.',r'''
    std::vector<unsigned char> received{3, 'c'};
    received.insert(received.end(), {'a', 't'});
    const std::size_t length = received[0];
    assert(length <= 8 && received.size() >= 1 + length);
    const std::string message(received.begin() + 1, received.begin() + 1 + length);
    std::cout << message << '\n';
''','cat\n',[
 ('Receive first fragment','length 3 and byte c','The frame is incomplete even though a read delivered data.'),
 ('Receive more','a and t complete the payload','Buffering must retain earlier bytes until the frame is complete.'),
 ('Validate and extract','message cat','The model caps lengths at 8. A real parser also retains any following frame bytes and handles EOF or errors mid-frame.')],
 'Assuming one send matches one recv call confuses stream transport with application framing.',
 'What should happen if the peer claims a million-byte frame but the service limit is 8?',
 'Reject the length before allocating or waiting for that payload. Framing includes resource limits as well as byte boundaries.',
 'A delimiter can be simpler for text, provided escaping and maximum line length are defined.')
add(52,'Limit admission before work exhausts memory','A server has 64 MiB reserved for connection state and budgets 2 MiB per admitted connection. Compute the maximum and test the next admission.',r'''
    const unsigned budget_mib = 64;
    const unsigned per_connection_mib = 2;
    const unsigned limit = budget_mib / per_connection_mib;
    const unsigned active = 32;
    std::cout << "limit=" << limit << " admit=" << std::boolalpha
              << (active < limit) << '\n';
    assert(limit == 32 && !(active < limit));
''','limit=32 admit=false\n',[
 ('Reserve a connection budget','64 MiB','This budget is separate from executable, queues, caches, and other memory.'),
 ('Bound per-connection cost','2 MiB','The estimate must include all state allocated for an admitted connection, not merely a socket handle.'),
 ('Apply admission','32 active means no room for another','Overload needs a defined reject, defer, or bounded-queue policy.')],
 'Creating an unlimited thread for every connection can exhaust memory long before CPU utilization reaches 100%.',
 'Would switching to an event loop remove the need for limits?',
 'No. It may reduce thread stacks, but connections, input buffers, queued output, timeouts, and application work still consume bounded resources.',
 'A worker pool caps active work; a bounded queue and admission policy are still needed to control waiting work.')
add(53,'Readiness permits an attempt, not unlimited blocking','Model draining currently available bytes from a nonblocking input. Stop on would-block and keep the partial application message.',r'''
    const std::array<int, 3> results{2, 1, -1}; // -1 models would-block, not EOF.
    int buffered = 0;
    for (int result : results) {
        if (result == -1) break;
        buffered += result;
    }
    std::cout << "buffered=" << buffered << " keep-open\n";
    assert(buffered == 3);
''','buffered=3 keep-open\n',[
 ('First successful read','2 bytes buffered','Readiness allowed an attempt; the result reports actual progress.'),
 ('Second successful read','3 bytes buffered','An edge-triggered design normally drains until the API reports no more available data.'),
 ('Would-block','keep connection open and return to the event loop','Would-block is not EOF and not proof that the application frame is complete.')],
 'In real read calls, -1 requires inspecting errno; this model uses -1 only as a named would-block event. Treating every -1 as would-block loses real errors.',
 'Why can a blocking socket stall an event loop even after a readiness notification?',
 'Availability can change, and a later read can wait for bytes that have not arrived. Nonblocking operations keep one connection from holding up all the others.',
 'Blocking I/O can be appropriate in a bounded worker design where waiting does not freeze unrelated connections.')
add(54,'Report latency and throughput with units','Three serial requests take 2, 8, and 5 milliseconds. Compute mean latency and throughput over the 15-millisecond interval.',r'''
    const std::array<double, 3> milliseconds{2, 8, 5};
    const double elapsed = std::accumulate(milliseconds.begin(), milliseconds.end(), 0.0);
    const double mean = elapsed / milliseconds.size();
    const double throughput = milliseconds.size() / (elapsed / 1000.0);
    std::cout << "mean=" << mean << "ms throughput=" << throughput << "/s\n";
''','mean=5ms throughput=200/s\n',[
 ('Define the interval','15 milliseconds, three completed requests','Throughput needs a count and an elapsed-time interval.'),
 ('Compute the mean','15 / 3 = 5 milliseconds per request','The mean hides the slow 8-millisecond request.'),
 ('Convert units','3 / 0.015 = 200 requests per second','The formula assumes the stated serial interval; concurrent completions require their actual observed interval.')],
 'Dividing by 15 without converting milliseconds to seconds gives a value per millisecond, not per second.',
 'Would a mean of 5 ms prove that every request meets a 6 ms deadline?',
 'No. The 8 ms request misses it. Report a distribution or relevant percentile and the workload, not only an average.',
 'A checksum or observable result keeps benchmark work relevant, but timing still needs warmup, repetitions, and controlled comparisons.')
add(55,'Remove repeated work before tuning instructions','For the values 2, 4, 6, 8, answer the sum over indexes [1, 4). Precompute prefix sums so repeated range queries need two reads and a subtraction.',r'''
    const std::vector<int> values{2, 4, 6, 8};
    std::vector<int> prefix(values.size() + 1, 0);
    for (std::size_t i = 0; i < values.size(); ++i) prefix[i + 1] = prefix[i] + values[i];
    const int answer = prefix[4] - prefix[1];
    std::cout << answer << '\n';
    assert(answer == 18);
''','18\n',[
 ('Build prefix','0, 2, 6, 12, 20','prefix[k] means the sum of values before index k.'),
 ('Choose [1, 4)','exclude the first value, include indexes 1, 2, 3','The half-open endpoints match the prefix definition.'),
 ('Subtract','20 - 2 = 18','All terms before the left endpoint cancel.')],
 'Using prefix[right] - prefix[left + 1] accidentally drops the first requested element. Sums also need a type wide enough for the input bounds.',
 'When is the preprocessing worse than a direct scan?',
 'For one tiny query, building and storing a prefix array adds work. It helps when many range queries reuse the same mostly unchanged data; frequent updates may need another design.',
 'Optimize the repeated algorithmic work first, then measure whether lower-level changes still matter.','Complete C++ program')
add(56,'Compute the ceiling before optimizing','Forty percent of a program’s time is in a part you can make twice as fast. Estimate whole-program speedup.',r'''
    const double fraction = 0.4;
    const double local_speedup = 2.0;
    const double new_time = (1 - fraction) + fraction / local_speedup;
    std::cout << "speedup=" << 1 / new_time << '\n';
    assert(new_time > 0.79 && new_time < 0.81);
''','speedup=1.25\n',[
 ('Normalize the old time','1.0 total: 0.6 unchanged, 0.4 targeted','The fraction must come from the same workload and timing boundary.'),
 ('Speed up the target','0.4 / 2 = 0.2','Only the selected part improves.'),
 ('Combine','new time 0.8; speedup 1 / 0.8 = 1.25','A local factor of two becomes a 25% whole-program throughput factor under this model.')],
 'Multiplying the entire program’s speed by the local speedup ignores the unchanged work.',
 'What is the limit even if the selected part becomes free?',
 'The remaining time is 0.6, so the ceiling is 1/0.6, about 1.67 times. This helps decide whether the optimization effort can meet the goal.',
 'If the target is too small, change the larger cost or the algorithm rather than polishing an irrelevant hot spot.')
add(57,'Validate lengths before using them','A packet model has an eight-byte header in a ten-byte buffer. It claims five payload bytes; reject the frame without an overflowing addition.',r'''
    const std::size_t buffer_size = 10;
    const std::size_t header_size = 8;
    const std::size_t claimed_payload = 5;
    const bool valid = header_size <= buffer_size
        && claimed_payload <= buffer_size - header_size;
    std::cout << (valid ? "accept" : "reject") << '\n';
    assert(!valid);
''','reject\n',[
 ('Check the fixed header','8 <= 10','Subtraction is only safe for the remaining size after this check.'),
 ('Compute remaining extent','10 - 8 = 2','Only two payload bytes are available.'),
 ('Validate the claim','5 <= 2 is false','Reject before copying, allocating from the claim, or interpreting nonexistent bytes.')],
 'Checking header_size + claimed_payload <= buffer_size can wrap if unchecked size_t inputs are large. Passing a wrapped check can lead to an undersized buffer.',
 'Should a parser accept a frame merely because the backing buffer happens to contain extra allocated memory?',
 'No. The valid input extent is a contract independent of allocation capacity. Reading beyond the received extent can expose stale data even if it stays inside an allocation.',
 'A bounded view carrying pointer and length reduces mismatched arguments; it does not remove the need for correct validation.')
add(58,'A mitigation detects only the corruption it covers','Model a saved guard beside a payload. Detect a changed guard while recognizing that an untouched guard does not prove the payload is valid.',r'''
    const unsigned expected_guard = 0xA55Au;
    unsigned saved_guard = expected_guard;
    saved_guard ^= 1u; // Deliberately model corruption; no invalid memory access.
    std::cout << (saved_guard == expected_guard ? "guard intact" : "detected") << '\n';
    assert(saved_guard != expected_guard);
''','detected\n',[
 ('Store a guard','expected value 0xA55A','A guard gives a later check one specific fact to test.'),
 ('Corrupt the modeled guard','one bit changes','The example changes an ordinary variable; it does not perform a real buffer overflow.'),
 ('Compare at the boundary','detected','Real stack canaries, address randomization, and non-executable memory have different coverage and assumptions.')],
 'A canary is not a bounds checker. An overwrite that misses it or another class of bug may remain undetected.',
 'Why should hardened builds still validate input lengths?',
 'Mitigations reduce some consequences or detect some faults; they do not make an invalid access correct. Preventing the out-of-bounds operation remains the primary rule.',
 'Use layered platform mitigations alongside safe interfaces and verification, and inspect the actual build settings rather than assuming flags took effect.')
add(59,'Prepare, validate, then commit','A settings update must preserve the old value if validation fails. Build the candidate separately, and replace the live state only after checking it.',r'''
    std::string live = "version=1";
    std::string candidate = "broken";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    candidate = "version=2";
    if (candidate.rfind("version=", 0) == 0) live.swap(candidate);
    std::cout << live << '\n';
    assert(live == "version=2");
''','version=1\nversion=2\n',[
 ('Prepare invalid input','live remains version=1','Uncommitted work is kept separate from visible state.'),
 ('Reject invalid candidate','still version=1','A failed attempt preserves the previous usable value.'),
 ('Validate and commit the next candidate','live becomes version=2','The model has one explicit commit point; a real file protocol must separately address atomic replacement and durability.')],
 'Writing half a new configuration directly over the live file can destroy the only good version before validation finishes.',
 'Does an atomic rename alone guarantee data survives a power failure?',
 'No. Atomic visibility and crash durability are different contracts. A real protocol needs the relevant file and directory synchronization rules for its filesystem and OS.',
 'A memory-only swap is sufficient for process-local state; durable external state requires a documented failure model.')
add(60,'Follow one value through build and execution','A complete program adds two parsed constant strings and formats the result. Distinguish source text, compiled operations, live objects, and output bytes.',r'''
    const std::string first = "12";
    const std::string second = "30";
    const int result = std::stoi(first) + std::stoi(second);
    const std::string line = std::to_string(result) + "\n";
    std::cout << line;
    assert(result == 42 && line == "42\n");
''','42\n',[
 ('Build the program','declarations checked; definitions linked','Compilation and linking produce an executable; they do not make these runtime string objects persistent objects in the running process.'),
 ('Execute parsing and addition','12 + 30 = 42','These particular strings are valid; external input would need error and range handling.'),
 ('Format and output','bytes 4, 2, newline','C++ stream output passes through library and OS layers; exact machine instructions and buffering depend on the build and platform.')],
 'Seeing the correct output alone does not tell you which functions were inlined, which pages faulted, or how many system calls occurred.',
 'Which evidence would answer a question about generated instructions, and which about I/O calls?',
 'Inspect the built executable’s disassembly for instructions. Use an appropriate runtime trace for system calls, with a stated workload. Source reading, binary inspection, and execution traces answer different questions.',
 'Use the least intrusive measurement that answers the actual question; heavy tracing can change timing.','Complete C++ program')
add(61,'Integrate ownership, work partitioning, and verification','Sum six readings with two workers. Keep the input alive, give each worker a disjoint output slot, join, and compare with a sequential reference.',r'''
    const std::vector<int> readings{2, 4, 6, 8, 10, 12};
    std::array<int, 2> partial{0, 0};
    const auto work = [&](std::size_t worker) {
        const std::size_t begin = worker * 3;
        partial[worker] = std::accumulate(readings.begin() + begin,
            readings.begin() + begin + 3, 0);
    };
    std::thread first(work, 0);
    std::thread second(work, 1);
    first.join(); second.join();
    const int total = partial[0] + partial[1];
    const int reference = std::accumulate(readings.begin(), readings.end(), 0);
    assert(total == reference);
    std::cout << "parallel=" << total << " reference=" << reference << '\n';
''','parallel=42 reference=42\n',[
 ('Partition the input','worker 0 gets [0,3); worker 1 gets [3,6)','Every element belongs to exactly one range; neither range passes the end.'),
 ('Keep ownership outside the workers','main owns readings and partial until after joins','Captured references borrow live objects. Each worker writes a distinct result element.'),
 ('Join and check','12 + 30 = 42; reference 42','The reference checks this fixture’s result. More tests must cover empty, uneven, large, and boundary-valued inputs before generalizing the implementation.')],
 'This fixed six-element teaching program does not handle arbitrary sizes, worker-start failure recovery, or cancellation. Those are explicit extension tasks, not implied guarantees.',
 'Which change is needed first to support seven inputs with two workers?',
 'Compute ranges from size and worker count instead of hardcoding groups of three, then test that ranges cover every index once. Preserve lifetime and disjoint result ownership while changing the partition rule.',
 'Start with a sequential reference and a clear input contract. Parallelize only after measurements show enough work to justify the coordination.','Complete C++ program')

def generate():
 assert set(ROWS)==set(range(1,62))
 for n,row in ROWS.items():
  path=ROOT/row['filename'];path.parent.mkdir(parents=True,exist_ok=True)
  path.write_text(row['code']);path.with_name('expected.txt').write_text(row['output'])
 (ROOT/'teaching/systems-workshops.json').write_text(json.dumps(ROWS,ensure_ascii=False,indent=2)+'\n')
 print(f'Wrote {len(ROWS)} chapter workshops and complete C++17 demonstrations.')
if __name__=='__main__':generate()
