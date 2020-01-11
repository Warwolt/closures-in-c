SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
DEP = $(patsubst %.o,%.d,$(OBJ))
BIN = closure

LIB = -lm -lncurses

all: $(BIN)

-include $(DEP)

$(BIN): $(OBJ)
	@echo linking $(notdir $@)
	@gcc $^ $(LIB) -o $@

%.o: %.c
	@echo compiling $(notdir $<)
	@gcc -c $< -Wall -Werror -MMD -o $@

clean:
	@echo cleaning..
	@rm -f $(OBJ)
	@rm -f $(BIN)
