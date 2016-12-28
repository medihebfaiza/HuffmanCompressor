#include <stdio.h>
#include <stdlib.h>
/*
 _____                                              
|  ___| __ ___  __ _ _   _  ___ _ __   ___ ___  ___ 
| |_ | '__/ _ \/ _` | | | |/ _ \ '_ \ / __/ _ \/ __|
|  _|| | |  __/ (_| | |_| |  __/ | | | (_|  __/\__ \
|_|  |_|  \___|\__, |\__,_|\___|_| |_|\___\___||___/
                  |_|                               
*/

//crée un tableau de frequence d'apparition des caractères grace à notre fonction
float* createfreq(char* fichier){  
// on alloue 256 cases de flotant(frequence) 256 correspond au nombre de caractères ASCII
 float* frequences = (float*) malloc(256*sizeof(float)) ; 
 int z ;
 // on initialise le tableau frèquence à 0.0
 for(z=0; z<256; z++){
  frequences[z]=0.0; 
 }
 //Ici F correspond au fichier source
 FILE* F ;
 // on test la permission de lecture du fichier
 if ((F=fopen(fichier,"r"))!=NULL){
   int i, j ;
   unsigned int c ;
   i = 0 ;
   //On lit caractère par caractère et on calcul l'occurence de chaque caractère et le nombre total de caractère
   while ((c=fgetc(F))!=EOF){
    frequences[c]=frequences[c]+1;
    i++;
   }
   //on ferme notre fichier pour éviter des erreurs et on reparcours le tableau afin de diviser chaque occurence par le nombre total de caractère
   fclose(F) ;
   for (j=0 ; j<256 ; j++){
    frequences[j]=frequences[j]/i;
   }
   //on renvoie un pointeur qui pointe sur le début de notre tableau 
  return frequences ;
 }
 else{
  printf("Le fichier source ne s'ouvre pas\n");
  exit(1);
 }
}

// Cette fonction afficher toutes nos frèquences calculer
void drawfreq(float * freq){
  int i;
  for (i=0 ; i<256 ; i++) {
    if  (freq[i]!=0 ) printf("%c %i %f \n",i,i,freq[i]) ;
  }  
}
/*
    _    ____  ____  ____  _____ 
   / \  |  _ \| __ )|  _ \| ____|
  / _ \ | |_) |  _ \| |_) |  _|  
 / ___ \|  _ <| |_) |  _ <| |___ 
/_/   \_\_| \_\____/|_| \_\_____|
*/

// On crée une structure qui representera nos noeud avec une frequence, un pere, un fils gauche et un fils droit
typedef struct NOEUD{
 float f ;
 int pere, fg, fd ;
} NOEUD ;

//Cette fonction crée notre arbre 
void createtree(NOEUD* arbre, float* freq){
  int i ; 
  // de 0 à 255 nous avons nos caractères et leurs frequences et de 256 à 511 nous avons nos noeud
  for ( i=0 ; i<511 ; i++) {
    if (i<256){
     arbre[i].f=freq[i] ;
    }   
    else {
     arbre[i].f=0.0 ;
    }
    arbre[i].pere=-1; 
    arbre[i].fg=-1;
    arbre[i].fd=-1;
  } ; 
  // Ici nous avons un do while qui nous permet de générer notre arbre jusqu'à la racine 
  int min = -1 ; do {min++ ;} while (arbre[min].f==0.0) ;
  int tete = 256 ; 
  do { 
    for (i=0 ; i<511 ; i++){
      // On cherche la frequence  minimal qui n'a pas de pere 
      if ((arbre[i].f!=0) && (arbre[i].pere==-1) && (arbre[i].f<=arbre[min].f) && (i != tete)){
	min = i ;
      }
    }
    // on ajoute sa frequence à son pere 
    arbre[min].pere = tete ;
    arbre[tete].f += arbre[min].f ;
    if (arbre[tete].fd == -1){
     arbre[tete].fd = min ;
    }
    else {
     arbre[tete].fg = min ;
     tete++ ;
    }
    //On re initialise le minimum au premier noeud non nul qu'on trouve dans l'arbre 
    min = -1 ; do {min++ ;} while ((arbre[min].f==0.0 || arbre[min].pere!=-1 ) && min<511) ;
    }
  while(arbre[tete-1].f<1 && arbre[tete].f<1) ; 
}

