/**
 * @file linkedList_main.c
 * @brief Programme pour les tests de fonctions de gestion de liste chainee
 * ATTENTION : Il faut creer autant de tests que les cas d'execution pour chaque fonction a tester
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"
#include "teZZt.h"

BEGIN_TEST_GROUP(linkedList)

TEST(monom_degree_cmp) 
{
	monom_t v1 = {5.11, 7};
	monom_t v2 = {3., 5};
	monom_t v3 = {5.25, 7};

	printf("\nComparaison des monomes : \n");

	CHECK( monom_degree_cmp(&v1, &v2) > 0 );
	CHECK( monom_degree_cmp(&v2, &v1) < 0 );
	CHECK( monom_degree_cmp(&v1, &v3) == 0 );
}

TEST(monom_save2file) 
{
	monom_t v = {5., 7};

	// creation du flux de texte => buffer
	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	monom_save2file(file, &v);

	fclose(file);

	CHECK( 0 == strcmp(buffer, "(5.00, 7) ") ); 
}


TEST(LL_init_list) 
{
	cell_t * list;

	printf("\nInitialization of the linked list : \n");
	LL_init_list(&list);

	REQUIRE ( list == NULL );
}

// test de creation de cellule
TEST(LL_create_cell) 
{ 
	cell_t * new = NULL;
	monom_t  m1  = {3.245, 17};

	printf("\nCreate a new cell (3.245, 17) : \n");
	new = LL_create_cell(&m1);
	REQUIRE ( NULL != new );
	CHECK ( NULL == new->next );

	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	monom_save2file(file, &(new->val));
	fclose(file);
	CHECK( 0 == strcmp(buffer, "(3.25, 17) " ));

	// Libération
	free(new);
}

// test d'insertion de cellule - liste a une cellule
TEST(LL_add_cell1) 
{ 
	cell_t * list = NULL;
	cell_t *  new = NULL;
	monom_t m1    = {3.45, 17};

	printf("\nAdd a cell to a linked list : \n");

	new = LL_create_cell(&m1);
	REQUIRE ( new != NULL );

	LL_add_cell(&list, new);
	CHECK( list == new ); 

	CHECK( list->val.coef   == 3.45 );  // 3.45 est une valeur non approchee
	CHECK( list->val.degree == 17 );  
	CHECK( list->next == NULL );

	// Libération
	LL_free_list(&list);
}


// test d'insertion de cellule - liste a deux cellules
TEST(LL_add_cell2) 
{ 
	cell_t * list = NULL;
	cell_t * new  = NULL;
	monom_t m1    = {3.45, 17};
	monom_t m2    = {25.8, 9};

	printf("\nAdd two cells to a linked list : \n");

	new = LL_create_cell(&m1);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new ); 

	new = LL_create_cell(&m2);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new ); 

	// tester les valeurs de la liste
	CHECK( list->val.coef   == 25.8 );
	CHECK( list->val.degree == 9 	);
	CHECK( list->next->val.coef   == 3.45);
	CHECK( list->next->val.degree == 17);
	
	// Libération de la liste
	LL_free_list(&list);
	CHECK( NULL == list );
}


// test d'insertion de cellule - liste a trois cellules
TEST(LL_add_cell3) 
{ 
	cell_t *list = NULL;
	cell_t *new = NULL;
	monom_t m1 = {3.245, 17};
	monom_t m2 = {25.8, 9};
	monom_t m3 = {12.4, 3};

	printf("\nAdd three cells to a linked list : \n");

	new = LL_create_cell(&m1);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new ); 

	new = LL_create_cell(&m2);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new ); 

	// ajouter le m3 en tete de la liste
	new = LL_create_cell(&m3);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new );

	// tester les valeurs de la liste
	CHECK( list->val.coef   == 12.4 );
	CHECK( list->val.degree == 3 	);
	CHECK( list->next->val.coef   == 25.8);
	CHECK( list->next->val.degree == 9);
	CHECK( (list->next)->next->val.coef == 3.245);
	CHECK( (list->next)->next->val.degree == 17);
	

	// Libération
	LL_free_list(&list);
	CHECK( NULL == list );

}


// test pour la creation d'un polynome a partir d'un fichier - exemple
TEST(LL_create_list_fromFileName0) 
{
	cell_t * list;

	printf("\nCreate a linked list from file name0: \n");

	LL_create_list_fromFileName(&list, "notExist.txt");
	CHECK( NULL == list );
}

// Création d'un polynôme à partir d'un fichier
TEST(LL_create_list_fromFileName) 
{
	printf("\n Test : LL_save_list_fromFileName... \n");	
	cell_t * list;
	LL_init_list(&list);

	// Initialisation de la liste
	char * name = "./files/polynomial1.txt";

	// Création de la liste à partir du fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, name);
	CHECK( NULL != list );

	// Vérification de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	//LL_print_stdout(&list);

	// Libération
	LL_free_list(&list);
}


// Ecriture d'un polynôme sur un flux de sortie
TEST(LL_save_list_toFile) 
{
	printf("\n Test : LL_save_list_toFile... \n");	
	cell_t * list;

	// Initialisation
	LL_init_list(&list);
	CHECK( NULL == list );

	LL_create_list_fromFileName(&list, "./files/polynomial1.txt");
	CHECK( NULL != list);

	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE( NULL != file );

	LL_save_list_toFile(file, list, &monom_save2file);

	fclose(file);
	CHECK (0 == strcmp(buffer, "(5.00, 2) (17.00, 3) (10.00, 6) "));
	printf("buffer = %s\n", buffer);

	// Libération
	LL_free_list(&list);
}


TEST(LL_search_prev) 
{ 
	printf("\n Test : LL_search_prev... \n");
	cell_t * list;

	// Initialisation
	LL_init_list(&list);
	CHECK ( NULL == list);

	// Création de la liste via. un fichier
	LL_create_list_fromFileName(&list, "./files/polynomial3.txt");
	CHECK (NULL != list);

	CHECK (list->val.coef   == 2);
	CHECK (list->val.degree == 1);

	// Recherche de l'élément précédent le monôme {5., 6}
	monom_t m = {5, 6};
	cell_t ** previous = LL_search_prev(&list, &m, &monom_degree_cmp);

	CHECK ((*previous)->val.degree == 5);
	CHECK ((*previous)->val.coef   == 3);

	// Libération
	LL_free_list(&list);
}

// Test d'insertion de cellule - liste a n cellules
TEST(LL_add_cell_n) 
{ 
	printf("\n Test : LL_add_cell_n ...\n");
	cell_t *list;

	LL_init_list(&list);
	CHECK ( NULL == list);

	// Création d'une liste à n cellules
	LL_create_list_fromFileName(&list, "./files/polynomialn.txt");
	CHECK (NULL != list);

	// TO DO
	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Sauvegarde de la liste dans le buffer
	LL_save_list_toFile(file, list, &monom_save2file);

	// Test écriture de la liste à n cellules dans le fichier
	fclose(file);

	CHECK( 0 == strcmp(buffer, "(185.50, 1) (584.20, 3) (574.20, 6) (457.20, 12) ")); 
	//LL_print_stdout(&list);
	printf("buffer : %s", buffer);

	
	/// Ajout d'un monôme dans la liste à n éléments
	monom_t   m = {5, 2};
	cell_t *  cell = LL_create_cell(&m);
	LL_add_cell(LL_search_prev(&list, &m, &monom_degree_cmp), cell);

	
	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Test écriture
	LL_save_list_toFile(file, list, &monom_save2file);

	// Test écriture de la liste à n cellules dans le fichier
	fclose(file);
	CHECK( 0 == strcmp(buffer, "(185.50, 1) (5.00, 2) (584.20, 3) (574.20, 6) (457.20, 12) ")); 
	//LL_print_stdout(&list);

	// utiliser LL_save_list_toFile pour comparer la valeur de la liste
	// et LL_free_list

	LL_free_list(&list);
	CHECK (NULL == list);

}


// Test suppression d'un élément
TEST(LL_del_cell) 
{
	printf("\n Test : LL_del_cell ...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Création de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/polynomial1.txt");
	CHECK ( NULL != list);

	// Vérification des éléments de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	// Recherche du monôme m = {17, 3}
	monom_t m = {17, 3};
	cell_t ** previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Suppression
	LL_del_cell(previous_cell);

	// Affichage 
	//LL_print_stdout(&list);

	// Recherche du monôme m = {10, 6}
	m.coef   = 10; 
	m.degree = 6;
	previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Suppression
	LL_del_cell(previous_cell);

	// Affichage 
	//LL_print_stdout(&list);

	// Cas : monôme n'appartenant pas à la liste
	m.coef   = 500;
	m.degree = 84;

	previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);
	CHECK (&list == previous_cell);
	LL_del_cell(previous_cell);

	// Libération
	LL_free_list(&list);

}

// Libération de la liste
TEST(LL_free_list) 
{
	printf("\n Test : LL_free_list ...\n");
	cell_t * list;

	LL_init_list(&list);
	CHECK (NULL == list);

	// Libération liste vide
	LL_free_list(&list);
	CHECK( NULL == list );

	// Libération liste remplie
	LL_create_list_fromFileName(&list, "./files/polynomial1.txt");
	CHECK (NULL != list);

	LL_free_list(&list);
	CHECK(NULL == list);
}


// BONUS - 3eme Seance
TEST(LL_save_list_toFileName) 
{ 
	printf("\n Test : LL_save_list_toFileName ...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Création de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/polynomial1.txt");

	// Vérification des éléments de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	// Sauvegarde de la liste dans le fichier dont le nom est spécifié
	char * filename = "./files/polynomial1backup.txt";
	LL_save_list_toFileName(list, filename, &monom_save2file);

	// Libération
	LL_free_list(&list);
}


END_TEST_GROUP(linkedList)

int main(void) {
	RUN_TEST_GROUP(linkedList);
	return EXIT_SUCCESS;
}
