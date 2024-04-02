#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void chat(int connfd)
{
  char buff[MAX];
  int n;

  // infinite loop for chat
  for (;;) {
    memset(&buff, 0, MAX);

    // read the message from client and copy it in buffer
    read(connfd, buff, sizeof(buff));
    // print buffer which contains the client contents
    printf("From client: %s\t To client : ", buff);
    bzero(buff, MAX);
    n = 0;
    // copy server message in the buffer
    while ((buff[n++] = getchar()) != '\n')
      ;

    // and send that buffer to client
    write(connfd, buff, sizeof(buff));

    // if msg contains "Exit" then server exit and chat ended.
    if (strncmp("exit", buff, 4) == 0) {
      printf("Server Exit...\n");
      break;
    }
  }
}

void connect_tcp() {
  int sockfd, client_sock, client_size;
  struct sockaddr_in servaddr, clientaddr;
  char server_message[2000], client_message[2000];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    printf("Creating the socket failed.\n");
    exit(0);
  } else {
    printf("Socket successfully created with result '%d'\n", sockfd);
  }

  // assign IP, PORT
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  int bind_result = bind(sockfd, ((SA*) &servaddr), sizeof(servaddr));
  if (bind_result == -1) {
    printf("Binding to socket failed.\n");
    exit(0);
  } else {
    printf("Bind succeeded with result '%d'\n", sockfd);
  }


  int listen_result = listen(sockfd, 0);
  if (listen_result == -1) {
    printf("Listening failed.\n");
    exit(0);
  } else {
    printf("Listening on port '%d...'\n", PORT);
  }

  int len = sizeof(clientaddr);
  int confd = accept(sockfd, ((SA*) &clientaddr), &len);
  if (confd == -1) {
    printf("Connection failed!\n");
    exit(0);
  } else {
    printf("Connection succeeded!\n");
  }

  chat(confd);

  close(sockfd);
}

int main() {
  connect_tcp();
  return 0;
}
