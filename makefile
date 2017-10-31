all: assign3part2

assign3part2: assign3part2.c phypages.c pagetable.c
	cc -o assign3part2 assign3part2.c phypages.c pagetable.c

clean:
	rm -f assign3part2 *.o
