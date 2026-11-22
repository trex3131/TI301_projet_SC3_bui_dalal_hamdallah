/* arbre_foret.c : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière (fichier fourni vide, à compléter) */
#include <stdlib.h>
#include "arbre_foret.h"

#include "arbre.h"

int countBranches(t_rameau *r) {
    if (r == NULL) return 0;
    int n=1 ;
    for (int i = 0; i < r->nb_fils; i++) {
        n += countBranches(r->fils[i]);
    }
    return n;
}

int computeHeight(t_rameau *r) {
    if (r == NULL) return -1;
    int h = r->gen;
    for (int i = 0, i < r->nb_fils; i++) {
        int hf = computeHeight(r->fils[i]);
        if (hf > h) h = hf;
    }
    return h;
}