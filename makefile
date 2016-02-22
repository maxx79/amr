os_name=$(shell uname -s | tr [:upper:] [:lower:])
ifndef os_family
    os_family=unknown
endif
arch=$(shell getconf LONG_BIT)
ifndef arch
    arch=32
endif
arm=$(shell arch | grep arm)
ifdef arm
    arm=arm
    arm_postfix=-arm
endif
nix_family=$(shell uname -s | grep -i -E -c "linux|unix|nix|solaris|aix")
project_home=$(shell pwd)
java_home=$(shell echo $$JAVA_HOME)
include_amrnb=$(project_home)/include/opencore-amrnb
include_amrwb=$(project_home)/include/opencore-amrwb
ifdef java_home
    include_java=$(java_home)/include
else
    $(error Please set <java home>)
endif
java_include=$(java_home)/include
src_dir=$(project_home)/src/main/C
lib_dir=$(project_home)/lib/C
res_dir=$(project_home)/res
sources=*.c

all: init build clean

init:
	echo $(_ARCH)
build: compile.o link.o

compile.o:
	gcc -m$(arch) -c $(temp_dir) $(src_dir)/$(sources) $(src_dir)/list/$(sources) -I$(include_amrnb) -I$(include_amrwb) -I$(java_include) -I$(java_include)/linux
link.o:
	gcc -m$(arch) -shared -lc -o $(res_dir)/libs/linux/libamr$(arch)$(arm_postfix).so *.o $(lib_dir)/linux/$(arch)/$(arm)/*.a
clean: 
	rm -rf *.o
