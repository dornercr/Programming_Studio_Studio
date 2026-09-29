g++ -std=c++20 -O0 -g crash.cpp -o crash
gdb ./crash
(gdb) break process_record
(gdb) run
(gdb) info locals
(gdb) print record_id
(gdb) backtrace
