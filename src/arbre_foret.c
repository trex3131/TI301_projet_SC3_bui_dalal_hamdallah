/* arbre_foret.c : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière (fichier fourni vide, à compléter) */
#include <stdlib.h>
#include "arbre_foret.h"

#include "arbre.h"

int countBranches(t_rameau *r) {
    if (r == NULL) return 0;
    int n=1 ;
    for (int i = 0; i < r->nb_fils; i++) {
        n += countBranches(r->p_fils[i]);
    }
    return n;
}

int computeHeight(t_rameau *r) {
    if (r == NULL) return -1;
    int h = r->gen;
    for (int i = 0, i < r->nb_fils; i++) {
        int hf = computeHeight(r->p_fils[i]);
        if (hf > h) {
            h = hf;
        }
    }
    return h;
}

float computeTop(t_rameau *r) {
    if (r == NULL) return 0.0f;
    float top = r->y;
    for (int i = 0; i < r->nb_fils; i++) {
        if (tf > top) {
            top = tf;
        }
    }
    return top;
}

float computeXMin(t_rameau *r) {
    if (r == NULL) return 0.0f;
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
    if (r == NULL) return 0.0f;
    float xmax = r->x;
    for (int i = 0; i < r->nb_fils; i++) {
        float xf = computeXMax(r->p_fils[i]);
        if (xf > xmax) {
            xmax = xf;
        }
    }
    return xmax;
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
}