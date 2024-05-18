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

mailbox.o: ../../src/mailbox.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/mailbox.c -o mailbox.o

video.o: ../../src/video.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/video.c -o video.o

graphics_canvas.o: ../../src/graphics_canvas.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/graphics_canvas.c -o graphics_canvas.o

notmain.o : ../../src/notmain.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/notmain.c -o notmain.o

sketch.o : sketch.c
	$(ARMGNU)-gcc $(COPS) -c sketch.c -o sketch.o

kernel.img : ../../src/memmap start.o gpio.o mailbox.o video.o graphics_canvas.o notmain.o sketch.o
	$(ARMGNU)-ld start.o gpio.o mailbox.o video.o graphics_canvas.o notmain.o sketch.o -T ../../src/memmap -o kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
