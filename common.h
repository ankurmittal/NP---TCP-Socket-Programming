
extern int statusfd;

void show_err_sys(char *s);
int myselect(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
		struct timeval *timeout);
