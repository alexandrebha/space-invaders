#include <libTableauNoir.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {double x; double y;} coord_t;

typedef struct {coord_t coord_vai; int vitesse_vai; coord_t direction_vai;} deplacement_vai_t;

typedef struct {
  Image vaisseau_im;
  int vaisseau_x;
  int vaisseau_y;
  int existence;
  int vie;
}vaisseau_t;

typedef struct {
  int degradation_x;
  int degradation_y;
}degradation_t;

typedef struct {
  int missile_x;
  int missile_y;
  int vie;
  int on_degradation;
}missile_t;

typedef struct {
  Image shield_im;
  int shield_x;
  int shield_y;
  degradation_t degradation[40];
  int nb_degradation;
}shield_t;

typedef struct {
  Image alien_im;
  int alien_x;
  int alien_y;
  int vie;
  int transparence;
}alien_t;

typedef struct {
  int bomb_x;
  int bomb_y;
  int vie;
  int couleur;
  int on_degradation;
}bomb_t;


typedef struct {
  Image bonus_im;
  int bonus_x;
  int bonus_y;
  int vie;
}bonus_t;

typedef struct {
  Image vie_im;
  int vie_x;
  int vie_y;
}vie_t;

typedef struct {
  Image demarrage_im;
  int demarrage_x;
  int demarrage_y;
}demarrage_t;

typedef struct {
  Image manuel_im;
  int manuel_x;
  int manuel_y;
}manuel_t;


typedef struct {
  Image score_im;
  int score_x;
  int score_y;
  int nb_score;
}score_t;


typedef struct {
  score_t score[4];
  int record_x;
  int record_y;
}record_t;

typedef struct {
  Image im_win;
  int win_x;
  int win_y;
  int compteur;
}win_t;

typedef struct {
  Image game_over_im;
  int game_over_x;
  int game_over_y;
}game_over_t;

typedef struct {
  Image press_enter;
  Image press_esc;
}fin_t;


typedef struct {
  vaisseau_t vaisseau;
  missile_t missile;
  bomb_t bomb[8];
  alien_t alien[5][11];
  shield_t shield[4];
  bonus_t bonus;
  vie_t vie[2];
  demarrage_t demarrage;
  manuel_t manuel;
  score_t score[4];
  record_t record[3];
  win_t win;
  game_over_t game_over;
  fin_t fin;
  Image im_score[10];
  Image im_press;
  int fini;
  int sens_deplacement_alien;
  int demarrage_ou_pas;
  int manuel_ou_pas;
  int fin_ou_pas;
}donnees_t;

//-----------------------------------VAISSEAU-----------------------------------

void init_vaisseau(vaisseau_t* vaisseau){
  vaisseau->vaisseau_im = chargerImage("piqure.png");
  vaisseau->vaisseau_im = rotozoomImage(vaisseau->vaisseau_im,44.8,0.17,0.17);
  vaisseau->vaisseau_x=0-tn_largeur(vaisseau->vaisseau_im)/2;
  vaisseau->vaisseau_y=-399 + tn_hauteur(vaisseau->vaisseau_im);
  vaisseau->vie=3;
  vaisseau->existence=0;
  /*L'existence défini la présence du vaisseau sur l'écran*/
}

/* La fonction deplacement_vai permet de déplacer le vaisseau avec deux touches du clavier
 */
void deplacement_vai(EtatSourisClavier esc, vaisseau_t* vaisseau){
  if(esc.touchesClavier[80]&&vaisseau->vaisseau_x>-500){
         vaisseau->vaisseau_x = vaisseau->vaisseau_x - 4; 
        }
  if(esc.touchesClavier[79]&&vaisseau->vaisseau_x<500-tn_largeur(vaisseau->vaisseau_im)){
	 vaisseau->vaisseau_x = vaisseau->vaisseau_x + 4;
       }
	 
}


void afficher_vaisseau(vaisseau_t* vaisseau){
  afficherImage(vaisseau->vaisseau_im,vaisseau->vaisseau_x,vaisseau->vaisseau_y);
}

/*Si l'existence du vaisseau est égal à un alors le vaisseau change de coordonnées et sera affiché hors de l'écran
 */
void eliminer_vaisseau(vaisseau_t* vaisseau){
  if(vaisseau->existence==1){
    vaisseau->vaisseau_x=2000;
    vaisseau->vaisseau_y=2000;
  }
}
/*La fonction eliminer_vaisseau_for_all elimine le vaisseau peu importe l'existence
 */
void eliminer_vaisseau_for_all(vaisseau_t* vaisseau){
  vaisseau->vaisseau_x=2000;
  vaisseau->vaisseau_y=2000;
}


//----------------------------------MISSILE-------------------------------------
/*Quand la vie du missile vaut 0 alors le missile n'existe pas encore
 */
void init_missile(missile_t* missile){
  missile->vie =0;
}

/*La fonction tirer_missile permet au vaisseau de tirer un missile avec la touche espace. Le missile sera un segment vert de 15 pixel de longueur. Lorsque le missile est créer la vie de ce dernier sera alors de 1
 */
void tirer_missile(EtatSourisClavier esc, vaisseau_t* vaisseau, missile_t* missile, shield_t* shield){
  if(esc.touchesClavier[44]==1 && (missile->vie ==0 || missile->vie ==2)){
  missile->vie = 1;
  missile->missile_x = vaisseau->vaisseau_x + tn_largeur(vaisseau->vaisseau_im)/2;
  missile->missile_y = vaisseau->vaisseau_y-(tn_hauteur(vaisseau->vaisseau_im)/2);
   if(esc.touchesClavier[44]){
    choisirTypeStylo(3,255,255,255);
    tracerSegment(missile->missile_x, missile->missile_y, missile->missile_x, missile->missile_y +15);
    choisirTypeStylo(3,0,255,0);
    tracerSegment(missile->missile_x, missile->missile_y+15, missile->missile_x, missile->missile_y +30);
    
    }
  }
}

