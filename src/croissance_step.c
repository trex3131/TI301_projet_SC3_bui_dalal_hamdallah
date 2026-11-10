//
// Created by flasque on 14/10/2026.
//
#include <stdio.h>
#include <stdlib.h>

#include "croissance_step.h"
#include "croissance.h"

// code des fonctions exportées

t_arbre_step createTreeStep(float x, float y, int seed)
{
    t_arbre_step a;

    a.arbre    = createTree(x, y, seed);
    a.bourgeons = createEmptyQueue();
    enqueue(&a.bourgeons, a.arbre.pied);
    a.saison   = 0;

    return a;
}

void growTreeStep(t_arbre_step *a)
{
    int s = a->saison + 1;
    t_queue G = createEmptyQueue();

    while (!isEmptyQueue(a->bourgeons)) {
        t_rameau *bud = dequeue(&a->bourgeons);

        if (canGrow(95.0f)) {
            int n = howManyBranches(70.0f, 25.0f);
            growBud(&a->arbre, bud, n, s);
            for (int i = 0; i < n; i++) {
                enqueue(&G, bud->p_fils[i]);
            }
        } else {
            enqueue(&G, bud);
        }
    }

    a->bourgeons = G;
    a->saison    = s;
}

void clearBuds(t_arbre_step *a)
{
    while (!isEmptyQueue(a->bourgeons)) {
        dequeue(&a->bourgeons);
    }
}