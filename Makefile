all:
	i686-elf-as bootloader/boot.asm -o bootloader/boot.o
	i686-elf-gcc -c kernel/prankster_kernel.c -o kernel/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T bootloader/linker.ld -o pranksteros.bin -ffreestanding -O2 -nostdlib bootloader/boot.o kernel/kernel.o -lgcc