CC?=gcc

#clean and build
all: clean trivial_test arr_test

# Build the trivial_test
trivial_test: dpst.c memory.c spd3.c test.c -lpthread
	$(CC) -o $@ $^

# Build the array_sum_test
arr_test: dpst.c memory.c spd3.c arr_sum_test.c -lpthread
	$(CC) -o $@ $^

#remove the executable file if not needed
clean: 
	rm -f ./trivial_test
	rm -f ./arr_test
