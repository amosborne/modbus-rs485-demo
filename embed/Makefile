# path to msp430 gcc compiler
CC := C:/ti/msp430-gcc/bin/msp430-elf-gcc
# path to msp430 headers
CFLAGS := -I C:/ti/msp430-gcc/include -O2 -Wall -g
# path to msp430 linker scripts
LDFLAGS := -L C:/ti/msp430-gcc/include -Wl,-Map,build/main.map

# path to msp430 hex generator
HX := C:/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.6.LTS/bin/hex430
# path to msp430 flasher
FL := C:/ti/MSPFlasher_1.3.20/MSP430Flasher

# path to code formatter
FT := D:/AStyle/bin/AStyle

SRCS := $(wildcard ./src/*.c)                      # identify all source files
OBJS := $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))  # identify one object file per source file

.PHONY: all impact clean format

all: format ./build/main.txt

./build/main.txt: $(OBJS) # link the object files into a single executable
	$(CC) $(LDFLAGS) -mmcu=MSP430G2553 $^ -o ./build/main.out
	$(HX) --memwidth=8 --romwidth=8 --ti_txt --outfile $@ ./build/main.out

./obj/%.o: ./src/%.c  # compile a source file into an object file
	$(CC) $(CFLAGS) -mmcu=MSP430G2553 -c $< -o $@

impact: all # flash the target device
	$(FL) -n MSP430G2xx3 -w ./build/main.txt -v -z [RESET, VCC=3300] -g -j fast

format: $(SRCS)
	$(FT) --suffix=none $^

clean:  # delete all object files
	rm -f $(OBJS)
	rm -f ./build/main.out
	rm -f ./build/main.txt
