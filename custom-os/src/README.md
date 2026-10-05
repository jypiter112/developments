### Proof of concept bootlader + OS

Tiny x86 OS in 32-bit protected-mode kernel writting in C and asm. 16-bit boot sector that upgrades to 32-bit before loading kernel.<br>
- Ran in qemu with
`qemu-system-x86_64 -drive format=raw,file=boot/os.img`
- Or with testing
`make clean && make run`

**Abstract boot flow**
1. BIOS loads sector 1 to `0x7C00` with `0xAA55` ending signature
		1. 512 bytes and 16-bits
2. `boot.asm` reads `KERNEL_SECTORS` from sector 2 to `0x1000` with `int 0x13`
3. enables A20, loads GDT, sets CR0.PE
4. far jump to 32-bit mode
5. segment registers and stack `0x9000`setted
6. jump to `0x1000`
7. `kernel_entry.asm`  zeroes `.bss` and calls `kmain()`

### Memory structure

| Address | Contents                      |
| ------- | ----------------------------- |
| 0x1000  | Kernel image                  |
| 0x7C00  | Boot sector (nuked)           |
| 0x90000 | Stack                         |
| 0xB8000 | VGA buff 80x25 2 bytes / cell |

### Limits
- Kernel must fit in KERNEL_SECTORS * 512 bytes (10 kb)
	- Raise: KERNEL_SECTORS and count= in makefile to raise size
- No BIOS calls in protected mode
- No interrupts

### Debugging cheatsheet
```bash
qemu-system-i386 -drive format=raw,file=os.img -s -S & gdb -ex 'target remote :1234' -ex 'set architecture i386' \ -ex 'break *0x1000' -ex 'continue' ```

Qemu flags:
`-monitor stdio` (inspect registers with `info registers`), `-d int,cpu_reset -no-reboot` (log faults instead of silently rebooting), `-serial stdio` (route COM1 to your terminal).
```


## File structure / project layout

| Path     | Purpose                        | Examples                           |
| -------- | ------------------------------ | ---------------------------------- |
| drivers/ | Device drivers                 | keyboard.c, vga.c                  |
| include/ | Headers                        | klib.h, vga.h                      |
| kernel/  | Kernel entry, kmain            | kernel.c, entry.asm                |
| lib/     | Custom libraries made/imported | klib.c, custom replacement to libc |

### Development pattern flow
1. Add created `new_path/` to `wildcard kernel/*.c new_path/*.c` in Makefile
2. rebuild `make clean && make run`

### Road map
1. IDE driver <-- currently working on as of 10/5/26
2. Stack implementation
3. FAT filesystem

**Sources/References/AI help used**

[Writing a Simple Operating System from Scratch](https://angom.myweb.cs.uwindsor.ca/teaching/cs330/WritingOS.pdf)
[Linux kernel](https://github.com/torvalds/linux)
[Building a bootlader from scratch](https://medium.com/@aayushgid/building-a-bootloader-from-scratch-an-x86-assembly-guide-6407fe5be487)
Claude
