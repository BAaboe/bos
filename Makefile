BUILD_DIR=build
BOOT_SRC := $(wildcard src/boot/*.s)
BOOT_OBJS := $(patsubst src/boot/%.s, $(BUILD_DIR)/boot/%.o, $(BOOT_SRC))
KERNEL_SRC := $(wildcard src/kernel/*.c)
KERNEL_OBJS := $(patsubst src/kernel/%.c, $(BUILD_DIR)/kernel/%.o, $(KERNEL_SRC))

bin = build/benjios.bin
iso = grub/benjios.iso

$(BUILD_DIR)/boot/%.o: src/boot/%.s
	i686-elf-as $< -o $@

$(BUILD_DIR)/kernel/%.o: src/kernel/%.c
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O3 -Wall -Wextra

bin: $(BOOT_OBJS) $(KERNEL_OBJS)
	i686-elf-gcc -T linker.ld -o $(bin) -ffreestanding -O3 -nostdlib $^ -lgcc

iso: bin
	cp $(bin) grub/isodir/boot
	grub-mkrescue -o $(iso) grub/isodir

run: iso
	qemu-system-i386 -cdrom $(iso)
