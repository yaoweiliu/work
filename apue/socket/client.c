#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>
#include <unistd.h>
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <semaphore.h>
  
#define MAXLINE 4096
static sem_t sem; 

void handler(int arg)
{
    int value;
    sem_post(&sem);
    printf("%s: receive sigterm,", __func__);
    sem_getvalue(&sem, &value);
    printf(" 0 value is %d\n", value);
}
  
int main(int argc, char** argv)  
{  
    int sockfd, n,rec_len;  
    char recvline[4096], sendline[4096];  
    char buf[MAXLINE];  
    struct sockaddr_in servaddr;
    int value;
  
    if( argc != 2){  
        printf("usage: ./client <ipaddress><default 192.168.8.1>\n");  
        exit(0);  
    } 

    sem_init(&sem, 0, 0);
    signal(SIGINT, handler);
  
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        exit(0);  
    }  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(8000);  
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){  
        printf("inet_pton error for %s\n",argv[1]);  
        exit(0);  
    }  

    sem_getvalue(&sem, &value);
    printf("%s: 0 value is %d\n", __func__, value);
  
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  
  
    printf("send msg to server: \n");  
    fgets(sendline, 4096, stdin);  
    if(send(sockfd, sendline, strlen(sendline), 0) < 0)  
    {  
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        exit(0);  
    }
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  
       perror("recv error");  
       exit(1);  
    }  
    buf[rec_len]  = '\0';  
    printf("Received : %s\n",buf);  
    close(sockfd);

    sem_getvalue(&sem, &value);
    printf("%s: 1 value is %d\n", __func__, value);
    sem_wait(&sem);
    sem_getvalue(&sem, &value);
     printf("%s: 2 value is %d\n", __func__, value);

    sem_destroy(&sem);
    
    exit(0);  
}

