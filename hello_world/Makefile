NR_TASKLETS ?= 16
CFLAGS_DPU ?= -O3
CFLAGS_HOST ?= --std=c99 -O3

all: hello_world hello_world_host

hello_world: hello_world.c
	dpu-upmem-dpurte-clang -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_DPU} -o hello_world hello_world.c

hello_world_host: hello_world_host.c
	gcc -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_HOST} -D_GNU_SOURCE hello_world_host.c main.c -o hello_world_host `dpu-pkg-config --cflags --libs dpu` -lpthread

docker_build:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS}"

docker_run:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS} && ./hello_world_host"

clean:
	rm -rf hello_world hello_world_host

.PHONY: all docker_build docker_run
