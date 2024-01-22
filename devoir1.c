#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TAILLE_TABLE 100 // Taille de la table de hachage, ajustez selon vos besoins

// Définition des structures
typedef struct cellule {
    char cle[15];
    int valeur;
    struct cellule* suivant;
} Cellule;

typedef Cellule* Liste;

typedef struct table_hachage {
    int taille;
    Liste* alveoles;
} Table_hachage;

typedef Table_hachage* TableHachage;

// Déclaration des fonctions
int hachage(TableHachage table, char* cle);
TableHachage cree_table_hachage(int taille);
void insere(TableHachage table, char* cle);
int recherche(TableHachage table, char* cle);
void supprime(TableHachage table, char* cle);
void afficher(TableHachage table);
void afficherPlusFrequents(TableHachage table, int k);

int main() {
    TableHachage table = cree_table_hachage(TAILLE_TABLE);

    // Exemple de texte
    char texte[] = "Dans le jardin, les roses rouges fleurissent avec élégance. Les oiseaux chantent joyeusement parmi les branches verdoyantes. Un chat curieux se promène parmi les parterres de fleurs. Les roses, les oiseaux et le chat créent une atmosphère paisible et charmante. Le soleil brille et réchauffe doucement la scène.
"

    // Tokenization du texte
    char* token = strtok(texte, " ");
    while (token != NULL) {
        insere(table, token);
        token = strtok(NULL, " ");
    }

    // Affichage des mots avec leurs fréquences
    afficher(table);

    // Affichage du mot le plus fréquent
    // (vous pourriez stocker l'information lors de l'insertion pour optimiser cette opération)
    int maxFreq = 0;
    char motPlusFrequent[15];
    for (int i = 0; i < TAILLE_TABLE; i++) {
        Liste liste = table->alveoles[i];
        while (liste) {
            if (liste->valeur > maxFreq) {
                maxFreq = liste->valeur;
                strcpy(motPlusFrequent, liste->cle);
            }
            liste = liste->suivant;
        }
    }
    printf("\nLe mot le plus frequent est : %s\n", motPlusFrequent);

    // Affichage des k mots les plus fréquents
    int k;
    printf("\nEntrez le nombre de mots les plus frequents que vous souhaitez afficher : ");
    scanf("%d", &k);
    afficherPlusFrequents(table, k);

    // Libération de la mémoire
    // (vous pouvez ajouter une fonction de libération pour être plus propre)
    free(table->alveoles);
    free(table);

    return 0;
}

// Implémentation des fonctions
int hachage(TableHachage table, char* cle) {
    int codeh = 0;
    for (; *cle; cle++) {
        codeh += *cle;
    }
    return (codeh % table->taille);
}

TableHachage cree_table_hachage(int taille) {
    TableHachage table = (TableHachage)malloc(sizeof(Table_hachage));
    table->taille = taille;
    table->alveoles = (Liste*)malloc(table->taille * sizeof(Liste));
    for (int i = 0; i < table->taille; i++)
        table->alveoles[i] = NULL;
    return table;
}

void insere(TableHachage table, char* cle) {
    int codeh = hachage(table, cle);
    Liste liste = table->alveoles[codeh];

    while (liste) {
        if (!strcasecmp(liste->cle, cle)) {
            liste->valeur++;
            return;
        }
        liste = liste->suivant;
    }

    // Nouveau mot
    Liste nouvelleCellule = (Liste)malloc(sizeof(Cellule));
    strcpy(nouvelleCellule->cle, cle);
    nouvelleCellule->valeur = 1;
    nouvelleCellule->suivant = table->alveoles[codeh];
    table->alveoles[codeh] = nouvelleCellule;
}

int recherche(TableHachage table, char* cle) {
    int codeh = hachage(table, cle);
    Liste liste = table->alveoles[codeh];

    while (liste) {
        if (!strcasecmp(liste->cle, cle))
            return liste->valeur;
        liste = liste->suivant;
    }

    // Mot non trouvé
    return 0;
}

void supprime(TableHachage table, char* cle) {
    int codeh = hachage(table, cle);
    Liste liste = table->alveoles[codeh];
    Liste* precedent = &table->alveoles[codeh];

    while (liste) {
        if (strcmp(cle, liste->cle) == 0) {
            *precedent = liste->suivant;
            free(liste);
            return;
        }
        precedent = &liste->suivant;
        liste = liste->suivant;
    }
}

void afficher(TableHachage table) {
    printf("Mots avec leurs frequences :\n");
    for (int i = 0; i < table->taille; i++) {
        Liste liste = table->alveoles[i];
        while (liste) {
            printf("%s : %d\n", liste->cle, liste->valeur);
            liste = liste->suivant;
        }
    }
}

void afficherPlusFrequents(TableHachage table, int k) {
    printf("Les %d mots les plus frequents :\n", k);
    for (int i = 0; i < k; i++) {
        int maxFreq = 0;
        char mot[15];
        for (int j = 0; j < table->taille; j++) {
            Liste liste = table->alveoles[j];
            while (liste) {
                if (liste->valeur > maxFreq) {
                    maxFreq = liste->valeur;
                    strcpy(mot, liste->cle);
                }
                liste = liste->suivant;
            }
        }
        printf("%s : %d\n", mot, maxFreq);
        supprime(table, mot); // Supprime le mot le plus frequent pour ne pas le répéter
    }
}