/* Le missile se déplace tout seul tant que sa vie est égal à 1 et sa coordonnée en ordonnée inférieur à 500. Si le missile ne se déplace pas alors sa vie est égal à deux
 */
void deplacement_missile(missile_t* missile){
  if(missile->missile_y<500&&missile->vie==1){
    missile->missile_y = missile->missile_y + 5;
  }
  else {
    missile->vie = 2;
  }
}

/*Si la vie du missile égal à deux, cela signifie que le missile ne se déplace plus alors on doit l'éliminer. On verra plus loin dans le code que lorsque le missile rencontre un obstacle alors sa vie deviendra égale à deux et le missile sera automatiquement éliminé
 */
void eliminer_missile(missile_t* missile){
  if(missile->vie==2){
    missile->missile_x =-1200;
    missile->missile_y =1200;
  }
}

/*La fonction eliminer_missile_for_all élimine le missile peu importe sa vie
 */
void eliminer_missile_for_all(missile_t* missile){
  missile->missile_x =-1200;
  missile->missile_y =1200;
}


void afficher_missile(missile_t* missile){
  choisirTypeStylo(3,0,255,0);
  tracerSegment(missile->missile_x, missile->missile_y+25, missile->missile_x, missile->missile_y +50);
}

//----------------------------------ALIENS-------------------------------------


void init_aliens(alien_t alien[5][11]){
  Image im_rouge = chargerImage("virus_rouge_noir.jpg");
  im_rouge = rotozoomImage(im_rouge,0,0.139,0.146);
   for(int i = 0; i<11; i++){
     alien[0][i].vie=0; //La vie de l'alien est égale à 0 signifie que l'alien apparait à l'écran
     alien[0][i].alien_im = im_rouge;
     alien[0][i].alien_x = -320 +(20+tn_largeur(im_rouge))*i;
     alien[0][i].alien_y = 344;
   }
     Image im_bleu = chargerImage("virus_bleu_noir1.jpg");
     im_bleu = rotozoomImage(im_bleu,0,0.1493,0.149);
     for(int j = 1; j<3; j++){
       for(int k = 0; k<11; k++){
	 alien[j][k].vie=0;
	 alien[j][k].alien_im = im_bleu;
	 alien[j][k].alien_x = -320 +(20+tn_largeur(im_bleu))*k;
	 alien[j][k].alien_y = 344-50*j;
	 }
     }
     Image im_violet = chargerImage("virus_violet_noir.jpg");
     im_violet = rotozoomImage(im_violet,0,0.2247,0.240);
     for(int l = 3; l<5; l++){
       for(int m = 0; m<11; m++){
	 alien[l][m].vie=0;
	 alien[l][m].alien_im = im_violet;
	 alien[l][m].alien_x = -320 +(20+tn_largeur(im_violet))*m;
	 alien[l][m].alien_y = 344-50*l;
	 }
     }
}

/*La fonction deplacement_alien prend en paramètre un tableau de tableau d'alien,en tout 55 aliens. Elle prend également en paramètre un pas qui correspond au nombre de pixel de déplacment horizontale par itération. La descente quant à elle correspond au nombre de pixels de déplacement verticale par itération. Le sens permet de déterminer si les aliens effectueront un deplacemnt verticale,horizontale et si ce dernier a lieu vers la driote ou vers la gauche. Enfin le count est un compteur qui permet ici de ralentir le dépalcement des aliens. Ici les aliens effectueront un deplacement toute les 200 itérations.
Si le sens est égale à 0 alors le déplacement est verticale vers le bas ici
Si le sens est égale à 1 alors le déplacement est horizontale vers la droite 
Si le sens est égale à -1 alors le déplacement est horizontale vers la gauche
 */
void deplacement_alien(alien_t alien[5][11], int pas, int descente, int count, int* sens){
      if(*sens!=0&&count%200==0){
	for(int k = 0; k<5; k++){
	  for(int i = 0; i<11; i++){
	    if(alien[k][i].vie!=1){
	      alien[k][i].alien_x = alien[k][i].alien_x + *sens * pas;
	    }
	  }
	}
	for(int d=0; d<11; d++){
	  if(alien[0][d].alien_x==460||alien[0][d].alien_x==-500){
	    *sens = 0;
	    count = count + 1;
	  }
	}
      }
	if(*sens==0&&count%200==0){
	  for(int l = 0; l<5; l++){
	    for(int j = 0; j<11; j++){
	      if(alien[l][j].vie!=1){
		alien[l][j].alien_y = alien[l][j].alien_y - descente;
	      }
	    }
	  }
	  for(int b=0; b<11; b++){
	    if(alien[0][b].alien_x==460){
	      *sens = -1;
	      count = count+1;
	    }
	  }
	  for(int a=0; a<11; a++){
	    if(alien[0][a].alien_x==-500){
	      *sens = 1;
	      count = count +1;
	    }
	  }
	}
}

/*La fonction eliminer_alien permet d'éliminer un alien lorsque sa vie est égale à 1, ce qui équivaut à un alien touché par un missile
 */
void eliminer_alien(alien_t alien[5][11]){
  for(int i = 0; i<5; i++){
    for(int j = 0; j<11; j++){
      if(alien[i][j].vie==1){
	alien[i][j].alien_x=5000;
	alien[i][j].alien_y=5000;
	alien[i][j].vie=1;
      }
    }
  }
}

/*La fonction eliminer_aliens_for_all permet d'éliminer tous les aliens peu importe la vie de ces derniers
 */
void eliminer_aliens_for_all(alien_t alien[5][11]){
  for(int i = 0; i<5; i++){
      for(int j = 0; j<11; j++){
	alien[i][j].alien_x=5000;
	alien[i][j].alien_y=5000;
      }
    }
}

/*La fonction alien_en_bas permet d'arrêter le jeu car les aliens sont trop bas et touche le vaisseau
 */
