#include <errno.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
int statusfd;

void show_err_sys(char *s) {
	int n, buflenght;
	char *error =  "";
	char *colon = "";
	if(errno !=0) {
		error = strerror(errno);
		colon = ": ";
	}
	buflenght =  strlen(error) + strlen(s) + 1 + strlen(colon);
	char *buf  = malloc(buflenght);
	strcpy(buf, s);
	strcat(buf, colon);
	strcat(buf, error);
	n = write(statusfd, buf, buflenght);
	getchar();
	exit(1);
}

int myselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
		struct timeval *timeout)
{
	int n;
	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		show_err_sys("select error");
	return(n);      /* can return 0 on timeout */
}

ssize_t myread(int fd, void *ptr, size_t nbytes)
{
	ssize_t     n;
	if ( (n = read(fd, ptr, nbytes)) == -1)
		show_err_sys("read error");
	return(n);
}
