/**
* @file version3.c
* @brief Programme de la version 3 du jeu Snake pour le projet SAE1.01.
* @author Le Chevère Yannis
* @version 3.0
* @date 17/11/2024
*
* Déplacement d'un serpent de taille N dans les quatre directions avec les touches 'z', 'q', 's', 'd'.
* La tête du serpent commencent la la position (40, 20) et la queu est a sa gauche.
* Le serpent s'arrête quand la touche 'a' est pressée ou quant il touche un mur ou un pavé ou se
* heurt lui même.
*
*/

/* Déclaration des fichiers inclus */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

/** Largeur maximale du plateau. */
#define LARGEUR_MAX 80
/**< Longueur maximale du plateau. */      
#define LONGUEUR_MAX 40       

/** @brief Déclaration des constantes */
const char TETE = 'O';              /**< Caractère qui représente la tête du serpent. */
const char CORPS = 'X';             /**< Caractère qui représente le corps du serpent. */
const char VIDE = ' ';              /**< Caractère qui représente une case vide. */
const char HAUT = 'z';              /**< Touche pour déplacer le serpent vers le haut. */
const char BAS = 's';               /**< Touche pour déplacer le serpent vers le bas. */
const char DROITE = 'd';            /**< Touche pour déplacer le serpent à droite. */
const char GAUCHE = 'q';            /**< Touche pour déplacer le serpent à gauche. */
const char ARRET_DU_JEU = 'a';      /**< Touche pour arrêter le jeu. */
const char CAR_BORDURE_PAVE = '#';  /**< Caractère utilisé pour afficher les bordures et pavés. */
const int NBRE_PAVES = 4;           /**< Nombre de pavés à placer sur le plateau. */
const int TAILLE_PAVE = 5;          /**< Taille des pavés (carrés). */
const int DISTANCE_PAVE_BORDURE = 2; /**< La distance minimale entre les pavés et la bordure */
const int TAILLE_SERPENT = 10;      /**< Taille du serpent. */
const int COORD_X_DEPART = 40;      /**< Position de départ en X du serpent. */
const int COORD_Y_DEPART = 20;      /**< Position de départ en Y du serpent. */
const int COORD_MIN = 1;            /**< Coordonnée minimale sur le plateau. */
const int TEMPORISATION = 200000;   /**< Temporisation entre les déplacements en microsecondes. */
const int MARGE_BORDURE = 1;        /**< Distance des bords du plateau */
const char PLATEAU[LARGEUR_MAX + 1][LONGUEUR_MAX + 1]; /**< Définition du tableau plateau */

/** @typedef Plateau_de_jeu
* @brief Définition du plateau de jeu comme une matrice de caractères.
*/
typedef char plateau_de_jeu[LARGEUR_MAX + 1][LONGUEUR_MAX + 1];

/* Déclaration des procédures et des fonctions (les prototypes) */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction, bool *colision, char PLATEAU[LARGEUR_MAX + 1][LONGUEUR_MAX + 1]);
void initPlateau(plateau_de_jeu tableau);
void initPaves(plateau_de_jeu tableau);
void dessinerPlateau(plateau_de_jeu tableau);
void gotoXY(int x, int y);
void disableEcho();
void enableEcho();
int kbhit(void);

