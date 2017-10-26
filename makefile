all:
	gcc main.c -std=c99 -o fortes -l cgraph 
clean:
	rm fortes
