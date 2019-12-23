#!/bin/sh

make

if grub-file --is-x86-multiboot pranksteros.elf; then
  echo It can multiboot!
  cp bootloader/grub.cfg isodir/boot/grub/grub.cfg
  cp pranksteros.elf isodir/boot/pranksteros.elf
  grub-mkrescue -o pranksteros.iso isodir
else
  echo It cannot multiboot...
fi