// Fonction qui affiche notre arbre
void drawtree(NOEUD* arbre){
  int i ;
  for(i=0 ; i<511 ; i++){
    if (arbre[i].f!= 0.0){
      printf("%i %s %f %s %i %s %i %s %i \n",i," f: ",arbre[i].f, " p: ",arbre[i].pere ," fd ",arbre[i].fd," fg ",arbre[i].fg  ) ;
    };
   };
}

/*
  ____ ___  ____    _    ____ _____ 
 / ___/ _ \|  _ \  / \  / ___| ____|
| |  | | | | | | |/ _ \| |  _|  _|  
| |__| |_| | |_| / ___ \ |_| | |___ 
 \____\___/|____/_/   \_\____|_____|
*/

// ON décale j elements du tableau T[i] 
void decaler(int** T , int i, int j){
  int k ;
  for (k=j+1 ; k>0 ; k--){
    T[i][k] = T[i][k-1] ;
  }
  T[i][0]=-1 ;
}

//Cette fonction crée le code de chaque caractere elle renvoie un tableau bi-dimensionnelle 
int** createcodes(NOEUD* arbre){
  int i, j, pere, fils ;
  // on alloue 256 case qui contiendront nos caractere  et chaque case contient 8 autres cases qui contiendront le code binaire du caractere 
  int** tabCodage= (int**) malloc(256*sizeof(int*)); 
  for (i=0 ; i<256 ; i++){
    tabCodage[i] = NULL ;
  }
  for (i=0 ; i<256 ; i++){
    if (arbre[i].pere!=-1){
      tabCodage[i] = (int*) malloc(256*sizeof(int));
      tabCodage[i][0] = -1 ; 
      fils = i ;
      j = 0 ;
      //On rempli notre tableau bidimensionnel 
      do {
	pere = arbre[fils].pere ;
	if (arbre[pere].fd==fils){
	  decaler(tabCodage,i,j) ;
	  tabCodage[i][0] = 1 ;    
	}
	else {
          decaler(tabCodage,i,j) ;
          tabCodage[i][0] = 0 ;
	}
	j++ ;
        fils = pere ;
      }
      while (arbre[fils].pere!=-1) ;
    }
  }
  return tabCodage ;
}

//Affiche notre tableau de codage 
void drawcodes(int** tabCodage){
  int i,j;
  for (i=0 ; i<256 ; i++) {
    if  (tabCodage[i]!=NULL){
      j = 0 ; 
      printf("%c : ",i) ;
      while (tabCodage[i][j]!=-1) {
       printf("%i",tabCodage[i][j]) ;
       j++ ;
      }
      printf("\n") ;
    }
  }  
  }

//Enregistre notre tableau de codage dans un fichier 
void savecodes(int** tabCodage){
  FILE* fichier=fopen("codes.txt","w") ;
  int i,j ;
  for (i=0 ; i<256 ; i++) {
    if  (tabCodage[i]!=NULL){
      j = 0 ; 
      fputc('(',fichier) ;
      fputc(i,fichier) ;
      fputc(')',fichier) ;
      while (tabCodage[i][j]!=-1) {
	fputc(tabCodage[i][j]+48,fichier) ;
       j++ ;
      }
    }
  }  
} 
/*
 ____  _             _                    
/ ___|| |_ ___   ___| | ____ _  __ _  ___ 
\___ \| __/ _ \ / __| |/ / _` |/ _` |/ _ \
 ___) | || (_) | (__|   < (_| | (_| |  __/
|____/ \__\___/ \___|_|\_\__,_|\__, |\___|
                               |___/ 
*/

// Affiche notre buffer et ignore les cases vides (Mise à -1)
void afficherBuffer(int* buffer){
  int i = 0 ;
  printf("buffer :  ") ;
  while (i<8) {
    if (buffer[i] == -1){
      printf(" ") ;
    }
    else {
      printf("%i",buffer[i]) ;
    }    
    i++ ;
  }
  printf("|") ;
  while (i<256)  {
    if (buffer[i] == -1){
      printf(" ") ;
    }
    else {
      printf("%i",buffer[i]) ;
    }    
    i++ ;
  }
  printf("\n");
}

