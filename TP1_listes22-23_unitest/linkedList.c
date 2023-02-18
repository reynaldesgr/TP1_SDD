/**
 * @file linkedList.c
 * @brief Implementation file of linked list module
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

/** Example
 * @fn void LL_init_list(cell_t **adrHeadPt)
 * @brief Initialize a void list
 * @param [in, out] adrHeadPt address of head pointer of the list 
 */
void LL_init_list(cell_t **adrHeadPt)
{
    *adrHeadPt = NULL;
}


/** TO DO
 * @brief Create a new cell for linked list from its data 
 * @param [in] m address of the data
 * @return address of the new cell
 */
cell_t * LL_create_cell(monom_t * m)
{
    cell_t * new  = (cell_t *) malloc(sizeof(cell_t));
    new->val      = *m;
    new->next     = NULL;
    return new;
}


/** TO DO
 * @brief Insert a cell into a linked list at the given position
 * @param [in, out] previous_cell address of previous pointer of the cell
 * @param [in] new_cell address of the cell to be added to the linked list
 */
void LL_add_cell(cell_t ** previous_cell, cell_t * new_cell)
{   
    if (*previous_cell != NULL)
    {
        if ((*previous_cell)->val.degree > new_cell->val.degree)
        {
            new_cell->next = (*previous_cell);
            *previous_cell = new_cell;
        }
        else if ((*previous_cell)->val.degree < new_cell->val.degree)
        {
            new_cell->next = (*previous_cell)->next;
            (*previous_cell)->next = new_cell;
        }else{
            (*previous_cell)->val.coef += new_cell->val.coef;
        }
    }
    else
    {
        *previous_cell = new_cell;
    }
}


/** TO DO
 * @brief Create a linked list from a file (in which the data is sorted)
 * @param [in, out] adrHeadPt address of head pointer of a linked list
 * @param [in] filename name of a file containing the data for a linked list
 * @return head pointer of the linked list
 */
cell_t ** LL_create_list_fromFileName(cell_t ** adrHeadPt, char * filename)
{
    FILE * file = fopen(filename, "r");

    if (file)
    {
        cell_t  * new_cell             = NULL;
        cell_t ** previous_cell        = NULL;
        monom_t   m;

        while (!feof(file))
        {
            fscanf(file, "%lf %d", &m.coef, &m.degree);

            new_cell      = LL_create_cell(&m);
            previous_cell = LL_search_prev(adrHeadPt, &(new_cell->val), &monom_degree_cmp);

            if(*previous_cell != NULL)
            {
                if (monom_degree_cmp(&(*previous_cell)->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->val.coef += new_cell->val.coef;
                    //LL_del_cell(&new_cell);
                    free(new_cell);
                }
                else if ((*previous_cell)->next && monom_degree_cmp(&(*previous_cell)->next->val, &new_cell->val) == 0)
                {
                    (*previous_cell)->next->val.coef += new_cell->val.coef;
                    // LL_del_cell(&new_cell);
                    free(new_cell);
                }
                else
                {
                    LL_add_cell(&(*previous_cell), new_cell);
                }
            }
            else
            {
                LL_add_cell(previous_cell, new_cell);
            }
        }
        fclose(file);
    }
    else
    {
        *adrHeadPt = NULL;
    }

    return adrHeadPt;
}

/** TO DO
 * @brief Write the linked list to an output stream
 * @param [in] stream file pointer of an output stream
 * @param [in] list head pointer of a linked list
 * @param ptf function pointer for printing the data of a cell on an output stream
 */
void LL_save_list_toFile(FILE * stream, cell_t * list, void (*ptf) (FILE *, monom_t *))
{
    if (stream != NULL)
    {   
        cell_t * cell = list;

        while (cell != NULL)
        {
            ptf(stream, &cell->val);
            cell = cell->next;
        }
    }
} 

/** TO DO
 * @brief Save a linked list into a file
 * @param [in, out] list head pointer of a linked list
 * @param [in] filename name of the backup file
 * @param ptf fonction pointer for writing the data of a cell to a output stream
 */
void LL_save_list_toFileName(cell_t * list, char * filename, void (*ptf)(FILE *, monom_t *))
{
    FILE * file = fopen(filename, "w+");
    LL_save_list_toFile(file, list, ptf);
    fclose(file);
} 

/** TO DO
 * @brief Search a value in a linked list, and return the address of the previous pointer
 * @param [in] adrHeadPt address of the head pointer
 * @param [in] m address of the value to search
 * @param comp fonction pointer for comparaison of two values
 * @return the address of the previous pointer
 */
cell_t ** LL_search_prev(cell_t ** adrHeadPt, monom_t * m, int (*comp) (monom_t *, monom_t *))
{
    cell_t ** prev = adrHeadPt;

    while (*prev && (*prev)->next != NULL && comp(&(*prev)->next->val, m) < 0){
        prev = &(*prev)->next;
    }
    return prev;
}


/** TO DO
 * @brief Delete a cell from a linked list
 * @param [in, out] previous_cell address of the previous pointer of the cell to delete
 */
void LL_del_cell(cell_t ** previous_cell)
{
    cell_t * deleted_cell;
    
    if (*previous_cell != NULL)
    {
        deleted_cell = (*previous_cell)->next;

        if (deleted_cell)
        {
            (*previous_cell)->next = deleted_cell->next;
        }

        free(deleted_cell);
    }
}


/** TO DO
 * @brief Free the memory location occupied by a linked list
 * @param [in, out] adrHeadPt address of head pointer of a linked list
 */
void LL_free_list(cell_t ** adrHeadPt)
{
    if (adrHeadPt != NULL)
    {
        cell_t * current = *adrHeadPt;
        cell_t * next;

        while (current)
        {
            next = current->next;
            free(current);
            current = next;
        }
        *adrHeadPt = NULL;
    }
}

/** Afficher la liste chaînée
 * @brief Print the list on the stdout stream
 * @param [in, out] adrHeadPt adress of head pointer of a linked list
 */
void LL_print_stdout(cell_t ** adrHeadPt)
{
    cell_t * cell = *adrHeadPt;

    printf("\n == Writing the list on stdout ... == \n");
    if (cell != NULL)
    {
        while (cell != NULL)
        {
            printf("(%.2lf, %d) ", cell->val.coef, cell->val.degree);
            cell = cell->next;
        }
    }
}