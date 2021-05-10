OUT_DIR = build
DEPS = ${OUT_DIR}/boot.o ${OUT_DIR}/kernel.o ${OUT_DIR}/console.o ${OUT_DIR}/gdt.o ${OUT_DIR}/idt.o ${OUT_DIR}/irq.o ${OUT_DIR}/mem.o
CCFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -g
ASFLAGS = -g
LDFLAGS = -ffreestanding -nostdlib -lgcc -T linker.ld -g

${OUT_DIR}/system.iso: ${OUT_DIR}/kernel.bin
	mkdir -p iso/boot/grub
	cp ${OUT_DIR}/kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o ${OUT_DIR}/system.iso iso

${OUT_DIR}/%.o: %.s
	i686-elf-as $< -o $@ $(ASFLAGS)

${OUT_DIR}/%.o: %.c
	i686-elf-gcc -c $< -o $@ $(CCFLAGS)

${OUT_DIR}/kernel.bin: $(DEPS)
	i686-elf-gcc -o $@ $^ $(LDFLAGS)

run: ${OUT_DIR}/system.iso
	qemu-system-i386 -s $<

clean:
	rm -rf iso build

$(shell mkdir -p $(OUT_DIR))