/**
* @brief Fonction principal du jeu.
* Initialise le plateau de jeu, place le serpent et les pavés, puis lance la boucle principale.
* @return EXIT_SUCCESS en cas de succès.
*/
int main()
{
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    // Initialisation des variables
    plateau_de_jeu plateau;
    int x, y;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char touche = DROITE;
    char direction = DROITE;

    initPlateau(plateau);// Initialisation du plateau de jeu
    system("clear");
    disableEcho();
    bool collision = false; // Indicateur de collision
    x = COORD_X_DEPART; // Position de départ en X
    y = COORD_Y_DEPART; // Position de départ en Y
    // Initialisation des coordonnées du serpent (de la tête vers la queue)
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = x--; // Déplacement vers la gauche pour la queue
        lesY[i] = y;
    }
    dessinerPlateau(plateau);
    // Boucle principale du jeu
    do
    {
        // Vérification si une touche est appuyée
        if (kbhit())
        {
            touche = getchar();
        }
        // Mise à jour de la direction du serpent en fonction de la touche appuyée
        if (touche == HAUT && direction != BAS)
        {
            direction = HAUT; // Si la touche est 'z', le serpent va vers le haut
        }

        else if ((touche == BAS) && (direction != HAUT))
        {
            direction = BAS; // Si la touche est 's', le serpent va vers le bas
        }

        else if ((touche == GAUCHE) && (direction != DROITE))
        {
            direction = GAUCHE; // Si la touche est 'q', le serpent va vers la gauche
        }

        else if ((touche == DROITE) && (direction != GAUCHE))
        {
            direction = DROITE; // Si la touche est 'd', le serpent va vers la droite
        }
        // Le serpent se déplace et la collision est vérifiée
        progresser(lesX, lesY, direction, &collision, plateau);
        usleep(TEMPORISATION);

    } while ((touche != ARRET_DU_JEU) && (collision != true));
    enableEcho();
    system("clear");
    return EXIT_SUCCESS;
}

/**
* @brief Affiche un caractère à une position donnée sur le terminal.
* @param x Coordonnée en X.
* @param y Coordonnée en Y.
* @param c Caractère à afficher.
*/
void afficher(int x, int y, char c)
{
    // Vérifie que la position (x, y) est dans les limites du plateau
    if (((y >= COORD_MIN) && (y <= LONGUEUR_MAX + 1)) && ((x >= COORD_MIN) && (x <= LARGEUR_MAX + 1)))
    {
        gotoXY(x, y); // Déplace le curseur à la position (x, y)
        printf("%c", c); // Affiche le caractère à cette position
    }
}

/**
* @brief Efface un caractère à une position donnée sur le terminal.
* @param x Coordonnée en X.
* @param y Coordonnée en Y.
*/
void effacer(int x, int y)
{
    afficher(x, y, VIDE); // Affiche un espace vide pour effacer l'ancien caractère
}

/**
* @brief Dessine le serpent sur le plateau.
* @param lesX Tableau des coordonnées X du serpent.
* @param lesY Tableau des coordonnées Y du serpent.
*/
void dessinerSerpent(int lesX[], int lesY[]) {
    afficher(lesX[0], lesY[0], TETE);// Dessiner la tete
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        afficher(lesX[i], lesY[i], CORPS); // Dessiner le corps
    }
    fflush(stdout); // Permet l'affichage plus
}

/**
* @brief Fait avancer le serpent dans une direction donnée et vérifie les collisions.
* @param lesX Tableau des coordonnées X du serpent.
* @param lesY Tableau des coordonnées Y du serpent.
* @param direction Direction de déplacement.
* @param colision Pointeur vers un booléen indiquant si une collision a été détectée.
* @param tableau Plateau de jeu.
*/
void progresser(int lesX[], int lesY[], char direction, bool *colision, char PLATEAU[LARGEUR_MAX + 1][LONGUEUR_MAX + 1])
{
    // Efface la queue du serpent de l'affichage en utilisant les coordonnées de la dernière case
    effacer(lesX[TAILLE_SERPENT - 1], lesY[TAILLE_SERPENT - 1]); 

    // Déplace chaque case du serpent vers l'avant, en commençant par la fin
    for (int i = TAILLE_SERPENT - 1; i > 0; i--)
    {
        lesX[i] = lesX[i - 1]; // Déplace la position X du segment actuel vers celle du segment précédent
        lesY[i] = lesY[i - 1]; // Déplace la position Y du segment actuel vers celle du segment précédent
    }

    // Change la direction de la tête du serpent en fonction de la direction indiquée
    if (direction == DROITE)
    {
        lesX[0]++; // Avance vers la droite
    }
    else if (direction == GAUCHE)
    {
        lesX[0]--; // Avance vers la gauche
    }
    else if (direction == HAUT)
    {
        lesY[0]--; // Monte
    }
    else if (direction == BAS)
    {
        lesY[0]++; // Descend
    }
    // Vérifie si la tête du serpent entre en collision avec la bordure du plateau
    if (PLATEAU[lesX[0]][lesY[0]] == CAR_BORDURE_PAVE)
    {
        *colision = true; // Collision si la tête touche un mur
    }
    // Vérifie si la tête du serpent entre en collision avec son propre corps
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        if ((lesX[0] == lesX[i]) && (lesY[0] == lesY[i]))
        {
            *colision = true; // Collision si la tête touche un autre segment.
        }
    }
    // Redessine le serpent après mise à jour de ses positions.
    dessinerSerpent(lesX, lesY);
}

