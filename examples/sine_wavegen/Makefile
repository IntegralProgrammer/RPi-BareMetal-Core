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

timer.o : ../../src/timer.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/timer.c -o timer.o

clock.o : ../../src/clock.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/clock.c -o clock.o

uart.o : ../../src/uart.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/uart.c -o uart.o

pwm.o : ../../src/pwm.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/pwm.c -o pwm.o

headphonejack.o : ../../src/headphonejack.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/headphonejack.c -o headphonejack.o

midi.o: ../../lib/midi/midi.c
	$(ARMGNU)-gcc $(COPS) -c ../../lib/midi/midi.c -o midi.o

wavegen.o: ../../lib/dsp/primitives/wavegen.c
	$(ARMGNU)-gcc $(COPS) -c ../../lib/dsp/primitives/wavegen.c -o wavegen.o

notmain.o : ../../src/notmain.c
	$(ARMGNU)-gcc $(COPS) -c ../../src/notmain.c -o notmain.o

sketch.o : sketch.c
	$(ARMGNU)-gcc $(COPS) -c sketch.c -o sketch.o

kernel.img : ../../src/memmap start.o gpio.o clock.o uart.o pwm.o headphonejack.o timer.o midi.o wavegen.o notmain.o sketch.o
	$(ARMGNU)-ld start.o gpio.o clock.o uart.o pwm.o headphonejack.o timer.o midi.o wavegen.o notmain.o sketch.o -T ../../src/memmap -o kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
