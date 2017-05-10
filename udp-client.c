/*
  Simple udp client
*/
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFLEN 1024  //Max length of buffer
 
void die(char *s)
{
  perror(s);
  exit(1);
}

int main(int argc, char **argv)
{
  if(argc != 3){
    printf("Usage: %s <host> <port>\n", argv[0]);
    return 1;
  }

  char *host = argv[1];
  int port = atoi(argv[2]);

  struct sockaddr_in si_other;
  int s, i, msgLen, rcvLen, slen=sizeof(si_other);
  char buf[BUFLEN];
  char message[BUFLEN];
 
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    die("socket");
  }
  
  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(port);

  printf("Connecting to %s:%d via UDP\n", host, port);
  if (inet_aton(host , &si_other.sin_addr) == 0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }
 
  while(1)
    {
      printf("Press enter for mssql probe or enter message: ");
      memset(message,'\0', BUFLEN);
      fgets(message, BUFLEN , stdin);
      for(i=0; i< BUFLEN; i++){
	if(message[i] == '\n'){
	  message[i] = '\0';
	  break;
	}
      }
      message[BUFLEN-1] = 0;
      
      if((msgLen = strlen(message)) < 1) {
	// send mssql probe package
	message[0] = 2;
	message[1] = '\0';
	msgLen = 1;
      }

      printf("Sending message %s [%d]\n", message, msgLen);
      
      //send the message
      if (sendto(s, message, msgLen , 0 , (struct sockaddr *) &si_other, slen)==-1){
	die("sendto()");
      }
         
      //receive a reply and print it
      //clear the buffer by filling null, it might have previously received data
      memset(buf,'\0', BUFLEN);
      //try to receive some data, this is a blocking call
      if ((rcvLen = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1){
	die("recvfrom()");
      }

      printf("Got response from server '%s' [%d]\n", buf, rcvLen);
      for(i=0; i < BUFLEN && i < rcvLen; i++){
	printf("%c", buf[i]);
      }
      printf("\n");
    }
 
  close(s);
  return 0;
}
