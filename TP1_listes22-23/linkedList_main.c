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

// Test 0 : Test comparateur de degre (monome)
TEST(monom_degree_cmp) 
{
	printf("\n> Test 0 : monom_degree_cmp (comparateur de degre)...\n");
	monom_t v1 = {5.11, 7};
	monom_t v2 = {3., 5};
	monom_t v3 = {5.25, 7};

	printf("\nComparaison des monomes : \n");

	CHECK( monom_degree_cmp(&v1, &v2) > 0 );
	CHECK( monom_degree_cmp(&v2, &v1) < 0 );
	CHECK( monom_degree_cmp(&v1, &v3) == 0 );
}

// Test 1 : Sauvegarde d'un monome dans un fichier
TEST(monom_save2file) 
{
	printf("\n> Test 1 : monom_save2file (sauvegarde monome dans un fichier)...\n");
	monom_t v = {5., 7};

	// creation du flux de texte => buffer
	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Appel monom_save2file
	monom_save2file(file, &v);

	// Fermeture du fichier
	fclose(file);

	CHECK( 0 == strcmp(buffer, "(5.00, 7) ") ); 
}

// Test 2 : Initialisation d'une liste chainee
TEST(LL_init_list) 
{
	printf("\n> Test 2 : Initiliasation liste chainee...\n");
	cell_t * list;

	printf("\nInitialization of the linked list : \n");
	LL_init_list(&list);

	REQUIRE ( list == NULL );
}

// Test 3 : Creation d'une cellule
TEST(LL_create_cell) 
{
	printf("\n> Test 3 : Creation cellule...\n");
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

	// Liberation
	free(new);
}

// Test 4 : Insertion de cellule - liste a 1 cellule
TEST(LL_add_cell1) 
{
	printf("\n> Test 4 : Insertion de cellule - liste a 1 cellule...\n");
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

	// Liberation
	LL_free_list(&list);
}


// Test 5 : Insertion de cellule - liste a 2 cellules
TEST(LL_add_cell2) 
{
	printf("\n> Test 5 : Insertion de cellule - liste a 2 cellules...\n"); 
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

	// Tester les valeurs de la liste
	CHECK( list->val.coef   == 25.8 );
	CHECK( list->val.degree == 9 	);
	CHECK( list->next->val.coef   == 3.45);
	CHECK( list->next->val.degree == 17);
	
	// Liberation de la liste
	LL_free_list(&list);
	CHECK( NULL == list );
}


// Test 6 : Insertion de cellule - liste a 3 cellules
TEST(LL_add_cell3) 
{
	printf("\n> Test 6 : Insertion de cellule - liste a 3 cellules...\n");
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

	// Ajouter le m3 en tete de la liste
	new = LL_create_cell(&m3);
	REQUIRE ( new != NULL );
	LL_add_cell(&list, new);
	CHECK( list == new );

	// Tester les valeurs de la liste
	CHECK( list->val.coef   == 12.4 );
	CHECK( list->val.degree == 3 	);
	CHECK( list->next->val.coef   == 25.8);
	CHECK( list->next->val.degree == 9);
	CHECK( (list->next)->next->val.coef == 3.245);
	CHECK( (list->next)->next->val.degree == 17);
	

	// Liberation
	LL_free_list(&list);
	CHECK( NULL == list );

}


// Test 7 : Creation d'un polynome a partir d'un fichier inexistant - exemple
TEST(LL_create_list_fromFileName0) 
{
	printf("\n> Test 7 : Creation d'un polynome a partir d'un fichier inexistant...\n");
	cell_t * list;

	printf("\nCreate a linked list from file name0: \n");

	LL_create_list_fromFileName(&list, "notExist.txt");
	CHECK( NULL == list );
}

// Test 8 : Creation d'un polynôme a partir d'un fichier
TEST(LL_create_list_fromFileName) 
{
	printf("\n> Test 8 : Creation d'un polynome a partir d'un fichier...\n");
	cell_t * list;
	LL_init_list(&list);

	// Initialisation de la liste
	char * name = "./files/creation/polynomial1.txt";

	// Creation de la liste à partir du fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, name);
	CHECK( NULL != list );

	// Verification de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	// Liberation
	LL_free_list(&list);
}


