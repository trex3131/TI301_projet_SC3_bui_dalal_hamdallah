/* campagne.c : partie 3, étape 1 — une campagne de simulations : R forêts, mesures écrites dans deux fichiers CSV */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "campagne.h"
#include "foret.h"

/***
 * @brief écrit la ligne de commentaire qui rappelle tous les réglages de la campagne
 * @param f : fichier ouvert en écriture
 * @param c : la campagne
 */
static void writeSettings(FILE *f, t_campagne c)
{
    fprintf(f, "# K=%d E=%d A=%.3f S=%d R=%d graine=%d identique=%d HMAX=%d P_max=%d\n",
            c.K, c.E, c.A, c.S, c.R, c.graine, c.identique, H_MAX, P_MAX);
}

t_campagne createCampaign(void)
{
    t_campagne c;

    c.K = 50;
    c.E = 3;
    c.A = 0.25;
    c.S = 120;
    c.R = 1;
    c.graine = 1;
    c.identique = 0;
    strcpy(c.sortie, "output/campagne");

    return c;
}

int runCampaign(t_campagne c)
{
    // place pour la sortie et le suffixe "_arbres.csv"
    char nom_arbres[LONGUEUR_NOM + 16];
    char nom_forets[LONGUEUR_NOM + 16];
    FILE *f_arbres;
    FILE *f_forets;
    clock_t debut = clock();

    sprintf(nom_arbres, "%s_arbres.csv", c.sortie);
    sprintf(nom_forets, "%s_forets.csv", c.sortie);

    f_arbres = fopen(nom_arbres, "wt");
    if (f_arbres == NULL)
    {
        perror(nom_arbres);
        return 0;
    }
    f_forets = fopen(nom_forets, "wt");
    if (f_forets == NULL)
    {
        perror(nom_forets);
        fclose(f_arbres);
        return 0;
    }

    // commentaire et en-tête des deux fichiers
    writeSettings(f_arbres, c);
    fprintf(f_arbres, "foret;arbre;rameaux;hauteur;cime;extension;tronque\n");
    writeSettings(f_forets, c);
    fprintf(f_forets, "foret;rameaux;cime_moy;extension_moy\n");

    for (int r = 0; r < c.R; r++)
    {
        // une seule forêt en mémoire à la fois
        t_foret f = createForest(c.K, c.E, H_MAX, c.A, P_MAX);
        int total_rameaux = 0;
        double somme_cimes = 0;
        double somme_extensions = 0;

        plantForest(&f, c.graine + r, c.identique);
        simulateForest(&f, c.S);

        for (int i = 0; i < f.K; i++)
        {
            t_arbre_foret *a = &f.arbres[i];
            int rameaux = a->step.arbre.nb_rameaux;
            float extension = a->xmax - a->xmin;

            fprintf(f_arbres, "%d;%d;%d;%d;%.3f;%.3f;%d\n",
                    r, i, rameaux, a->hauteur, a->cime, extension, a->tronque);

            total_rameaux += rameaux;
            somme_cimes += a->cime;
            somme_extensions += extension;
        }

        fprintf(f_forets, "%d;%d;%.4f;%.4f\n",
                r, total_rameaux, somme_cimes / f.K, somme_extensions / f.K);
        printf("foret %d / %d (graine %d) : %d rameaux\n", r + 1, c.R, c.graine + r, total_rameaux);

        freeForest(&f);
    }

    fclose(f_arbres);
    fclose(f_forets);

    printf("duree du calcul : %.2f s\n", (double) (clock() - debut) / CLOCKS_PER_SEC);
    printf("resultats : %s et %s\n", nom_arbres, nom_forets);

    return 1;
}
