# This is a sample Makefile which compiles source files named:
# - tcpechotimeserv.c
# - tcpechotimecli.c
# - time_cli.c
# - echo_cli.c
# and creating executables: "server", "client", "time_cli"
# and "echo_cli", respectively.
#
# It uses various standard libraries, and the copy of Stevens'
# library "libunp.a" in ~cse533/Stevens/${slib}_solaris2.10 .
#
# It also picks up the thread-safe version of "readline.c"
# from Stevens' directory "threads" and uses it when building
# the executable "server".
#
# It is set up, for illustrative purposes, to enable you to use
# the Stevens code in the ~cse533/Stevens/${slib}_solaris2.10/lib
# subdirectory (where, for example, the file "unp.h" is located)
# without your needing to maintain your own, local copies of that
# code, and without your needing to include such code in the
# submissions of your assignments.
#
# Modify it as needed, and include it with your submission.

slib = ../unpv13e
#slib = /home/courses/cse533/Stevens/unpv13e_solaris2.10

CC = gcc

LIBS = -lresolv  -lnsl -lpthread\
	${slib}/libunp.a\
	
FLAGS = -g -O2

CFLAGS = ${FLAGS} -I${slib}/lib

all: client server echo_cli time_cli


time_cli: time_cli.o common.o
	${CC} ${FLAGS} -o time_cli time_cli.o common.o ${LIBS}
time_cli.o: time_cli.c
	${CC} ${CFLAGS} -c time_cli.c


echo_cli: echo_cli.o common.o
	${CC} ${FLAGS} -o echo_cli echo_cli.o common.o ${LIBS}
echo_cli.o: echo_cli.c 
	${CC} ${CFLAGS} -c echo_cli.c


echo_srv.o: echo_srv.c 
	${CC} ${CFLAGS} -c echo_srv.c

# server uses the thread-safe version of readline.c

server: tcpechotimesrv.o readline.o echo_srv.o
	${CC} ${FLAGS} -o server tcpechotimesrv.o echo_srv.o readline.o ${LIBS}
tcpechotimesrv.o: tcpechotimesrv.c
	${CC} ${CFLAGS} -c tcpechotimesrv.c


client: tcpechotimecli.o
	${CC} ${FLAGS} -o client tcpechotimecli.o ${LIBS}
tcpechotimecli.o: tcpechotimecli.c
	${CC} ${CFLAGS} -c tcpechotimecli.c


# pick up the thread-safe version of readline.c from directory "threads"

readline.o: ${slib}/threads/readline.c
	${CC} ${CFLAGS} -c ${slib}/threads/readline.c

common.o: common.c
	${CC} ${CFLAGS} -c common.c


clean:
	rm echo_cli echo_cli.o server tcpechotimesrv.o client tcpechotimecli.o time_cli time_cli.o readline.o

