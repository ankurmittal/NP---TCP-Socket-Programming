#include	"unp.h"

int main(int argc, char **argv)
{
	int	 sockfd, n, maxfdp1;
	char recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	int stdineof = 0;
	fd_set rset;

	if (argc != 2)
		err_quit("usage: time_cli <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(stdin), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(stdin), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
			if ( (n = read(sockfd, recvline, MAXLINE)) == 0) {
				if (stdineof == 1)
					return; /* normal termination */
				else
					err_quit("str_cli: server terminated prematurely");
			}
			recvline[n] = 0;	/* null terminate */
			if (fputs(recvline, stdout) == EOF)
				err_sys("fputs error");
		}
		if (FD_ISSET(fileno(stdin), &rset)) { /* input is readable */
			if ( (n = Read(fileno(stdin), recvline, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR); /* send FIN */
				FD_CLR(fileno(stdin), &rset);
				continue;
			}
			Writen(sockfd, recvline, n);
		}
	}
	if (n < 0)
		err_sys("read error");

}
