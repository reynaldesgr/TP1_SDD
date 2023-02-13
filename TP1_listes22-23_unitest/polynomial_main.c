/**
 * @file polynomial_main.c
 * @brief Programme pour les tests des operations sur les polynomes
 * ATTENTION : Il faut creer autant de tests que les cas d'execution pour chaque fonction a tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "polynomial.h"
#include "teZZt.h"

BEGIN_TEST_GROUP(polynomial)

TEST(LL_init_list) {
	cell_t *list;

	printf("\nInitialization of the linked list : \n");
	LL_init_list(&list);

	REQUIRE ( list == NULL );
}

// Test 1: dérivation polynômes
// Exemple
TEST(Poly_derive1) 
{
	cell_t * poly = NULL;
	FILE   * file = NULL;
	char   buffer[1024];

	printf("\nDerive of polynomial 1 : \n");

	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file );
	LL_create_list_fromFileName(&poly, "./files/poly1.txt");
	LL_save_list_toFile(file, poly, monom_save2file);
	fclose(file);

	printf("buffer = %s\n", buffer);
	CHECK( 0 == strcmp(buffer, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file );
	poly_derive(&poly);
	LL_save_list_toFile(file, poly, monom_save2file);
	fclose(file);

	CHECK( 0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) ") );
	LL_free_list(&poly);
}


// Test 2 : Dérivation polynômes
TEST(Poly_derive) 
{ 
	FILE * file = NULL;
	cell_t * poly;
	LL_init_list(&poly);

	char   buffer[1024];

	printf("\nDerive of polynomial 3 : \n");

	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file );
	LL_create_list_fromFileName(&poly, "./files/poly3.txt");
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	CHECK( 0 == strcmp(buffer, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) ") );

	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file );
	poly_derive(&poly);
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	CHECK( 0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) ") );

	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file );
	poly_derive(&poly);
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	printf("buffer = %s\n", buffer);
	CHECK( 0 == strcmp(buffer, "(8.00, 0) (30.00, 1) (72.00, 2) (60.00, 3) "));

	LL_free_list(&poly);
}

// Test 1: Addition de deux polynômes
TEST(Poly_addition1) 
{ 
	cell_t * poly1;
	cell_t * poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	char buffer[1024];

	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);
	
	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/polyadd1.txt");
	LL_create_list_fromFileName(&poly2, "./files/polyadd2.txt");
	/*
	// Addition : P1 + P1
	poly_add(&poly1, &poly1);

	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(10.00, 1) (6.00, 2) (12.00, 3) "));
	*/

	/*
	// Addition : 2P1 + P2
	file = fmemopen(buffer, 1024, "w");
	poly_add(&poly1, &poly2);

	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(10.00, 1) (6.00, 2) (12.00, 3) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));
	*/

	// Addition : P1 + P2
	poly_add(&poly1, &poly2);
	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(5.00, 1) (3.00, 2) (6.00, 3) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));
}

/*
TEST(Poly_produit) { // test sur le calcul du produit de deux polymones
	cell_t *list;

	//TO DO
}


TEST(LL_save_list_toFileName) { // test pour l'ecriture d'un polynome dans un fichier
	cell_t *list;

	//TO DO
}
*/

END_TEST_GROUP(polynomial)

int main(void) {
	RUN_TEST_GROUP(polynomial);
	return EXIT_SUCCESS;
}
