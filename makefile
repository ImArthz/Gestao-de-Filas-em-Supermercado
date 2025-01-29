CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99
TARGET = supermercado
OBJS = main.o cliente.o caixa.o

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c Cliente.h Caixa.h
	$(CC) $(CFLAGS) -c main.c

cliente.o: Cliente.c Cliente.h
	$(CC) $(CFLAGS) -c Cliente.c

caixa.o: Caixa.c Caixa.h Cliente.h
	$(CC) $(CFLAGS) -c Caixa.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)