// Test 9 : Ecriture d'un polynome sur un flux de sortie
TEST(LL_save_list_toFile) 
{
	printf("\n> Test 9 : Ecriture d'un polynome sur un flux de sortie...\n");	
	cell_t * list;

	// Initialisation
	LL_init_list(&list);
	CHECK( NULL == list );

	LL_create_list_fromFileName(&list, "./files/creation/polynomial1.txt");
	CHECK( NULL != list);

	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE( NULL != file );

	LL_save_list_toFile(file, list, &monom_save2file);

	fclose(file);
	CHECK (0 == strcmp(buffer, "(5.00, 2) (17.00, 3) (10.00, 6) "));
	printf("buffer = %s\n", buffer);

	// Liberation
	LL_free_list(&list);
}


// Test 10 : Recherche d'une cellule precedente
TEST(LL_search_prev) 
{ 
	printf("\n> Test 10 : Recherche classique d'une cellule precedente...\n");
	cell_t * list;

	// Initialisation
	LL_init_list(&list);
	CHECK ( NULL == list);

	// Creation de la liste via. un fichier
	LL_create_list_fromFileName(&list, "./files/creation/polynomial3.txt");
	CHECK (NULL != list);

	CHECK (list->val.coef   == 2);
	CHECK (list->val.degree == 1);

	// Recherche de l'element precedent le monôme {5., 6}
	monom_t m = {5, 6};
	cell_t ** previous = LL_search_prev(&list, &m, &monom_degree_cmp);

	CHECK ((*previous)->val.degree == 5);
	CHECK ((*previous)->val.coef   == 3);

	// Liberation
	LL_free_list(&list);
}

// Test 11 : Recherche d'un monome qui n'existe pas dans la liste (renverra la derniere cellule)
TEST(LL_search_prev) 
{ 
	printf("\n> Test 11 : Recherche d'un monome inexistant dans la liste...\n");
	cell_t * list;

	// Initialisation
	LL_init_list(&list);
	CHECK ( NULL == list);

	// Creation de la liste via. un fichier
	LL_create_list_fromFileName(&list, "./files/creation/polynomial3.txt");
	CHECK (NULL != list);

	CHECK (list->val.coef   == 2);
	CHECK (list->val.degree == 1);

	// Recherche de l'element precedent le monôme {5., 6}
	monom_t m_inexistant = {15, 60};
	cell_t ** previous = LL_search_prev(&list, &m_inexistant, &monom_degree_cmp);

	// Renvoie le dernier monome
	CHECK ((*previous)->val.degree == 15);
	CHECK ((*previous)->val.coef   == 7);

	// Liberation
	LL_free_list(&list);
}

// Test 12 : Recherche de la tête de la liste (renverra la tete - car la liste est un pointeur sur la tete)
TEST(LL_search_prev_head){
	printf("\n> Test 12 : Recherche de la tete de liste...\n");
	cell_t * list;

	// Initialisation
	LL_init_list(&list);

	// Creation de la liste via. un fichier
	LL_create_list_fromFileName(&list, "./files/creation/polynomial3.txt");

	monom_t m = {2, 1};
	cell_t ** previous_head = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Verification
	CHECK ((*previous_head)->val.degree == 1);
	CHECK ((*previous_head)->val.coef   == 2);

	// Liberation
	LL_free_list(&list);
}


// Test 13 : Recherche de la fin de la liste (renverra l'avant-dernier element)
TEST(LL_search_prev_end){
	printf("\n> Test 13 : Recherche du precedent du dernier element...\n");
	cell_t * list;

	// Initialisation
	LL_init_list(&list);

	// Creation de la liste via. un fichier
	LL_create_list_fromFileName(&list, "./files/creation/polynomial3.txt");

	monom_t m = {7, 15};
	cell_t ** previous_head = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Verification
	CHECK ((*previous_head)->val.degree == 7);
	CHECK ((*previous_head)->val.coef   == 4);

	// Liberation
	LL_free_list(&list);

}	

