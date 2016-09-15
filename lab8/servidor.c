#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include "errorHandling.c"

#define LISTENQ 10
#define MAXDATASIZE 100
#define MAXLINE 4096

pid_t Fork() {
  int pid;
  if ((pid = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  } else return pid;
}

void remoteExec(int connfd, const char *addr) {
  ssize_t n;
  char recvline[MAXLINE + 1];

  n = Read(connfd, recvline, MAXLINE);
  while (n) {
    recvline[n] = 0;
    //ecoe o comando na tela
    printf("%s$ %s\n", addr,recvline);

    if (recvline[n - 1] == EOF) {
      recvline[n - 1] = 0;
      Write(connfd, recvline, MAXLINE);
      printf("Detected EOF\n");
      return;
    } else { Write(connfd, recvline, MAXLINE); }
    n = Read(connfd, recvline, MAXLINE);
  }
}

void gettime(char* timestr) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  strftime(timestr, 20, "%F %T", tm);
}

int main (int argc, char **argv) {
   int    listenfd, connfd;
   struct sockaddr_in servaddr;
   char error[MAXLINE + 1];

   //verifique o numero de argumentos
   if (argc != 2) {
     strcpy(error, "uso: ");
     strcat(error, argv[0]);
     strcat(error," <Porta>");
     perror(error);
     exit(EXIT_FAILURE);
   }

   // crie um socket para comunicação, e aborte em caso de erro, reportando o mesmo.
   listenfd = Socket(AF_INET, SOCK_STREAM, 0);

   //parametros de socket
   bzero(&servaddr, sizeof(servaddr)); //inicialize com zeros
   servaddr.sin_family      = AF_INET; //servidor de enderecos IPv4
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //servidor para todas as interfaces disponiveis
   servaddr.sin_port        = htons(atoi(argv[1]));   // Porta como argumento

   //associe o socket com o endereco, reportando erros.
   Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
   //marque o socket como um socket passivo ( para receber conexoes )
   Listen(listenfd, LISTENQ);
   printf("Listening on port %s...\n", argv[1]);
   for ( ; ; ) {
      struct sockaddr_in peer;
      int pid;
      socklen_t addrlen = sizeof(struct sockaddr);
      //aceite o primeiro pedido de conexao da fila de conexoes pendentes
      connfd = Accept(listenfd, (struct sockaddr *) &peer, &addrlen);
      if ((pid = Fork()) > 0) {
        //parent code
        //fecha a conexao
        close(connfd);
      } else if (pid == 0) {
        //child code
        struct sockaddr_in p = peer;
        char ipstr[INET6_ADDRSTRLEN + 6];
        char timestr[20];
        FILE *log = Fopen("log.txt", "a");
        //converta o endereco ip para string
        inet_ntop(AF_INET, &p.sin_addr, ipstr, sizeof(ipstr));
        //imprima o endereco no stdout
        sprintf(ipstr + strlen(ipstr), ":%d", ntohs(p.sin_port));
        gettime(timestr);
        printf("%s>%s connected.\n", timestr, ipstr);
        fprintf(log,"%s>%s connected.\n", timestr, ipstr);

        remoteExec(connfd, ipstr);
        gettime(timestr);
        printf("%s>%s disconnected.\n", timestr, ipstr);
        fprintf(log, "%s>%s disconnected.\n", timestr, ipstr);
        close(connfd);
        fclose(log);
        exit(EXIT_SUCCESS);
      }
   }
   //encerra o socket receptor de conexoes
   close(listenfd);
   return(0);
}
