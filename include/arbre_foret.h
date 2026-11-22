/* arbre_foret.h : partie 2, étapes 1 et 3 — l'arbre de la forêt, ses mesures et sa pousse à la lumière */

#ifndef FORET_ARBRE_FORET_H
#define FORET_ARBRE_FORET_H

#include "arbre.h"
#include "croissance_step.h"

typedef struct s_arbre_foret {
    t_arbre_step step;    // l'arbre, sa file de bourgeons et sa saison (partie 1, étape 3)
    int id;               // numéro de l'arbre dans la forêt
    int hauteur;          // génération maximale de ses rameaux
    float cime;           // altitude maximale des extrémités de ses rameaux
    float xmin, xmax;     // positions x minimale et maximale des extrémités de ses rameaux
    int nb_bourgeons;     // nombre de bourgeons (le nombre d'éléments de la file)
    int tronque;          // 1 si l'arbre a atteint P_max rameaux : il ne pousse plus
} t_arbre_foret;

/* ---------------------------------------------------------------- étape 1 : mesures récursives */

/***
 * @brief nombre de rameaux du sous-arbre de r
 * @param r : racine du sous-arbre
 * @return le nombre de rameaux (0 si r vaut NULL)
 */
int countBranches(t_rameau *r);

/***
 * @brief plus grande génération d'un rameau du sous-arbre de r
 * @param r : racine du sous-arbre
 * @return la génération maximale (-1 si r vaut NULL)
 */
int computeHeight(t_rameau *r);

/***
 * @brief plus grande altitude d'une extrémité de rameau du sous-arbre de r (la cime)
 * @param r : racine du sous-arbre (non NULL)
 * @return l'altitude maximale
 */
float computeTop(t_rameau *r);

/***
 * @brief plus petite position x d'une extrémité de rameau du sous-arbre de r
 * @param r : racine du sous-arbre (non NULL)
 * @return la position x minimale
 */
float computeXMin(t_rameau *r);

/***
 * @brief plus grande position x d'une extrémité de rameau du sous-arbre de r
 * @param r : racine du sous-arbre (non NULL)
 * @return la position x maximale
 */
float computeXMax(t_rameau *r);

/***
 * @brief option : xmin et xmax du sous-arbre de r en un seul parcours
 * @param r : racine du sous-arbre (non NULL)
 * @param p_xmin, p_xmax : adresses où ranger les deux positions
 */
void computeExtension(t_rameau *r, float *p_xmin, float *p_xmax);

/***
 * @brief nombre de bourgeons (rameaux sans fils) du sous-arbre de r
 * @param r : racine du sous-arbre
 * @return le nombre de bourgeons (0 si r vaut NULL)
 */
int countBuds(t_rameau *r);

/***
 * @brief libère tous les rameaux du sous-arbre de r, tableaux de fils compris
 * @param r : racine du sous-arbre
 */
void freeBranches(t_rameau *r);

/***
 * @brief crée l'arbre numéro id, réduit à son pied d'extrémité (x ; y), avec ses mesures
 * @param id : numéro de l'arbre dans la forêt
 * @param x, y : extrémité du pied
 * @param seed : graine (createTree réinitialise le générateur avec elle)
 * @return l'arbre créé
 */
t_arbre_foret createForestTree(int id, float x, float y, int seed);

/***
 * @brief libère la file des bourgeons puis tous les rameaux de l'arbre
 * @param a : adresse de l'arbre
 */
void freeForestTree(t_arbre_foret *a);

/* ---------------------------------------------------------------- étape 3 : pousse avec une lumière donnée */

/***
 * @brief fait naître n rameaux au bourgeon b (saison a->step.saison), longueur (0,5 + 0,5 L) x u,
 *        et met à jour les mesures de l'arbre
 * @param a : l'arbre auquel appartient b
 * @param b : le bourgeon qui pousse
 * @param n : nombre de nouveaux rameaux (1 à 3)
 * @param lumiere : lumière reçue par b
 */
void growBudInLight(t_arbre_foret *a, t_rameau *b, int n, double lumiere);

/***
 * @brief fait jouer une saison à l'arbre a, avec la même lumière pour tous ses bourgeons
 * @param a : l'arbre
 * @param lumiere : lumière reçue par chaque bourgeon
 */
void growSeason(t_arbre_foret *a, double lumiere);

#endif //FORET_ARBRE_FORET_H
