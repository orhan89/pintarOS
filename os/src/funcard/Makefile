PLATFORM=funcard
ARCH=at90s8515

CC=avr-gcc
AS=avr-as
OBJCOPY=avr-objcopy
SIZE=avr-size

ROOT_DIR=../..
INCLUDE_DIR=$(ROOT_DIR)/include
SRC_DIR=$(ROOT_DIR)/src
BUILD_DIR=./build

CFLAGS=-mmcu=$(ARCH) -Wall -Os -mno-interrupts -mcall-prologues -fno-jump-tables -mtiny-stack -I$(INCLUDE_DIR) -I. -DDAY=0x00 -DMONTH=0x00 -DYEAR=0x00
ASFLAGS=-mmcu=$(ARCH) -Wall -I$(INCLUDE_DIR) -DDAY=0x00 -DMONTH=0x00 -DYEAR=0x00

SRCC= \
	$(SRC_DIR)/hal.c \
	$(SRC_DIR)/transmission.c \
	$(SRC_DIR)/command.c \
	$(SRC_DIR)/response.c \
	$(SRC_DIR)/state.c \
	$(SRC_DIR)/fs.c \
	$(SRC_DIR)/tea.c \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/$(PLATFORM)/funcard.c

SSRC= \
	$(SRC_DIR)/$(PLATFORM)/io.S \
	$(SRC_DIR)/$(PLATFORM)/eepromi2c.S \
	$(SRC_DIR)/$(PLATFORM)/eedata.S

OBJ=$(SRCC:.c=.o) $(SSRC:.S=.o)

all: flash eeprom

pintaros: $(OBJ)
	$(CC) $(CFLAGS) -Wl,-Map,pintaros.map -o pintaros $(OBJ)
	$(SIZE) pintaros

flash: pintaros
	$(OBJCOPY) -j .text -j .data -O binary pintaros flash.bin
	$(OBJCOPY) -j .text -j .data -O ihex pintaros flash.hex
	$(OBJCOPY) -j .text -j .data -O srec pintaros flash.srec

eeprom: pintaros
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O binary pintaros eeprom.bin
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex pintaros eeprom.hex
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex pintaros eeprom.srec

clean: clean-pintaros clean-flash clean-eeprom
	rm -rf core

clean-pintaros:
	rm -rf $(OBJ) pintaros  pintaros.map

clean-flash:
	rm -rf flash.bin flash.hex flash.srec

clean-eeprom:
	rm -rf eeprom.bin eeprom.hex eeprom.srec


