/* campagne.h : partie 3, étape 1 — une campagne de simulations : R forêts, mesures écrites dans deux fichiers CSV */

#ifndef FORET_CAMPAGNE_H
#define FORET_CAMPAGNE_H

#define H_MAX 160           // nombre de rangées des grilles d'occupation et de lumière, pour toute la partie 3
#define P_MAX 20000         // nombre de rameaux à partir duquel un arbre est tronqué
#define LONGUEUR_NOM 256    // longueur maximale d'un nom de fichier

typedef struct s_campagne {
    int K, E, S;                // nombre d'arbres, écartement, nombre de saisons
    double A;                   // part de lumière retenue par une extrémité de rameau
    int R;                      // nombre de forêts à simuler
    int graine;                 // la forêt numéro r est plantée avec la graine graine + r
    int identique;              // 1 : plantation identique
    char sortie[LONGUEUR_NOM];  // début du nom des deux fichiers de résultats
} t_campagne;

/***
 * @brief crée une campagne avec les réglages par défaut : K = 50, E = 3, A = 0,25, S = 120, R = 1, graine 1,
 *        plantation au hasard, sortie "output/campagne"
 * @return la campagne créée
 */
t_campagne createCampaign(void);

/***
 * @brief simule les R forêts de la campagne et écrit leurs mesures, après S saisons, dans <sortie>_arbres.csv
 *        et <sortie>_forets.csv ; affiche une ligne par forêt, puis la durée totale du calcul
 * @param c : la campagne
 * @return 1, ou 0 si l'un des deux fichiers n'a pas pu être créé
 */
int runCampaign(t_campagne c);

#endif //FORET_CAMPAGNE_H
