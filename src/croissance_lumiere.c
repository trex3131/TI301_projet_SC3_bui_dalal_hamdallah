/* croissance_lumiere.c : partie 2, étape 2 — le nombre de nouveaux rameaux selon la lumière (code fourni, à compléter) */

#include "croissance_lumiere.h"
#include "croissance.h"
#include "alea.h"

/* ---------------------------------------------------------------- code fourni */

int drawFromTable(float pourcentages[], int n)
{
    int res = n - 1;        // aucune des n - 1 premières tranches : c'est la dernière
    int trouve = 0;
    float cumul = 0;
    double u = randomReal();    // un seul tirage

    for (int i = 0; i < n - 1 && !trouve; i++)
    {
        cumul += pourcentages[i];
        // comparaison avec cumul / 100.0, comme howManyBranches : on retrouve exactement les arbres de la partie 1
        if (u < cumul / 100.0)
        {
            res = i;
            trouve = 1;
        }
    }

    return res;
}

int canGrowInLight(double lumiere)
{
    int res;

    if (lumiere >= 0.7)
    {
        res = canGrow(95);
    }
    else if (lumiere >= 0.4)
    {
        res = canGrow(70);
    }
    else if (lumiere >= 0.15)
    {
        res = canGrow(40);
    }
    else
    {
        // dormance : le bourgeon pousse dans 0 % des cas, mais le tirage est fait, comme pour toute autre lumière
        res = canGrow(0);
    }

    return res;
}

/* ---------------------------------------------------------------- à compléter : vos fonctions, à partir d'ici */

int howManyBranchesInLight(double lumiere)
{
    float pleine[3] = {70, 25, 5};
    float moyenne[2] = {85, 15};
    float faible[1] = {100};
    int res;

    // un seul tirage, quelle que soit la lumière ; drawFromTable renvoie un numéro de tranche, à partir de 0
    if (lumiere >= 0.7)
    {
        res = drawFromTable(pleine, 3) + 1;
    }
    else if (lumiere >= 0.4)
    {
        res = drawFromTable(moyenne, 2) + 1;
    }
    else
    {
        res = drawFromTable(faible, 1) + 1;
    }

    return res;
}
