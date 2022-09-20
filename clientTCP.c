//Client ordinaire TCP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <unistd.h> 
#include <arpa/inet.h> 


int main(){

    int sock;
    if( (sock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
        perror("Erreur de creation socket client");
        exit(1);
    }

    struct sockaddr_in srv;
    socklen_t sl = sizeof(struct sockaddr_in); // taille d'une addresse IPV4

    /* Attributs du serveur */
    srv.sin_family = AF_INET;
    srv.sin_port = htons(20000); 
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");

    if( connect(sock, (struct sockaddr*) &srv,sl) < 0 ){
        perror("Erreur de connect");
        exit(1);
    }
    
    struct in_addr *ipclient = malloc(sizeof(struct in_addr) );
    ipclient->s_addr = gethostid();

    char * phrase = "mon numero IP est ";
    char* monip = inet_ntoa( *ipclient ) ;
    char * ip2 = malloc( strlen(monip) + strlen(phrase) + 1);
    strcpy( ip2, phrase);
    strcat( ip2 , monip);
    

    int nb_octets = write(sock, ip2, strlen(ip2));
    if(nb_octets<0){
        perror("Erreur d'ecriture");
    }
    
    close(sock);
    return 0;
}