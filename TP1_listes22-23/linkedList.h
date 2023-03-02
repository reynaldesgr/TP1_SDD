/**
 * @file linkedList.h
 * @brief Header file of linked list module
 */

#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "valCell.h"

/** cell_t
 * @struct cell_t
 * @brief Data structure of the linked list's cell
 * 2 fields :
 *      - 'val' : value of a cell
 *      - 'next' : pointer to the next cell
 */
 typedef struct cell_t{
  monom_t val;
  struct cell_t * next;
}cell_t;


/** LL_init_list
 * @fn void LL_init_list(cell_t **adrHeadPt)
 * @brief Initialize a void list
 * @param [in, out] adrHeadPt address of head pointer of the list 
 */
void LL_init_list(cell_t ** adrHeadPt);

/** LL_create_cell
 * @brief create a new cell for linked list from its data 
 * @param [in] m address of the data
 * @return address of the new cell
 */
cell_t * LL_create_cell(monom_t * m);

/** LL_add_cell
 * @brief Insert a cell into a linked list at the given position
 * @param [in, out] previous_cell address of previous pointer of the cell
 * @param [in] new_cell address of the cell to be added to the linked list
 */
void LL_add_cell(cell_t ** previous_cell, cell_t * new_cell);

/** LL_create_list_from_FileName
 * @brief Create a linked list from a file (in which the data is sorted)
 * @param [in, out] adrHeadPt address of head pointer of a linked list
 * @param [in] filename name of a file containing the data for a linked list
 * @return head pointer of the linked list
 */
cell_t ** LL_create_list_fromFileName(cell_t ** adrHeadPt, char * filename);

/** LL_save_list_toFile
 * @brief Write the linked list to an output stream
 * @param [in] stream file pointer of an output stream
 * @param [in] list head pointeur of a linked list
 * @param ptf function pointer for printing the data of a cell on an output stream
 */
void LL_save_list_toFile(FILE * stream, cell_t * list, void (*ptf)(FILE *, monom_t *));

/** LL_save_list_toFileName
 * @brief Save a linked list into a file
 * @param [in, out] list head pointer of a linked list
 * @param [in] filename name of the backup file
 * @param ptf fonction pointer for writing the data of a cell to a output stream
 */
void LL_save_list_toFileName(cell_t * list, char * filename, void (*ptf)(FILE *, monom_t *));

/** LL_search_prev
 * @brief Search a value in a linked list, and return the address of the previous pointer
 * @param [in] adrHeadPt address of the head pointer
 * @param [in] m address of the value to search
 * @param comp fonction pointer for comparison of two values
 * @return the address of the previous pointer
 */
cell_t ** LL_search_prev(cell_t ** adrHeadPt, monom_t * m, int (*comp) (monom_t *, monom_t *));

/** LL_del_cell
 * @brief Delete a cell from a linked list
 * @param [in, out] previous_cell address of the previous pointer of the cell to delete
 */
void LL_del_cell(cell_t ** previous_cell, cell_t ** adrHeadPt);

/** LL_free_list
 * @brief Free the memory location occupied by the cells of a linked list
 * @param [in, out] adrHeadPt address of head pointer of a linked list
 */
void LL_free_list(cell_t ** adrHeadPt);

/** LL_print_stdout
 * @brief Print the list on the stdout stream
 * @param [in, out] adrHeadPt adress of head pointer of a linked list
 */
void LL_print_stdout(cell_t ** adrHeadPt);

#endif