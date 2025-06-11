/**
* @file version2.c
* @brief Programme faisant avancer un serpent dans ls quatre directions.
* @author Le Chevère Yannis
* @version 2.0
* @date 09/11/2024
*
* Déplacement d'un serpent de taille N dans les quatre directions avec les touches 'z', 'q', 's', 'd'.
* La tête du serpent commencent la la position (20, 20) et la queu est a sa droite.
* Le serpent s'arrête quand la touche 'a' est pressée.
*
*/

/* Déclaration des fichiers inclus */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* Déclaration des constantes */
const char TETE = 'O';
const char CORPS = 'X';
const char VIDE = ' ';
const char HAUT = 'z';
const char BAS = 's';
const char DROITE = 'd';
const char GAUCHE = 'q';
const char ARRET_DU_SERPENT = 'a';
const int TAILLE_SERPENT = 10;
const int COORDONNEE_DEPART_X_SERPENT = 20;
const int COORDONNEE_DEPART_Y_SERPENT = 20;
const int LIMITE_ECRAN = 1; 
const int TEMPORISATION = 200000;

/* Déclaration des procédures et des fonctions (les prototypes) */
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction);
void gotoXY(int x, int y);
void disableEcho();
void enableEcho();
int kbhit();

/* Définition de la fonction main */
/**
 * @brief Fonction principale du jeu.
 * @details Initialise le serpent, prend en compte les touches tapêr par l'utilisateur pour la direction,
 * et boucle jusqu'à ce que l'utilisateur décide d'arrêter le jeu en appuyant sur la touche 'a'.
 * @return int Code de sortie du programme.
 */
int main()
{
    // Déclaration des variables
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT]; // Les tableaux X et Y
    int verifArret = 1;
    char direction = DROITE; // Direction initiale du serpent
    // Initialisation du tableau du serpent, 'O' a la position des coordonnées, et 'X' aux positions a gauche de la tete
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = COORDONNEE_DEPART_X_SERPENT - i;
        lesY[i] = COORDONNEE_DEPART_Y_SERPENT;
    }
    system("clear"); // Nettoie le terminal pour afficher que le jeu
    disableEcho();
    dessinerSerpent(lesX, lesY); // Dessine le serpent à l'initialisation
    // Boucle qui vérifie si la touche 'a' est tapée, si 'a' pas tapée alors continue a faire avancer le serpent
    while (verifArret == 1) {
        
        if (kbhit() == 1) // Vérifie si une touche est tapée, si kbhit renvoie 1 alors une touche est tapée
        {   
            char cara = getchar(); // Vérifie si la touche tapée est 'a', si oui, alors arrete la boucle et nettoie l'interface
            if (cara == ARRET_DU_SERPENT)
            {
                verifArret = 0;
                system("clear");
            }
            else if ((cara == HAUT) && (direction != BAS))
            {
                direction = HAUT;
            }
            else if ((cara == DROITE) && (direction != GAUCHE))
            {
                direction = DROITE;
            }
            else if ((cara == BAS) && (direction != HAUT))
            {
                direction = BAS;
            }
            else if ((cara == GAUCHE) && (direction != DROITE))
            {
                direction = GAUCHE;
            }
        }
        effacer(lesX[TAILLE_SERPENT - 1], lesY[TAILLE_SERPENT - 1]); // Efface le dernier element du tableau
        progresser(lesX, lesY, direction); // Calcule la prochaine position du serpent
        dessinerSerpent(lesX, lesY);
        usleep(TEMPORISATION);
    }
    enableEcho();
    gotoXY(0,0);
    return EXIT_SUCCESS;
}

void afficher(int x, int y, char c)
{
    if (x >= LIMITE_ECRAN && y >= LIMITE_ECRAN)
    {
        gotoXY(x, y);
        printf("%c", c);
    }
}

void effacer(int x, int y)
{
    gotoXY(x, y);
    printf("%c", VIDE);
}

void dessinerSerpent(int lesX[], int lesY[])
{
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        if (i == 0) // Dessiner la tete
        {
            afficher(lesX[i], lesY[i], TETE);
        }
        else if (lesX[i] > 0) // Dessiner le corps
        {
            afficher(lesX[i], lesY[i], CORPS);
        }
    }
}

void progresser(int lesX[], int lesY[], char direction)
{
    // Calcule la position des prochains caracteres
    for (int i = TAILLE_SERPENT - 1; i > 0; i--)
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }
    if (direction == HAUT)
    {
        lesY[0]--;
    }
    if (direction == DROITE)
    {
        lesX[0]++;
    }
    if (direction == BAS)
    {
        lesY[0]++;
    }
    if (direction == GAUCHE)
    {
        lesX[0]--;
    }
}

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