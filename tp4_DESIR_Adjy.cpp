#include <iostream>
#include <fstream>
using namespace std;

#include "types.hpp"

/*
    DESIR Adjy Sedar
    Groupe 2
    Licence 2 Info
*/

/* pré-définition des fonctions utilisées dans ce module
 * Le code des fonctions est donnée après la fonction main
 */
void creerMatrice(MatriceAdjacence &m, int taille);
void effacerMatrice(MatriceAdjacence &mat);
void afficher(MatriceAdjacence mat);
bool charger(char *nom, MatriceAdjacence &mat);
void initialiserPile(Pile &p);

bool estVide(Pile p);
void empiler(Pile &p, int v);
int depiler(Pile &p);
void afficherPile(Pile &p);

void parcoursEnProfondeurRecursive(MatriceAdjacence mat, Couleur *coul, int *parent);
void parcoursEnProfondeurNonRecursive(MatriceAdjacence mat, Couleur *coul, int *parent);
void affCoul(Couleur *coul, int taille);
void affPar(int *parent, int taille);
void affDist(int *distance, int taille);
void afficherCheminVers(int sf, int *parent);
void testPile();

int main(int argc, char *argv[]){
  
  if(argc!=2){
    cout << "Erreur - il manque le nom du fichier à lire ou la matrice de depart\n";
    return -1;
  }

  MatriceAdjacence mat = {0, nullptr};
  

  if(!charger(argv[1], mat)) 
    return -1;

  // if( (atoi(*(argv + 2)) >= mat.ordre) || (atoi(*(argv + 2)) < 0) ){
  //   cout<<"indice de sommet "<<atoi(*(argv + 2))<< " incorrect ! valeurs autoris´ees dans [0,"<<mat.ordre - 1<<"]";
  //   return -1;
  // }

  afficher(mat);

  Couleur *coul = new Couleur [mat.ordre];
  int *parent = new int[mat.ordre];

 
  cout<<"Parcours recurssive"<<endl;
  parcoursEnProfondeurRecursive(mat,  coul, parent);
  
  affCoul(coul, mat.ordre);
  affPar(parent, mat.ordre);

  cout<<endl;
  cout<<endl;

  cout<<"Parcours non recurssive"<<endl;
  parcoursEnProfondeurNonRecursive(mat,  coul, parent);
  
  affCoul(coul, mat.ordre);
  affPar(parent, mat.ordre);
  cout<<endl;

  
  effacerMatrice(mat);
  cout<<"fin";

  return 1;
}

void creerMatrice(MatriceAdjacence &m, int taille){
  // raz éventuelle de la matrice
  if(m.lignes!=nullptr) delete m.lignes;
  // initialisation du nombre de lignes/colonnes de la matrice
  m.ordre = taille;
  // allocation mémoire du tableau de lignes
  m.lignes = new Maillon*[taille];
  // initialisation de chaque ligne à "vide"
  for(int i=0; i<taille; i++) m.lignes[i]=nullptr;
}

void effacerMatrice(MatriceAdjacence &mat){
  for(int l=0; l<mat.ordre; l++){// effacer chaque ligne
    while(mat.lignes[l]!=nullptr){// tq la ligne n'est pas vide
      // effacer le premier élément qui s'y trouve
      Maillon *cour = mat.lignes[l];// 1er élément de la liste
      mat.lignes[l] = cour->suiv;// élément suivant éventuel
      delete cour; // effacer le 1er élement courant
    }
  }
  // effacer le tableau de lignes
  delete mat.lignes;
  mat.lignes = nullptr;
  // raz de la taille
  mat.ordre = 0;
      
}

bool charger(char *nom, MatriceAdjacence &mat){
  ifstream in;
  
  in.open(nom, std::ifstream::in);
  if(!in.is_open()){
    printf("Erreur d'ouverture de %s\n", nom);
    return false;
  }

  int taille;
  in >> taille;



  // créer la matrice
  creerMatrice(mat, taille);
  
  int v; // coefficient lu

  for(int l=0; l<mat.ordre; l++){ // lire et créer une ligne complète
    Maillon *fin=nullptr;// pointeur vers la fin d'une liste chaînée
    for(int c=0; c<mat.ordre; c++){ // lire et créer chaque colonne de la ligne courante
      in >> v;// lecture du coefficient (0 ou 1)
      if(v!=0){// créer un maillon et l'insérer en fin de liste
	// créer un nouveau maillon
	Maillon *nouveau = new Maillon;
	nouveau->col = c;
	nouveau->coef = v;
	nouveau->suiv = nullptr;
	// insérer le maillon en fin de liste
	if(fin!=nullptr){// il y a déjà des éléments dans la liste
	  fin->suiv = nouveau;// insertion en fin
	  fin = nouveau;// maj du pointeur vers le dernier élément de la liste
	}else{// c'est le premier coefficient de la liste
	  mat.lignes[l] = nouveau;// ajout au début de la liste
	  fin = nouveau;// maj du pointeur vers le dernier élément de la liste
	}
      }// if - rien à faire si v vaut 0
    }// for c
  }// for l

  in.close();
  return true;
}

void afficher(MatriceAdjacence mat){
  // affichage de chacune des lignes
  for(int l=0; l<mat.ordre; l++){// affichage de la ligne l
    int c=0;
    Maillon *mcur=mat.lignes[l];
    while(c<mat.ordre){
      if(mcur==nullptr){// le coefficients de la ligne >=c sont nuls
	cout << "0 ";
	c++;
      }else if(mcur->col != c){
	// on est sur un coefficient nul, qui se trouve avant c
	cout << "0 ";
	c++;
      }else{// afficher le coefficient
	cout << mcur->coef << " ";
	mcur = mcur->suiv;
	c++;
      }   
    }// while
    cout << endl;// fin de la ligne l
  }// for
}