void alien_en_bas(alien_t alien[5][11], vaisseau_t* vaisseau){
  for(int i = 0; i<5; i++){
    for(int j = 0; j<11; j++){
      if(alien[i][j].alien_y-tn_hauteur(alien[i][j].alien_im)<-246){
	vaisseau->vie =0;
      }
    }
  }
}


void afficher_alien(alien_t alien[5][11]){
  for(int j = 0; j<5; j++){
    for(int i = 0; i<11; i++){
      afficherImage(alien[j][i].alien_im,alien[j][i].alien_x,alien[j][i].alien_y);
    }
  }
}


/*void liberer_alien_rouge(alien_t alien[5][11]){
  int count = 0;
  for(int i = 0; i<11; i++){
    if(alien[0][i].vie==1){
      count = count + 1;
    }
    if(count==11){
      libererImage(alien[0][i].alien_im);
    }
  }
  }*/

/*void liberer_alien_violet(alien_t alien[5][11]){
  int count = 0;
  for(int i = 0; i<11; i++){
    if(alien[1][i].vie==1 || alien[2][i].vie==1){
      count = count + 1;
    }
    printf("%d\n",count);
    if(count==22){
      libererImage(alien[1][i].alien_im);
    }
  }
  }*/
//----------------------------------BOMBS---------------------------------------

void init_bomb(bomb_t bomb[8]){
  for(int j = 0; j<8; j++){
    bomb[j].vie = 0;//La vie de la bombe est égale à 0 signifie que la bombe n'eiste pas encore
    bomb[j].bomb_x=-1000;
    bomb[j].bomb_y=-1000;
  }
}

/*La fonction aleatoire permet de faire tourner un algorithme qui va choisir aléatoirement un nombre en tre un intervalle et renverra se nombre. Ici l'intervalle est [a,b]
 */
int aleatoire(int a, int b){
  return(a+rand()%(b+1-a));
}

/*La fonction lacher_bomb permet à un alien de lacher une bomb lorsque la fonction aleatoire(1,400) renverra et et si la vie de la bombe égale 0 ou à deux. Ensuite la vie de la bombe devient égale à 1 une fois qu'elle a été créer. La fonction aléatoire est appelée deux fois afin de déterminer quel alien lachera la bombe. La couleur de la bombe sera fixé par la ligne du lanceur de la bombe. La couleur de la bombe sera la même que l'alien lanceur.
 */
void lacher_bomb(bomb_t bomb[8], alien_t alien[5][11]){
  for(int j = 0; j<8; j++){
    if(aleatoire(1,400)==1 && (bomb[j].vie == 0 || bomb[j].vie == 2)){
      int k = aleatoire(0,4);
      int i = aleatoire(0,10);
      if(alien[k][i].vie==0){
	bomb[j].vie = 1;
	bomb[j].bomb_x= alien[k][i].alien_x+(tn_largeur(alien[k][i].alien_im)/2);
	bomb[j].bomb_y = alien[k][i].alien_y;
	bomb[j].couleur = k*2;
      }
    }
  }
}

/*La fonction deplacement_bomb permet de déplacer la bomb de 4 pixels par itération et cela tant que la vie de la bombe est égale à 1 et si l'ordonnée de celle-ci est supérieur à -400. Si ce n'est pas le cas alors la bombe n'est pas sur l'écran est sa vie devient égale à 2
 */
void deplacement_bombs(bomb_t bomb[8]){
  for(int j = 0; j<8; j++){
    if(bomb[j].bomb_y>-400&&bomb[j].vie==1){
      bomb[j].bomb_y = bomb[j].bomb_y -4;
    }
    else{
      bomb[j].vie=2;
    }
    if(bomb[j].bomb_x%60!=0){// si l'abscisse de la bombe est égale à deux alors la bombe n'est pas à sa place car les aliens ont des mouvements de 60 pixels en terme d'abscisse.
      bomb[j].vie=2;
    }
  }
}


/*La fonction eliminer_bomb permet d'éliminer la bombe qui n'est plus afficher à l'écran c'est à lorsque sa vie est égale à deux
 */
void eliminer_bomb(bomb_t bomb[8]){
  for(int i = 0; i<8; i++){
    if(bomb[i].vie==2){
      bomb[i].bomb_x=-1500;
      bomb[i].bomb_y=-1500;
    }
  }
}

/*La fonction eliminer_bombs_for_all permet d'éliminer toutes les bombes peut importe leurs vie
 */
void eliminer_bombs_for_all(bomb_t bomb[4]){
  for(int k = 0; k<4; k++){
    bomb[k].bomb_x=-1500;
    bomb[k].bomb_y=-1500;
  }
}


  
void afficher_bomb(bomb_t bomb[8]){
  for(int j=0; j<8; j++){
    if(bomb[j].vie != 2){
      if(bomb[j].couleur==0){
      choisirTypeStylo(3,214,82,70);
  tracerSegment(bomb[j].bomb_x, bomb[j].bomb_y-27, bomb[j].bomb_x, bomb[j].bomb_y-52);
      }
      if(bomb[j].couleur==2 || bomb[j].couleur==4){
	choisirTypeStylo(3,10,89,155);
  tracerSegment(bomb[j].bomb_x, bomb[j].bomb_y-27, bomb[j].bomb_x, bomb[j].bomb_y-52);
      }
      if(bomb[j].couleur==6 || bomb[j].couleur==8){
	choisirTypeStylo(3,108,9,117);
  tracerSegment(bomb[j].bomb_x, bomb[j].bomb_y-27, bomb[j].bomb_x, bomb[j].bomb_y-52);
      }
    }
  }
}

//-------------------------------DEGRADATION------------------------------------

/*Une dégrdation est un point noir qui va s'afficher sur le bouclier lorsqu'un missile ou une bombe touche un bouclier. La dégradation agira comme un trou dans le bouclier. Les missiles et les bombes pourront donc à  un moment traverser un bouclier trop endommagé. 
 */


/*La fonction dégradation missile est nécessaire pour l'affichage d'une dégradation produite par un missile
 */
