#include<stdio.h>
#include<stdlib.h>
#include<string.h>
	
	
/* ----------------------------- Jeu du FreeCell par KHAOULA ELMAJNI ------------------------------ */
/*               		    --> définition des structures de données <--      			            */
/*                 			  --> phase 1: initialisation des zones <--                			    */
/*                  			--> création des cartes et zones <--                    		    */
/*                    			--> définition des régles du jeu <--                   			    */
/*                     			   --> déplacement des cartes <--                		            */
/* ------------------------------------------------------------------------------------------------ */

	
	char erreur[180];
	int record = 0;
	//____________________________________________DEFINITION DES STRUCTURE DE DONNEES____________________________________________//
	
	//structure de données d'une carte
	typedef struct carte {
	    char couleur; //N pour le noir, R pour le rouge
	    int numero; //le numéro de la carte (1,2,3...11,12,13)
	    char type;  //C pour carreaux, P pour piques, O pour coeur & T pour tréfle
	} Carte;
	
	//les 52 cartes du jeu (global)
	Carte *cartes;
	
	//structure de données d'une pile
	typedef struct casePile {
	    Carte *carte;
	    struct casePile *next;
	} CasePile;
	
	
	//la structure de donnée de la zone 1 : la zone Centrale du jeu
	typedef struct {
	    CasePile *tab[8];
	} Zone1;
	
	//la structure de donnée de la zone 2 : la zone de gauche pour reserver les carte
	typedef struct {
	    CasePile *tab[4];
	} Zone2;
	
	//la structure de donnée de la zone 3 : la zone de droite pour le Rangement des cartes
	typedef struct {
	    CasePile *tab[4];
	} Zone3;
	
	Zone1 *zone1;
	Zone2 *zone2;
	Zone3 *zone3;
	
	//fonction pour créer une nouvelle carte
	Carte *createCarte(int numero, char type) {
	    Carte *newElement;
	    newElement = (Carte *) malloc(sizeof(Carte));
	    newElement->numero = numero;
	    newElement->type = type;
	    return newElement;
	}
	
	//fonction pour créer un élment de la pile de la case des cartes
	CasePile *createCasePile(Carte *carte) {
	    CasePile *newElement;
	    newElement = (CasePile *) malloc(sizeof(CasePile));
	    newElement->carte = carte;
	    newElement->next = NULL;
	    return newElement;
	}
	
	//fonction pour tester si la pile est vide
	int estVide(CasePile *pile) {
	    if (pile == NULL) return 1;
	    return 0;
	}
	
	//fonction pour ajouter une carte dans la pile (case)
	void push(CasePile **pile, Carte *carte) {
	    CasePile *newElement;
	    newElement = createCasePile(carte);
	    newElement->next = *pile;
	    *pile = newElement;
	}
	
	//fonction pour supprimer une carte de la pile (case)
	void pop(CasePile **pile) {
	    if (*pile == NULL) return;
	    CasePile *tmp;
	    tmp = *pile;
	    *pile = (*pile)->next;
	    free(tmp);
	}
	
	//fonction pour retourner la tete (carte) de la pile
	Carte *peek(CasePile *pile) {
	    if (pile == NULL)return NULL;
	    return pile->carte;
	}
	
	//fonction pour initialiser les 8 piles (cases) de la zone 1
	void createZone1() {
	    zone1 = (Zone1 *) malloc(sizeof(Zone1));
	    int i;
	    for (i = 0; i < 8; i++) {
	        zone1->tab[i] = NULL;
	    }
	}
	
	//fonction pour initialiser les 4 piles (cases) de la zone 2
	void createZone2() {
	    zone2 = (Zone2 *) malloc(sizeof(Zone2));
	    int i;
	    for (i = 0; i < 4; i++) {
	        zone2->tab[i] = NULL;
	    }
	}
	
	//fonction pour initialiser les 4 piles (cases) de la zone 3
	void createZone3() {
	    zone3 = (Zone3 *) malloc(sizeof(Zone3));
	    int i;
	    for (i = 0; i < 4; i++) {
	        zone3->tab[i] = NULL;
	    }
	}
	
	void afficheCarteNull() {
	    printf("[ , , ] ");
	}
	
	void afficheCarte(Carte *carte) {
	    if (carte == NULL) {
	        afficheCarteNull();
	    } else printf("[%d, %c, %c] ", carte->numero, carte->couleur, carte->type);
	}
	
	//___________________________________DEFINITION DES FONCTIONS______________________________________//
	
	// _______________________________Phase 1 : Initialisation du jeu_____________________________
	
	Carte *createCartes() {
	    static Carte cartes[52];
	    int type[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
	    int i;
	    //initialisation des cartes de jeu
	    for (i = 0; i < 13; i++) {
	        cartes[i].type = 'C'; //C pour carreaux
	        cartes[i].couleur = 'R';  //1 pour le rouge
	        cartes[i].numero = type[i]; //affectation des numÃ©ros des cartes
	    }
	    for (i = 13; i < 26; i++) {
	        cartes[i].type = 'P'; //P pour piques
	        cartes[i].couleur = 'N';  //0 pour le noir
	        cartes[i].numero = type[i - 13];//affectation des numÃ©ros des cartes
	    }
	    for (i = 26; i < 39; i++) {
	        cartes[i].type = 'O'; //O pour coeur
	        cartes[i].couleur = 'R';  //R pour le rouge
	        cartes[i].numero = type[i - 26];//affectation des numÃ©ros des cartes
	    }
	    for (i = 39; i < 52; i++) {
	        cartes[i].type = 'T'; //T pour tréfle
	        cartes[i].couleur = 'N';  //N pour le noir
	        cartes[i].numero = type[i - 39];//affectation des numÃ©ros des cartes
	    }
	
	    return cartes;
	}
	
	//fonction qui prépare les zones avant le début du jeu
	void initialisation() {
	    cartes = createCartes();
	    int i, j, tmp = 0;
	    createZone1();
	    createZone2();
	    createZone3();
	
		//affectation des cartes aux 4 premiers piles à gauche(cases de 7 cartes chacune)
	    for (i = 0; i < 4; i++) {
	        for (j = 0; j < 7; j++) {
	            afficheCarte(cartes + tmp);
	            printf("\n");
	            push(&zone1->tab[i], cartes + tmp);
	            tmp += 1;
	        }
	    }
	
		//affectation des cartes aux 4 derniers piles à droite(cases de 6 cartes chacune)
	    for (i = 4; i < 8; i++) {
	        for (j = 0; j < 6; j++) {
	            afficheCarte(cartes + tmp);
	            printf("\n");
	            push(&zone1->tab[i], cartes + tmp);
	            tmp += 1;
	        }
	    }
	}
	
	//_____________________________________________Phase 2 : Jouer__________________________________________________//
	
	//fonction qui retourne une carte de la pile à partir de son indice
	Carte *getCarteDuPile(CasePile *casePile, int index) {
	    CasePile *tmp;
	    int i = 0;
	    for (tmp = casePile; tmp != NULL; tmp = tmp->next) {
	        Carte *carte = tmp->carte;
	        if (i == index) {
	            return carte;
	        }
	        i += 1;
	    }
	    return NULL;
	}
	
	//fonction qui affiche le contenu des zones à chaque mouvement
	void MAJ_Zones() {
	    
	    int i, j;
	    printf("\n___________________________________ZONE 2 :___________________________________\n\n");
	    for (i = 0; i < 4; i++) {
	    	printf("Colonne %d : ",i+1);
	        CasePile *casePile = zone2->tab[i];
	        if (casePile == NULL) {
	            afficheCarteNull();
	        } else {
	            Carte *carte = zone2->tab[i]->carte;
	            afficheCarte(carte);
	        }
	    }
	    
	    printf("\n___________________________________ZONE 3 :___________________________________\n\n");
	    for (i = 0; i < 4; i++) {
	    	printf("Colonne %d : ",i+1);
	        CasePile *casePile = zone3->tab[i];
	        for (j = 0; j < 13; j++) {
	            if (casePile == NULL) {
	                afficheCarteNull();
	            } else {
	                Carte *carte = zone3->tab[i]->carte;
	                afficheCarte(carte);
	            }
	        }
	        printf("\n");
	    }
	
	    printf("\n___________________________________ZONE 1 :___________________________________\n\n");
	    for (i = 0; i < 4; i++) {
	    	printf("Colonne %d : ",i+1);
	        CasePile *casePile = zone1->tab[i];
	        for (j = 0; j < 7; j++) {
	            Carte *carte = getCarteDuPile(casePile, j);
	            afficheCarte(carte);
	        }
	        printf("\n");
	    }
	
	    for (i = 4; i < 8; i++) {
	    	printf("Colonne %d : ",i+1);
	        CasePile *casePile = zone1->tab[i];
	        for (j = 0; j < 6; j++) {
	            Carte *carte = getCarteDuPile(casePile, j);
	            afficheCarte(carte);
	        }
	        printf("\n");
	    }
	}
	
	//	
	int calculerRecord() {
	    return 0;
	}
	
	//fonction qui retourne la carte de la tete d'une pile (colonne) de la zone 1
	Carte *getTetePileZone1(int colonne) {
	    CasePile *Col = zone1->tab[colonne - 1];
	    if (Col != NULL) {
	        Carte *top = peek(Col);
	        return top;
	    } else {
	        strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne est vide !\n");
	        return NULL;
	    }
	}
	
	//fonction qui retourne une carte de la tete d'une pile (colonne) de la zone 2
	Carte *getTetePileZone2(int colonne) {
	    CasePile *Col = zone2->tab[colonne - 1];
	    if (Col != NULL) {
	        Carte *top = peek(Col);
	        return top;
	    } else {
	        strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne est vide !\n");
	        return NULL;
	    }
	}
	
	//fonction qui retourne une carte de la tete d'une pile (colonne) de la zone 3
	Carte *getTetePileZone3(int colonne) {
	    CasePile *Col = zone3->tab[colonne - 1];
	    if (Col != NULL) {
	        Carte *top = peek(Col);
	        return top;
	    } else {
	        strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne est vide !");
	        return NULL;
	    }
	}
	
	//_______________________________________Déplacement d'une carte____________________________________________//
	
	void deplacement(int fromZone, int from, int toZone, int to) {
		
		//---> De la zone1 vers la zone2
	    if (fromZone == 1 && toZone == 2) {
	        Carte *carte = peek(zone1->tab[from - 1]);
	        pop(&zone1->tab[from - 1]);
	        push(&zone2->tab[to - 1], carte);
	        record++;
	        
	    }
	    
	    //---> De la zone1 vers la zone1
		if (fromZone == 1 && toZone == 1) {
	        Carte *carte = peek(zone1->tab[from - 1]);
	        pop(&zone1->tab[from - 1]);
	        push(&zone1->tab[to - 1], carte);
	        record++;
	    }
	    
	    //---> De la zone1 vers la zone3
		if (fromZone == 1 && toZone == 3) {
	        Carte *carte = peek(zone1->tab[from - 1]);
	        pop(&zone1->tab[from - 1]);
	        push(&zone3->tab[to - 1], carte);
	        record++;
	    }
		
		//---> De la zone2 vers la zone3
		if (fromZone == 2 && toZone == 3) {
	        Carte *carte = peek(zone2->tab[from - 1]);
	        pop(&zone2->tab[from - 1]);
	        push(&zone3->tab[to - 1], carte);
	        record++;
	    }
		
		//---> De la zone2 vers la zone2
		if (fromZone == 2 && toZone == 2) {
	        Carte *carte = peek(zone2->tab[from - 1]);
	        pop(&zone2->tab[from - 1]);
	        push(&zone2->tab[to - 1], carte);
	        record++;
	    }
		
		//---> De la zone2 vers la zone1
		if (fromZone == 2 && toZone == 1) {
	        Carte *carte = peek(zone2->tab[from - 1]);
	        pop(&zone2->tab[from - 1]);
	        push(&zone1->tab[to - 1], carte);
	        record++;
	    }
	
	}
	
	//____________________________________________Les Régles du jeu FreeCell ________________________________________//
	
	int deplaceCarte(int ZoneSrc, int de, int ZoneDst, int a) {
	    Carte *bougerDe, *bougerA;
	    //la zone source 1
	    if (ZoneSrc == 1) {
	        bougerDe = getTetePileZone1(de);
	    } else if (ZoneSrc == 2) {
	        bougerDe = getTetePileZone2(de);
	    }
	    
		//la zone de destination 1
	    if (ZoneDst == 1) {
	        bougerA = getTetePileZone1(a);
	    } else if (ZoneDst == 2) {
	        bougerA = getTetePileZone2(a);
	    } else if (ZoneDst == 3) {
	        bougerA = getTetePileZone3(a);
	    }
		
		//si la zone zource 1 et destination 1
	    if (ZoneSrc == 1 && ZoneDst == 1) {
	        if (bougerDe->couleur != bougerA->couleur) {
	            if (bougerA->numero == bougerDe->numero - 1) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : Numéro reçu n'est pas inférieur par 1 !");
	            }
	        } else {
	            strcpy((char *) erreur, "Mouvement pas autorisée : Attention\a : les cartes sont de meme couleur !");
	        }
	    }
	    
	    //si la zone zource 1 et destination 2
	    if (ZoneSrc == 1 && ZoneDst == 2) {
	        CasePile *casePile1 = zone2->tab[0];
	        CasePile *casePile2 = zone2->tab[1];
	        CasePile *casePile3 = zone2->tab[2];
	        CasePile *casePile4 = zone2->tab[3];
	        
	        //test des colonnes d'arrivé
	        if (a == 1) {
	            if (casePile1 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 1 n'est pas vide ");
	            }
	        }
	        if (a == 2) {
	            if (casePile2 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 2 n'est pas vide ");
	            }
	        }
	        if (a == 3) {
	            if (casePile3 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 3 n'est pas vide ");
	            }
	        }
	        if (a == 4) {
	            if (casePile4 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 4 n'est pas vide ");
	            }
	        }
	
	    }
	    
	    //si la zone zource 2 et destination 2
	    if (ZoneSrc == 2 && ZoneDst == 2) {
	        CasePile *casePile1 = zone2->tab[0];
	        CasePile *casePile2 = zone2->tab[1];
	        CasePile *casePile3 = zone2->tab[2];
	        CasePile *casePile4 = zone2->tab[3];
	        
	        //test sur les colonnes
	        if (a == 1) {
	            if (casePile1 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 1 n'est pas vide ");
	            }
	        }
	        if (a == 2) {
	            if (casePile2 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 2 n'est pas vide ");
	            }
	        }
	        if (a == 3) {
	            if (casePile3 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 3 n'est pas vide ");
	            }
	        }
	        if (a == 4) {
	            if (casePile4 == NULL) {
	                return 1;
	            } else {
	                strcpy((char *) erreur, "Mouvement pas autorisée : La Colonne 4 n'est pas vide ");
	            }
	        }
	
	    }
	    
	    //La zone source est 1 || 2 et destination 3
	    if ((ZoneSrc == 1 || ZoneSrc == 2) && ZoneDst == 3) {
	        CasePile *casePile1 = zone3->tab[0];
	        CasePile *casePile2 = zone3->tab[1];
	        CasePile *casePile3 = zone3->tab[2];
	        CasePile *casePile4 = zone3->tab[3];
			
			//test sur les colonnes
	        if (a == 1) {
	            if (casePile1 == NULL) {
	                if (bougerDe->numero == 1) {
	                    return 1;
	                } else {
	                    strcpy((char *) erreur, "La premiere carte doit etre un As (1)! ");
	                }
	            } else {
	                Carte *derCarteCol1 = peek(casePile1);
	                if (derCarteCol1->couleur == bougerDe->couleur) {
	                    if (derCarteCol1->numero == bougerDe->numero - 1) {
	                        return 1;
	                    } else {
	                        strcpy((char *) erreur, "La carte recu n\'est pas superieur par 1 !");
	                    }
	                } else {
	                    strcpy((char *) erreur, "ils n\'ont pas de Meme couleur ! ");
	                }
	            }
	        }
	
	
	        if (a == 2) {
	            if (casePile2 == NULL) {
	                //si un As
	                if (bougerDe->numero == 1) {
	                    return 1;
	                } else {
	                    strcpy((char *) erreur, "La premiere carte doit etre un As(1) ");
	                }
	            } else {
	                Carte *lastCarteInCol2 = peek(casePile2);
	                if (lastCarteInCol2->couleur == bougerDe->couleur) {
	                    if (lastCarteInCol2->numero == bougerDe->numero - 1) {
	                        return 1;
	                    } else {
	                        strcpy((char *) erreur, "La carte recu n\'est pas superieur par 1  !");
	                    }
	                } else {
	                    strcpy((char *) erreur, "les cartes n\'ont pas de Meme couleur ! ");
	                }
	            }
	        }
	
	        if (a == 3) {
	            if (casePile3 == NULL) {
	                //si un As
	                if (bougerDe->numero == 1) {
	                    return 1;
	                } else {
	                    strcpy((char *) erreur, "La premiere carte doit etre un As(1) ");
	                }
	            } else {
	                Carte *lastCarteInCol3 = peek(casePile3);
	                if (lastCarteInCol3->couleur == bougerDe->couleur) {
	                    if (lastCarteInCol3->numero == bougerDe->numero - 1) {
	                        return 1;
	                    } else {
	                        strcpy((char *) erreur, "La carte recu n\'est pas superieur par 1 ");
	                    }
	                } else {
	                    strcpy((char *) erreur, "les cartes n\'ont pas de Meme couleur ! ");
	                }
	            }
	        }
	
	        if (a == 4) {
	            if (casePile4 == NULL) {
	                //si un As
	                if (bougerDe->numero == 1) {
	                    return 1;
	                } else {
	                    strcpy((char *) erreur, "La premiere carte doit etre un As(1) ");
	                }
	            } else {
	                Carte *lastCarteInCol4 = peek(casePile4);
	                if (lastCarteInCol4->couleur == bougerDe->couleur) {
	                    if (lastCarteInCol4->numero == bougerDe->numero - 1) {
	                        return 1;
	                    } else {
	                        strcpy((char *) erreur, "La carte recu n\'est pas superieur par 1 ");
	                    }
	                } else {
	                    strcpy((char *) erreur, "les cartes n\'ont pas de Meme couleur ! ");
	                }
	            }
	        }
	
	    }
	    if (ZoneSrc == 3) {
	        strcpy((char *) erreur, "Pas possible!!! \nLes cartes de la zone 3 à ne pas bouger !!! ");
	    }
	    return 0;
	}
	
	//______________________________________________Phase 3 : terminer_______________________________________________//
	
	//fonction pour tester si le jeu est terminé ou non (si toutes les piles de la zone3 sont pleines par des cartes rangées 
	int Jeufini() {
		int i,j,calc=0;
		for(i=0;i<4;i++){
			for(j=0;j<13;i++)
				if(zone3->tab[j]->carte != NULL){
					calc=1;
				}
				else {
					calc=0;
					break;
				}	
		}
		if(calc){
			printf("Yuppy!! Vous avez Gagnez !!!(-;");
    		printf("\t\t..... Bien jouer .....\t\t");
    		return 1;
		}
		else{			
	    	return 0;
		}

	}
	
	int blocage() {
		//if(deplaceCarte())
	    return 0;
	}
	
	//______________________________________________Début du programme______________________________________//
	
	int main() {
		printf("\n___________________________________BONJOUR CHER VISITEUR : BIENVENUE DANS MA VERSION FREECELL___________________________________\n\n");
	    initialisation();
	    while (1) {
	        
	        MAJ_Zones();
	        printf("\n________________________________________Indication :___________________________________\n");
	        printf("Entrer le numero de la zone source || destination : \n\tchoix : 1  ===> zone 1\n\tchoix : 2  ===> zone 2\n\tchoix : 3  ===> zone 3\n\n");
	        printf("Entrer le numero de la colonne source || destination : \n\tchoix : 1  ===> colonne 1\n\tchoix : 2  ===> colonne 2\n\tchoix : 3  ===> colonne 3 \n\tchoix : 4  ===> colonne 4\n\n");	        
			int colSrc, colDst, zoneSrc, zoneDst;
	        printf("Entrez le numero de la zone source :\n");
	        scanf("%d", &zoneSrc);
	        printf("Entrez le numero de la colonne source :\n");
	        scanf("%d", &colSrc);
	        printf("Entrez le numero de la zone destination :\n");
	        scanf("%d", &zoneDst);
	        printf("Entrez le numero de la colonne destination :\n");
	        scanf("%d", &colDst);
	        
	        if (deplaceCarte(zoneSrc, colSrc, zoneDst, colDst)) {
	            deplacement(zoneSrc, colSrc, zoneDst, colDst);
	            MAJ_Zones();
	        } else {
	            printf("%s\n", erreur);
	        }
	        if (Jeufini()) {
	            printf("Le jeu est termine, tu as : %d points", calculerRecord());
	            break;
	        }
	        if (blocage()) {
	            printf("Oups!!! Cas de Blocage\n");
	            break;
	        }
	    }
	    return 0;
	}
	
	//____________________________________________________________La fin__________________________________________________//
	

