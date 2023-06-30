ARMGNU = arm-none-eabi
COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding

clean :
	rm -f *.o
	rm -f *.elf
	rm -f kernel.img

start.o : ../../src/start.s
	$(ARMGNU)-as ../../src/start.s -o start.o

gpio.o : ../../src/gpio.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/gpio.c -o gpio.o

clock.o : ../../src/clock.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/clock.c -o clock.o

notmain.o : ../../src/notmain.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/notmain.c -o notmain.o

sketch.o : sketch.c
	$(ARMGNU)-gcc $(COPS) -c sketch.c -o sketch.o

kernel.img : ../../src/memmap start.o gpio.o clock.o notmain.o sketch.o
	$(ARMGNU)-ld start.o gpio.o clock.o notmain.o sketch.o -T ../../src/memmap -o kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
