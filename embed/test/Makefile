SRC_DIR := ../src
INC_DIR := ../include
OBJ_DIR := obj
BIN_DIR := bin

CMOCKA_INC := "C:/Program Files (x86)/cmocka/include"
CMOCKA_BIN := "C:/Program Files (x86)/cmocka/bin"

ASTYLE_BIN := D:/AStyle/bin/AStyle

CC := gcc
CFLAGS := -I$(SRC_DIR) -I$(INC_DIR) -I$(CMOCKA_INC) -Wall
LDFLAGS := -L$(CMOCKA_BIN) -lcmocka

TESTS := test_bsl430_protocol_parser

.PHONY: all clean

all: $(foreach TEST, $(TESTS), $(BIN_DIR)/$(TEST))

$(BIN_DIR)/test_bsl430_protocol_parser: $(OBJ_DIR)/bsl430_protocol_parser.o \
										$(OBJ_DIR)/test_bsl430_protocol_parser.o | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@ -Wl,--wrap=load_pc -Wl,--wrap=bad_command -Wl,--wrap=erase_segment -Wl,--wrap=rx_data_block
	./$@

$(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	$(RM) -rf $(OBJ_DIR)
	$(RM) -rf $(BIN_DIR)

format: $(wildcard *.c)
	$(ASTYLE_BIN) --suffix=none $^
