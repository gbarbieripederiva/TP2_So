
include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
DEBUGGING=../0000-sampleCodeModule.elf
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES=$(wildcard [^_]*.c)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS=$(SOURCES:.c=.o)

%.o: %.c
	$(GCC) -g $(GCCFLAGS) -I./include -c _loader.c $(SOURCES) 

%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@


all: $(MODULE)

debugger:$(MODULE) $(DEBUGGING)

$(MODULE): $(OBJECTS_ASM) $(OBJECTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS) $(OBJECTS_ASM) -o ../$(MODULE)

$(DEBUGGING): $(OBJECTS_ASM) $(OBJECTS)
	$(LD) -T sampleCodeModule.ld _loader.o $(OBJECTS) $(OBJECTS_ASM) --oformat=elf64-x86-64 -o $(DEBUGGING)

clean:
	rm -rf *.o ../*.elf

.PHONY: all clean