/* main_p2_etape4.c : partie 2, tests de l'étape 4 (cible CMake p2_etape_4) — code fourni
   colonnes et rangées ; occupation de l'arbre exemple1.txt seul dans une forêt de 10 colonnes et 8 rangées */

#include <stdio.h>
#include <stdlib.h>
#include "foret.h"
#include "arbres_utils.h"
#include "arbre.h"

/***
 * @brief affiche la grille d'occupation, la rangée la plus haute en premier
 * @param f : la forêt
 */
static void displayOccupation(t_foret *f)
{
    for (int r = f->HMAX - 1; r >= 0; r--)
    {
        printf("  rangée %d :", r);
        for (int c = 0; c < f->C; c++)
        {
            printf(" %d", f->occupation[r][c]);
        }
        printf("\n");
    }

    return;
}

/***
 * @brief libère les tableaux d'une forêt dont les arbres n'ont pas été plantés (test seulement)
 * @param f : la forêt
 */
static void freeArrays(t_foret *f)
{
    free(f->arbres);
    for (int r = 0; r < f->HMAX; r++)
    {
        free(f->occupation[r]);
        free(f->lumiere[r]);
    }
    free(f->occupation);
    free(f->lumiere);

    return;
}

int main(void)
{
    t_foret f = createForest(3, 4, 160, 0.25, 20000);
    t_foret g;
    int somme = 0;

    // la forêt de l'exemple : 12 colonnes
    printf("columnOf 6.4 : %d, 11.99 : %d, 12.3 : %d, -0.5 : %d ; rowOf 2.7 : %d, 163.2 : %d\n",
           columnOf(&f, 6.4f), columnOf(&f, 11.99f), columnOf(&f, 12.3f), columnOf(&f, -0.5f), rowOf(&f, 2.7f), rowOf(&f, 163.2f));
    printf("attendu :      6           11           0           11            2              159\n");
    freeArrays(&f);

    // exemple1.txt dans une forêt de 1 arbre, 10 colonnes, 8 rangées (seul le champ step.arbre est rempli)
    g = createForest(1, 10, 8, 0.5, 20000);
    g.arbres[0].step.arbre = readTree("data/exemple1.txt");
    addTreeToGrid(&g, g.arbres[0].step.arbre.pied);
    displayOccupation(&g);
    for (int r = 0; r < g.HMAX; r++)
    {
        for (int c = 0; c < g.C; c++)
        {
            somme += g.occupation[r][c];
        }
    }
    printf("somme des compteurs : %d (attendu : 10) ; attendu, colonnes 3 4 5 : rangée 4 : 0 1 1 ; 3 : 0 1 0 ; 2 : 0 3 1 ; 1 : 2 1 0\n", somme);
    freeBranches(g.arbres[0].step.arbre.pied);
    freeArrays(&g);

    // À ÉCRIRE : deux ou trois autres cas construits à la main, dont un rameau d'altitude supérieure à HMAX
    //    (il est compté dans la dernière rangée)

    // cas 1 : un pied vertical en (2,5 ; 3) puis deux fils, l'un qui sort à gauche de la forêt, l'autre à droite
    //         forêt de 2 arbres, E = 3 (6 colonnes), 5 rangées
    {
        t_foret h = createForest(2, 3, 5, 0.25, 20000);
        t_arbre t = createTree(2.5f, 3, 1);

        t.pied->p_fils = malloc(2 * sizeof(t_rameau *));
        t.pied->nb_fils = 2;
        t.pied->p_fils[0] = growBranch(&t, t.pied, 1, -80, 3.0f);   // x = 2,5 - 3 sin(80°) = -0,454 : colonne 5
        t.pied->p_fils[1] = growBranch(&t, t.pied, 1, 80, 4.0f);    // x = 2,5 + 4 sin(80°) = 6,439 : colonne 0
        addTreeToGrid(&h, t.pied);
        printf("cas 1 : pied (3 ; 2) = %d, fils gauche (3 ; 5) = %d, fils droit (3 ; 0) = %d (attendu : 1 1 1)\n",
               h.occupation[3][2], h.occupation[3][5], h.occupation[3][0]);
        freeBranches(t.pied);
        freeArrays(&h);
    }

    // cas 2 : un rameau d'altitude 12,5 >= HMAX = 5, et un autre pile à l'altitude 5 : dernière rangée (4)
    {
        t_foret h = createForest(1, 4, 5, 0.25, 20000);
        t_arbre t = createTree(1.5f, 12.5f, 1);
        t_arbre u = createTree(1.2f, 5.0f, 1);
        int total = 0;

        addTreeToGrid(&h, t.pied);
        addTreeToGrid(&h, u.pied);
        for (int r = 0; r < h.HMAX; r++)
        {
            for (int c = 0; c < h.C; c++)
            {
                total += h.occupation[r][c];
            }
        }
        printf("cas 2 : case (4 ; 1) = %d, somme des compteurs = %d (attendu : 2 2)\n", h.occupation[4][1], total);
        freeBranches(t.pied);
        freeBranches(u.pied);
        freeArrays(&h);
    }

    // cas 3 : bornes des cases (6 colonnes) : x = 3 exactement est dans la colonne 3, y = 1 exactement dans la rangée 1 ;
    //         x = -6 (multiple de C) revient dans la colonne 0
    {
        t_foret h = createForest(2, 3, 4, 0.25, 20000);
        t_arbre t = createTree(3.0f, 1.0f, 1);
        t_arbre u = createTree(-6.0f, 0.5f, 1);

        addTreeToGrid(&h, t.pied);
        addTreeToGrid(&h, u.pied);
        printf("cas 3 : case (1 ; 3) = %d, case (0 ; 0) = %d (attendu : 1 1)\n", h.occupation[1][3], h.occupation[0][0]);
        freeBranches(t.pied);
        freeBranches(u.pied);
        freeArrays(&h);
    }

    return 0;
}
