# Compiles the BlackDOS operating system
# Author: Nick Gallimore

# Compile bootloader assembly
#nasm bootload.asm

# Creates blank disk image
dd if=/dev/zero of=floppya.img bs=512 count=2880

# Puts bootload into sector 0
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

# Compiles kernel.c
bcc -ansi -c -o kernel.o kernel.c

# Compile kernel assembly
#as86 kernel.asm -o kasm.o

# Puts disk map at 256 and disk directory at 257
dd if=map of=floppya.img bs=512 count=1 seek=256 conv=notrunc

# Puts config into sector 258
dd if=config of=floppya.img bs=512 count=1 seek=258 conv=notrunc

# Links kernel.o with kasm.o
ld86 -o kernel -d kernel.o kasm.o

# Puts kernel into sector 259
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=259
