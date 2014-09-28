#include "unp.h"

static struct hostent* gethostinfo(char *target, struct sockaddr_in servaddr);

int main(int argc, char **argv)
{
	int sockfd, n;
	socklen_t len;
	struct sockaddr_in servaddr;
	struct hostent *hostp;

	if (argc != 2)
		err_quit("usage: client <IPaddress or hostname>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_port   = htons(13);        /* daytime server */
	hostp = gethostinfo(argv[1], servaddr);
	if(hostp == NULL) 
	{
	    close(sockfd);
	    exit(-1);
	}
}


struct hostent* gethostinfo(char *target, struct sockaddr_in servaddr)
{
	struct hostent *hostp;
	struct in_addr ** addr_list;

	if (inet_pton(AF_INET, target, &servaddr.sin_addr) <= 0) 
	{
		hostp = gethostbyname(target);
		if(hostp == (struct hostent *)NULL)
		{
			printf("HOST or IP not valid.\n");
			return NULL;
		}
		else 
		{
			addr_list = (struct in_addr **) hostp->h_addr_list;
			printf("IP Address: %s\n", inet_ntoa(*addr_list[0]));
		}
	}
	else
	{
		hostp = gethostbyaddr(&servaddr.sin_addr, sizeof(servaddr.sin_addr), AF_INET);
		if(hostp == (struct hostent *)NULL)
		{
			printf("IP not valid.\n");
			return NULL;
		}
		printf("Hostname is: %s\n", hostp->h_name);


	}
	return hostp;

}
