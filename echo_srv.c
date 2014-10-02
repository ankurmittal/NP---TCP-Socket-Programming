#include "unp.h"
void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
		 n = write(sockfd, buf, n);
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		perror("Can't Read from client");
}
