LAB_DIR=$(mktemp -d)
printf 'Private state directory: %s\n' "$LAB_DIR"
./build/operations/harbor_worker "$LAB_DIR/worker.db" 0
