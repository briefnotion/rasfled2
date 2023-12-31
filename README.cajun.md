# README - rasfled2

Language: [English](https://github.com/briefnotion/rasfled2/blob/main/README.md), [Simple](https://github.com/briefnotion/rasfled2/blob/main/README.simple.md), [CyberPunk](https://github.com/briefnotion/rasfled2/blob/main/README.cyberpunk.md), [Technical](https://github.com/briefnotion/rasfled2/blob/main/README.technical.md), [Corporate](https://github.com/briefnotion/rasfled2/blob/main/README.corporate%20language.md), [Jive](https://github.com/briefnotion/rasfled2/blob/main/README.jive.md), [Cajun](https://github.com/briefnotion/rasfled2/blob/main/README.cajun.md)

All Screenshots: [Demo Images](https://github.com/briefnotion/rasfled2/tree/main/demo_images)

# RasFLED: 
Lumière et Contrôle Multicolores en Terre de Raspberry Pi

Écoutez ça, mes amis, RasFLED arrive en fanfare, apportant avec lui le frisson de la lumière multicolore à la puissance du Raspberry Pi. L'ordinateur, souple et adaptable à souhait, peut aisément gérer une variété de modules de contrôle. Les modules individuels, dans cette version actuelle (2.021_230814), sont conçus pour fonctionner sur un Raspberry Pi 3b et comportent fièrement 4 modules distincts.

### Module 1 - Lumières Éblouissantes et Commutateurs Merveilleux :
Immergez-vous dans le royaume éblouissant du Module 1, où les lumières scintillent et les commutateurs dansent. Tout est pensé pour les voitures, avec chaque commutateur lié aux portes ouvertes ou fermées. Ouvrez une porte, et la féérie lumineuse se déploie, illuminant les environs, accompagnée d'avertissements. Refermez la porte, et l'obscurité retrouve sa place.

### Module 2 - Interface Futuriste :
Pénétrez l'univers futuriste du Module 2, une interface cybernétique qui transcende le réel. Même dans l'obscurité, cette interface vous offre un contrôle absolu. Utilisez le sort -tty pour lancer le système sans tête, et plongez-vous dans une expérience visuelle hors du commun. Écrit dans la langue des génies de l'OpenGL, cette interface vous ouvre les portes vers les autres modules, tout en vous donnant le pouvoir de contrôler les fonctions de base telles qu'une minuterie d'alerte, un mode jour-nuit, les feux de détresse, des changements de couleurs ensorcelants, et bien plus encore.

### Module 3 - L'Automobile à la Croisée des Chemins :
Lancez-vous dans le Module 3, où le mariage de la technologie et de l'automobile prend vie. En activant l'onglet "Automobile" et en équipant votre fidèle destrier d'un lecteur de bus CAN connecté à l'interface OBD2, une multitude d'informations jaillira de façon passive et s'étalera devant vos yeux ébahis. Vitesse, accélération, paramètres du régulateur de vitesse, tachymètre, sélection de rapport de transmission, températures diverses, pressions variées, tension de la batterie... Un festin d'informations ! Vous pouvez également laisser le module automobile diriger les feux lumineux en synchronisation avec la vitesse et la sélection de rapport. Il envoie également ses agents aux commutateurs de porte, interagissant avec le système de la voiture. Si la communication avec le bus CAN venait à s'interrompre, le système reprendrait le contrôle des commutateurs du Module 1 par précaution.

### Module 4 - Réception Céleste : Le Ciel est à Vous !
Embarquez pour le Module 4 et entrez dans une dimension céleste grâce à votre propre station de base réceptrice ADS-B mobile. C'est avec raffinement que j'ai créé une antenne pour ce récepteur radio RT que j'ai installée sur le toit de ma voiture, faisant ainsi de moi le capitaine d'une station mobile de réception ADS-B. Une version authentique de Flight Aware fait son travail, tandis que l'interface surveille passivement les journaux pour afficher son statut sur l'onglet ADSB. L'interface est simple, avec des fonctionnalités élémentaires pour démarrer et arrêter le système Flight Aware, ainsi que la possibilité de prendre des instantanés de l'état actuel. C'est un plaisir que de jongler avec le code nécessaire pour afficher ces informations ADS-B sur l'écran, tandis que les traces lumineuses des avions jusqu'à 100 miles de distance s'animent telle une mélodie visuelle, comblant les heures interminables de route.

***

Ceci marque la seconde incarnation de Rasfled, la première étant également accessible sur https://github.com/briefnotion/RasFLED. À l'heure où je couche ces mots sur le papier, la première version reste tout aussi compétente et fonctionnelle, à ceci près qu'elle s'affiche dans une fenêtre de console texte plutôt que dans l'éclatant écrin de l'OpenGL. Si Rasfled1 parvient à mener sa mission avec une charge CPU réduite de 20% et à fonctionner à distance via une interface SSH, il se heurte malheureusement à des graphiques en format texte et à un agaçant gel du système à chaque clic de souris, prisonnier d'un interrupteur de souris sur l'écran terminal. Rasfled2, de son côté, se présente sous un jour plus raffiné, son interface graphique personnalisable et sa maîtrise de plus de 10 couleurs avec un contrôle pixel par pixel étant le fruit d'une réécriture inspirée.

J'avoue, je m'éloigne du sujet initial de ce bref récit. J'espère cependant avoir su capturer l'essence de Rasfled2, ce programme conçu pour éclairer le tableau de bord et offrir un spectacle lumineux féérique au gré des événements.
