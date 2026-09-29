set -eu
: "${IMAGE:?Set the built image whose entrypoint is app}"
test "$(docker run --rm "$IMAGE" check)" = healthy
if docker run --rm "$IMAGE" wrong >out.txt 2>err.txt; then exit 1; else status=$?; fi
test "$status" = 2
