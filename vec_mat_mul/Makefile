NR_TASKLETS ?= 16
CFLAGS_DPU ?= -O3
CFLAGS_HOST ?= --std=c99 -O3

all: vec_mat_mul vec_mat_mul_host

vec_mat_mul: vec_mat_mul.c common.h
	dpu-upmem-dpurte-clang -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_DPU} -o vec_mat_mul vec_mat_mul.c

vec_mat_mul_host: vec_mat_mul_host.c main.c common.h
	gcc -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_HOST} -D_GNU_SOURCE vec_mat_mul_host.c main.c -o vec_mat_mul_host `dpu-pkg-config --cflags --libs dpu` -lpthread

docker_build:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS}"

docker_run:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS} && ./vec_mat_mul_host"

clean:
	rm -rf vec_mat_mul vec_mat_mul_host

.PHONY: all docker_build docker_run
