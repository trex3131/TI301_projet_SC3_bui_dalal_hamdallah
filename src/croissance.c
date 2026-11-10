//
// Created by flasque on 12/10/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "croissance.h"
#include "queue.h"

// prototypes des fonctions locales

float clampDirection(float);

// code des fonctions locales

float clampDirection(float dir)
{
    float res = dir;

    if (res < -80)
    {
        res = -80;
    }
    else if (res > 80)
    {
        res = 80;
    }

    return res;
}

// code des fonctions exportées

int canGrow(float probabilite)
{
    return (randomReal() < probabilite / 100.0);
}

int howManyBranches(float p1, float p2)
{
    double u = randomReal();
    if (u < p1 / 100.0)              return 1;
    if (u < (p1 + p2) / 100.0)       return 2;
    return 3;
}

void growBud(t_arbre *a, t_rameau *b, int n, int saison)
{
    b->p_fils = malloc((size_t)n * sizeof(t_rameau *));
    if (b->p_fils == NULL) {
        exit(EXIT_FAILURE);
    }
    b->nb_fils = (unsigned char)n;

    for (int i = 0; i < n; i++) {
        float dir;
        float longueur;

        if (i == 0) {
            dir = b->dir + (float)randomBetween(-10.0, 10.0);
        } else {
            int cote    = randomInt(2);
            float angle = (float)randomBetween(30.0, 60.0);
            dir = b->dir + (cote == 0 ? -angle : angle);
        }

        longueur = (float)randomBetween(0.7, 1.3);
        dir = clampDirection(dir);

        b->p_fils[i] = growBranch(a, b, saison, dir, longueur);
    }
}

void growTree(t_arbre *a, int nbSaisons)
{
    t_queue F = createEmptyQueue();
    t_queue G;

    enqueue(&F, a->pied);

    for (int s = 1; s <= nbSaisons; s++) {
        G = createEmptyQueue();

        while (!isEmptyQueue(F)) {
            t_rameau *bud = dequeue(&F);

            if (canGrow(95.0f)) {
                int n = howManyBranches(70.0f, 25.0f);
                growBud(a, bud, n, s);
                for (int i = 0; i < n; i++) {
                    enqueue(&G, bud->p_fils[i]);
                }
            } else {
                enqueue(&G, bud);
            }
        }
        F = G;
    }

    while (!isEmptyQueue(F)) {
        dequeue(&F);
    }
}