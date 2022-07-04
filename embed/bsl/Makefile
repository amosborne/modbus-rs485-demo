SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BLD_DIR := build

CC := C:/ti/msp430-gcc/bin/msp430-elf-gcc
CFLAGS := -IC:/ti/msp430-gcc/include -I$(INC_DIR) -O2 -Wall -g
LDFLAGS := -LC:/ti/msp430-gcc/include -Wl,-Map,$(BLD_DIR)/main.map

# todo: switch to intel hex generator (msp430-elf-objcopy)
HX := C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.6.LTS/bin/hex430
FL := C:/ti/MSPFlasher_1.3.20/MSP430Flasher

ASTYLE_BIN := D:/AStyle/bin/AStyle

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TRGT := $(BLD_DIR)/main.txt

.PHONY: all impact test format clean

all: $(TRGT)

impact: $(TRGT)
	$(FL) -n MSP430G2xx3 -w ./build/main.txt -v -z [RESET, VCC=3300] -g -j fast

$(TRGT): $(OBJS) | $(BLD_DIR)
	@echo
	$(CC) $(LDFLAGS) -mmcu=MSP430G2553 \
	$^ \
	-o $@
	@echo
	$(HX) --memwidth=8 --romwidth=8 \
	--ti_txt --outfile $@ $@

$(BLD_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo
	$(CC) $(CFLAGS) -mmcu=MSP430G2553 \
	-c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

test:
	@$(MAKE) --directory=test

format:
	@echo
	@$(ASTYLE_BIN) --suffix=none --style=java --indent=spaces=4 \
	--indent-switches --break-blocks --pad-oper --pad-paren-in --pad-header \
	--align-pointer=name --convert-tabs --max-code-length=100 --recursive \
	--preserve-date --verbose --formatted --lineend=windows --mode=c *.c *.h

clean:
	@echo
	$(RM) --recursive $(OBJ_DIR)
	$(RM) --recursive $(BLD_DIR)
	@$(MAKE) --directory=test clean
