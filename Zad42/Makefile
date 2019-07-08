# Common definitions
CC=arm-elf-gcc
GDB=arm-elf-gdb
ASFLAGS=-D__ASSEMBLY__ -g -mcpu=arm9 -c -O0 -Wall
CCFLAGS=-g -mcpu=arm9 -O0 -Wall
LDFLAGS+=-nostartfiles -Wl -lc -lgcc -T /opt/arm_user/elf32-littlearm.lds -g -Ttext 0x20000000 -Tdata 0x300000 -n
EXTOBJS=/opt/arm_user/include/cstartup.o /opt/arm_user/include/lowlevel.o
OBJS=main.o dbg_print.o

# Output file base name
PROGRAM_NAME=lab

# Main tagret: build the program
all: $(PROGRAM_NAME).elf

# Run the debugger with application
run: $(PROGRAM_NAME).elf
	$(GDB) $<

# Linking
$(PROGRAM_NAME).elf: $(EXTOBJS) $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Running assembler
%.o: %.S
	$(CC) $(ASFLAGS) $^ -o $@

# Running C compiler
%.o: %.c
	$(CC) -c $(CCFLAGS) $^ -o $@

# Removing output files
clean:
	rm -f $(OBJS) $(PROGRAM_NAME).elf 

# Declaration of "dummy" targets
.PHONY: all clean run
