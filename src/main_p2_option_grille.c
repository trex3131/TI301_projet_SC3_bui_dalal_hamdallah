/* main_p2_option_grille.c : partie 2, option — mettre à jour la grille pendant la pousse (cible CMake p2_option_grille)
   variante : la lumière est recalculée avant chaque arbre, au lieu d'une seule fois au début de la saison ;
   on compare les hauteurs des arbres 0 et K - 1 dans les deux versions */

#include <stdio.h>
#include "foret.h"

#define K_TEST 24
#define E_TEST 3
#define A_TEST 0.25
#define S_TEST 32
#define HMAX_TEST 160
#define PMAX_TEST 20000
#define NB_GRAINES 200

/***
 * @brief variante de growForestSeason : la lumière est recalculée avant chaque arbre, les arbres déjà poussés
 *        pendant la saison font donc de l'ombre à ceux qui poussent après eux
 * @param f : la forêt
 */
static void growForestSeasonVariant(t_foret *f)
{
    for (int i = 0; i < f->K; i++)
    {
        computeLight(f);
        growSeasonInForest(f, &f->arbres[i]);
    }

    f->saison++;
}

/***
 * @brief simule une forêt de test avec l'une des deux versions, et range les hauteurs et cimes des arbres 0 et K - 1
 *        ainsi que leurs moyennes sur tous les arbres
 * @param graine : graine de la forêt
 * @param variante : 0 pour la version normale (une lumière par saison), 1 pour la variante
 * @param h : reçoit hauteur arbre 0, hauteur arbre K - 1, hauteur moyenne
 * @param c : reçoit cime arbre 0, cime arbre K - 1, cime moyenne
 */
static void simulate(int graine, int variante, double h[3], double c[3])
{
    t_foret f = createForest(K_TEST, E_TEST, HMAX_TEST, A_TEST, PMAX_TEST);

    plantForest(&f, graine, 1);     // plantation identique : aucun arbre ne part avec de l'avance
    for (int s = 0; s < S_TEST; s++)
    {
        if (variante)
        {
            growForestSeasonVariant(&f);
        }
        else
        {
            growForestSeason(&f);
        }
    }

    h[0] = f.arbres[0].hauteur;
    h[1] = f.arbres[f.K - 1].hauteur;
    c[0] = f.arbres[0].cime;
    c[1] = f.arbres[f.K - 1].cime;
    h[2] = 0;
    c[2] = 0;
    for (int i = 0; i < f.K; i++)
    {
        h[2] += f.arbres[i].hauteur;
        c[2] += f.arbres[i].cime;
    }
    h[2] /= f.K;
    c[2] /= f.K;

    freeForest(&f);
}

int main(void)
{
    double h[3], c[3];
    double somme_h[2][3] = {{0}}, somme_c[2][3] = {{0}};
    int gagne0[2] = {0, 0};     // nombre de forêts où l'arbre 0 est strictement plus haut que l'arbre K - 1

    printf("K = %d, E = %d, A = %.2f, %d saisons, plantation identique, graines 1 a %d\n\n",
           K_TEST, E_TEST, A_TEST, S_TEST, NB_GRAINES);

    for (int v = 0; v < 2; v++)
    {
        simulate(1, v, h, c);
        printf("%s, graine 1 : arbre 0 hauteur %2.0f cime %6.3f ; arbre %d hauteur %2.0f cime %6.3f\n",
               v ? "variante" : "normale ", h[0], c[0], K_TEST - 1, h[1], c[1]);
    }
    printf("\n");

    for (int g = 1; g <= NB_GRAINES; g++)
    {
        for (int v = 0; v < 2; v++)
        {
            simulate(g, v, h, c);
            for (int k = 0; k < 3; k++)
            {
                somme_h[v][k] += h[k];
                somme_c[v][k] += c[k];
            }
            if (h[0] > h[1])
            {
                gagne0[v]++;
            }
        }
    }

    printf("moyennes sur %d forets         arbre 0          arbre %d         tous les arbres\n", NB_GRAINES, K_TEST - 1);
    for (int v = 0; v < 2; v++)
    {
        printf("%s  hauteur        %8.2f         %8.2f         %8.2f\n", v ? "variante" : "normale ",
               somme_h[v][0] / NB_GRAINES, somme_h[v][1] / NB_GRAINES, somme_h[v][2] / NB_GRAINES);
        printf("          cime           %8.3f         %8.3f         %8.3f\n",
               somme_c[v][0] / NB_GRAINES, somme_c[v][1] / NB_GRAINES, somme_c[v][2] / NB_GRAINES);
        printf("          arbre 0 plus haut que l'arbre %d dans %d forets sur %d\n",
               K_TEST - 1, gagne0[v], NB_GRAINES);
    }

    return 0;
}
