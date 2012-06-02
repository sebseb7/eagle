PROJECT=oled

MCU = atmega88
MCU_AVRDUDE = atmega88




ifeq ($(OSTYPE),)
OSTYPE      = $(shell uname)
endif
ifneq ($(findstring Darwin,$(OSTYPE)),)
USB_DEVICE = /dev/cu.SLAB_USBtoUART
else
USB_DEVICE = /dev/ttyUSB0
endif


#########################################################################

SRC=$(wildcard *.c libs/*.c)
OBJECTS=$(SRC:.c=.o) 
DFILES=$(SRC:.c=.d) 
LSTFILES=$(SRC:.c=.lst) 
HEADERS=$(wildcard *.h libs/*.h)



#  Compiler Options
GCFLAGS = -mmcu=$(MCU) -Wa,-adhlns=$(<:.c=.lst) -I. -gstabs -DF_CPU=8000000 -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall -Wstrict-prototypes  -std=gnu99 -MD -MP -Ilibs
#LDFLAGS =  -Wl,-Map=pwbl.map,--cref    -lm -Wl,--section-start=.text=0x1800
LDFLAGS = -mmcu=$(MCU)  


#  Compiler Paths
GCC = avr-gcc
LD = avr-gcc
REMOVE = rm -f
SIZE = avr-size
OBJCOPY = avr-objcopy

#########################################################################

all: $(PROJECT).hex Makefile stats

$(PROJECT).hex: $(PROJECT).elf Makefile
	$(OBJCOPY) -R .eeprom -O ihex $(PROJECT).elf $(PROJECT).hex 

$(PROJECT).elf: $(OBJECTS) Makefile
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROJECT).elf

stats: $(PROJECT).elf Makefile
	$(SIZE) -C --mcu=$(MCU) $(PROJECT).elf

clean:
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(PROJECT).hex
	$(REMOVE) $(DFILES)
	$(REMOVE) $(LSTFILES)
	$(REMOVE) $(PROJECT).elf

#########################################################################

%.o: %.c Makefile $(HEADERS)
	$(GCC) $(GCFLAGS) -o $@ -c $<

#########################################################################

flash: all
	avrdude -p m88 -F -c STK500v2 -P $(USB_DEVICE) -U flash:w:$(PROJECT).hex
#	avrdude  -p $(MCU_AVRDUDE) -c arduino -P $(USB_DEVICE) -b19200 -U flash:w:$(PROJECT).hex
#avrdude -p m168 -c STK500v2 -P /dev/cu.SLAB_USBtoUART -U lfuse:w:0xf7:m -U hfuse:w:0xdc:m -U efuse:w:0x01:m
