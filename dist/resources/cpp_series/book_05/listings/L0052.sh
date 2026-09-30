ulimit -c unlimited
./crash || true
coredumpctl list ./crash 2>/dev/null || ls -1 core* 2>/dev/null
gdb ./crash core
(gdb) bt
(gdb) frame 0
(gdb) info locals
