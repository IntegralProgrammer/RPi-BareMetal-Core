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

sd.o : ../../src/sd.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/sd.c -o sd.o

timer.o : ../../src/timer.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/timer.c -o timer.o

uart.o : ../../src/uart.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/uart.c -o uart.o

mbr.o : ../../lib/disklayout/mbr.c
	$(ARMGNU)-gcc $(COPS) -c ../../lib/disklayout/mbr.c -o mbr.o

notmain.o : ../../src/notmain.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/notmain.c -o notmain.o

sketch.o : sketch.c
	$(ARMGNU)-gcc $(COPS) -c sketch.c -o sketch.o

kernel.img : ../../src/memmap start.o gpio.o sd.o timer.o uart.o mbr.o notmain.o sketch.o
	$(ARMGNU)-ld start.o gpio.o sd.o timer.o uart.o mbr.o notmain.o sketch.o -T ../../src/memmap -o kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