void degradation_missile(degradation_t degradation){
  choisirTypeStylo(20,0,0,0);
  int x = degradation.degradation_x;
  int y = degradation.degradation_y+60;
      tracerPoint(x,y);
}

/*La fonction dégradation missile est nécessaire pour l'affichage d'une dégradation produite par une bombe
 */
void degradation_bomb(degradation_t degradation){
  choisirTypeStylo(20,0,0,0);
  int x = degradation.degradation_x;
  int y = degradation.degradation_y;
      tracerPoint(x,y);
}

/*
 */
void degradation(shield_t shield){
  for(int i = 0; i<40; i++){
    degradation_missile(shield.degradation[i]);
    degradation_bomb(shield.degradation[i]);
  }
}

/*La fonction eliminer_degradations_for_all permet d'éliminer toute les dégradations en les affichants toute à des coordonnées hors de l'écran
 */
void eliminer_degradations_for_all(shield_t shield[4]){
   for(int i = 0; i<4; i++){
     for(int j = 0; j<20; j++){
       shield[i].degradation[j].degradation_x=-5000;
       shield[i].degradation[j].degradation_y=-5000;
     }
   }
}


//----------------------------------SHIELD--------------------------------------

//shield veut dire bouclier


void init_shield(shield_t shield[4]){
  Image im_shield = chargerImage("masque(2).png");
  im_shield = rotozoomImage(im_shield,3.14,0.8,0.8);
   for(int i = 0; i<4; i++){
   shield[i].shield_im = im_shield;
   shield[i].shield_x = -399.2 +(100+tn_largeur(im_shield))*i;
   shield[i].shield_y = -151;
   shield[i].nb_degradation = 0;
   }
}

/*La fonction eliminer_shields_for_all permet d'éliminer tous les boucliers
 */
void eliminer_shields_for_all(shield_t shield[4]){
  for(int i = 0; i<4; i++){
    shield[i].shield_x=0;
    shield[i].shield_y=-2000;
  }
}

void afficher_shield(shield_t shield[4]){
  for(int i = 0; i<4; i++){
   afficherImage(shield[i].shield_im,shield[i].shield_x,shield[i].shield_y);
   degradation(shield[i]);   
  }
}



//----------------------------MISSILE OBSTACLE----------------------------------
      
/*
La fonction missile_on_degradation prend en paramètre:
un shield et un pointeur sur un missile
Elle retourne :
true si le missile est sur une degradation d'un bouclier
false sinon
 */
bool missile_on_degradation(shield_t shield, missile_t* missile){
  bool resultat = false;
  for(int i = 0; i<40; i++){
    if((missile->missile_x>=shield.degradation[i].degradation_x-10)
       &&(missile->missile_x<=shield.degradation[i].degradation_x+10)
       &&(missile->missile_y+15>=shield.degradation[i].degradation_y-20)
       &&(missile->missile_y+15<=shield.degradation[i].degradation_y+20)){
      resultat = true;
    }
  }
  return resultat;
}


/*La fonction missile_on_shield renvoie true si le missile est sur le bouclier et false sinon
 */
bool missile_on_shield(missile_t* missile, shield_t shield){
  bool resultat = false;
  if((missile->missile_x>shield.shield_x)
     &&(missile->missile_x<shield.shield_x+tn_largeur(shield.shield_im))
     &&(missile->missile_y+15>shield.shield_y-80)){
    resultat = true;
  }
  return resultat;
}

/*La fonction missile_shield_passe_ou_pas permet de savoir si le missile crée une dégradation où alors si le missile passe et traverse le bouclier si celui-ci est endommagé dans la zone où le missile doit passer
 */
void missile_shield_passe_ou_pas(missile_t* missile, shield_t shield[4]){
  for(int i=0; i<4; i++){
     if(missile_on_shield(missile,shield[i])){
       if(!missile_on_degradation(shield[i],missile)
	   &&(missile->missile_y<shield[i].shield_y-60)){
	   missile->vie = 2;
	   shield[i].degradation[shield[i].nb_degradation].degradation_x=missile->missile_x;
	   shield[i].degradation[shield[i].nb_degradation].degradation_y=missile->missile_y;
	   //shield[i].degradation[shield[i].nb_degradation].origine=1;
	   shield[i].nb_degradation = shield[i].nb_degradation + 1;
	 }
     }
  }
}
/*La fonction missile_on_alien permet de modifier la vie de l'alien à 1 lorsque celui-ci est touché par un missile
 */
void missile_on_alien(missile_t* missile, alien_t alien[5][11]){
  for(int i=0; i<5; i++){
    for(int j=0; j<11; j++){
      if((missile->missile_x>alien[i][j].alien_x)&&(missile->missile_x<alien[i][j].alien_x+tn_largeur(alien[i][j].alien_im))){
	if(missile->missile_y+15==alien[i][j].alien_y-tn_hauteur(alien[i][j].alien_im)){
	  missile->vie = 2;
	  alien[i][j].vie=1;
	}
      }
    }
  }
}

/*La fonction missile_on_bonus permet de changer la vie du bonus si celui-ci est touché par un missile
 */
void missile_on_bonus(donnees_t* donnees){
  if(donnees->missile.missile_x <= donnees->bonus.bonus_x + tn_largeur(donnees->bonus.bonus_im)
     &&(donnees->missile.missile_x >= donnees->bonus.bonus_x)
     &&(donnees->missile.missile_y+15 == donnees->bonus.bonus_y-tn_hauteur(donnees->bonus.bonus_im))){
    donnees->missile.vie=2;
    donnees->bonus.vie=2;//Lorsque la vie du bonus est égale àn deux le bonus sera éliminé
  }
}

 void missile_obstacle(donnees_t* donnees){
   missile_on_alien(&donnees->missile, donnees->alien);
   missile_shield_passe_ou_pas(&donnees->missile,donnees->shield);
   missile_on_bonus(donnees);
}
	 
