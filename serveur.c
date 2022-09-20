//Serveur
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXMSGSIZE 1024
int nbClients = 0;  //Variable Globale

/*Semaphore */
sem_t S1; // Acces a la variable nbClients

//gestion du client admin
void* f1 (void* arg){
    printf("thread client cree\n");
    
    int nb_octets;
    int sock;
    char message[MAXMSGSIZE];

    if( ( sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){
        perror("Erreur de creation socket 1");
        exit(1);
    }

	struct sockaddr_in srv, clt;
	socklen_t sl = sizeof(struct sockaddr_in);

	/* Attributs du serveur */
	srv.sin_family = AF_INET;
	srv.sin_port = htons(30000);
    inet_aton("127.0.0.1", &(srv.sin_addr));
	
    /* liaison socket - serveur */
    if( bind(sock, (struct sockaddr *) &srv, sl)  < 0  ){
        perror("Erreur de bind 1");
        exit(1);
    }

    fflush(stdout);

	while (1) {

        printf("Attente admin sur port 30000...\n") ;
        nb_octets = recvfrom(sock, message, MAXMSGSIZE, 0,(struct sockaddr *) &clt, &sl) ;
        printf("Admin accepte\n");

        message[nb_octets] ='\0';
        printf("Requete recue : %s\n", message);

        sem_wait(&S1) ; 
		sendto(sock, &nbClients , sizeof(int) , 0, (struct sockaddr *) &clt, sl);
        sem_post(&S1) ;
         
        printf("Reponse envoyee\n");

        fflush(stdout);
    }

    close(sock);
    pthread_exit(NULL);
}

//Fonction - prise en charge d'un client ordinaire
void* newClient(void* sock) {
    int* new_sock = (int*) sock;

    char message[MAXMSGSIZE];
    bzero(message, MAXMSGSIZE);

    int nb_octets = read(*new_sock, message, MAXMSGSIZE);
    printf("Le serveur a recu %d octets du client : %s\n", nb_octets, message);

    sem_wait(&S1) ; 
    nbClients--;
    sem_post(&S1) ; 

    close(*new_sock);
    pthread_exit(NULL);
}

//gestion du client ordinaire TCP
void* f2 (void* arg){

    printf("thread admin cree\n");

    int new_sock, sock;
    pthread_t t3;

    if( (sock = socket(AF_INET, SOCK_STREAM, 0))== -1  ){
        perror("Erreur de creation socket 2");
        exit(1);
    }
     
    struct sockaddr_in srv, clt;
    socklen_t sl = sizeof(struct sockaddr_in);
    
	/* Attributs du serveur */
	srv.sin_family = AF_INET;
	srv.sin_port = htons(20000);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* liaison socket - serveur */
    if( bind(sock, (struct sockaddr *) &srv, sl)  < 0  ){
        perror("Erreur de bind 2");
        exit(1);
    }

    if( listen(sock, 5) != 0){
        perror("Erreur de listen");
        exit(1);
    }

    while(1){

        printf("Attente client ordinaire sur le port 20000...\n");

        new_sock = accept(sock, (struct sockaddr *) &clt, &sl);
        if(new_sock < 0){
            perror("Erreur de accept");
            exit(1);
        }

        printf("Client Accepte \n");

        sem_wait(&S1) ; 
        nbClients++;
        sem_post(&S1) ; 

        pthread_create(&t3, NULL, newClient, &new_sock);

    }

    close(sock);
    pthread_exit(NULL);
}

int main(){
    sem_init(&S1, 0, 1); // 1 = la variable est accessible et 0 = la variable est en cours d'utilisation
    pthread_t t1, t2;

    if( pthread_create(&t1, NULL, f1, NULL) == -1){
        perror("pthread_create1");
        return EXIT_FAILURE;
    }

    if( pthread_create(&t2, NULL, f2, NULL) == -1){
        perror("pthread_create2");
        return EXIT_FAILURE;
    }
    while(1);
    return 0;
}
