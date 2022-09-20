Pour compiler :
gcc -Wall clientadminUDP.c -o clientAdmin
gcc -Wall clientTCP.c -o client
gcc -Wall serveur.c -o serveur -lpthread

Pour exécuter :
Lancer le serveur depuis un terminal : ./serveur
Puis simuler les connexions client : ./client ou ./clientAdmin