//----------------------------BOMB OBSTACLE-------------------------------------

/*
La fonction bomb_on_degradation prend en paramètre:
un shield et une bomb
Elle retourne :
true si la bombe est sur une degradation d'un bouclier
false sinon
 */
bool bomb_on_degradation(shield_t shield, bomb_t bomb){
  bool resultat = false;
    for(int i = 0; i<40; i++){
      if((bomb.bomb_x>shield.degradation[i].degradation_x-10)){
	if(bomb.bomb_x<shield.degradation[i].degradation_x+10){
	  if(bomb.bomb_y-25<=shield.degradation[i].degradation_y+10){
	    if(bomb.bomb_y-25>=shield.degradation[i].degradation_y-10){
	      //    printf("a\n");
	    resultat = true;
	    }
	  }
	}
      }
      else{
	//printf("b\n");
      }
    }
    return resultat;
}

/*La fonction bomb_on_shield renvoie true si la bomb est sur le bouclier et false sinon
 */

bool bomb_on_shield(bomb_t bomb, shield_t shield){
  bool resultat = false;
  if(bomb.bomb_x>=shield.shield_x){
    if(bomb.bomb_x<=shield.shield_x+tn_largeur(shield.shield_im)){
      if(bomb.bomb_y-25<=shield.shield_y/*(tn_hauteur(shield.shield_im)/2)-24)*/){
	if(bomb.bomb_y>=shield.shield_y-40){
	  resultat = true;
	}
      }
    }
  }
  return resultat;
}
  
/*La fonction bomb_shield_passe_ou_pas permet de savoir si la bombe crée une dégradation ou alors si la bombe passe et traverse le bouclier si celui-ci est endommagé dans la zone où la bomb doit passer
 */
void bomb_shield_passe_ou_pas(bomb_t bomb[8], shield_t shield[4]){
  for(int i = 0; i<4; i++){
    for(int j = 0; j<8; j++){
      if(bomb_on_shield(bomb[j],shield[i])){
	if(!bomb_on_degradation(shield[i], bomb[j]) && (bomb[j].bomb_y>=shield[i].shield_y-5)){
	  shield[i].degradation[shield[i].nb_degradation].degradation_x=bomb[j].bomb_x;
	  shield[i].degradation[shield[i].nb_degradation].degradation_y=bomb[j].bomb_y-25-7;
	  //shield[i].degradation[shield[i].nb_degradation].origine=2;
	  bomb[j].vie=2;
	  shield[i].nb_degradation = shield[i].nb_degradation + 1;
	}
      }
    }
  }
}
/*La fonction bomb_on_vaisseau change diminue de 1 la vie du vaisseau qui est initialement à 3 si la bomb touche le vaisseau
 */
void bomb_on_vaisseau(bomb_t bomb[8], vaisseau_t* vaisseau){
  for(int j= 0; j<8; j++){
    if((bomb[j].bomb_x>=vaisseau->vaisseau_x+(tn_largeur(vaisseau->vaisseau_im)/2)-15)){
      if(bomb[j].bomb_x<=vaisseau->vaisseau_x+(tn_largeur(vaisseau->vaisseau_im)/2+15)){
	if(bomb[j].bomb_y-25<=vaisseau->vaisseau_y+(tn_hauteur(vaisseau->vaisseau_im)/2)-33){
	  bomb[j].vie=2;
	  vaisseau->vaisseau_x=0-tn_largeur(vaisseau->vaisseau_im)/2;
	  vaisseau->vaisseau_y=-399 + tn_hauteur(vaisseau->vaisseau_im);
	  vaisseau->vie=vaisseau->vie-1;
	}
      }
    }
  }
}

void bomb_obstacle(donnees_t* donnees){
  bomb_shield_passe_ou_pas(donnees->bomb,donnees->shield);
  bomb_on_vaisseau(donnees->bomb,&donnees->vaisseau);
}


//------------------------------------BONUS-------------------------------------

void init_bonus(bonus_t* bonus){
  Image pangolin = chargerImage("pangolin.jpg");
  pangolin = rotozoomImage(pangolin,0,0.15,0.15);
  bonus->bonus_x = -500;
  bonus->bonus_y = -1000;
  bonus->bonus_im = pangolin;
  bonus->vie=0;
}

/*La fonction bonus permet de créer un bonus qui se déplacera horizontalement vers la gauche en haut de l'écran. Ce bonus ne sera créer que si les aliens ont une ordonnée inférieur à 400 et si la fonction aléatoire(1,10000) renvoie 1. Une fois le bonus créer sa vie est égale à 1.
 */
void bonus(donnees_t* donnees){
  for(int i = 0; i<11; i++){
    if(donnees->alien[0][i].alien_y<300 || donnees->alien[0][i].alien_y>400){
      if(donnees->bonus.vie==0 || donnees->bonus.vie==3){
	if(aleatoire(1,10000)==1){
	  donnees->bonus.vie=1;
	  donnees->bonus.bonus_x = 500-tn_largeur(donnees->bonus.bonus_im);
	  donnees->bonus.bonus_y = 327;
	}
      }
    }
  }
}

/*Tant que le bonus possède une vie égale à 1 alors le bonus se déplace horizontalement vers la gauche
 */ 
void deplacement_bonus(bonus_t* bonus){
  if(bonus->vie==1){
    bonus->bonus_x = bonus->bonus_x - 3;
  }
  if(bonus->bonus_x+tn_largeur(bonus->bonus_im)<-500){
    bonus->vie=0;
  }
}

/*La fonction eliminer_bonus permet d'éliminer le bonus lorsque sa vie est égale à 2. La vie est égale à 2 lorsqu'un missile touche le bonus
 */
void eliminer_bonus(bonus_t* bonus){
  if(bonus->vie==2){
    bonus->bonus_x = -500;
    bonus->bonus_y = -1000;
  }
}

/*La fonction eliminer_bonus_for_all permet d'éliminer le bonus peu importe la vie de ce dernier
 */
