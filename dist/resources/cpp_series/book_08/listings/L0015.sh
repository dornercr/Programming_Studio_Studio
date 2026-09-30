sh check.sh
docker build -t harbor-entrypoint .
docker run --rm harbor-entrypoint
