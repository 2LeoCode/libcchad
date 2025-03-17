SHELL = bash

NAME = toolbox

CC = clang-19
AR = ar rcs
MKDIR = mkdir -p
RM = rm -rf

SRC_DIR = src
INC_DIR = inc

BUILD_DIR = .build
OUT_DIR = .

CFLAGS = -Wall -Werror -Wextra -pedantic -std=c23 -MMD -I$(INC_DIR)
LDFLAGS =

SRC = $(shell find $(SRC_DIR) -type f \( -name '*.c' -a ! -name '*_test.c' \))
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:.o=.d)

TEST_SRC = $(shell find $(SRC_DIR) -type f -name '*_test.c')
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%_test.c=$(BUILD_DIR)/%_test.o)
TEST_DEP = $(TEST_BIN:.o=.d)

TARGET = $(OUT_DIR)/lib$(NAME).a
TEST_TARGET = $(OUT_DIR)/$(NAME)_test

ifndef BUILD_TYPE
	BUILD_TYPE = debug
endif

ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -g3 -fsanitize=address
else ifeq ($(BUILD_TYPE), production)
	CFLAGS += -O3 -ndebug
else ifneq ($(BUILD_TYPE), lsp)
	$(error Invalid value for BUILD_TYPE: $(BUILD_TYPE))
endif

all: $(NAME)

-include $(DEP)
-include $(TEST_DEP)

$(NAME): $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(TARGET)
	$(CC) $(CFLAGS) $(TEST_OBJ) -o $@ -lcriterion -L. -l$(NAME) 

$(BUILD_DIR)/%_test.o: $(SRC_DIR)/%_test.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

compile_flags.txt:
	echo '$(CFLAGS)' | tr ' ' '\n' > $@

clean:
	$(RM) $(BUILD_DIR)

fclean: clean
	$(RM) $(TARGET) $(TEST_TARGET)
	if [ -z "$$(ls -A '$(OUT_DIR)' 2>/dev/null)" ]; then \
		$(RM) $(OUT_DIR); \
	fi

re: fclean all

.PHONY: all $(NAME) test clean fclean re