void eliminer_bonus_for_all(bonus_t* bonus){
  bonus->vie=3;
  bonus->bonus_x = 1000;
  bonus->bonus_y = 3000;
}


void afficher_bonus(bonus_t* bonus){
  afficherImage(bonus->bonus_im, bonus->bonus_x, bonus->bonus_y);
}
	

//------------------------------------SCORE-------------------------------------
void init_score(donnees_t* donnees){
  Image im_score0= chargerImage("chiffre0_bis.png");
  donnees->im_score[0]=im_score0;
  Image im_score1= chargerImage("chiffre1_bis.png");
  donnees->im_score[1]=im_score1;
  Image im_score2= chargerImage("chiffre2_bis.png");
  donnees->im_score[2]=im_score2;
  Image im_score3= chargerImage("chiffre3_bis.png");
  donnees->im_score[3]=im_score3;
  Image im_score4= chargerImage("chiffre4_bis.png");
  donnees->im_score[4]=im_score4;
  Image im_score5= chargerImage("chiffre5_bis.png");
  donnees->im_score[5]=im_score5;
  Image im_score6= chargerImage("chiffre6_bis.png");
  donnees->im_score[6]=im_score6;
  Image im_score7= chargerImage("chiffre7_bis.png");
  donnees->im_score[7]=im_score7;
  Image im_score8= chargerImage("chiffre8_bis.png");
  donnees->im_score[8]=im_score8;
  Image im_score9= chargerImage("chiffre9_bis.png");
  donnees->im_score[9]=im_score9;
  for(int i = 0; i<4; i++){
    donnees->score[i].nb_score=0;
    donnees->score[i].score_im = im_score0;
    donnees->score[i].score_x = -490+50*i;
    donnees->score[i].score_y = 400;    
  }
}

/*La fonction score_alien_rouge augmente le score de 100 si un alien rouge (aliens les plus haut) est touché
 */
void score_alien_rouge(score_t score[4], alien_t alien[5][11]){
  for(int i = 0; i<11; i++){
    if(alien[0][i].vie==1){
      alien[0][i].vie=2;
      if(score[1].nb_score<10){
	score[1].nb_score=score[1].nb_score+1;
	if(score[1].nb_score>=10){
	  score[1].nb_score = score[1].nb_score - 10;
	  score[0].nb_score=score[0].nb_score+1;
	}
      }
    }
  }
}

/*La fonction score_alien_bleu augmente le score de 40 si un alien bleu (aliens du milieu) est touché
 */
void score_alien_bleu(score_t score[4], alien_t alien[5][11]){
  for(int i = 0; i<11; i++){
    for(int j = 1; j<3; j++){
      if(alien[j][i].vie==1){
	alien[j][i].vie=2;
	if(score[2].nb_score<10){
	  score[2].nb_score=score[2].nb_score+4;
	  if(score[2].nb_score>=10){
	    score[2].nb_score = score[2].nb_score - 10;
	    score[1].nb_score=score[1].nb_score+1;
	    if(score[1].nb_score>=10){
	      score[0].nb_score=score[0].nb_score+1;
	      score[1].nb_score=score[1].nb_score-10;
	    }
	  }
	}
      }
    }
  }
}

/*La fonction score_alien_violet augmente le score de 10 si un alien violet (alien les plus bas) est touché
 */
void score_alien_violet(score_t score[4], alien_t alien[5][11]){
  for(int i = 0; i<11; i++){
    for(int k = 3; k<5; k++){
      if(alien[k][i].vie==1){
	alien[k][i].vie=2;
	if(score[2].nb_score<10){
	  score[2].nb_score=score[2].nb_score+1;
	  if(score[2].nb_score>=10){
	    score[2].nb_score = score[2].nb_score - 10;
	    score[1].nb_score=score[1].nb_score+1;
	    if(score[1].nb_score>=10){
	      score[0].nb_score=score[0].nb_score+1;
	    }
	  }
	}
      }
    }
  }
}

/*La fonction score_bonus augmente le score de 500 si le bonus est touché
 */
void score_bonus(score_t score[4], bonus_t* bonus){
  if(bonus->vie==2){
    bonus->vie=3;
    if(score[1].nb_score<10){
      score[1].nb_score=score[1].nb_score+5;
      if(score[1].nb_score>=10){
	score[1].nb_score = score[1].nb_score - 10;
	score[0].nb_score=score[0].nb_score+1;
      }
    }
  }
}
    


void score(score_t score[4], alien_t alien[5][11], bonus_t* bonus){
  score_alien_rouge(score,alien);
  score_alien_bleu(score,alien);
  score_alien_violet(score,alien);
  score_bonus(score,bonus);
}
    
    
    
  

/*La fonction changement_score_im permet d'affecter au tableau de score l'image qui correspond au score actuelle
 */
void changement_score_im(donnees_t* donnees){
  for(int i = 0; i<4; i++){
    if(donnees->score[i].nb_score==0){
      donnees->score[i].score_im= donnees->im_score[0];
    }
    if(donnees->score[i].nb_score==1){
      donnees->score[i].score_im= donnees->im_score[1];
    }
    if(donnees->score[i].nb_score==2){
      donnees->score[i].score_im= donnees->im_score[2];
    }
    if(donnees->score[i].nb_score==3){
      donnees->score[i].score_im= donnees->im_score[3];
    }
    if(donnees->score[i].nb_score==4){
      donnees->score[i].score_im= donnees->im_score[4];
    }
    if(donnees->score[i].nb_score==5){
      donnees->score[i].score_im= donnees->im_score[5];
    }
    if(donnees->score[i].nb_score==6){
      donnees->score[i].score_im= donnees->im_score[6];
    }
    if(donnees->score[i].nb_score==7){
     donnees->score[i].score_im= donnees->im_score[7];
    }
    if(donnees->score[i].nb_score==8){
      donnees->score[i].score_im= donnees->im_score[8];
    }
    if(donnees->score[i].nb_score==9){
      donnees->score[i].score_im= donnees->im_score[9];
    }
  }
}

