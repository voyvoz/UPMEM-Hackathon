NR_TASKLETS ?= 16
CFLAGS_DPU ?= -O3
CFLAGS_HOST ?= -O3
DOCKER_NAME ?= upmem-docker

all: db_scan db_scan_host

db_scan: db_scan.c
	dpu-upmem-dpurte-clang -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_DPU} -o db_scan db_scan.c

db_scan_host: db_scan_host.c main.cpp
	gcc -DNR_TASKLETS=${NR_TASKLETS} ${CFLAGS_HOST} -D_GNU_SOURCE db_scan_host.c main.cpp -o db_scan_host -lstdc++ `dpu-pkg-config --cflags --libs dpu` -lpthread

docker_build:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src ${DOCKER_NAME} bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS}"

docker_run:
	docker run -it --rm --name=upmem --mount type=bind,source=${PWD},target=/src ${DOCKER_NAME} bash -c "cd src && make NR_TASKLETS=${NR_TASKLETS} && ./db_scan_host db_1.csv"

clean:
	rm -rf db_scan db_scan_host

.PHONY: all docker_build docker_run
