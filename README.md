# myos

Docker
------
build:
docker build buildenv -t myos

run:
docker run --rm -it -v $(pwd):/root/env myos

make:
make build-x86_64