//On converti notre buffer qui contient une série de 8 bit en caractère ASCII 
unsigned int convertir(int* buffer){
  unsigned int r;
  int i, deux;
  r = 0 ;
  deux = 1 ;
  i=7 ;
  while (buffer[i]==-1) {
   i-- ;
  }
  for ( i = i ; i>=0 ; i--){
    r = r + deux * buffer[i] ;
    deux = deux*2 ;
  }
  printf("Le caractére '%c' de code ASCII %i est enregistré \n",r,r) ; //
  return (unsigned int)r ;
}

//On prend un fichier source on le compresse grace à notre tableau de codage et  on envoie le résultat dans un fichier destination
void stockage(char* source, char* destination, int** tabCodage){
  int i, j ;
  int* buffer = (int*) malloc(256*sizeof(int)) ; 
  FILE* S=fopen(source,"r") ;
  FILE* D=fopen(destination,"w") ;
  unsigned int c ;
  i=0 ;
  j=0 ;
  //Initialisation du buffer à -1
  while(i<256){
   buffer[i] = -1 ;
   i++ ;
  }
  i = 0 ;
  //Chargement du fichier source caractére par caractére
  while ((c=fgetc(S))!=EOF){
    while(tabCodage[(unsigned int)c][j]!=-1){
      buffer[i] = tabCodage[(unsigned int)c][j] ;
      j++ ;
      i++ ;
    }
    afficherBuffer(buffer) ;
    if (i >7){
      fputc(convertir(buffer),D) ;
      j = i-8 ;
      i = 0 ;
      while(i<248){
	buffer[i] = buffer[8+i] ;
	i++ ;
      }
      while(i<256){
	buffer[i] = -1 ;
	i++ ;
      }
      i = j ;
      j = 0 ;    
    }
    else {
      j = 0 ;
    }
  }
  if (i!=0){
   //écrit le caractère dans le fichier destination 
   fputc(convertir(buffer),D) ;
  }
  fclose(S) ;
  fclose(D) ;
}

/*
 __  __    _    ___ _   _ 
|  \/  |  / \  |_ _| \ | |
| |\/| | / _ \  | ||  \| |
| |  | |/ ___ \ | || |\  |
|_|  |_/_/   \_\___|_| \_|
*/
 
int main(){
  int i ;
  char c[1] ;
  char source[20] ;
  char destination[20] ;

  printf("\n") ;
  printf(".##.....##.##.....##.########.########.##.....##....###....##....## \n") ;
  printf(".##.....##.##.....##.##.......##.......###...###...##.##...###...## \n") ;
  printf(".##.....##.##.....##.##.......##.......####.####..##...##..####..## \n") ;
  printf(".#########.##.....##.######...######...##.###.##.##.....##.##.##.## \n") ;
  printf(".##.....##.##.....##.##.......##.......##.....##.#########.##..#### \n") ;
  printf(".##.....##.##.....##.##.......##.......##.....##.##.....##.##...### \n") ;
  printf(".##.....##..#######..##.......##.......##.....##.##.....#Compressor \n") ;
  printf("              Par El Khamiss, Mohamed Iheb & Mohamed                \n") ;

  while (c[0]!='0') {
    printf("\n1- Compresser. \n") ;
    printf("0- Quitter. \n") ;
    scanf("%1s",c) ;
    switch(c[0]) {
      case '1' :
       printf("Donner le nom du fichier source : ") ;
       scanf("%20s",source) ; 
       printf("Donner le nom du fichier destination : ") ;
       scanf("%20s",destination) ;

       printf("\n.:: Tableau des frequences ::.\n") ;
       float* freq=createfreq(source) ;
       drawfreq(freq) ;

       printf("\n.::          Arbre         ::.\n") ;
       NOEUD arbre[511] ;
       createtree(arbre,freq) ;
       drawtree(arbre) ;

       printf("\n.::   Tableau des codes    ::.\n") ;
       int** tabCodage=createcodes(arbre) ;
       drawcodes(tabCodage) ;
       savecodes(tabCodage) ;

       printf("\n.:: Conversion et stockage ::.\n") ;
       stockage(source,destination,tabCodage) ;
  
       free(freq);
       for (i=0;i<256;i++){
         if (tabCodage[i]!=NULL) {
           free(tabCodage[i]) ;
         }
       }
       free(tabCodage);
       printf("\n.::   Compression finie   ::.\n") ;
       break ;
     }
    }
 return 0 ;
}
