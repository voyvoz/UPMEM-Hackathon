NR_TASKLETS ?= 16
CFLAGS_DPU ?= -O3
CFLAGS_HOST ?= --std=c99 -O3

all: greyscale greyscale_host

greyscale: greyscale.c
	dpu-upmem-dpurte-clang -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_DPU} -o greyscale greyscale.c

greyscale_host: greyscale_host.c main.c
	gcc -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_HOST} -D_GNU_SOURCE greyscale_host.c main.c -o greyscale_host `dpu-pkg-config --cflags --libs dpu` -lpthread -lm

docker_build:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS}"

docker_run:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src upmem-docker bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS} && ./greyscale_host"

clean:
	rm -rf greyscale greyscale_host img_grey_dpu.jpg img_grey_cpu.jpg

.PHONY: all docker_build docker_run
