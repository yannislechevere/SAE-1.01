# 🐍 Jeu du Snake en C (Terminal)

Ce projet est une implémentation du célèbre jeu **Snake**, jouable dans le **terminal**, et développé en **langage C**.  
Il est divisé en **quatre versions**, chacune ajoutant de nouvelles fonctionnalités au jeu.

La **dernière version** est également accompagnée d'une **documentation générée avec Doxygen**.

---

## 🔧 Compilation

Pour compiler une version du jeu :

# Détails des Versions

---

## **Version 1 – Déplacement automatique**
<details> 
  <summary>Détails</summary>

> Le serpent se déplace automatiquement vers la droite.  
> Aucun contrôle clavier.  
> Le jeu se termine si l'utilisateur tape la touche "A".

🔗 [Dossier Version1](https://github.com/yannislechevere/SAE-1.01/tree/master/Version1)

</details>


## **Version 2 – Contrôle clavier ZQSD**
<details> 
  <summary>📋 Détails</summary>

> Le joueur contrôle le serpent avec les touches :
> - Z : Haut
> - Q : Gauche
> - S : Bas
> - D : Droite
> Le serpent continue dans la direction choisie.

🔗 [Dossier Version1](https://github.com/yannislechevere/SAE-1.01/tree/master/Version2)

</details>
🧱 Version 3 – Bordures et obstacles
<details> <summary>📋 Détails</summary>
Apparition de bordures fixes.

Introduction de pavés (obstacles).

Le serpent meurt en touchant un mur ou un pavé.

📄 Fichier : version3/snake_v3.c
📘 Consignes : version3/consignes.pdf

</details>
🍎 Version 4 – Pommes, téléportation, victoire
<details> <summary>📋 Détails</summary>
Des pommes apparaissent aléatoirement.

Le serpent grandit en mangeant une pomme.

Après 10 pommes, le joueur gagne.

Trous dans les murs : ils permettent la téléportation vers le bord opposé.

📄 Fichier : version4/snake_v4.c
📘 Consignes : version4/consignes.pdf

📚 Documentation Doxygen disponible dans le dossier Doxygen/html/index.html.

</details>
📄 Documentation Doxygen
La documentation de la version 4 peut être générée avec Doxygen.

✅ Génération
Depuis la racine du projet :

bash
Copier
Modifier
doxygen Doxyfile
Le résultat se trouve dans le dossier :
Doxygen/html/index.html

🧠 Idées futures
Interface menu

Ajout de scores

Mode 2 joueurs

Amélioration visuelle avec ncurses

👤 Auteur
Projet pédagogique réalisé pour apprendre la programmation en C via un jeu classique.
Contributions bienvenues !

