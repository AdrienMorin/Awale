Bonjour et Bienvenue dans le serveur de jeux Awale en ligne !

Le projet est divisé en 2 parties :
- Serveur de jeu
- CLient (représentant les joueurs)

Afin de lancer le server, rendez-vous dans le dossier src/Server puis lancez les commandes suivantes :
- make clean
- make
- ./server

Afin de lancer un client, rendez-vous dans le dossier src/Client puis lancez les commandes suivantes :
- make clean
- make
- ./client 127.0.0.1

Si vous souhaitez lancer un nouveau client, inutile de recompiler, il suffit de lancer la dernière commande.

Voici la liste des commandes que vous pouvez utiliser :

- login <pseudo> <motdepasse>
    -> pour vous login au serveur à l'aide d'un pseudo et mot de passe.
- register <username> <password>
    -> pour créer un compte assocé à un pseudo et mot de passe.
- list <ingame> or <available>
    -> pour lister les utilisateurs en fonction de leur status.
    list ingame renverra les utilisateurs actuellement en jeu, tandis que list available renverra les utilisateurs disponibles.
- chat <username> [message]
    -> permet d'envoyer un message à un utilisateur donné. exemple : chat adrien bonjour, comment vas-tu ?
- challenge <username>
    -> permet de challenger un utilisateur, c'est à dire, lui proposer de jouer contre vous.
- accept
    -> pour accepter un challenge et lancer une partie.
- refuse
    -> pour refuser un challenge.
- play <case>
    -> permet de jouer une case
- quit
    -> pour quitter une partie
- disconnect
    -> pour se déconnecter du serveur
- surrender
    -> pour terminer la partie en se déclarant perdant.