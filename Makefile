LIBS= -pthread
.c.o:
	clang -c -g -Wall ${CFLAGS} $<

all: server client

server: server.o tcp_listen.o error.o wrapunix.o wrapsock.o wrappthread.o str_echo.o writen.o csum.o readline.o wrapstdio.o
	clang -g -o server server.o tcp_listen.o error.o wrapunix.o wrapsock.o wrappthread.o str_echo.o writen.o csum.o readline.o wrapstdio.o ${LIBS}

client: client.o wrapsock.o error.o tcp_connect.o wrapunix.o strclithread.o writen.o wrapstdio.o readline.o wrappthread.o csum.o
	clang -g -o client client.o wrapsock.o error.o tcp_connect.o wrapunix.o strclithread.o writen.o wrapstdio.o readline.o wrappthread.o csum.o ${LIBS}

clean: 
	rm *.o server client
