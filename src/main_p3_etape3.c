/* main_p3_etape3.c : partie 3, étape 3 — l'outil d'analyse, réglé par la ligne de commande (cible CMake p3_etape_3)
   usage : p3_etape_3 fichier colonne [nombre_de_classes [fichier_svg]]
   exemple : p3_etape_3 output/C1_arbres.csv cime 24 output/C1_cime.svg */

#include <stdio.h>
#include <stdlib.h>

#include "statistiques.h"

#define NB_CLASSES_DEFAUT 20
#define SVG_DEFAUT "output/histogramme.svg"

int main(int argc, char *argv[])
{
    const char *nom_fichier;
    const char *colonne;
    const char *nom_svg = SVG_DEFAUT;
    int nb_classes = NB_CLASSES_DEFAUT;
    int n;
    double min, largeur;
    double *v;
    int *effectifs;

    // sans fichier ou sans colonne : le mode d'emploi
    if (argc < 3 || argc > 5)
    {
        printf("usage : p3_etape_3 fichier colonne [nombre_de_classes [fichier_svg]]\n");
        printf("  nombre de classes par defaut : %d ; fichier SVG par defaut : %s\n", NB_CLASSES_DEFAUT, SVG_DEFAUT);
        printf("exemple : p3_etape_3 output/C1_arbres.csv cime 24 output/C1_cime.svg\n");
        return 1;
    }

    nom_fichier = argv[1];
    colonne = argv[2];
    if (argc >= 4)
    {
        nb_classes = atoi(argv[3]);
        if (nb_classes <= 0)
        {
            printf("Nombre de classes invalide : %s\n", argv[3]);
            return 1;
        }
    }
    if (argc == 5)
    {
        nom_svg = argv[4];
    }

    // readColumn affiche elle-même un message si le fichier ou la colonne n'existe pas
    v = readColumn(nom_fichier, colonne, &n);
    if (v == NULL)
    {
        return 1;
    }
    if (n == 0)
    {
        printf("La colonne %s de %s ne contient aucune valeur\n", colonne, nom_fichier);
        free(v);
        return 1;
    }

    printf("--- %s, colonne %s, %d classes\n", nom_fichier, colonne, nb_classes);
    displaySummary(v, n);

    effectifs = computeHistogram(v, n, nb_classes, &min, &largeur);
    printf("minimum %.4f, largeur d'une classe %.4f\n", min, largeur);
    displayHistogram(effectifs, nb_classes, min, largeur);

    histoToSVG(effectifs, nb_classes, min, largeur, colonne, nom_svg);
    printf("dessin : %s\n", nom_svg);

    free(effectifs);
    free(v);

    return 0;
}
