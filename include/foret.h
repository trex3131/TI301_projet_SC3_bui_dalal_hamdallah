/* foret.h : partie 2, étapes 4 à 6 — la forêt, ses grilles d'occupation et de lumière, sa pousse (code fourni, à compléter) */

#ifndef FORET_FORET_H
#define FORET_FORET_H

/* ---------------------------------------------------------------- à compléter : la structure t_foret, ici, avant le code fourni */

#include "arbre_foret.h"

typedef struct s_foret {
    int K, E, C, HMAX;        // nombre d'arbres, écartement, colonnes (C = K x E), rangées
    double A;                 // part de lumière retenue par une extrémité de rameau
    int P_max;                // nombre maximal de rameaux d'un arbre
    t_arbre_foret *arbres;    // tableau dynamique des K arbres
    int **occupation;         // HMAX x C compteurs d'extrémités de rameaux
    double **lumiere;         // HMAX x C lumières
    int saison;               // nombre de saisons déjà écoulées
} t_foret;

/* ---------------------------------------------------------------- code fourni (ne compile qu'une fois t_foret écrite) */

/***
 * @brief colonne de la case qui contient l'abscisse x (la forêt fait le tour : modulo C)
 * @param f : la forêt
 * @param x : position, éventuellement négative ou au moins égale à C
 * @return la colonne, entre 0 et C - 1
 */
int columnOf(t_foret *f, float x);

/***
 * @brief rangée de la case qui contient l'altitude y (au moins HMAX : dernière rangée)
 * @param f : la forêt
 * @param y : altitude
 * @return la rangée, entre 0 et HMAX - 1
 */
int rowOf(t_foret *f, float y);

/* ---------------------------------------------------------------- à compléter : vos prototypes, à partir d'ici */

/* étape 4 : la grille d'occupation */

/***
 * @brief crée une forêt de K arbres pas encore plantés : tableau des arbres et grilles allouées, occupation à 0
 * @param K : nombre d'arbres
 * @param E : écartement (la forêt a C = K x E colonnes)
 * @param HMAX : nombre de rangées
 * @param A : part de lumière retenue par une extrémité de rameau
 * @param P_max : nombre de rameaux à partir duquel un arbre est tronqué
 * @return la forêt créée
 */
t_foret createForest(int K, int E, int HMAX, double A, int P_max);

/***
 * @brief ajoute 1 au compteur de la case où se trouve l'extrémité de r
 * @param f : la forêt
 * @param r : le rameau
 */
void addToGrid(t_foret *f, t_rameau *r);

/***
 * @brief ajoute à la grille l'extrémité de r et de tous les rameaux de son sous-arbre
 * @param f : la forêt
 * @param r : racine du sous-arbre
 */
void addTreeToGrid(t_foret *f, t_rameau *r);

/* étape 5 : la lumière */

/***
 * @brief calcule la lumière de toutes les cases à partir de l'occupation : L = (1 - A)^n,
 *        n = nombre d'extrémités strictement au-dessus dans la même colonne
 * @param f : la forêt
 */
void computeLight(t_foret *f);

/***
 * @brief lumière de la case où se trouve l'extrémité de r
 * @param f : la forêt (lumière déjà calculée)
 * @param r : le rameau
 * @return la lumière, entre 0 et 1
 */
double getLight(t_foret *f, t_rameau *r);

/* étape 6 : la forêt qui pousse */

/***
 * @brief plante les K arbres (arbre i en x = i x E + E / 2) et initialise le hasard avec seed
 * @param f : la forêt, créée par createForest
 * @param seed : graine de la simulation
 * @param identique : 1 pour des pieds d'altitude 1, 0 pour des altitudes tirées entre 0,5 et 1,5
 */
void plantForest(t_foret *f, int seed, int identique);

/***
 * @brief fait jouer une saison à l'arbre a, chaque bourgeon recevant la lumière de sa case
 * @param f : la forêt (lumière déjà calculée)
 * @param a : l'arbre
 */
void growSeasonInForest(t_foret *f, t_arbre_foret *a);

/***
 * @brief une saison de toute la forêt : computeLight, puis la saison de chaque arbre, de 0 à K - 1
 * @param f : la forêt
 */
void growForestSeason(t_foret *f);

/***
 * @brief fait jouer S saisons à la forêt
 * @param f : la forêt
 * @param S : nombre de saisons
 */
void simulateForest(t_foret *f, int S);

/***
 * @brief libère tous les arbres, puis le tableau des arbres et les deux grilles
 * @param f : la forêt
 */
void freeForest(t_foret *f);

#endif //FORET_FORET_H
