all: WitsShell

WitsShell: WitsShell.c
	gcc WitsShell.c -std=c17 -Wall -Wextra -pedantic -lm -o witsshell 

clean:
	rm -f witsshell