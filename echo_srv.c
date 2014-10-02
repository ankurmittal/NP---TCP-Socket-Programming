#include "unp.h"
void str_echo(int sockfd)
{
	int n;
	char buf[MAXLINE];
again:
	while ( (n = readline(sockfd, buf, MAXLINE)) > 0)
		 n = write(sockfd, buf, n);
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n <=0) {
		printf("Echo client termination: socket read returned with value %d", n);
		if(n < 0)
			printf(", errno = %d\n", errno);
		else
			printf("\n");
	}
}
