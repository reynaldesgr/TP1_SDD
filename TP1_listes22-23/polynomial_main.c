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

// Test 0 : initialisation de la liste chainee
TEST(LL_init_list)
{
	printf("\n> Test 0 : Initialisation de la liste chainee...\n");
	cell_t *list;

	printf("\nInitialization of the linked list : \n");
	LL_init_list(&list);

	REQUIRE(list == NULL);
}

// Test 1 : Derivation d'un polynome classique (non nul)
TEST(Poly_derive1)
{
	printf("\n> Test 1 : Derivation classique d'un polynome...\n");
	cell_t *poly;

	// Initialisation
	LL_init_list(&poly);

	FILE *file = NULL;
	char buffer[1024];

	// Creation du polynome et ouverture buffer
	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	LL_create_list_fromFileName(&poly, "./files/derivation/polyderiv1.txt");
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	// Verification de lecture du polynome
	CHECK(0 == strcmp(buffer, "(5.00, 1) (4.00, 2) (5.00, 3) (6.00, 4) (3.00, 5) "));

	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	// Derivation
	poly_derive(&poly);

	// Sauvegarde dans le buffer
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	// Verification
	CHECK(0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) "));

	// Liberation
	LL_free_list(&poly);
}

// Test 2 : Derivation d'un polynome non nul et avec une constante (verification de la suppression du monome avec la constante lors de la derivation)
TEST(Poly_derive2)
{
	printf("\n> Test 2 : Derivation d'un polynome non nul avec un terme de degre 0...\n");
	FILE *file = NULL;
	cell_t *poly;
	LL_init_list(&poly);

	char buffer[1024];

	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);
	LL_create_list_fromFileName(&poly, "./files/derivation/poly.txt");
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	CHECK(0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) "));

	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);
	poly_derive(&poly);
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	printf("buffer = %s\n", buffer);
	CHECK(0 == strcmp(buffer, "(8.00, 0) (30.00, 1) (72.00, 2) (60.00, 3) "));

	LL_free_list(&poly);
}

// Test 3 : Derivation d'un polynome nul
TEST(Poly_derive3)
{
	printf("\n >Test 3 : Derivation d'un polynome nul...\n");

	cell_t *poly;
	LL_init_list(&poly);

	poly_derive(&poly);
	CHECK(NULL == poly);
}

// Test 4 : Derivation d'un polynome non nul avec uniquement une constante (verification si la derivation renvoie le polynome nul)
TEST(Poly_derive4)
{
	printf("\n >Test 4 : Derivation d'un polynome de degré 0 (constante)...\n");
	FILE *file = NULL;

	cell_t *poly;
	LL_init_list(&poly);

	char buffer[1024];
	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	LL_create_list_fromFileName(&poly, "./files/derivation/polycst.txt");
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	CHECK(0 == strcmp(buffer, "(5.00, 0) "));

	file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);
	poly_derive(&poly);
	LL_save_list_toFile(file, poly, monom_save2file);

	fclose(file);

	CHECK(NULL == poly);
}

