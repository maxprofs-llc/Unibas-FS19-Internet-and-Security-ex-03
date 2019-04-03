#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) (((a) < (b)) ? (b) : (a))
#define SRV_IP "127.0.0.1"
#define PORT_ME 9931
#define PORT_OTHER 9932

void diep(char *s)
{
  perror(s);
  exit(1);
}

void do_stuff();
void send_chat(char *c);

int s, slen;
struct sockaddr_in si_other;

int main(int argc, char *argv[])
{
	
	if(argc != 4){
		printf("Invalid Arguments\n");
		printf("Usage: ip port_me port_other\n");
		exit(0);
	}
	char* ip = argv[1];
	int port_me = atoi(argv[2]);
	int port_other = atoi(argv[3]);

  struct sockaddr_in si_me;
  int i;

  slen = sizeof(si_other);
  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    diep("socket");

  memset((char *)&si_other, 0, sizeof(si_other));
  memset((char *)&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT_ME);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT_OTHER);
  if (inet_aton(SRV_IP, &si_other.sin_addr) == 0)
  {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
  if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1)
    diep("bind");

  do_stuff();
  close(s);
  return 0;
}

void do_stuff()
{
  while (1)
  {
    char buf[255];
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set); //stdin
    FD_SET(s, &set);
    int nfds = max(s, 0) + 1;
    int nr = select(nfds, &set, NULL, NULL, NULL);
    if (nr < 0)
    {
      diep("select() failed");
    }
    if (FD_ISSET(0, &set)) //New stdin
    {
      scanf("%s", buf);
      send_chat(buf);
    }
    if (FD_ISSET(s, &set))
    {
      if (recvfrom(s, buf, 255, 0, (struct sockaddr *)&si_other, &slen) == -1)
      {
        diep("recvfrom()");
      }
      printf("recv %s\n", buf);
    }
  }
}

void send_chat(char *c)
{
  if (sendto(s, c, strlen(c) + 1, 0, (struct sockaddr *)&si_other, slen) == -1)
  {
    diep("sendto()");
  }
}