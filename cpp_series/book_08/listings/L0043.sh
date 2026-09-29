set -eu
: "${REGISTRY:?Set an authorized registry hostname}"
: "${REPOSITORY:?Set the repository path}"
: "${REVISION:?Set the source revision tag}"
: "${LOCAL_IMAGE:?Set an existing locally tested image}"
remote="$REGISTRY/$REPOSITORY:$REVISION"
docker tag "$LOCAL_IMAGE" "$remote"
docker push "$remote"
docker image inspect "$remote" --format '{{json .RepoDigests}}'