void eliminer_score_for_all(donnees_t* donnees){
  for(int i = 0; i<4; i++){
    donnees->score[i].score_x=-1000;
    donnees->score[i].score_y=-1000;
  }
}

void afficher_score(donnees_t* donnees){
  for(int i = 0; i<4; i++){
    afficherImage(donnees->score[i].score_im,donnees->score[i].score_x,donnees->score[i].score_y);
  }
}

//---------------------------------------VIE------------------------------------

void init_vie(vie_t vie[2]){
  Image image_vie =  chargerImage("piqure.png");
  image_vie = rotozoomImage(image_vie,44.8,0.08,0.08);
  for(int i = 0; i<2; i++){
    vie[i].vie_x = 420+20*i;
    vie[i].vie_y = -340;
    vie[i].vie_im = image_vie;
  }
}

/*Le nombre de vie afficher dans le coin de l'écran correspond au nombre de vaisseau encore utilisable apres la mort du vaisseau encore en fonction
 */
void afficher_vie(vie_t vie[2], vaisseau_t* vaisseau){
  for(int i = 0; i<vaisseau->vie-1; i++){
    afficherImage(vie[i].vie_im, vie[i].vie_x, vie[i].vie_y);
  }
}


//-----------------------------------ELIMINER----------------------------------

/*La fonction eliminer_donnees est utilisé afin d'éliminer les objets à faire disparaitre, par exemple quand un alien est touché par un missile il faut que l'alien disparaisse mais le missile aussi
 */
void eliminer_donnees(donnees_t* donnees){
  eliminer_alien(donnees->alien);
  eliminer_bomb(donnees->bomb);
  eliminer_bonus(&donnees->bonus);
  eliminer_missile(&donnees->missile);
  eliminer_vaisseau(&donnees->vaisseau);
}


/*La fonction eliminer_all est appelé en fin de jeu. Lorsque le jeu est terminé il faut que les boucliers, les aliens si il en reste, le vaisseau, les dégradations, le bonus et les bombes disparaissent
 */
void eliminer_all(donnees_t* donnees){
    eliminer_aliens_for_all(donnees->alien);
    eliminer_bombs_for_all(donnees->bomb);
    eliminer_shields_for_all(donnees->shield);
    eliminer_degradations_for_all(donnees->shield);
    eliminer_missile_for_all(&donnees->missile);
    eliminer_vaisseau_for_all(&donnees->vaisseau);
    eliminer_bonus_for_all(&donnees->bonus);
    eliminer_score_for_all(donnees);
}


void liberer_les_images(donnees_t* donnees){
  libererImage(donnees->vaisseau.vaisseau_im);
  for(int k = 0; k<5; k++){
    for(int i = 0; i<11; i++){
      libererImage(donnees->alien[k][i].alien_im);
    }
  }
  for(int j = 0; j<4; j++){
    libererImage(donnees->shield[j].shield_im);
  }
  libererImage(donnees->bonus.bonus_im);
}

//------------------------DEMARRAGE + INITIALISATION DE FIN---------------------



void init_interface_demarrage(donnees_t* donnees){
  donnees->demarrage_ou_pas = 1;
  Image image_dem = chargerImage("interface_dem2.png");
  Image im_press = chargerImage("press_enter.jpg");
  image_dem = rotozoomImage(image_dem,0,0.9,1.02);
  donnees->demarrage.demarrage_im = image_dem;
  donnees->demarrage.demarrage_x=-460;
  donnees->demarrage.demarrage_y=400;
  donnees->im_press = im_press;
}

void init_manuel(donnees_t* donnees){
  donnees->manuel_ou_pas = 1;
  Image image_manuel = chargerImage("manuel.png");
  donnees->manuel.manuel_im = image_manuel;
  donnees->manuel.manuel_x=-500;
  donnees->manuel.manuel_y=400;
}

void init_fin(donnees_t* donnees){
  donnees->win.compteur = 0;
  Image image_win = chargerImage("you_win.png");
  image_win = rotozoomImage(image_win,0,2.08333,2.96296);
  donnees->win.win_x = -500;
  donnees->win.win_y = 400;
  donnees->win.im_win= image_win;
  Image image_game_over = chargerImage("game_over_covid.jpg");
  image_game_over = rotozoomImage(image_game_over,0,1.333,0.899);
  donnees->game_over.game_over_im = image_game_over;
  donnees->game_over.game_over_x = -500;
  donnees->game_over.game_over_y = 400;
  donnees->fin_ou_pas=0;
  Image esc = chargerImage("press_esc.png");
  Image enter = chargerImage("press_enter.png");
  donnees->fin.press_esc = esc;
  donnees->fin.press_enter = enter;
  
}
/*La fonction initialiser_donnees rassemble toutes les initialisations 
 */
void initialiser_donnees(donnees_t* donnees){
    donnees->fini = 0;
    init_vaisseau(&donnees->vaisseau);
    init_missile(&donnees->missile);
    init_shield(donnees->shield);
    init_aliens(donnees->alien);
    init_bomb(donnees->bomb);
    init_bonus(&donnees->bonus);
    init_vie(donnees->vie);
    init_score(donnees);
    init_interface_demarrage(donnees);
    init_manuel(donnees);
    init_fin(donnees);
    donnees->sens_deplacement_alien = 1;
}


void first_initialiser_donnees(donnees_t* donnees){
  EtatSourisClavier esc = lireEtatSourisClavier();
  if(esc.touchesClavier[40]==1){
    initialiser_donnees(donnees);
  }
}

/*La fonction interface_demarrage permet de changer la caractéristique demarrage_ou_pas lorsqu'on appuie sur la touche entrée
 */
void interface_demarrage(donnees_t* donnees, EtatSourisClavier esc){
  if(donnees->demarrage_ou_pas==1
     &&esc.touchesClavier[40]==1){
    donnees->demarrage_ou_pas = 0;
  }
}

