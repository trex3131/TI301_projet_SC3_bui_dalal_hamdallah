#ifndef CROISSANCE_STEP_H
#define CROISSANCE_STEP_H

#include "arbre.h"
#include "queue.h"

typedef struct s_arbre_step {
    t_arbre arbre;        /* l'arbre lui-même (pied, nb_rameaux, graine) */
    t_queue bourgeons;    /* bourgeons qui peuvent pousser à la prochaine saison */
    int     saison;       /* nombre de saisons déjà écoulées */
} t_arbre_step;

t_arbre_step createTreeStep(float x, float y, int seed);
void         growTreeStep(t_arbre_step *a);
void         clearBuds(t_arbre_step *a);

#endif