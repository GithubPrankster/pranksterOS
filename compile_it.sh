#!/bin/sh

make

if grub-file --is-x86-multiboot pranksteros.bin; then
  echo It can multiboot!
  cp bootloader/grub.cfg isodir/boot/grub/grub.cfg
  cp pranksteros.bin isodir/boot/pranksteros.bin
  grub-mkrescue -o pranksteros.iso isodir
else
  echo It cannot multiboot...
fi