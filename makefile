C_SRC := $(wildcard *.c)
C_OBJ := $(C_SRC:.c=.o)


%.o: %.c
	gcc -c $^ -o $@

all: $(C_OBJ)
	echo $(C_OBJ)