FILENAME = traceroute

CC = gcc

FLAGS = -Wall -Wextra -std=gnu99

SRCS = main.c send.c receive.c traceroute_utils.c
OBJS = main.o send.o receive.o traceroute_utils.o

$(FILENAME): $(OBJS)
	$(CC) $(FLAGS) -o $(FILENAME) $(OBJS)

main.o: main.c
	$(CC) $(FLAGS) -c main.c -o main.o

send.o: send.c
	$(CC) $(FLAGS) -c send.c -o send.o

receive.o: receive.c
	$(CC) $(FLAGS) -c receive.c -o receive.o

traceroute_utils.o: traceroute_utils.c
	$(CC) $(FLAGS) -c traceroute_utils.c -o traceroute_utils.o

clean:
	rm -f $(OBJS)

distclean:
	rm -f $(OBJS) $(FILENAME)

.PHONY: clean distclean