/**
* @brief Place un pavé aléatoirement sur le plateau. 
* @param tableau Plateau de jeu à modifier.
*/
void initPaves(plateau_de_jeu plateau)
{
    int x, y;
    do{
        // Générer des coordonnées aléatoires pour le pavé
        x = rand() % (LARGEUR_MAX - TAILLE_PAVE);
        y = rand() % (LONGUEUR_MAX - TAILLE_PAVE);
    } while ( (x <= DISTANCE_PAVE_BORDURE) || (y <= DISTANCE_PAVE_BORDURE) || // Vérifier que le pavé ne touche pas les bordures
    ( ( (x >= (COORD_X_DEPART + 1) - TAILLE_PAVE) && (x < (COORD_X_DEPART + 1) + TAILLE_SERPENT) ) &&
    ( (y >= (COORD_Y_DEPART + 1) - TAILLE_PAVE) && (y < COORD_X_DEPART + 1) ) ) ); // Éviter le placement sur le serpent
    // Placer le pavé
    for (int i = y; i < TAILLE_PAVE + y; i++)
    {
        for (int j = x; j < TAILLE_PAVE + x; j++)
        {
            plateau[j][i] = CAR_BORDURE_PAVE; // Utilise le caractère de bordure pour le pavé
        }
    }
}

/**
* @brief Initialise le plateau de jeu avec des bordures et des cases vides.
* @param tableau Plateau de jeu à initialiser.
*/
void initPlateau(plateau_de_jeu plateau)
{
    for (int lig = 0; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 0; col <= LONGUEUR_MAX; col++)
        {
            // Vérifie si la case actuelle est une bordure
            if (((lig == MARGE_BORDURE) || (lig == LARGEUR_MAX)) || ((col == MARGE_BORDURE) || (col == LONGUEUR_MAX)))
            {
                plateau[lig][col] = CAR_BORDURE_PAVE; // Si c'est une bordure, on place un caractère spécial représentant la bordure
            }
            else
            {   
                plateau[lig][col] = VIDE; // Si ce n'est pas une bordure, la case est considérée comme vide
            }
        }
    }
    // Initialise les pavés sur le plateau
    for (int i = 0; i < NBRE_PAVES; i++)
    {
        initPaves(plateau);
    }
}

/**
* @brief Affiche le plateau de jeu. 
* Cette fonction parcourt toutes les cases du plateau et les affiche à l'écran en utilisant
* la fonction afficher().
* @param plateau Plateau de jeu à afficher, un tableau bidimensionnel représentant le plateau.
*/
void dessinerPlateau(plateau_de_jeu plateau)
{
    for (int lig = 1; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 1; col <= LONGUEUR_MAX; col++) // On commence à 1 pour ignorer les bordures
        {
            afficher(lig, col, plateau[lig][col]); // Affiche le caractère contenu dans la case du plateau
        }
    }
}

/** 
* Fonctions et procédures donner  
*/

void gotoXY(int x, int y)
{
    printf("\033[%d;%df", y, x);
}

int kbhit()
{
    // la fonction retourne :
    // 1 si un caractere est present
    // 0 si pas de caractere present
    int unCaractere = 0;
    struct termios oldt, newt;
    int ch;
    int oldf;
    // mettre le terminal en mode non bloquant
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    // restaurer le mode du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if ((ch != EOF))
    {
        ungetc(ch, stdin);
        unCaractere = 1;
    }
    return unCaractere;
}

void disableEcho()
{
    struct termios tty;
    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;
    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho()
{
    struct termios tty;
    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;
    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}