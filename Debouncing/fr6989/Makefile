GCC_DIR =  /opt/ti/mspgcc/bin
SUPPORT_FILE_DIRECTORY = /opt/ti/mspgcc/include

# Please set your device here
DEVICE  = msp430fr6989
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb
OBJCOPY = $(GCC_DIR)/msp430-elf-objcopy
FLASH   = MSP430Flasher

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2 -g -Werror
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY) -T $(DEVICE).ld

all: program

${DEVICE}.elf: main.o
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(DEVICE).elf

${DEVICE}.hex: ${DEVICE}.elf
	$(OBJCOPY) -O ihex $(DEVICE).elf $(DEVICE).hex

program: ${DEVICE}.hex
	$(FLASH) -n $(DEVICE) -w $(DEVICE).hex -v -z '[VCC]'

debug: all
	$(GDB) $(DEVICE).out

clean:
	rm *.o *.elf *.hex
