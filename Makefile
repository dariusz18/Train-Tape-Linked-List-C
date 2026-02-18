CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra
EXEC=tema1
$(EXEC):tema1.c
	$(CC) $(CFLAGS) tema1.c -o $(EXEC)
.PHONY:build clean
build: $(EXEC)
run:
	./$(EXEC)
clean:
	rm -f $(EXEC)
