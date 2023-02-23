#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

// #include "linkedList.h"

/** poly_derive
 * @brief compute 'in place' the derive of a polynomial
 * @param [in, out] adrPolyHeadPt address of a polynomial's head pointer
 */
void poly_derive(cell_t ** adrPolyHeadPt);

/** poly_add
 * @brief compute P1 = P1 + P2, P2 become empty
 * @param adrPolyHeadPt1 [in, out] address of the 1st polynomial's head pointer
 * @param adrPolyHeadPt2 [in, out] address of the 2nd polynomial's head pointer
 */
void poly_add(cell_t ** adrPolyHeadPt1, cell_t ** adrPolyHeadPt2);

/** poly_prod
 * @brief P1 * P2
 * @param xxx [in, out] head pointer of the 1st polynomial
 * @param xxx [in, out] head pointer of the 2nd polynomial
 * @return P1*P2
 */
cell_t * poly_prod (cell_t * adrHeadPt1, cell_t * adrHeadPt2);

#endif