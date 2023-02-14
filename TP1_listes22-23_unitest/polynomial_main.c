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
	printf("\nTest Derivation 1...\n");
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
TEST(Poly_derive2) 
{ 
	printf("\nTest Derivation 2...\n");
	FILE   * file = NULL;
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
	printf("\nTest Addition 1...\n");
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

	// Addition : P1 + P2
	poly_add(&poly1, &poly2);
	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	printf("%s", buffer);
	CHECK(0 == strcmp(buffer, "(5.00, 1) (3.00, 2) (6.00, 3) (15.00, 4) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));
}

// Test2 : Plusieurs additions...
TEST(Poly_addition2)
{
	printf("\nTest Addition 2...\n");
	
	cell_t * poly1;
	cell_t * poly2;
	cell_t * poly3;

	LL_init_list(&poly1);
	LL_init_list(&poly2);
	LL_init_list(&poly3);

	char buffer[1024];

	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);
	
	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/polyadd1.txt");
	LL_create_list_fromFileName(&poly2, "./files/polyadd2.txt");
	LL_create_list_fromFileName(&poly3, "./files/polyadd3.txt");

	// Addition : P1 + P1
	poly_add(&poly1, &poly1);

	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(10.00, 1) (6.00, 2) (12.00, 3) "));
	
	// Addition : (P1 + P1) + P2 = 2P1 + P2
	file = fmemopen(buffer, 1024, "w");
	poly_add(&poly1, &poly2);

	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(10.00, 1) (6.00, 2) (12.00, 3) (15.00, 4) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));

	// Addition : (2P1 + P2) + P3
	file = fmemopen(buffer, 1024, "w");
	poly_add(&poly1, &poly3);
	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	printf("buffer = %s\n", buffer);
	CHECK(0 == strcmp(buffer, "(10.00, 1) (6.00, 2) (12.00, 3) (15.00, 4) (7.00, 5) (11.00, 6) (7.00, 8) (8.00, 9) (19.00, 10) "));

	// Libération
	LL_free_list(&poly1);
	LL_free_list(&poly2);
}

// Test 3: Addition de polynômes 
TEST(Poly_addition3)
{
	printf("\nTest Addition 3...\n");

	cell_t * poly3;

	LL_init_list(&poly3);

	char buffer[1024];

	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);


	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly3, "./files/polyadd4.txt");

	LL_save_list_toFile(file, poly3, monom_save2file);
	fclose(file);

	// Vérification du polynôme
	CHECK( 0 == strcmp(buffer, "(2.00, 1) (3.00, 3) (28.00, 4) (5.00, 5) (124.00, 6) (90.00, 7) (482.00, 8) (8.00, 9) (12.00, 45) "));

	// Addition avec un polynôme contenant un seul monome
	cell_t 	* polym;
	cell_t  * new_cell;
	monom_t   m			= {1, 15};

	new_cell = LL_create_cell(&m);

	LL_init_list(&polym);
	LL_add_cell(LL_search_prev(&polym, &m, monom_degree_cmp), new_cell);

	file = fmemopen(buffer, 1024, "w");
	poly_add(&poly3, &polym);

	
	// Récupération du polynôme dans le buffer
	LL_save_list_toFile(file, poly3, monom_save2file);
	fclose(file);
	CHECK( 0 == strcmp(buffer, "(2.00, 1) (3.00, 3) (28.00, 4) (5.00, 5) (124.00, 6) (90.00, 7) (482.00, 8) (8.00, 9) (1.00, 15) (12.00, 45) "));

	// Pleins d'additions 
	file = fmemopen(buffer, 1024, "w");

	poly_add(&poly3, &polym);
	poly_add(&poly3, &poly3);
	poly_add(&poly3, &polym);
	fclose(file);

	// Récupération du polynôme dans le buffer
	LL_save_list_toFile(file, poly3, monom_save2file);
	CHECK (0 == strcmp(buffer, "(2.00, 1) (3.00, 3) (28.00, 4) (5.00, 5) (124.00, 6) (90.00, 7) (482.00, 8) (8.00, 9) (1.00, 15) (12.00, 45) "));

	// Addition avec un polynôme NULL
	LL_free_list(&polym);

	poly_add(&poly3, &polym);

	// Résultat de l'addition
	LL_save_list_toFile(file, poly3, monom_save2file);
	fclose(file);
	CHECK (0 == strcmp(buffer, "(2.00, 1) (3.00, 3) (28.00, 4) (5.00, 5) (124.00, 6) (90.00, 7) (482.00, 8) (8.00, 9) (1.00, 15) (12.00, 45) "));

	// Libération
	LL_free_list(&poly3);
}

// Test 1 : Calcul du produit de deux polynomes
TEST(Poly_produit)
{ 
	printf("\nTest Produit 1...\n");
	
	cell_t * prod;
	cell_t * poly1;
	cell_t * poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	char buffer[1024];

	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);


	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/polyprod1.txt");
	LL_create_list_fromFileName(&poly2, "./files/polyprod2.txt");

	// P1 * P2
	prod = poly_prod(poly1, poly2);

	LL_save_list_toFile(file, prod, monom_save2file);
	fclose(file);
	CHECK (0 == strcmp(buffer, "(1.00, 4) (3.00, 6) "));

	// P1 * P1
	file =  fmemopen(buffer, 1024, "w");
	LL_free_list(&poly1);
	LL_create_list_fromFileName(&poly1, "./files/polyprod1.txt");

	prod = poly_prod(poly1, poly_prod(poly1, poly1));

	LL_save_list_toFile(file, prod, monom_save2file);
	fclose(file);

	CHECK (0 == strcmp(buffer, "(1.00, 6) (9.00, 8) (27.00, 10) (27.00, 12) "));

	// Libération
	LL_free_list(&poly1);
	LL_free_list(&poly2);
}

// Test : Ecriture d'un polynome dans un fichier
TEST(LL_save_list_toFileName) 
{ 
	printf("\nTest : LL_save_list_toFileName... (Polynomial)\n");
	cell_t * poly;
	char     buffer[1024];
	FILE   * file;

	LL_init_list(&poly);

	// Création du polynôme
	LL_create_list_fromFileName(&poly, "./files/poly3.txt");

	// Ecriture du polynôme dans un fichier
	LL_save_list_toFileName(poly, "./files/polybackupfilename1.txt", monom_save2file);
	file = fopen("./files/polybackupfilename1.txt", "r");

	fgets(buffer, 1024, file);
	CHECK (0 == strcmp(buffer, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) "));
	
	// Libération
	LL_free_list(&poly);
}

END_TEST_GROUP(polynomial)

int main(void) {
	RUN_TEST_GROUP(polynomial);
	return EXIT_SUCCESS;
}