void initialiserPile(Pile &p){// initialise la tete de liste a nul
  p.sp = nullptr;
}

bool estVide(Pile p){
  return (p.sp == nullptr); // verifie si la tete est vide ou pas
}

void empiler(Pile &p, int v){ //qui ajoute la valeur v en sommet de la pile p ;
  MaillonPile *maillon = new MaillonPile;
  // cout<<"empile "<<v<<endl;
  maillon->valeur = v;
  maillon->dessous = p.sp;
  
  p.sp = maillon;
}

void afficherPile(Pile &p){ // afficher les valeurs de la pile
  for(MaillonPile *ptr = p.sp; ptr != nullptr; ptr = ptr->dessous){
    cout<<ptr->valeur<<" ";
  }

}
int depiler(Pile &p){ // Enlever les valeurs de la pile
  if(estVide(p)) // ne fait rien si la pile est vide
    return -1;
  
  MaillonPile *supp = p.sp; // recupere le maillon a supprimer
  p.sp = p.sp->dessous; // La pile pointe sur le maillon du dessous
  int val = supp->valeur; // Recupere la valeur du maillon
  delete supp; // supprime le maillon
  // cout<<"depile "<<val<<endl;
  return val; // retourne la valeur
}

void explorerDepuis(MatriceAdjacence mat, int val,Couleur *coul, int *parent){ 
  //Permer d'explorer les noeuds 
  *(coul + val) = GRIS;
  for(Maillon *ptr = mat.lignes[val]; ptr != nullptr; ptr = ptr->suiv){
    //Parcours pour chaque matrice adjacente
    int adj = ptr->col;

    if(*(coul + adj) == BLANC){ // verifie si le noeud n'a pas ete visite
      *(parent + adj) = val;
      explorerDepuis(mat,adj,coul, parent); //Explore le noeud adjacent
    }  
 }
 *(coul + val) = NOIR;
}
void parcoursEnProfondeurRecursive(MatriceAdjacence mat, Couleur *coul, int *parent){
  //Permet de faire le parcours en profondeur recursivement
  for(int i=0; i < mat.ordre; i++){
    //Initialisation
    *(parent + i) = INDEFINI;
    *(coul + i) = BLANC;
  }

  for(int i=0; i < mat.ordre; i++){
    if (*(coul + i) == BLANC){
      explorerDepuis( mat, i,coul, parent);
    }
  }
}

void parcoursEnProfondeurNonRecursive(MatriceAdjacence mat, Couleur *coul, int *parent){
  // Faire le parcours en profondeur de maniere iterative
  
  for(int i=0; i < mat.ordre; i++){
    //Initialisation
    *(parent + i) = INDEFINI;
    *(coul + i) = BLANC;
  }

  
  Pile maPile; // Ma pile

  initialiserPile(maPile); // initialiser ma pile
  
  for(int i = 0; i< mat.ordre; i++){

    int val = i; // Recupere un noeud a partir duquel qu'on va explorer
    empiler(maPile, val); // empile ce neoud
    *(coul + i) = GRIS; // le noeud a ete visite

    while(val != -1){ // Tant que le noeud n'est pas vide
   
      bool aEmpiler = false; //un compteur temporaire
     
      for(Maillon *ptr = mat.lignes[val]; ptr != nullptr; ptr = ptr->suiv){ // Explore les noeuds adjacents du noeud
        int adj = ptr->col; // le noeud adjacent
          
        if(*(coul + adj) == BLANC){ // Si ce noeud n'a pas ete visite
          *(coul + adj) = GRIS; // le noeud a ete visite
          *(parent + adj) = val; // le parent du noeud
          empiler(maPile, adj); // mettre le noeud dans la pile
          val = adj;// Mon nouveau noeud a explore est mon noeud
          aEmpiler = true; // J'ai empiler un noeud
          break;
        }
      } 

      if(!aEmpiler){ // Verifie si j'ai enfile un noeud

        val = depiler(maPile); // je depile un nouveau noeud
        
        if(val != -1) // Verifie si la valeur est different de -1
          *(coul + val) = NOIR; // Le noeud est fini
      }

    }
  }
}


void affCoul(Couleur *coul, int taille){
  cout<<"Couleurs : ";
  for(int i=0; i<taille; i++){
    if(*(coul + i) == BLANC)
      cout<<"B";
    if(*(coul + i) == NOIR)
      cout<<"N";
    if(*(coul + i) == GRIS)
      cout<<"G";
    cout<<" ";
  }
  cout<<endl;
    
}


void affPar(int *parent, int taille){
  cout<<"Parents  : ";
  for(int i=0; i<taille; i++){
    if(*(parent + i) == INDEFINI)
      cout<<"X";
    else
      cout<<*(parent + i);
    cout<<" ";
  }
  cout<<endl;
}


void afficherCheminVers(int sf, int *parent){
  
  if(sf == INDEFINI)
    return;
  
  afficherCheminVers(parent[sf],parent);
  cout<<sf<<" ";
}


void testPile(){
  Pile maPile;
    initialiserPile(maPile);
    empiler(maPile, 5);
    empiler(maPile, 6);
    empiler(maPile, 7);
    afficherPile(maPile);
    cout<<endl;
    int a = depiler(maPile);
    cout<<a<<endl;
    a = depiler(maPile);
    cout<<a<<endl; 
    a = depiler(maPile);
    cout<<a<<endl; 
    a = depiler(maPile);
    cout<<a<<endl;  
    a = depiler(maPile);
    cout<<a<<endl;
    afficherPile(maPile);
    cout<<"fin";
}


