#include<stdio.h>
#include<stdlib.h>


//___________________________________DEFINITION DES STRUCTURE DE DONNEES______________________________________//

	//structure de données d'une carte
	typedef struct carte{
		int couleur; //0 pour le noir, 1 pour le rouge
	 	char numero; //le numéro de la carte (A,2,3...J,Q,K)
	    int type;  //0 pour carreaux, 1 pour piques, 2 pour coeur & 3 pour tréfle		
	}Carte;
	
	//structure de données d'une pile
	typedef struct casePile{
		Carte *carte;
		struct casePile *next;
	}CasePile;
	
	//fonction pour créer une nouvelle carte		
	Carte* createCarte(int numero, int type){
		Carte *newElement;
		newElement=(Carte*)malloc(sizeof(Carte));
		newElement->numero = numero;
		newElement->type= type;
		return newElement;
	}
	
	//fonction pour créer un élément de la pile de la case des cartes
	CasePile* createCasePile(Carte *carte){
		CasePile *newElement;
		newElement=(CasePile*)malloc(sizeof(CasePile));
		newElement->carte=carte;
		newElement->next=NULL;
		return newElement;
	}
	
	//fonction pour tester si la pile est vide
	int estVide(CasePile *pile){
		if(pile==NULL) return 1;
		return 0;
	}
	
	//fonction pour ajouter une carte dans la pile (case)
	void push(CasePile **pile, Carte *carte){
		 CasePile *newElement;
		 newElement=createCasePile(carte);
		 newElement->next=*pile;
		 *pile=newElement;
	}
	
	//fonction pour supprimer une carte de la pile (case)
	CasePile* pop(CasePile **pile){
		if(*pile==NULL) return NULL;
		CasePile *tmp;
		tmp=*pile;
		*pile=(*pile)->next;
		free(tmp);
	}
	
	//fonction pour retourner la tete (carte) de la pile
	Carte* peek(CasePile *pile){
	if(pile==NULL)return NULL;
	 return pile->carte;
	}
	
	 //la structure de donnée de la zone 1 : la zone Centrale du jeu
	typedef struct{
	 CasePile *tab[8];
	}Zone1;
	
	//la structure de donnée de la zone 2 : la zone de gauche pour reserver les carte
	typedef struct{
		CasePile *tab[4];
	}Zone2;
	
	 //la structure de donnée de la zone 3 : la zone de droite pour le Rangement des cartes
	typedef struct colonne_rangement{
		CasePile *tab[4];
	}Zone3;
	
	void createZone1(){
		Zone1 *zone1 = (Zone1*)malloc(sizeof(Zone1));
		int i;
		for(i=0;i<8;i++){
			zone1->tab[i] = NULL;
		}
	}
	
	void createZone2(){
		Zone2 *zone2 = (Zone2*)malloc(sizeof(Zone2));
		int i;
		for(i=0;i<4;i++){
			zone2->tab[i] = NULL;
		}
	}
	
	void createZone3(){
		Zone3 *zone3 = (Zone3*)malloc(sizeof(Zone3));
		int i;
		for(i=0;i<4;i++){
			zone3->tab[i] = NULL;
		}
	}
	
	//___________________________________DEFINITION DES FONCTIONS______________________________________//
						// __________________Initialisation___________________
						
	Carte *createCartes(){
		static Carte cartes[52];
		char type[13]={'A','2','3','4','5','6','7','8','9','0','J','Q','K'};
		int i;
		for (i=0;i<13;i++)
	     {
	     	cartes[i].type = 0; //0 pour carreaux
	     	cartes[i].couleur = 1;  //1 pour le rouge
	     	cartes[i].numero = type[i]; //affectation des numéros des cartes
	     }
	     for (i=13;i<26;i++)
	     {
			 cartes[i].type = 1; //1 pour piques
			 cartes[i].couleur = 0;  //0 pour le noir
			 cartes[i].numero = type[i-13];//affectation des numéros des cartes
	     }
	     for (i=26;i<39;i++)
	     {
			 cartes[i].type = 2; //2 pour coeur 
			 cartes[i].couleur = 1;  //1 pour le rouge
			 cartes[i].numero = type[i-26];//affectation des numéros des cartes
	     }
	     for (i=39;i<52;i++)
	     {
			 cartes[i].type = 3; //3 pour tréfle
			 cartes[i].couleur = 0;  //0 pour le noir
			 cartes[i].numero = type[i-39];//affectation des numéros des cartes
	     }
	     
	     return cartes;
	}
	
	void initialisation(Zone1 *zone1, Zone2 *zone2,Zone3 *zone3){
		Carte *cartes= createCartes();
		int i,j,k,tmp=0;
		createZone1();
		createZone2();
		createZone3();
		
	    for (i=0;i<4;i++)
	     {
			 zone2->tab[i] = (CasePile*) malloc(sizeof(CasePile));
			 zone3->tab[i] = (CasePile*) malloc(sizeof(CasePile));
	     }
	      
        for(i=0;i<4;i++)
        {
	        for (j=0;j<7;j++)
	        {
				push(&zone1->tab[i],cartes+j);
		        tmp = j;
	        }
        }
        
        for(i=4;i<8;i++)
        {
	        for (j=0;j<6;j++)
	        {
	           push(&zone1->tab[i],cartes+tmp+j);
	        }
        }
	}
	
	
	int main(){
		Carte *cartes= createCartes();
		int i;

        Zone1 *zone1; 
		Zone2 *zone2;
		Zone3 *zone3;
		
        initialisation(zone1,zone2,zone3);
        
		return 0;
	}
	
