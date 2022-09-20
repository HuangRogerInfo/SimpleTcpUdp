//Client admin UDP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h> 


int main(){

    int sock;
     if( (sock = socket(AF_INET, SOCK_DGRAM, 0))== -1 ) {
        perror("Erreur de creation socket admin");
        exit(1);
    }

    struct sockaddr_in srv;
    socklen_t sl = sizeof(struct sockaddr_in);
    
    /* Attributs du serveur */
    srv.sin_family = AF_INET;
    srv.sin_port = htons(30000);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
   
    //Envoi
    printf("Envoi de la requete : je veux connaitre le nombre de clients\n");
    char* demande = "Je veux connaitre le nombre de clients";
    sendto(sock, demande, strlen(demande), 0, (struct sockaddr *) &srv, sl);
    
    //Reçu
    int nbClients;
    recvfrom(sock, &nbClients, sizeof(int), 0, NULL, NULL);
    printf("Reçu : %d\n", nbClients);
    close(sock);
    return 0;

}