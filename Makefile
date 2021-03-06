KERNEL_NAME = pranksteros
KERNEL_SOURCES = $(wildcard kernel/*.c) $(wildcard kernel/plibc/*.c)
KERNEL_SOURCES_ASM = $(wildcard bootloader/*.asm)
KERNEL_OBJS = $(patsubst %.c,%.o,$(KERNEL_SOURCES)) \
              $(patsubst %.asm,%.o,$(KERNEL_SOURCES_ASM))

COMPILE_FLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -g
AS_FLAGS = -g
 
CROSS_COMP = i686-elf
CC = $(CROSS_COMP)-gcc
AS = $(CROSS_COMP)-as
OBJCOPY = $(CROSS_COMP)-objcopy
 
GRUB_MKRESCUE = grub-mkrescue
ISO_DIR = isodir
GRUB_CFG = grub.cfg

QEMU = qemu-system-i386
QEMU_FLAGS = -no-shutdown -no-reboot
QEMU_BP = *kernel_main
GDB = gdb
 
.PHONY: all clean debug
 
all: kernel/$(KERNEL_NAME).iso
 
kernel/$(KERNEL_NAME).elf: $(KERNEL_OBJS)
	$(CC) -T bootloader/linker.ld -o $@ -nostdlib $^ -lgcc
 
kernel/$(KERNEL_NAME).iso: kernel/$(KERNEL_NAME).elf
	mkdir -p $(ISO_DIR)/boot/grub
	cp $< $(ISO_DIR)/boot
	@echo 'Creating $(ISO_DIR)/boot/grub/$(GRUB_CFG)'
	@echo 'set timeout=0'                        >  $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	@echo 'set default=0'                        >> $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	@echo 'menuentry "pranksterOS" {'              >> $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	@echo '  multiboot /boot/$(KERNEL_NAME).elf' >> $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	@echo '}'                                    >> $(ISO_DIR)/boot/grub/$(GRUB_CFG)
	@echo 'Creating $@'
	$(GRUB_MKRESCUE) -o $@ $(ISO_DIR)/

debug: kernel/$(KERNEL_NAME).iso
	$(QEMU) -cdrom $< $(QEMUFLAGS) -S -s &
	$(GDB) kernel/$(KERNEL_NAME).elf \
		-ex 'target remote localhost:1234' \
		-ex 'layout src' \
		-ex 'layout regs' \
		-ex 'break $(QEMU_BP)' \
		-ex 'continue'

run: kernel/$(KERNEL_NAME).iso
	$(QEMU) -cdrom kernel/$(KERNEL_NAME).iso
 
%.o: %.c
	$(CC) ${COMPILE_FLAGS} -c $< -o $@
 
%.o: %.asm
	$(AS) $(AS_FLAGS) $< -o $@
 
clean:
	rm -f $(KERNEL_OBJS) \
		kernel/$(KERNEL_NAME).elf \
		kernel/$(KERNEL_NAME).iso
	rm -rf $(ISO_DIR)