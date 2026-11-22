/* main_p3_etape1.c : partie 3, étape 1 — le programme de campagne, réglé par la ligne de commande (cible CMake p3_etape_1)
   exemple : p3_etape_1 --K 50 --E 3 --A 0.25 --S 120 --R 40 --graine 101 --sortie output/C1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "campagne.h"

int main(int argc, char *argv[])
{
    t_campagne c = createCampaign();    // valeurs par défaut
    int ok = 1;

    for (int i = 1; i < argc && ok; i++)
    {
        // --identique est le seul paramètre sans valeur
        if (strcmp(argv[i], "--identique") == 0)
        {
            c.identique = 1;
        }
        else if (strcmp(argv[i], "--K") != 0 && strcmp(argv[i], "--E") != 0 && strcmp(argv[i], "--A") != 0
                 && strcmp(argv[i], "--S") != 0 && strcmp(argv[i], "--R") != 0
                 && strcmp(argv[i], "--graine") != 0 && strcmp(argv[i], "--sortie") != 0)
        {
            printf("Parametre inconnu : %s\n", argv[i]);
            ok = 0;
        }
        else if (i + 1 >= argc)
        {
            printf("Valeur manquante pour le parametre %s\n", argv[i]);
            ok = 0;
        }
        else
        {
            const char *nom = argv[i];
            const char *valeur = argv[++i];

            if (strcmp(nom, "--K") == 0)
            {
                c.K = atoi(valeur);         // conversion texte -> entier
            }
            else if (strcmp(nom, "--E") == 0)
            {
                c.E = atoi(valeur);
            }
            else if (strcmp(nom, "--A") == 0)
            {
                c.A = atof(valeur);         // conversion texte -> réel
            }
            else if (strcmp(nom, "--S") == 0)
            {
                c.S = atoi(valeur);
            }
            else if (strcmp(nom, "--R") == 0)
            {
                c.R = atoi(valeur);
            }
            else if (strcmp(nom, "--graine") == 0)
            {
                c.graine = atoi(valeur);
            }
            else    // --sortie
            {
                if (strlen(valeur) >= LONGUEUR_NOM)
                {
                    printf("Nom de sortie trop long (au plus %d caracteres)\n", LONGUEUR_NOM - 1);
                    ok = 0;
                }
                else
                {
                    strcpy(c.sortie, valeur);
                }
            }
        }
    }

    // des réglages absurdes feraient planter la simulation (tableaux de taille nulle, A hors de [0 ; 1])
    if (ok && (c.K <= 0 || c.E <= 0 || c.S < 0 || c.R <= 0 || c.A < 0 || c.A > 1))
    {
        printf("Reglages invalides : K, E et R doivent etre positifs, S au moins 0, et A entre 0 et 1\n");
        ok = 0;
    }

    if (!ok)
    {
        printf("usage : p3_etape_1 [--K n] [--E n] [--A a] [--S n] [--R n] [--graine g] [--sortie nom] [--identique]\n");
        return 1;
    }

    return runCampaign(c) ? 0 : 1;
}
