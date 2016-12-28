#include <stdio.h>
#include <stdlib.h>

// Cette fonction permet de lire le fichier des codes et de regénerer le tableau de codage  
int** getcodes(){
  //on ouvre le fichier codes.txt
  FILE* source=fopen("codes.txt","r") ;
  //on teste si il est ouvert
  if (source!=NULL) {
    int i,j ;
    //on alloue 256 case mémoire pour stocker le code de chaque caractére
    int** tabCodage = (int**) malloc(256*sizeof(int*)) ; 
    //on initialise les cases à NULL
    for (i=0 ; i<256 ; i++){
    tabCodage[i] = NULL ;
    }
    //on crée trois téte de lecture pour parcourir notre fichier codes.txt
    unsigned int g,m,d ;
    g = fgetc(source) ;
    m = fgetc(source) ;
    //on lit toujours le dernier caractére avec d et on s'arréte quand il vaut EOF
    while((d=fgetc(source))!=EOF){
      //si g est un ( et d est un ) alors m est un caractére 
      if (g=='(' && d==')'){
	i = m ;
	//on alloue 16 cases pour ce caractére dans le tableau de codage   
	tabCodage[i] = (int*) malloc(16*sizeof(int)) ;
	j = 0 ;
	//on initialise la premiére case à -1
	tabCodage[i][j] = -1 ;
	//on incrémente g et m 
	g = m ;
	m = d ;
      }
      //si d est un 1 ou un 0 on le place dans ça case correspondante et on incrémente j,g et m
      else if ((d=='0')||(d=='1')) {
	tabCodage[i][j] = d-48 ;
	j++ ;
        tabCodage[i][j] = -1 ;
	g = m ;
	m = d ;
      }
      //si on reconnait pas d on incrémente tout simplement
      else {
	g = m ;
	m = d ;
      }
    }
    return tabCodage ; 
  }
  else {
    printf("Can't find codes.txt") ;
    return NULL ;
  }
}

//fonction qui affiche le tableau de codage
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


//cette fonction convertit un caractére en binaire et met son code dans T apartir de i
void convertir(unsigned int c, int* T, int* i){
  //on copie le code ASCII de c dans dec
  unsigned int dec = (unsigned int)c ;
  int bin[8] ;
  int j = 7 ;
  //on covertit dec en binaire dans le tableau bin avec la méthode de division classique 
  while (dec!=0 && j>=0){
    bin[j] = dec%2 ;
    dec = dec/2 ;
    j-- ;
  }
  //si bin n'est pas remplit on met les cases restantes à 0 
  while (j!=-1){
    bin[j] = 0 ;
    j-- ;
  }
  //copier le code de bin vers T à partir de i et incrémenter i 
  for (j=0 ; j<8 ; j++){
    T[*i] = bin[j] ;
    *i = *i+1 ;
  }
}

//cette fonction prend en paramétre deux tableaux délimités par -1 et test si ils sont égaux
int egaux(int* T1, int* T2){
  int i = 0 ;
  while ((T1[i]==T2[i])&&(T1[i]!=-1)&&(T2[i]!=-1)){
    i++ ;
  }
  return (T1[i]==T2[i]) ;
}

//cette fonction teste l'existance d'un tableau unidimentionnel de j cases dans un deuxiéme bidimentionnel et renvoi ca position
int rechercher(int* Buffer, int j, int** T2){
  int i = 0 ;
  int T1[j+2] ; 
  while (i <= j){
    T1[i] = Buffer[i] ;
    i++ ;
  }
  T1[i] = -1 ;
  i = 0 ;
  while (i<256){
    if ((T2[i]!=NULL)&&(egaux(T1,T2[i]))){
       return i ;
    }
   i++ ;
  }
  return -1 ;
}

//cette fonction affiche le buffer en ignorant les cas vides (-1)
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

//cette fonction va ouvrir le fichier compresser et le decompresser dans destination à l'aide du tableau de codage
void decompresser(char* source, char* destination, int** tabCodage){
  //on ouvre les fichiers
  FILE* S=fopen(source,"r") ; 
  FILE* D=fopen(destination,"w") ;
  //on alloue 16 cases mémoires pour le buffer 
  int* buffer = (int*) malloc(256*sizeof(int)) ;
  // i pour le buffer , j pour la recherche des codes, d et f pour décaler le buffer 
  int i,j,d,f ;
  // c c'est pour lire les caractéres du fichier
  unsigned int c ;
  //on initialise les cases du buffer à -1
  i = 0 ;
  while (i<256){
    buffer[i] = -1 ;
    i++ ;
  }
  i = 0 ;
  //on teste l'existance du fichier
  if (S!=NULL){
    //on lit le fichier source caractére par caractére 
    while ((c=fgetc(S))!=EOF){
      printf("'%c' de code ASCII %i Lu \n",c,c) ;
      //on le convertit en binaire et on met son code dans le buffer à la position i
      convertir(c,buffer,&i) ;
      afficherBuffer(buffer) ;
      //on cherche dans le tableau de codage les caractéres qu'on a sur le buffer
      do {
        j = 0 ;
	//on cherche dans le buffer le code de caractére correspondant 
        while ((rechercher(buffer,j,tabCodage)==-1)&&(j<i)){
          j++ ;
        }
	//on écrit le caractére trouvé dans le fichier 
        printf("'%c' trouvé \n",rechercher(buffer,j,tabCodage)) ;//////
        fputc(rechercher(buffer,j,tabCodage),D) ;

	//on décale le buffer (on écrase le code qu'on a trouvé) 
        d = 0 ;
        f = j ;
        do {
          f++ ;
          buffer[d] = buffer[f] ;
          d++ ;
        }
        while(f<i) ;
        while (f<255){
          f++ ;
          buffer[d] = -1 ;
	  d++ ;
        }
        i = i - ( j + 1 ) ;
        afficherBuffer(buffer) ;////////
      }
      while(rechercher(buffer,j,tabCodage)!=-1) ;
      //on s'arréte quand le buffer ne contient aucun code existant dans le tableau de codage
    }
    free(buffer) ;
  }
 
  else{
    printf("Le fichier source ne s'ouvre pas\n");
    exit(1); 
  }
}


int main () {
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
  printf(".##.....##..#######..##.......##.......##.....##.##....Decompressor \n") ;
  printf("              Par El Khamiss, Mohamed Iheb & Mohamed                \n") ;

  while (c[0]!='0') {
    printf("\n1- Decompresser. \n") ;
    printf("0- Quitter. \n") ;
    scanf("%1s",c) ;
    switch(c[0]) {
      case '1' :
       printf("Donner le nom du fichier source : ") ;
       scanf("%20s",source) ; 
       printf("Donner le nom du fichier destination : ") ;
       scanf("%20s",destination) ;

       printf("\n.::   Tableau des codes    ::.\n") ;
       int** tabCodage = getcodes() ;
       drawcodes(tabCodage) ;

       printf("\n.::     Decompression      ::.\n") ;
       decompresser(source,destination,tabCodage) ;

       for (i=0;i<256;i++){
         if (tabCodage[i]!=NULL) {
           free(tabCodage[i]) ;
         }
       }
       free(tabCodage);
       printf("\n.::  Decompression finie   ::.\n") ;
       break ;
     }
    }
return 0 ;
}
