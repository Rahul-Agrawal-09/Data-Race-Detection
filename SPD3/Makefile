CC?=gcc

#clean and build
all: clean dpst

# Build the test
test: dpst.c memory.c spd3.c test.c -lpthread
	$(CC) -o $@ $^

# Build the array_sum_test
arr_test: dpst.c memory.c spd3.c arr_sum_test.c -lpthread
	$(CC) -o $@ $^

#remove the executable file if not needed
clean: 
	rm -f ./dpst
