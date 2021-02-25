# myos

Docker
------
* build: `docker build buildenv -t myos`

* run: `docker run --rm -it -v $(pwd):/root/env myos`

* make: `make build-x86_64`


QEMU
------
* run:`qemu-system-x86_64 -cdrom dist/x86_64/kernel.iso`
