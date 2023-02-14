#include <stdio.h>
#include <stdlib.h>
//#include <float.h>  // FLT_EPSILON DBL_EPSILON LDBL_EPSILON
//#include <math.h>

#include "linkedList.h"

/** TO DO
 * @brief compute 'in place' the derive of a polynomial 
 * @param [in, out] adrPolyHeadPt address of a polynomial's head pointer
 */
void poly_derive(cell_t ** adrPolyHeadPt)
{
    if (*adrPolyHeadPt != NULL)
    {
        cell_t *    current   = *adrPolyHeadPt;
        cell_t **   previous = adrPolyHeadPt;
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

/** TO DO
 * @brief compute P1 = P1 + P2, P2 become empty
 * @param adrPolyHeadPt1 [in, out] address of the 1st polynomial's head pointer
 * @param adrPolyHeadPt2 [in, out] address of the 2nd polynomial's head pointer
 */
void poly_add(cell_t ** adrPolyHeadPt1, cell_t ** adrPolyHeadPt2)
{
    cell_t *    current1;
    cell_t *    current2;
    cell_t **   previous;

    if (*adrPolyHeadPt1 == NULL && *adrPolyHeadPt2)
    {
        *adrPolyHeadPt1 = *adrPolyHeadPt2;
    }
    else
    {
        if (*adrPolyHeadPt2)
        {
            current2 = *adrPolyHeadPt2;
            current1 = *adrPolyHeadPt1;
            while (current2 && current1)
            {
                previous = LL_search_prev(&current1, &(current2->val), &monom_degree_cmp);
                if (monom_degree_cmp(&((*previous)->val), &(current2->val)) < 0)
                {
                    //printf("\n Ajout de : %.3lf %d après %.3lf %d \n", current2->val.coef, current2->val.degree,(*previous)->val.coef, (*previous)->val.degree);
                    LL_add_cell(previous, LL_create_cell(&(current2->val)));
                }
                else
                {
                    //printf("\nAddition de : %.3lf %d avec %.3lf %d \n", current1->val.coef, current1->val.degree,(*previous)->val.coef, (*previous)->val.degree); 
                    (*previous)->val.coef += (current2->val.coef);
                }
                current2 = current2->next;
                current1 = current1->next;
            }

        }
    }
    if (adrPolyHeadPt2 != adrPolyHeadPt1)
    {
        LL_free_list(adrPolyHeadPt2);
    }
}

/** TO DO
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

    while (current1){
        while (current2){
            prod.coef   = current1->val.coef   * current2->val.coef;
            prod.degree = current1->val.degree + current2->val.degree;

            new_cell       = LL_create_cell(&prod);
            previous_cell = LL_search_prev(&adrHeadPt, &prod, monom_degree_cmp);

            if(*previous_cell){
                if (monom_degree_cmp(&(*previous_cell)->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->val.coef       += new_cell->val.coef;
                }
                else if ((*previous_cell)->next && monom_degree_cmp(&(*previous_cell)->next->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->next->val.coef += new_cell->val.coef;
                }
                else
                {
                    LL_add_cell(&(*previous_cell), new_cell);
                }
            }else{
                LL_add_cell(previous_cell, new_cell);
            }
            current2 = current2->next;
        }
        current2 = adrHeadPt2;
        current1 = current1->next;
    }

    return adrHeadPt;
}
