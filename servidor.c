#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 10
#define MAXDATASIZE 100

int main (int argc, char **argv) {
   int    listenfd, connfd;
   struct sockaddr_in servaddr;
   char   buf[MAXDATASIZE];
   time_t ticks;

   // crie um socket para comunicação, e aborte em caso de erro, reportando o mesmo.
   if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("socket");
      exit(1);
   }

   //parametros de socket
   bzero(&servaddr, sizeof(servaddr)); //inicialize com zeros
   servaddr.sin_family      = AF_INET; //servidor de enderecos IPv4
   servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //servidor para todas as interfaces disponiveis
   servaddr.sin_port        = htons(1300);   // Porta 1300

   //associe o socket com o endereco, reportando erros.
   if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
      perror("bind");
      exit(1);
   }
   
   //marque o socket como um socket passivo ( para receber conexoes )
   if (listen(listenfd, LISTENQ) == -1) {
      perror("listen");
      exit(1);
   }

   for ( ; ; ) {
      //aceite o primeiro pedido de conexao da fila de conexoes pendentes
      if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
         perror("accept");
         exit(1);
      }

      struct sockaddr_in getpeer;
      char ipstr[INET6_ADDRSTRLEN];
      int addrlen = sizeof(struct sockaddr);
      //obtenha o endereco do cliente
      if (getpeername(connfd, (struct sockaddr*) &getpeer, &addrlen) < 0) {
        perror("getpeername error:");
        exit(1);
      }

      inet_ntop(AF_INET, &getpeer.sin_addr, ipstr, sizeof(ipstr));
      //imprima o endereco no stdout
      printf("socket address: %s:%d\n",
        ipstr, ntohs(getpeer.sin_port));
      
      //receba o tempo do sistema
      ticks = time(NULL);
      //ponha na string buf
      snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
      //escreva no socket a string no buffer
      write(connfd, buf, strlen(buf));

      //fecha a conexao
      close(connfd);
   }
   return(0);
}