// Test 14 : Insertion de cellule - liste a n cellules
TEST(LL_add_cell_n) 
{ 
	printf("\n> Test 12 : Insertion de cellule - liste a n cellules");
	cell_t *list;

	LL_init_list(&list);
	CHECK ( NULL == list);

	// Creation d'une liste à n cellules
	LL_create_list_fromFileName(&list, "./files/creation/polynomialn.txt");
	CHECK (NULL != list);

	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Sauvegarde de la liste dans le buffer
	LL_save_list_toFile(file, list, &monom_save2file);

	// Test ecriture de la liste à n cellules dans le fichier
	fclose(file);

	CHECK( 0 == strcmp(buffer, "(185.50, 1) (584.20, 3) (574.20, 6) (457.20, 12) ")); 
	
	/// Ajout d'un monôme dans la liste à n elements
	monom_t   m = {5, 2};
	cell_t *  cell = LL_create_cell(&m);
	LL_add_cell(LL_search_prev(&list, &m, &monom_degree_cmp), cell);

	
	file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Test ecriture
	LL_save_list_toFile(file, list, &monom_save2file);

	// Test ecriture de la liste à n cellules dans le fichier
	fclose(file);
	CHECK( 0 == strcmp(buffer, "(185.50, 1) (5.00, 2) (584.20, 3) (574.20, 6) (457.20, 12) ")); 


	// utiliser LL_save_list_toFile pour comparer la valeur de la liste
	// et LL_free_list

	// Liberation
	LL_free_list(&list);
	CHECK (NULL == list);
}

// Test 15 : Ajout d'une cellule dont le degre est deja present dans la liste
TEST(LL_add_same_cell)
{
	printf("\n> Test 13 : Ajout d'une cellule dont le degre est deja present dans la liste...\n"); 
	cell_t * list = NULL;

	// Initialisation
	LL_init_list(&list);

	// Creation de la liste
	LL_create_list_fromFileName(&list, "./files/add/polyadd6.txt");

	CHECK( list->val.coef   == -5);
	CHECK( list->val.degree ==  1);

	// Monome a ajouter
	monom_t m = {-5, 1};

	// Creation de la nouvelle cellule a ajouter
	cell_t * new;
	new = LL_create_cell(&m);
	LL_print_stdout(&list);
	LL_add_cell(LL_search_prev(&list, &m, &monom_degree_cmp), new);

	// Teste les valeurs de la liste apres ajout
	CHECK( list->val.coef   == -10);
	CHECK( list->val.degree ==  1);
	

	// Liberation
	LL_free_list(&list);
	free(new);
}

// Test 16 : Ajout cellule dans une liste vide
TEST(LL_empty_list){
	printf("\n> Test 16 : Ajout d'une cellule dans une liste vide..\n"); 
	cell_t * list = NULL;

	// Initialisation
	LL_init_list(&list);

	// Creation de la cellule
	monom_t m = {5.32, 10};
	cell_t * new;

	new = LL_create_cell(&m);

	LL_add_cell(&list, new);

	// Teste les valeurs de la liste apres ajout
	CHECK( list->val.coef   == 5.32);
	CHECK( list->val.degree ==  10);
	CHECK( list->next 		== NULL); // 1 element dans la liste seulement
	

	// Liberation
	LL_free_list(&list);

}

// Test 17 : Suppression classique d'un element
TEST(LL_del_cell) 
{
	printf("\n Test 14 : Suppression classique d'un element...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Creation de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/creation/polynomial1.txt");
	CHECK ( NULL != list);

	// Verification des elements de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	// Recherche du monome m = {17, 3}
	monom_t m = {17, 3};
	cell_t ** previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Suppression
	LL_del_cell(previous_cell, &list);

	// Recherche du monôme m = {10, 6}
	m.coef   = 10; 
	m.degree = 6;
	previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);

	// Suppression
	LL_del_cell(previous_cell, &list);

	// Cas : monome n'appartenant pas a la liste
	m.coef   = 500;
	m.degree = 84;

	previous_cell = LL_search_prev(&list, &m, &monom_degree_cmp);
	CHECK (&list == previous_cell);
	LL_del_cell(previous_cell, &list);

	// Liberation
	LL_free_list(&list);

}

