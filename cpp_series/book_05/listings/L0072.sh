g++ -O2 -g workload.cpp -o workload
perf record -g -- ./workload
perf report --stdio --sort comm,dso,symbol | head -40
