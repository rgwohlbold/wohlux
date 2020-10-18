DEPS = boot.o kernel.o console.o gdt.o
CCFLAGS = -std=gnu99 -ffreestanding -Wall -Wextra -g
ASFLAGS = -g
LDFLAGS = -ffreestanding -nostdlib -lgcc -T linker.ld -g

system.iso: kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/
	cp grub.cfg iso/boot/grub/
	grub-mkrescue -o system.iso iso

%.o: %.s
	i686-elf-as $< -o $@ $(ASFLAGS)

%.o: %.c
	i686-elf-gcc -c $< -o $@ $(CCFLAGS)

kernel.bin: $(DEPS)
	i686-elf-gcc -o $@ $^ $(LDFLAGS)

run: system.iso
	qemu-system-i386 -s system.iso

clean:
	rm *.o
	rm -rf iso
	rm kernel.bin
	rm system.iso