// Test 5: Addition classique de deux polynomes (sans polynomes nuls, sans monomes de même degre avec un signe oppose)
TEST(Poly_addition1)
{
	printf("\n >Test 5 : Addition classique de deux polynomes...\n");
	cell_t *poly1;
	cell_t *poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	char buffer[1024];

	// Ouverture du buffer
	FILE *file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/add/polyadd1.txt");
	LL_create_list_fromFileName(&poly2, "./files/add/polyadd2.txt");

	// Addition : P1 + P2
	poly_add(&poly1, &poly2);
	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);
	printf("%s", buffer);
	CHECK(0 == strcmp(buffer, "(5.00, 1) (3.00, 2) (6.00, 3) (15.00, 4) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));

	// Liberation
	LL_free_list(&poly1);
	LL_free_list(&poly2);
}

// Test 6 : Plusieurs additions classiques de polynomes
TEST(Poly_addition2)
{
	printf("\n> Test 6 : Additions classiques de polynomes...\n");

	cell_t *poly1;
	cell_t *poly2;
	cell_t *poly3;

	LL_init_list(&poly1);
	LL_init_list(&poly2);
	LL_init_list(&poly3);

	char buffer[1024];

	FILE *file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/add/polyadd1.txt");
	LL_create_list_fromFileName(&poly2, "./files/add/polyadd2.txt");
	LL_create_list_fromFileName(&poly3, "./files/add/polyadd3.txt");

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

	// Liberation
	LL_free_list(&poly1);
	LL_free_list(&poly2);
}

// Test 7 : Addition d'un polynome non nul avec le polynome nul
TEST(Poly_addition3)
{
	printf("\n> Test 7 : Addition d'un polynome nul avec un polynome non nul ...\n");
	cell_t *poly1;
	cell_t *poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	char buffer[1024];

	FILE *file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	// Récupération des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/add/polyadd1.txt");

	// Addition : P1 + P2
	poly_add(&poly1, &poly2);
	LL_save_list_toFile(file, poly1, monom_save2file);

	fclose(file);

	printf("BUFFER == %s\n", buffer);
	CHECK(0 == strcmp(buffer, "(5.00, 1) (3.00, 2) (6.00, 3) "));

	LL_free_list(&poly1);

	file = fmemopen(buffer, 1024, "w");
	LL_create_list_fromFileName(&poly2, "./files/add/polyadd2.txt");

	poly_add(&poly1, &poly2);

	LL_save_list_toFile(file, poly1, monom_save2file);
	fclose(file);

	printf("BUFFER == %s\n", buffer);
	CHECK(0 == strcmp(buffer, "(15.00, 4) (7.00, 5) (5.00, 6) (8.00, 9) (10.00, 10) "));

	LL_free_list(&poly2);
}

// Test 8 : Addition du polynome nul avec le polynome nul
TEST(Poly_addition4)
{
	printf("\n> Test 8 : Addition de deux polynomes nuls...\n");
	cell_t *poly1;
	cell_t *poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	poly_add(&poly1, &poly2);

	CHECK(NULL == poly1);

	poly_add(&poly2, &poly1);

	CHECK(NULL == poly2);
}

// Test 9 : Addition d'un polynome avec un autre polynome dont les coefficients sont opposés
TEST(Poly_addition5)
{
	printf("\n> Test 9 : Addition de deux polynomes de signes opposés (P2 = - P1)...\n");
	cell_t *poly1;
	cell_t *poly2;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	// Creation poly1
	LL_create_list_fromFileName(&poly1, "./files/add/polyadd5.txt");

	// Creation poly2
	LL_create_list_fromFileName(&poly2, "./files/add/polyadd6.txt");

	// Addition : poly1 + poly2
	poly_add(&poly1, &poly2);

	// Verification : poly1 + poly2 = poly1 - poly1 = null
	CHECK(NULL == poly1);
}

// Test 10 : Calcul du produit de deux polynomes
TEST(Poly_produit)
{
	printf("\n> Test 10 : Produit classique de deux polynomes non nuls...\n");

	cell_t *prod;
	cell_t *poly1;
	cell_t *poly2;
	cell_t *prod2;

	LL_init_list(&prod);
	LL_init_list(&poly1);
	LL_init_list(&poly2);

	char buffer[1024];

	FILE *file = fmemopen(buffer, 1024, "w");
	REQUIRE(NULL != file);

	// Recuperation des polynôme via. fichiers
	LL_create_list_fromFileName(&poly1, "./files/prod/polyprod1.txt");
	LL_create_list_fromFileName(&poly2, "./files/prod/polyprod2.txt");

	// P1 * P2
	prod = poly_prod(poly1, poly2);

	LL_save_list_toFile(file, prod, monom_save2file);

	LL_free_list(&prod);
	LL_free_list(&poly1);
	LL_free_list(&poly2);

	fclose(file);
	CHECK(0 == strcmp(buffer, "(1.00, 4) (3.00, 6) "));

	// P1 * P1
	file = fmemopen(buffer, 1024, "w");
	// LL_free_list(&poly1);
	LL_create_list_fromFileName(&poly1, "./files/prod/polyprod1.txt");

	// LL_free_list(&prod);
	prod2 = poly_prod(poly1, poly1);
	prod = poly_prod(poly1, prod2);
	LL_free_list(&prod2);
	LL_save_list_toFile(file, prod, monom_save2file);
	fclose(file);

	CHECK(0 == strcmp(buffer, "(1.00, 6) (9.00, 8) (27.00, 10) (27.00, 12) "));

	// Liberation
	LL_free_list(&poly1);
	LL_free_list(&poly2);
	LL_free_list(&prod);
}

// Test 11: Produit avec un polynome NULL => polynome NULL
TEST(Poly_produit0)
{
	printf("\n> Test 11 : Produit d'un polynome non nul avec un polynome nul...\n");
	cell_t *poly1;
	cell_t *poly2;
	cell_t *res0;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	// Creation poly1
	LL_create_list_fromFileName(&poly1, "./files/add/polyadd5.txt");

	// P1 = P1 * P2 avec P2 NULL
	res0 = poly_prod(poly1, poly2);

	// Verification
	CHECK(NULL == res0);

	// Libération
	LL_free_list(&poly1);
	LL_free_list(&poly2);
	free(res0);
}

// Test 12 : Produit de deux polynomes nuls
TEST(Poly_produit0_v2)
{
	printf("\n> Test 12 : Produit d'un polynome non nul avec un polynome nul...\n");
	cell_t *poly1;
	cell_t *poly2;
	cell_t *res0;

	LL_init_list(&poly1);
	LL_init_list(&poly2);

	// P1 = P1 * P2 avec P2 NULL
	res0 = poly_prod(poly1, poly2);

	// Verification
	CHECK(NULL == res0);
}

// Test 13: Ecriture d'un polynome dans un fichier
TEST(LL_save_list_toFileName)
{
	printf("\n> Test 13 : Ecriture d'un polynome dans un fichier...\n");
	cell_t *poly;
	char buffer[1024];
	FILE *file;

	LL_init_list(&poly);

	// Creation du polynôme
	LL_create_list_fromFileName(&poly, "./files/derivation/poly.txt");

	// Ecriture du polynôme dans un fichier
	LL_save_list_toFileName(poly, "./files/saves/polybackupfilename1.txt", monom_save2file);
	file = fopen("./files/saves/polybackupfilename1.txt", "r");

	fgets(buffer, 1024, file);

	printf("BUFFER = %s\n", buffer);
	fclose(file);
	CHECK(0 == strcmp(buffer, "(5.00, 0) (8.00, 1) (15.00, 2) (24.00, 3) (15.00, 4) "));

	// Liberation
	LL_free_list(&poly);
}
END_TEST_GROUP(polynomial)

int main(void)
{
	RUN_TEST_GROUP(polynomial);
	return EXIT_SUCCESS;
}
