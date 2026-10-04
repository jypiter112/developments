CC      = gcc
CFLAGS  = -m32 -ffreestanding -fno-pie -fno-pic -fno-stack-protector \
          -fno-asynchronous-unwind-tables -nostdlib -O2 -Wall -Wextra \
          -Iinclude -MMD -MP

C_SRCS  = $(wildcard kernel/*.c drivers/*.c lib/*.c)
C_OBJS  = $(C_SRCS:.c=.o)
# entry.o MUST be first so _start lands at 0x1000
OBJS    = kernel/entry.o $(C_OBJS)

os.img: boot/boot.bin kernel/kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=64 status=none
	dd if=boot/boot.bin     of=$@ conv=notrunc status=none
	dd if=kernel/kernel.bin of=$@ bs=512 seek=1 conv=notrunc status=none

boot/boot.bin: boot/boot.asm
	nasm -f bin $< -o $@

kernel/entry.o: kernel/entry.asm
	nasm -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel/kernel.bin: $(OBJS) kernel/kernel.ld
	ld -m elf_i386 -T kernel/kernel.ld --oformat binary $(OBJS) -o $@

run: os.img
	qemu-system-i386 -drive format=raw,file=os.img

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) boot/boot.bin kernel/kernel.bin os.img

-include $(C_OBJS:.o=.d)
