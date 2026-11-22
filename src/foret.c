/* foret.c : partie 2, étapes 4 à 6 — la forêt, ses grilles d'occupation et de lumière, sa pousse (code fourni, à compléter) */

#include <stdlib.h>
#include <math.h>
#include "foret.h"
#include "croissance_lumiere.h"
#include "queue.h"
#include "alea.h"

/* ---------------------------------------------------------------- code fourni */

int columnOf(t_foret *f, float x)
{
    // en C, -1 % 12 vaut -1 : on ramène le reste entre 0 et C - 1
    int c = (int) floorf(x) % f->C;

    if (c < 0)
    {
        c += f->C;
    }

    return c;
}

int rowOf(t_foret *f, float y)
{
    int r = (int) floorf(y);

    if (r >= f->HMAX)
    {
        r = f->HMAX - 1;
    }
    else if (r < 0)
    {
        r = 0;      // ne se produit pas (un fils est toujours plus haut que son parent) : simple précaution
    }

    return r;
}

/* ---------------------------------------------------------------- à compléter : vos fonctions, à partir d'ici */

/* ---------------------------------------------------------------- étape 4 : la grille d'occupation */

t_foret createForest(int K, int E, int HMAX, double A, int P_max)
{
    t_foret f;

    f.K = K;
    f.E = E;
    f.C = K * E;
    f.HMAX = HMAX;
    f.A = A;
    f.P_max = P_max;
    f.saison = 0;

    f.arbres = malloc((size_t) K * sizeof(t_arbre_foret));
    f.occupation = malloc((size_t) HMAX * sizeof(int *));
    f.lumiere = malloc((size_t) HMAX * sizeof(double *));
    if (f.arbres == NULL || f.occupation == NULL || f.lumiere == NULL)
    {
        exit(EXIT_FAILURE);
    }

    for (int r = 0; r < HMAX; r++)
    {
        f.occupation[r] = calloc((size_t) f.C, sizeof(int));     // calloc : compteurs à 0
        f.lumiere[r] = malloc((size_t) f.C * sizeof(double));
        if (f.occupation[r] == NULL || f.lumiere[r] == NULL)
        {
            exit(EXIT_FAILURE);
        }
        for (int c = 0; c < f.C; c++)
        {
            f.lumiere[r][c] = 1.0;
        }
    }

    return f;
}

void addToGrid(t_foret *f, t_rameau *r)
{
    f->occupation[rowOf(f, r->y)][columnOf(f, r->x)]++;
}

void addTreeToGrid(t_foret *f, t_rameau *r)
{
    addToGrid(f, r);
    for (int i = 0; i < r->nb_fils; i++)
    {
        addTreeToGrid(f, r->p_fils[i]);
    }
}

/* ---------------------------------------------------------------- étape 5 : la lumière */

void computeLight(t_foret *f)
{
    for (int c = 0; c < f->C; c++)
    {
        double L = 1.0;

        // du haut vers le bas : les extrémités d'une case n'ombragent que les rangées en dessous
        for (int r = f->HMAX - 1; r >= 0; r--)
        {
            f->lumiere[r][c] = L;
            for (int k = 0; k < f->occupation[r][c]; k++)
            {
                L *= 1.0 - f->A;
            }
        }
    }
}

double getLight(t_foret *f, t_rameau *r)
{
    return f->lumiere[rowOf(f, r->y)][columnOf(f, r->x)];
}

/* ---------------------------------------------------------------- étape 6 : la forêt qui pousse */

void plantForest(t_foret *f, int seed, int identique)
{
    // 1. les K arbres, pied à l'altitude 1 (createTree réinitialise le hasard à chaque fois)
    for (int i = 0; i < f->K; i++)
    {
        f->arbres[i] = createForestTree(i, (float) (i * f->E) + (float) f->E / 2, 1, seed);
    }

    // 2. une seule initialisation pour toute la forêt
    randomInit((uint64_t) seed);

    // 3. altitude des pieds, de l'arbre 0 à l'arbre K - 1
    if (!identique)
    {
        for (int i = 0; i < f->K; i++)
        {
            t_rameau *pied = f->arbres[i].step.arbre.pied;
            float y = (float) randomBetween(0.5, 1.5);

            pied->y = y;
            pied->longueur = y;     // le pied part du sol
            f->arbres[i].cime = y;
        }
    }

    // 4. les pieds dans la grille, à leur altitude définitive
    for (int i = 0; i < f->K; i++)
    {
        addToGrid(f, f->arbres[i].step.arbre.pied);
    }

    f->saison = 0;
}

void growSeasonInForest(t_foret *f, t_arbre_foret *a)
{
    t_queue G;

    a->step.saison++;
    if (a->tronque)
    {
        return;     // un arbre tronqué ne pousse plus et ne fait aucun tirage
    }

    G = createEmptyQueue();
    while (!isEmptyQueue(a->step.bourgeons))
    {
        t_rameau *bud = dequeue(&a->step.bourgeons);
        double L = getLight(f, bud);

        if (canGrowInLight(L))
        {
            int n = howManyBranchesInLight(L);

            growBudInLight(a, bud, n, L);
            for (int i = 0; i < n; i++)
            {
                addToGrid(f, bud->p_fils[i]);
                enqueue(&G, bud->p_fils[i]);
            }
        }
        else
        {
            enqueue(&G, bud);
        }
    }
    a->step.bourgeons = G;

    if (a->step.arbre.nb_rameaux >= f->P_max)
    {
        a->tronque = 1;
    }
}

void growForestSeason(t_foret *f)
{
    // 1. éclairer : toutes les lumières avant le moindre bourgeon
    computeLight(f);

    // 2. pousser, de l'arbre 0 à l'arbre K - 1
    for (int i = 0; i < f->K; i++)
    {
        growSeasonInForest(f, &f->arbres[i]);
    }

    f->saison++;
}

void simulateForest(t_foret *f, int S)
{
    for (int s = 0; s < S; s++)
    {
        growForestSeason(f);
    }
}

void freeForest(t_foret *f)
{
    for (int i = 0; i < f->K; i++)
    {
        freeForestTree(&f->arbres[i]);
    }
    free(f->arbres);
    f->arbres = NULL;

    for (int r = 0; r < f->HMAX; r++)
    {
        free(f->occupation[r]);
        free(f->lumiere[r]);
    }
    free(f->occupation);
    free(f->lumiere);
    f->occupation = NULL;
    f->lumiere = NULL;
}
