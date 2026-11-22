/* arbre_foret.c : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière */
#include <stdlib.h>

#include "arbre_foret.h"
#include "croissance_lumiere.h"
#include "queue.h"
#include "alea.h"

// définie dans croissance.c, sans prototype dans croissance.h
float clampDirection(float);

/* ---------------------------------------------------------------- étape 1 : mesures récursives */

int countBranches(t_rameau *r) {
    if (r == NULL) return 0;
    int n = 1;
    for (int i = 0; i < r->nb_fils; i++) {
        n += countBranches(r->p_fils[i]);
    }
    return n;
}

int computeHeight(t_rameau *r) {
    if (r == NULL) return -1;
    int h = r->gen;
    for (int i = 0; i < r->nb_fils; i++) {
        int hf = computeHeight(r->p_fils[i]);
        if (hf > h) {
            h = hf;
        }
    }
    return h;
}

float computeTop(t_rameau *r) {
    float top = r->y;
    for (int i = 0; i < r->nb_fils; i++) {
        float tf = computeTop(r->p_fils[i]);
        if (tf > top) {
            top = tf;
        }
    }
    return top;
}

float computeXMin(t_rameau *r) {
    float xmin = r->x;
    for (int i = 0; i < r->nb_fils; i++) {
        float xf = computeXMin(r->p_fils[i]);
        if (xf < xmin) {
            xmin = xf;
        }
    }
    return xmin;
}

float computeXMax(t_rameau *r) {
    float xmax = r->x;
    for (int i = 0; i < r->nb_fils; i++) {
        float xf = computeXMax(r->p_fils[i]);
        if (xf > xmax) {
            xmax = xf;
        }
    }
    return xmax;
}

// option : les deux bornes en un seul parcours (même complexité O(n), mais un parcours au lieu de deux)
void computeExtension(t_rameau *r, float *p_xmin, float *p_xmax) {
    if (r->x < *p_xmin) *p_xmin = r->x;
    if (r->x > *p_xmax) *p_xmax = r->x;
    for (int i = 0; i < r->nb_fils; i++) {
        computeExtension(r->p_fils[i], p_xmin, p_xmax);
    }
}

int countBuds(t_rameau *r) {
    if (r == NULL) return 0;
    if (r->nb_fils == 0) {
        return 1;
    }
    int n = 0;
    for (int i = 0; i < r->nb_fils; i++) {
        n += countBuds(r->p_fils[i]);
    }
    return n;
}

void freeBranches(t_rameau *r) {
    if (r == NULL) return;
    // on libère d'abord les fils : une fois r libéré, on ne peut plus lire r->p_fils
    for (int i = 0; i < r->nb_fils; i++) {
        freeBranches(r->p_fils[i]);
    }
    free(r->p_fils);
    free(r);
}

t_arbre_foret createForestTree(int id, float x, float y, int seed) {
    t_arbre_foret a;

    a.step = createTreeStep(x, y, seed);
    a.id = id;
    a.hauteur = 0;
    a.cime = y;
    a.xmin = x;
    a.xmax = x;
    a.nb_bourgeons = 1;
    a.tronque = 0;

    return a;
}

void freeForestTree(t_arbre_foret *a) {
    clearBuds(&a->step);
    freeBranches(a->step.arbre.pied);
    a->step.arbre.pied = NULL;
    a->step.arbre.nb_rameaux = 0;
    a->nb_bourgeons = 0;
}

/* ---------------------------------------------------------------- étape 3 : pousse avec une lumière donnée */

void growBudInLight(t_arbre_foret *a, t_rameau *b, int n, double lumiere) {
    b->p_fils = malloc((size_t)n * sizeof(t_rameau *));
    if (b->p_fils == NULL) {
        exit(EXIT_FAILURE);
    }
    b->nb_fils = (unsigned char)n;

    for (int i = 0; i < n; i++) {
        float dir;
        float longueur;

        // mêmes tirages, dans le même ordre, que growBud ; la somme est faite en double puis arrondie en float :
        // avec une somme en float, quelques directions de data/foret_exemple.txt diffèrent de 0,01
        if (i == 0) {
            dir = (float)(b->dir + randomBetween(-10.0, 10.0));
        } else {
            int cote     = randomInt(2);
            double angle = randomBetween(30.0, 60.0);
            dir = (float)(b->dir + (cote == 0 ? -angle : angle));
        }

        double u = randomBetween(0.7, 1.3);
        longueur = (float)((0.5 + 0.5 * lumiere) * u);   // L = 1 : même longueur qu'en partie 1
        dir = clampDirection(dir);

        t_rameau *fils = growBranch(&a->step.arbre, b, a->step.saison, dir, longueur);
        b->p_fils[i] = fils;

        // mise à jour des mesures avec le nouveau rameau seulement
        if (fils->gen > a->hauteur) a->hauteur = fils->gen;
        if (fils->y > a->cime)      a->cime = fils->y;
        if (fils->x < a->xmin)      a->xmin = fils->x;
        if (fils->x > a->xmax)      a->xmax = fils->x;
    }

    // b devient du bois, ses n fils sont des bourgeons
    a->nb_bourgeons += n - 1;
}

void growSeason(t_arbre_foret *a, double lumiere) {
    t_queue G = createEmptyQueue();

    a->step.saison++;

    while (!isEmptyQueue(a->step.bourgeons)) {
        t_rameau *bud = dequeue(&a->step.bourgeons);

        if (canGrowInLight(lumiere)) {
            int n = howManyBranchesInLight(lumiere);
            growBudInLight(a, bud, n, lumiere);
            for (int i = 0; i < n; i++) {
                enqueue(&G, bud->p_fils[i]);
            }
        } else {
            enqueue(&G, bud);
        }
    }

    a->step.bourgeons = G;
}
