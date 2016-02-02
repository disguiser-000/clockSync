#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPSTR "106.11.4.75"
#define PORT 80
#define BUFSIZE 1024

int main(int argc, char **argv)
{
        int sockfd, ret, i, h;
        struct sockaddr_in servaddr;
        char str1[4096], str2[4096], buf[BUFSIZE], *str, *strtempnum;
        socklen_t len;
        fd_set   t_set1;
        struct timeval  tv;

		char seps[]   = "\n";
		char *token1, *token2, *token3, token4[10], *token5;
	    long int timestamp;
		time_t newtime;
		char szBuff[30];
		char token6[30] = "date -s '";

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
                printf("socket error!\n");
                exit(0);
        };
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
                printf("inet_pton error!\n");
                exit(0);
        };
 
        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
                printf("connect error!\n");
                exit(0);
        }
        printf("Connect Success!\n");
 
        //send data
		memset(str2, 0, 4096);
        str = (char *)malloc(128);
        len = strlen(str2);
        sprintf(str, "%d", len);
 
        memset(str1, 0, 4096);
        strcat(str1, "GET /rest/sur HTTP/1.1\n");
        strcat(str1, "Host: adash.m.taobao.com\n");
        strcat(str1, "Content-Type: application/x-www-form-urlencoded\n");
        strcat(str1, "Content-Length: ");
        strcat(str1, str);
        strcat(str1, "\n\n");
 
        strcat(str1, str2);
        strcat(str1, "\r\n\r\n");
 
        ret = write(sockfd,str1,strlen(str1));

		if (ret < 0) {
			printf("Send failure! Code: %d msg: '%s'\n", errno, strerror(errno));
			exit(0);
        } else {
			printf("Send Success, total %d Byte.\n", ret);
        }
 
        FD_ZERO(&t_set1);
        FD_SET(sockfd, &t_set1);
 
        while(1){
                sleep(1);
                tv.tv_sec = 0;
                tv.tv_usec = 0;
                h = 0;
                h = select(sockfd +1, &t_set1, NULL, NULL, &tv);
 
                if (h <= 0) {
					close(sockfd);
//					printf("App Stop!\n");
					return -1;
                };
 
                if (h > 0) {
                        memset(buf, 0, 4096);
                        i = read(sockfd, buf, 4095);
                        if (i==0) {
							close(sockfd);
//							printf("Connect to Time server Failure.[Data Error]\n");
							return -1;
                        }
						token1 = strtok( buf, seps );
						while(token1 !=NULL)  
						{
							if (strstr(token1, "success") != NULL)
							{
								token2 = strtok( token1, ":" );
								token2 = strtok(NULL, ":");
								if (strstr(token2, "ret") != NULL)
								{
									token3 = strtok( token2, "," );
									memcpy(token4, token3, strlen(token3)-3);
									newtime = strtol(token4, NULL, 10);
									strftime(szBuff, sizeof(szBuff), "%Y-%m-%d %X", localtime(&newtime));
									strcat(token6, szBuff);
									strcat(token6, "'");
									system(token6);
									printf("Connect to Time server Success.\n");
								}
							}
							token1 = strtok(NULL, seps);
						}
				}
        }
        close(sockfd);

 
        return 0;
}
