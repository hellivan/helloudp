CC=gcc
CFLAGS=


server: udp-server.o
	$(CC) -o $@ $^ $(CFLAGS)

client: udp-client.o
	$(CC) -o $@ $^ $(CFLAGS)


server.o: udp-server.c
	$(CC) -c $< $(CFLAGS)

client.o: udp-client.c
	$(CC) -c $< $(CFLAGS)



clean:
	rm -f *.o server client
