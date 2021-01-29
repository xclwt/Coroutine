.PHONY : link clean

#DEFS = -DUSE_SYS_UCONTEXT
#DEFS = -DUSE_UC_LINK

C_SRC = $(shell find . -name "*.cpp")
C_SRC := $(filter-out ./co_performance.cpp, $(C_SRC))
C_OBJ = $(patsubst %.cpp, %.o, $(C_SRC))


S_SRC = $(shell find . -name "*.S")
S_OBJ = $(patsubst %.S, %.o, $(S_SRC))

CPP = g++
LD = ld
C_FLAGS = -c -g 
C_FLAGS += $(DEFS)
LD_FLAGS = -plugin /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/lto-wrapper -plugin-opt=-fresolution=/tmp/ccghzRbZ.res -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lc -plugin-opt=-pass-through=-lgcc_s -plugin-opt=-pass-through=-lgcc --build-id --eh-frame-hdr --hash-style=gnu -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie -o main /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/../../../../lib/Scrt1.o /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/../../../../lib/crti.o /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/crtbeginS.o -L/usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0 -L/usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/../../../../lib -L/lib/../lib -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/../../.. -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/crtendS.o /usr/lib/gcc/x86_64-pc-linux-gnu/10.2.0/../../../../lib/crtn.o -g

all1 :  $(C_OBJ) $(S_OBJ) link clean

%.o:%.cpp
	$(CPP) $(C_FLAGS) $< -o $@
	
%.o:%.S
	$(CPP) $(C_FLAGS) $< -o $@
	
link : 
	ld $(C_OBJ) $(S_OBJ) $(LD_FLAGS) -o Coroutine
	
clean :
	-rm $(C_OBJ) $(S_OBJ)
