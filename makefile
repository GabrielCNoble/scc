C_SRC := $(wildcard *.c)
C_OBJ := $(C_SRC:.c=.o)
C_FLAGS := -g


%.o: %.c
	gcc -c $^ $(C_FLAGS) -o $@

all: $(C_OBJ)
	gcc $(C_OBJ) $(C_FLAGS) -o scc.out

clean:
	rm $(C_OBJ)