/*La fonction manuel permet de changer la caractéristique manuel lorsqu'on appuie sur la touche fléche du haut
 */
void manuel(donnees_t* donnees, EtatSourisClavier esc){
  if(donnees->demarrage_ou_pas==0
     &&donnees->manuel_ou_pas ==1
     &&esc.touchesClavier[82]==1){
    donnees->manuel_ou_pas = 0;
  }
}


void afficher_interface_demarrage(donnees_t* donnees, int count){
  afficherImage(donnees->demarrage.demarrage_im,donnees->demarrage.demarrage_x,donnees->demarrage.demarrage_y);
  if(count%2==0){//permet le clignotement de "press enter to play"
      afficherImage(donnees->im_press,-270,-270);
    }
  count =  count +1;
}


void afficher_manuel(donnees_t* donnees){
  afficherImage(donnees->manuel.manuel_im,donnees->manuel.manuel_x,donnees->manuel.manuel_y);
}



//-------------------------------------FIN--------------------------------------

/*La fonction game_over permet de mettre fin au jeu si la vie du vaisseau est égale à 1
 */
void game_over(donnees_t* donnees){
  if(donnees->vaisseau.vie==0){
    donnees->fin_ou_pas = 1;
    liberer_les_images(donnees);
    eliminer_all(donnees);
  }
}

void afficher_game_over(donnees_t* donnees){
  if(donnees->vaisseau.vie==0){
    afficherImage(donnees->game_over.game_over_im,donnees->game_over.game_over_x,donnees->game_over.game_over_y);
    afficherImage(donnees->fin.press_esc,-440,-300);
    afficherImage(donnees->fin.press_enter,60,-300);
  }
}

/*La fonction win permet de mettre fin au jeu si il n'y a plus d'alien à éliminer
 */
void win(donnees_t* donnees){
  donnees->win.compteur=0;
  for(int i = 0; i<5; i++){
    for(int j = 0; j<11; j++){
      if(donnees->alien[i][j].vie==2){
	donnees->win.compteur=donnees->win.compteur+1;
      }
    }
  }
  if(donnees->win.compteur==55){
    donnees->fin_ou_pas = 1;
    eliminer_all(donnees);
    liberer_les_images(donnees);
  }
}

void afficher_win(donnees_t* donnees){
  if(donnees->win.compteur==55){
    afficherImage(donnees->win.im_win, donnees->win.win_x, donnees->win.win_y);
    afficherImage(donnees->fin.press_esc,-440,-100);
    afficherImage(donnees->fin.press_enter,60,-100);
  }
}

/*La fonction choix_fin permet de demander au joueur si il veut recommencer une partie ou si il veut quitter le jeu
 */
void choix_fin(donnees_t* donnees,EtatSourisClavier esc){
  game_over(donnees);
  win(donnees);
  esc = lireEtatSourisClavier();
  if(donnees->fin_ou_pas==1
     &&esc.touchesClavier[40]==1){
    initialiser_donnees(donnees);
  }
  if(donnees->fin_ou_pas==1
     &&esc.touchesClavier[41]==1){
    donnees->fini = 1;
  }
}

void afficher_fin(donnees_t* donnees){
  afficher_game_over(donnees);
  afficher_win(donnees);
}
//--------------------------------LIBERER IMAGE---------------------------------








void mettre_a_jour(donnees_t* donnees, int count){
  EtatSourisClavier esc = lireEtatSourisClavier();
  interface_demarrage(donnees,esc);
  if(donnees->demarrage_ou_pas==0){
    manuel(donnees,esc);
    if(donnees->manuel_ou_pas==0){
    tirer_missile(esc,&donnees->vaisseau,&donnees->missile, donnees->shield);
    lacher_bomb(donnees->bomb,donnees->alien);
    deplacement_vai(esc,&donnees->vaisseau);
    deplacement_bombs(donnees->bomb);
    missile_obstacle(donnees);
    bomb_obstacle(donnees);
    alien_en_bas(donnees->alien, &donnees->vaisseau);
    deplacement_missile(&donnees->missile);
    bonus(donnees);
    deplacement_bonus(&donnees->bonus);
    eliminer_donnees(donnees);
    deplacement_alien(donnees->alien,60,30,count,&donnees->sens_deplacement_alien);
    score(donnees->score, donnees->alien, &donnees->bonus);
    changement_score_im(donnees);
    choix_fin(donnees,esc);
    }
  }
  if(esc.touchesClavier[41] == 1){
    donnees->fini = 1;
  }
}



void afficher(donnees_t* donnees, int count){
  if(donnees->demarrage_ou_pas==1){
    afficher_interface_demarrage(donnees, count);
  }
  if(donnees->demarrage_ou_pas==0){
    if(donnees->manuel_ou_pas == 1){
      afficher_manuel(donnees);
    }
    if(donnees->manuel_ou_pas == 0){
      afficher_shield(donnees->shield);
      missile_shield_passe_ou_pas(&donnees->missile, donnees->shield);
      if(donnees->missile.vie != 2){
	afficher_missile(&donnees->missile);
      }
      afficher_alien(donnees->alien);
      afficher_bomb(donnees->bomb);
      afficher_bonus(&donnees->bonus);
      afficher_vaisseau(&donnees->vaisseau);
      afficher_vie(donnees->vie, &donnees->vaisseau);
      afficher_score(donnees);
      afficher_fin(donnees);
    }
  }
}


int main(){
  donnees_t donnees;
  int count = 0;
  creerTableau();
  fixerModeBufferisation(1);
  fixerTaille(1000,800);
  choisirCouleurFond(0,0,0);
  initialiser_donnees(&donnees);
  while(donnees.fini != 1){
    mettre_a_jour(&donnees, count);
    afficher(&donnees, count);
    tamponner();
    effacerTableau();
       count = count + 1;
     }
     return EXIT_SUCCESS;
}
