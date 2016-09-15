#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLINE 4096

int main(int argc, char **argv) {
   int    sockfd, n;
   char   recvline[MAXLINE + 1];
   char   error[MAXLINE + 1];
   struct sockaddr_in servaddr;

   //trate os argumentos
   if (argc != 2) {
      //usage
      strcpy(error,"uso: ");
      strcat(error,argv[0]);
      strcat(error," <IPaddress>");
      perror(error);
      exit(1);
   }

   //crie um socket para comunicacao, e aborte em caso de erro.
   if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      //Imprima o erro
      perror("socket error");
      //Aborte
      exit(1);
   }

   //parametros de socket
   bzero(&servaddr, sizeof(servaddr)); //inicialize com zeros
   servaddr.sin_family = AF_INET; //servidor de enderecos IPv4
   servaddr.sin_port   = htons(1300); //Porta 1300
   //converta o endereco IP de texto para binario. Reporte erros
   if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
      perror("inet_pton error");
      exit(1);
   }

   //conecte o socket com o endereco passado por argumento
   if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
      perror("connect error");
      exit(1);
   }

   struct sockaddr_in getsock;
   int addrlen = sizeof(struct sockaddr);
   //obtenha o endereco com o qual estamos comunicando
   if (getsockname(sockfd, (struct sockaddr*) &getsock, &addrlen) < 0) {
     perror("getsockname error:");
     exit(1);
   }
   //imprima o endereco no stdout
   printf("socket address: %s:%d\n",
     inet_ntoa(getsock.sin_addr), ntohs(getsock.sin_port));

   //leia MAXLINE bytes do socket
   while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
      recvline[n] = 0;
      //jogue a string em stdout
      if (fputs(recvline, stdout) == EOF) {
         perror("fputs error");
         exit(1);
      }
   }

   //reporte erros de read()
   if (n < 0) {
      perror("read error");
      exit(1);
   }

   exit(0);
}
