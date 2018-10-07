# ---------------------------------------------------------------------------
# Project name

NAME	= pulse_width_modulation

# Settings for compilator and linker

CC      = sdcc
CFLAGS  = -I./INCLUDE -c --stack-auto
LFLAGS  = --code-loc 0x2100 --xram-loc 0x6000 --stack-auto --stack-loc 0x80 

# Settings for system of autoincrement of build version

PROJECT  = $(shell type PROJECT)
VERSION  = $(shell type VERSION)
BUILD    = $(shell type BUILD)
TYPE     = $(shell type TYPE)

PROJNAME = ${PROJECT}-${VERSION}-${BUILD}-${TYPE}
TARBALL  = ${PROJNAME}.tar

# Settings for M3P

M3P		 = m3p
COMPORT	 = com1
COMLOG	 = $(COMPORT)_log.txt
BAUD	 = 9600	

# Directories with source

SRC_DIR = SRC
# ---------------------------------------------------------------------------

all: pulse_width_modulation

clean:
	del $(NAME).hex
	del $(NAME).bin
	del $(NAME).map
	del $(NAME).mem
	del $(NAME).lnk
	del pm3p_*.txt
	del com?_log.txt
	del $(TARBALL).gz
	del $(SRC_DIR)\*.asm
	del $(SRC_DIR)\*.rel
	del $(SRC_DIR)\*.rst
	del $(SRC_DIR)\*.sym
	del $(SRC_DIR)\*.lst 

load:
	$(M3P) lfile load.m3p


dist:
	tar -cvf $(TARBALL) --exclude=*.tar .
	gzip $(TARBALL)

term:
	$(M3P) echo $(COMLOG) $(BAUD)  openchannel $(COMPORT) +echo 6 term -echo bye



LIST_SRC = \
$(SRC_DIR)/animation.c \
$(SRC_DIR)/common.c \
$(SRC_DIR)/counter.c \
$(SRC_DIR)/led.c \
$(SRC_DIR)/main.c \
$(SRC_DIR)/max.c \
$(SRC_DIR)/system_timer.c \
$(SRC_DIR)/mode_selector.c 

LIST_OBJ = $(LIST_SRC:.c=.rel)

pulse_width_modulation : $(LIST_OBJ) makefile
	$(CC) $(LIST_OBJ) -o pulse_width_modulation.hex $(LFLAGS)
	$(M3P) hb166 pulse_width_modulation.hex pulse_width_modulation.bin bye


$(LIST_OBJ) : %.rel : %.c makefile
	$(CC) -c $(CFLAGS) $< -o $@  

