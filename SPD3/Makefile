CC?=gcc

#clean and build
all: clean dpst

# Build the dpst
dpst: dpst.c memory.c spd3.c test.c 
	$(CC) -o $@ $^

#remove the executable file if not needed
clean: 
	rm -f ./dpst
