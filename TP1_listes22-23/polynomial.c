#include <stdio.h>
#include <stdlib.h>
//#include <float.h>  // FLT_EPSILON DBL_EPSILON LDBL_EPSILON
//#include <math.h>

#include "linkedList.h"

/** poly_derive
 * @brief compute 'in place' the derive of a polynomial 
 * @param [in, out] adrPolyHeadPt address of a polynomial's head pointer
 */
void poly_derive(cell_t ** adrPolyHeadPt)
{
    if (*adrPolyHeadPt != NULL)
    {
        cell_t *    current   = *adrPolyHeadPt;
        cell_t **   previous  = adrPolyHeadPt;
        cell_t *    temp;
        int         is_deleted;

        while (current != NULL)
        {
            is_deleted              = 0;
            current->val.coef       = (current->val.coef) * (current->val.degree);
            current->val.degree    -= 1;

            if (current->val.degree < 0)
            {
                temp        = current->next;
                *previous   = temp;
                free(current);
                current     = temp;
                is_deleted  = 1;
            }

            if (is_deleted != 1)
            {
                previous = &(current->next);
                current  = current->next;
            }
        }
    }
}

/** poly_add
 * @brief compute P1 = P1 + P2, P2 become empty
 * @param adrPolyHeadPt1 [in, out] address of the 1st polynomial's head pointer
 * @param adrPolyHeadPt2 [in, out] address of the 2nd polynomial's head pointer
 */
void poly_add(cell_t ** adrPolyHeadPt1, cell_t ** adrPolyHeadPt2)
{
    cell_t **    current1;
    cell_t **    current2;
    cell_t **    previous;
    
    int deleted = 0;

    if (*adrPolyHeadPt1 == NULL && *adrPolyHeadPt2 != NULL)
    {
        *adrPolyHeadPt1 = *adrPolyHeadPt2;
    }
    else
    {
        if (*adrPolyHeadPt2 != NULL)
        {
            current1 = adrPolyHeadPt1;
            current2 = adrPolyHeadPt2;

            while (*current1 != NULL && *current2 != NULL)
            {
                // Recherche de la cellule precedente 
                previous = LL_search_prev(current1, &(*current2)->val, &monom_degree_cmp);

                // Si les monomes sont de degrés différents
                if (monom_degree_cmp(&((*previous)->val), &(*current2)->val) < 0)
                {
                    if ((*previous)->next != NULL && monom_degree_cmp(&((*previous)->next->val), &((*current2)->val)) == 0)
                    {
                        (*previous)->next->val.coef += (*current2)->val.coef;
                        if ((*previous)->next->val.coef == 0.)
                        {
                            LL_del_cell(previous, current1);
                            deleted = 1;
                        }
                    }
                    // Sinon
                    else
                    {
                        LL_add_cell(previous, LL_create_cell(&(*current2)->val));
                    }
                }
                else
                {
                    // Suppression des monomes de coefficients nuls
                    (*previous)->val.coef += (*current2)->val.coef;
                    if ((*previous)->val.coef == 0.)
                    {
                        LL_del_cell(previous, current1);
                        deleted = 1;
                    }
                }

                // Test : cellule supprime ?
                if (!deleted)
                {   // Si non : on avance
                    current1 = &(*current1)->next;
                }else
                {
                    // Si oui : pas la peine d'avancer
                    deleted = 0;
                }

                current2 = &(*current2)->next;
            }
        }

        // Liberation 
        if (adrPolyHeadPt1 != adrPolyHeadPt2)
        {
            LL_free_list(adrPolyHeadPt2);
        }
    }
}


/** poly_prod
 * @brief compute P1 * P2
 * @param adrHeadPt1 [in, out] head pointer of the 1st polynomial
 * @param adrHeadPt2 [in, out] head pointer of the 2nd polynomial
 * @return P1*P2
 */
cell_t * poly_prod (cell_t * adrHeadPt1, cell_t * adrHeadPt2)
{
    cell_t ** previous_cell;

    cell_t * adrHeadPt;
    cell_t * current1;
    cell_t * current2;
    cell_t * new_cell;

    monom_t  prod;

    LL_init_list(&adrHeadPt);

    current1 = adrHeadPt1;
    current2 = adrHeadPt2;

    while (current1 != NULL)
    {
        while (current2 != NULL)
        {
            prod.coef = current1->val.coef * current2->val.coef;
            prod.degree = current1->val.degree + current2->val.degree;

            // Creation de la nouvelle cellule 
            new_cell = LL_create_cell(&prod);

            // Recherche de la cellule precedente
            previous_cell  = LL_search_prev(&adrHeadPt, &prod, monom_degree_cmp);

            if(*previous_cell != NULL)
            {
                // Si il existe deja une cellule ayant le meme degre
                if (monom_degree_cmp(&(*previous_cell)->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->val.coef += new_cell->val.coef;
                    free(new_cell);
                }
                else if ((*previous_cell)->next != NULL && monom_degree_cmp(&(*previous_cell)->next->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->next->val.coef += new_cell->val.coef;
                    free(new_cell);
                }
                // Sinon
                else
                {
                    LL_add_cell(previous_cell, new_cell);
                }
            }
            else
            {
                LL_add_cell(previous_cell, new_cell);
            }
            // On se deplace au suivant dans le polynome 2
            current2 = current2->next;
        }
        current2 = adrHeadPt2;

        // On se deplace au suivant dans le polynome 1
        current1 = current1->next;
    }

    return adrHeadPt;
}