// Test 18 :  Suppression d'une cellule inexistante
TEST(LL_del_cell_unexisted)
{
	printf("\n Test 15 : Suppression d'une cellule inexistante...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Creation de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/creation/polynomial1.txt");

	// Monome qui n'exisgte pas dans la liste
	monom_t m = {1, 10};

	// Creation d'une cellule inexistante dans la liste
	cell_t * new = LL_create_cell(&m);

	LL_del_cell(&new, &list);
	

	CHECK ( NULL != list						      );
	CHECK ( 5 	 == list->val.coef					  );
	CHECK ( 2    == list->val.degree				  );
	CHECK (17    == (list->next)->val.coef			  );
	CHECK ( 3    == (list->next)->val.degree		  );
	CHECK (10    == ((list->next)->next)->val.coef    );
	CHECK ( 6    == ((list->next)->next)->val.degree  );

	// Liberation
	LL_free_list(&list);
	free(new);
}

// Test 19 : Liberation de la liste
TEST(LL_free_list) 
{
	printf("\n> Test 16 : Liberation de la liste chainee...\n");
	cell_t * list;

	LL_init_list(&list);
	CHECK (NULL == list);

	// Liberation liste vide
	LL_free_list(&list);
	CHECK( NULL == list );

	// Liberation liste remplie
	LL_create_list_fromFileName(&list, "./files/creation/polynomial1.txt");
	CHECK (NULL != list);

	LL_free_list(&list);
	CHECK(NULL == list);
}


// BONUS - 3eme Seance
// Test 20 : Sauvegarde de la liste dans un fichier (nom du fichier connu)
TEST(LL_save_list_toFileName) 
{ 
	printf("\n> Test 17 : Sauvegarde de la liste dans un fichier (nom du fichier connu)...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Creation de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/creation/polynomial1.txt");

	// Verification des elements de la liste
	CHECK	( list->val.coef   		  		 ==  5);
	CHECK	( list->val.degree 		  		 ==  2);
	CHECK	( list->next->val.coef    		 == 17);
	CHECK	( list->next->val.degree  	     ==  3);
	CHECK	( (list->next)->next->val.coef   == 10);
	CHECK	( (list->next)->next->val.degree ==  6);

	// Sauvegarde de la liste dans le fichier dont le nom est specifie
	char * filename = "./files/saves/polynomial1backup.txt";
	LL_save_list_toFileName(list, filename, &monom_save2file);
	FILE * file1 = fopen(filename, "r");
	char buffer[1024];
	fgets(buffer, 1024, file1);
	fclose(file1);

	char resultat[1024] = "(5.00, 2) (17.00, 3) (10.00, 6) ";
	
	CHECK(0 == strcmp(buffer, resultat));

	// Liberation
	LL_free_list(&list);
}

// Test 21 : Creation d'une liste/polynome a partir d'un fichier contenant de nombreux coefficients nuls 
TEST(LL_create_poly0){
	printf("\n> Test 18 : Creation d'un liste/polynome a partir d'un fichier contenant de nombreux coefficients nuls...\n");
	cell_t * list;

	// Initialisation de la liste
	LL_init_list(&list);
	CHECK (list == NULL);

	// Creation de la liste à partir d'un fichier ("polynomial1.txt")
	LL_create_list_fromFileName(&list, "./files/creation/polynomialfullof0.txt");

	// Creation du flux de texte => buffer
	char buffer[1024];
	FILE * file = fmemopen(buffer, 1024, "w");
	REQUIRE ( NULL != file);

	// Sauvegarde de la liste dans le buffer
	LL_save_list_toFile(file, list, &monom_save2file);
	fclose(file);

	// Verification
	CHECK(0 == strcmp(buffer, "(1.00, 0) (5.00, 1) (6.00, 4) (15.00, 7) (4.00, 7) (1.00, 14) (5.00, 17) "));
	
	// Liberation 
	LL_free_list(&list);
}

END_TEST_GROUP(linkedList)

int main(void) {
	RUN_TEST_GROUP(linkedList);
	return EXIT_SUCCESS;
}
