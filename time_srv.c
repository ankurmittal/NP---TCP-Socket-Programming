#include	"unp.h"
#include	<time.h>

void str_time(connfd)
{
	char buff[MAXLINE];
	time_t ticks;
	int maxfdp1, *iptr, n;
	fd_set rset;
	struct timeval interval;
	char buf[MAXLINE];

	ticks = time(NULL);
	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	n = write(connfd, buff, strlen(buff));
	for( ; ;) {
		interval.tv_sec = 5;
		interval.tv_usec = 0;
		FD_ZERO(&rset);
		FD_SET(connfd, &rset);
		maxfdp1 = connfd + 1;
		n = 1;

		if((n = select(maxfdp1, &rset, NULL, NULL, &interval)) < 0) {
			printf("Select error");
			return;
		} 
		if(n == 0) {
			ticks = time(NULL);
			snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
			n = write(connfd, buff, strlen(buff));
		}
		if (FD_ISSET(connfd, &rset)) { /* input is readable */
			if ( (n = read(connfd, buf, MAXLINE)) <= 0) {
				printf("Client Exited\n");
				return;
			}
		}

	}
}
