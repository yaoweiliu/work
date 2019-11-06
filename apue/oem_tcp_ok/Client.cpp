#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#include "event.h"

int main(int argc, char *argv[])
{
    int sockfd = -1;
    int ret;
    fd_set readfds;
    EVENT_HEADER header;

    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( -1 == sockfd ) {
        perror( "sock created" );
        exit( -1 );
    }
    
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    struct sockaddr_in server;    
    memset( &server, 0, sizeof( struct sockaddr_in ) );
    server.sin_family = AF_INET;
    server.sin_port = htons(7990);
    server.sin_addr.s_addr = inet_addr(BIND_IP);
    
    int count = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &count, sizeof(count));

    int res = -1;    
    res = connect(sockfd, (struct sockaddr*)&server, sizeof( server ) );
    if( -1 == res ){
        perror( "sock connect" );
        exit( -1 );
    }
	
	EV_TBL_CRADLE_START_MV_CMD_TYPE move_cmd;
	move_cmd.header.code = EV_TBL_CRADLE_START_MV_CMD;
	move_cmd.header.length = sizeof(move_cmd);
	move_cmd.header.sid = IOC;
	move_cmd.header.rid = COU;
	move_cmd.header.checksum = move_cmd.header.code + move_cmd.header.length + move_cmd.header.sid + move_cmd.header.rid;
	time_t now;
	time(&now); //get the system time
	move_cmd.header.sent = now;
	move_cmd.header.received = 0;
	move_cmd.targetPositionUm = 12300; //um
	move_cmd.targetSpeedUmPerSec = 100000; //um/s
	
    while(1) {
        write(sockfd, &move_cmd, sizeof(move_cmd));
        sleep(1);

        ret = select(sockfd+1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, &readfds)) {
            read(sockfd, &header, sizeof(header));
            printf("%s: recv code is %d\n", __func__, header.code);
        }
    }

    close( sockfd );

    return 0;
}


