#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	/*for (int i = 0; i < argc; i++){
	    printf ("argv[%d] = %s\n", i, argv[i]);
	}
	*/
	if(argc != 3){
		printf("Invalid Arguments\n");
		printf("Usage: ip port\n");
		exit(0);
	}
	int ip = strtol(argv[1],NULL,10);
	int port = atoi(argv[2]);
	printf("ip: %i\n",ip);
	printf("port: %i\n",port);

	//server
	int sockfd;
	  struct sockaddr_in si_me, si_other;
	  char buffer[1024];
	  socklen_t addr_size;

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	memset(&si_me, '\0', sizeof(si_me));
	  si_me.sin_family = AF_INET;
	  si_me.sin_port = htons(port);
	  si_me.sin_addr.s_addr = inet_addr("83.77.93.110");

	  bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me));
	  addr_size = sizeof(si_other);

	  while(1){
	  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)& si_other, &addr_size);
	  printf("[+]Data Received: %s", buffer);
	  }



	return EXIT_SUCCESS;
}
