LAB_DIR=$(mktemp -d)
printf '%s\n' "$LAB_DIR"
./build/lab/harbor_worker "$LAB_DIR/worker.db" 0
