#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Constantes
const int N = 10;
const int TIME = 200000;
const char TETE = 'O';
const char ARRET = 'a';
const char CORPS = 'X';
const char ESPACE = ' ';
const char COORDONNEE_X[92] = "Entrez la coordonnée de la position initiale de la tete du serpent en x (entre 1 et 40) : ";
const char COORDONNEE_Y[92] = "Entrez la coordonnée de la position initiale de la tete du serpent en y (entre 1 et 40) : ";
const char ERREUR_SAISIE_X[46] = "La coordonnée de x n'est pas entre 1 et 40.\n";
const char ERREUR_SAISIE_Y[46] = "La coordonnée de y n'est pas entre 1 et 40.\n";

// Prototypes des fonctions
void gotoXY(int x, int y);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[]);
int kbhit();

int main() {
    int x, y;
    int lesX[N], lesY[N];
    char touche;
    // Initialisation du serpent
    printf(COORDONNEE_X);
    scanf("%d", &x);
    while (x < 1 || x > 40) {
        printf(ERREUR_SAISIE_X);
        printf(COORDONNEE_X);
        scanf("%d", &x);
    }
    printf(COORDONNEE_Y);
    scanf("%d", &y);
    while (y < 1 || y > 40) {
        printf(ERREUR_SAISIE_Y);
        printf(COORDONNEE_Y);
        scanf("%d", &y);
    }
    // Position initiale du serpent
    for(int i = 0; i < N; i++) {
        lesX[i] = x - i;
        lesY[i] = y;
    }
    system("clear");
    // Boucle principale
    do {
        effacer(lesX[N-1], lesY[N-1]);  // Effacer l'ancienne position de la queue
        progresser(lesX, lesY);     // Calculer la nouvelle position du serpent
        dessinerSerpent(lesX, lesY);    // Afficher le serpent
        // Attendre un peu avant de continuer
        usleep(TIME);
        // Vérifier si une touche a été pressée
        if (kbhit()) {
            touche = getchar();
        }
    } while (touche != ARRET);
    gotoXY(0, 0);
    return EXIT_SUCCESS;
}

void gotoXY(int x, int y) {
    printf("\033[%d;%df", y, x);
}

void afficher(int x, int y, char c) {
    gotoXY(x, y);
    printf("%c", c);
}

void effacer(int x, int y) {
    afficher(x, y, ESPACE);
}

void dessinerSerpent(int lesX[], int lesY[]) {
    for(int i = 0; i < N; i++) {
        if (i == 0) {
            afficher(lesX[i], lesY[i], TETE);
        }
        else {
            afficher(lesX[i], lesY[i], CORPS);
        }
    }
}

void progresser(int lesX[], int lesY[]) {
    // Décaler les coordonnées vers la droite
    for(int i = N-1; i > 0; i--) {
        lesX[i] = lesX[i-1];
        lesY[i] = lesY[i-1];
    }
    lesX[0]++;  // La tête avance d'une colonne vers la droite
}

int kbhit() {
    // La fonction retourne :
    // 1 si un caractère est présent
    // 0 si pas de caractère présent
    int unCaractere = 0;
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Mettre le terminal en mode non bloquant
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restaurer le mode du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        unCaractere = 1;
    }

    return unCaractere;
}