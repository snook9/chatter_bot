//***********************Jonathan CASSAING************************************************************************************************
//***********************v1.0 -> 20/01/2009***********************************************************************************************
//***********************v1.1 -> 22/01/2009 : Prise en charge de Linux et Windows*********************************************************
//******************************************* Plusieurs problemes rencontres sous Windows, Patch en cours...******************************

//***********************ChatterBot_v1.1**************************************************************************************************
//****************************************************************************************************************************************
//****************************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MNAME 256							//Caracteres maximum pour les Key Words
#define MLIGNE 256							//Caracteres maximum pour les phrases
#define NAMEFIC "./Eliza_File.txt"					//Fichier du ChatterBot
#define CHATTERBOT	"__________ChatterBot_v1.1__________"		//Version du ChatterBot

#ifdef WIN32								//Si vous etes sous Windows 
#define CLEAN "cls"							//"cls" pour rafraichir l'ecran sous Windows
#elif defined (linux)						//Si vous etes sous Linux
#define CLEAN "clear"
#elif __APPLE__	
#define CLEAN "clear"					//"clear" pour rafraichir l'ecran sous Linux
#endif

int Cpt=1; 								//Variable globale de numerotation des structures (TELIZA.num)

typedef struct eliza{
	char cle1[MNAME];
	char cle2[MNAME];
	char cle3[MNAME];
	char cle4[MNAME];
	char phrase[MLIGNE];
	int num;
}TELIZA;

/*********************Fonctions administrateur*********************/
TELIZA tsaisie_mcle(void);						//Permet la saisie d'une structure TELIZA
void fenr_mcle(TELIZA *E, FILE * fq);					//Enregistre une structure a la fin du fichier "fq"
int fcreer_chatterbot(char *nomfic);					//Ajoute une structure au fichier "nomfic"
void faff_mcle(FILE *faff, TELIZA *E);					//Enregistre une structure au debut du fichier "fq", //Non utilisee
int faff_chatterbot(char *nomfic);					//Affichage du fichier "nomfic"
int frech_mcle(TELIZA *Eret, FILE * fq, int num1);			//Recherche une structure grace a son numero. La fonction retourne Num ou -1 si la structure n'a pas ete trouvee
int fmodif_mcle(TELIZA *Qamodif);					//Modifie une strucutre existante, return 1 si structure modifiee, return 0 si aucune structure modifiee
int fmodif_chatterbot(char *nomfic);					//Recherche une structure, la modifie et l'enregistre dans le fichier "nomfic"
/*********************Fonctions main*********************/
char menu_admin(void);							//Affiche le menu administrateur
char menu_util(void);							//Affiche le menu utilisateur
int gestion_admin(void);						//Gere le menu administrateur
int gestion_util(void);							//Gere le menu utilisateur
int CreateChatterFile(char *nomfic);					//Creation d'un fichier ChatterBot si aucun existe
/*********************Fonctions utilisateur*********************/
int str_inclus(char *mot, char *phrase);				//Recherche le mot "mot" dans la phrase "phrase"
int trouve_reponse(char *phUTIL, TELIZA *E);				//Recherche si "phUTIL" contient un des Key Words de "E". Retourne le nombre de Key Words
int fdialogue(FILE *fq, char *phUTIL, char *rep);			//Recherche si "phUTIL" contient un des Key Words d'une structure du fichier "fq"
int fchatterbot(char *UTIL, char *ficnameCHAT);				//Genere une session ChatterBot
int ftrace(char *UTIL);							//Affiche la derniere session


//****************************************************************************************************************************************
//********************Fonction int main (void)********************************************************************************************
//****************************************************************************************************************************************
int main(void)
{		
	char Etat;
	char Poubelle;
	
	CreateChatterFile(NAMEFIC);

	do
	{
		system(CLEAN);
		printf("\n"CHATTERBOT"\n\n");
		printf("\t(a) Mode administrateur\n");
		printf("\t(b) Mode utilisateur\n");
		printf("\t(c) Quitter\n");
		printf("\nVotre choix : ");
		scanf("%c",&Etat);
		scanf("%c",&Poubelle);

		switch(Etat)
		{
			case 'a':
			gestion_admin();
			break;

			case 'b':
			gestion_util();
			break;

			case 'c':
			printf("\nFin du programme\n\n");
			return 0;
			break;

			default :
			break;
		}
	}while(1);
}

//****************************************************************************************************************************************
//********************Fonction TELIZA tsaisie_mcle(void)**********************************************************************************
//****************************************************************************************************************************************
TELIZA tsaisie_mcle(void)
{
	TELIZA Eliza;
	TELIZA *pEliza;

	pEliza=&Eliza;
	memset(pEliza,0,sizeof(TELIZA));
	
	printf("\nEntrez le key word 1 : ");
	fgets(pEliza->cle1,MNAME,stdin);

	printf("\nEntrez le key word 2 : ");
	fgets(pEliza->cle2,MNAME,stdin);

	printf("\nEntrez le key word 3 : ");
	fgets(pEliza->cle3,MNAME,stdin);

	printf("\nEntrez le key word 4 : ");
	fgets(pEliza->cle4,MNAME,stdin);

	printf("\nEntrez la phrase de relance : ");
	fgets(pEliza->phrase,MLIGNE,stdin);

	pEliza->num=Cpt;

	Cpt++;						//Incremente la numerotation des structures
		
	return *pEliza;
}

//****************************************************************************************************************************************
//********************Fonction void fenr_mcle(TELIZA *E, FILE * fq)***********************************************************************
//****************************************************************************************************************************************
void fenr_mcle(TELIZA *E, FILE * fq)
{
	fseek(fq, 0, SEEK_END);	
	fwrite(E,sizeof(TELIZA),1,fq);
}

//****************************************************************************************************************************************
//********************Fonction int fcreer_chatterbot(char *nomfic)************************************************************************
//****************************************************************************************************************************************
int fcreer_chatterbot(char *nomfic)
{
	TELIZA Eliza1;
	TELIZA ElizaBuff;
	TELIZA *pElizaBuff;
	FILE *fq=NULL;	

	pElizaBuff=&ElizaBuff;

	fq=fopen(nomfic,"a+");
	if(fq==NULL)
	{
		printf("\nErreur d'ouverture (Fonction fcreer_chatterbot)");
		return 0;
	}

	fseek(fq, -sizeof(TELIZA), SEEK_END);	
	if( fread(pElizaBuff,sizeof(TELIZA),1,fq)!=0 )
	{
		Cpt=(pElizaBuff->num)+1;
	}

	Eliza1=tsaisie_mcle();

	fenr_mcle(&Eliza1, fq);

	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	return 1;
}

//****************************************************************************************************************************************
//********************Fonction void faff_mcle(FILE *faff, TELIZA *E)**********************************************************************
//****************************************************************************************************************************************
void faff_mcle(FILE *faff, TELIZA *E)			//Fonction non utilisee
{
	fseek(faff, 0, SEEK_SET);	
	fprintf(faff,"\n%s",E->cle1);
	fprintf(faff,"\n%s",E->cle2);
	fprintf(faff,"\n%s",E->cle3);
	fprintf(faff,"\n%s",E->cle4);
	fprintf(faff,"\n%s",E->phrase);
	fprintf(faff,"\n%d",E->num);
	
}

//****************************************************************************************************************************************
//********************Fonction int faff_chatterbot(char *nomfic)**************************************************************************
//****************************************************************************************************************************************
int faff_chatterbot(char *nomfic)
{
	FILE *fq=NULL;
	TELIZA Eliza;
	TELIZA *pEliza;	

	pEliza=&Eliza;
	memset(pEliza,0,sizeof(TELIZA));

	fq=fopen(nomfic,"r+");
	if(fq==NULL)
	{
		printf("\nErreur d'ouverture (Fonction faff_chatterbot)");
		return 0;
	}

	system(CLEAN);
	fseek(fq, 0, SEEK_SET);
	printf("\n____________________Affichage____________________\n");


	while(fread(pEliza,sizeof(TELIZA),1,fq)!=0 && !feof(fq))
	{
		printf("\n%s",pEliza->cle1);
		printf("%s",pEliza->cle2);
		printf("%s",pEliza->cle3);
		printf("%s",pEliza->cle4);
		printf("%s",pEliza->phrase);
		printf("%d",pEliza->num);
		printf("\n");
		memset(pEliza, 0, sizeof(TELIZA));
	}


	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	return 1;
	
}

//****************************************************************************************************************************************
//********************Fonction int frech_mcle(TELIZA *Eret, FILE * fq, int num1)**********************************************************
//****************************************************************************************************************************************
int frech_mcle(TELIZA *Eret, FILE * fq, int num1)
{
	memset(Eret,0,sizeof(TELIZA));
	fseek(fq, 0, SEEK_SET);

	while(fread(Eret,sizeof(TELIZA),1,fq)!=0 && !feof(fq))
	{
		if( (Eret->num) == num1 )
		{				
			
			return (Eret->num);
								
		}

	}
	
	return -1;
}

//****************************************************************************************************************************************
//********************Fonction void fmodif_mcle(TELIZA *Qamodif)**************************************************************************
//****************************************************************************************************************************************
int fmodif_mcle(TELIZA *Qamodif)
{
	int Test=-1;
	int Num;
	char Poubelle;
		
	do
	{
	printf("\nVoulez vous modifier la structure ?");
	printf("\n\t(0) NON");
	printf("\n\t(1) OUI\n");
	printf("Votre choix : ");	
	scanf("%d",&Test);
	scanf("%c",&Poubelle);
	}while( Test!=1 && Test!=0 );

	if( Test==1 )
	{
		Num = Qamodif->num;

		memset(Qamodif,0,sizeof(TELIZA));
	
		printf("\nEntrez le key word 1 : ");
		fgets(Qamodif->cle1,MNAME,stdin);

		printf("\nEntrez le key word 2 : ");
		fgets(Qamodif->cle2,MNAME,stdin);

		printf("\nEntrez le key word 3 : ");
		fgets(Qamodif->cle3,MNAME,stdin);

		printf("\nEntrez le key word 4 : ");
		fgets(Qamodif->cle4,MNAME,stdin);

		printf("\nEntrez la phrase de relance : ");
		fgets(Qamodif->phrase,MLIGNE,stdin);

		Qamodif->num = Num;

		return 1;

	}

	return 0;
}

//****************************************************************************************************************************************
//********************Fonction int fmodif_chatterbot(char *nomfic)************************************************************************
//****************************************************************************************************************************************
int fmodif_chatterbot(char *nomfic)
{
	int Num;
	char Poubelle;
	TELIZA *pEret;
	TELIZA Eret;
	FILE *fq;

	pEret = &Eret;

	printf("\nEntrez le numero de la question a modifier : ");	
	scanf("%d",&Num);
	scanf("%c",&Poubelle);
	fq=fopen(nomfic,"r+");
	if(fq==NULL)
	{
		printf("\nErreur d'ouverture (fmodif_chatterbot)");
	}

	Num = frech_mcle(pEret, fq, Num);

	if( Num==-1 )
	{
		printf("\nQuestion non trouvee\n");

	}
	else
	{
		if( fmodif_mcle(pEret)==1 )
		{
		fseek(fq, (Num-1)*sizeof(TELIZA), SEEK_SET);
		fwrite(pEret,sizeof(TELIZA),1,fq);
		printf("\nQuestion modifiee\n");
		}
	}

	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	return 0;
}

//****************************************************************************************************************************************
//********************Fonction char menu_admin(void)**************************************************************************************
//****************************************************************************************************************************************
char menu_admin(void)
{
	char Etat;
	char Poubelle;

	do
	{
		system(CLEAN);
		printf("\n\tMenu administrateur\n\n");
		printf("\t(a) Consulter un fichier chatterbot\n");
		printf("\t(b) Creer un fichier chatterbot\n");
		printf("\t(c) Maintenance d'un fichier\n");
		printf("\t(d) Quitter\n");
		printf("\nVotre choix : ");
		scanf("%c",&Etat);
		scanf("%c",&Poubelle);

	}while( Etat!='a' && Etat!='b' && Etat!='c' && Etat!='d');

	return Etat;
}

//****************************************************************************************************************************************
//********************Fonction int gestion_admin(void)************************************************************************************
//****************************************************************************************************************************************
int gestion_admin(void)
{
	char Etat;

	do
	{
		Etat=menu_admin();
		switch(Etat)
		{
			case 'a':
				faff_chatterbot(NAMEFIC);
				printf("\nTAPEZ ENTRER");
				getc(stdin);
			break;

			case 'b':
				fcreer_chatterbot(NAMEFIC);
			break;

			case 'c':
				fmodif_chatterbot(NAMEFIC);
				printf("\nTAPEZ ENTRER");
				getc(stdin);
			break;
	
			case 'd':
				return 0;	
			break;

			default :

			break;
		}
	}while(1);
}

//****************************************************************************************************************************************
//********************Fonction int str_inclus(char *mot, char *phrase)********************************************************************
//****************************************************************************************************************************************
int str_inclus(char *mot, char *phrase)
{
	int i=0;
	int j=0;

	while( phrase[i]!='\0' )			//Gestion de la casse
	{
		phrase[i]=toupper(phrase[i]);
		i++;
	}

	i=0;

	while( mot[i]!='\0' )
	{
		mot[i]=toupper(mot[i]);
		i++;
	}

	i=0;

	while( phrase[i]!='\0' )			//Recherche si "mot" se trouve dans "phrase"
	{
		while( phrase[i]==mot[j] )
		{
			if( phrase[i]=='\n' )
			{
				return 1;
			}
			i++;
			j++;
		}
	
		if( phrase[i]==' ' && mot[j]=='\n' )
		{
			return 1;
		}
		j=0;
		i++;
	}

	return 0;
}

//****************************************************************************************************************************************
//********************Fonction int trouve_reponse(char *phUTIL, TELIZA *E)****************************************************************
//****************************************************************************************************************************************
int trouve_reponse(char *phUTIL, TELIZA *E)		//Gestion du nombre de Key Words trouve
{
	int Cmp=0;

	if( str_inclus(E->cle1, phUTIL) )
	{
		Cmp++;
	}
	if( str_inclus(E->cle2, phUTIL) )
	{
		Cmp++;
	}
	if( str_inclus(E->cle3, phUTIL) )
	{
		Cmp++;
	}
	if( str_inclus(E->cle4, phUTIL) )
	{
		Cmp++;
	}
	return Cmp;
}

//****************************************************************************************************************************************
//********************Fonction int fdialogue(FILE *fq, char *phUTIL, char *rep)***********************************************************
//****************************************************************************************************************************************
int fdialogue(FILE *fq, char *phUTIL, char *rep)	//Le ChatterBot repond par la phrase de la structure qui possede le plus de Key Words
{
	TELIZA *pEliza;
	TELIZA Eliza;
	int Cmp=0;
	int Max=0;
	int Num_Max=0;

	pEliza=&Eliza;

	memset(pEliza,0,sizeof(TELIZA));
	fseek(fq, 0, SEEK_SET);

	while(fread(pEliza,sizeof(TELIZA),1,fq)!=0 && !feof(fq))
	{	
		Cmp=trouve_reponse(phUTIL, pEliza);
		if( Cmp>Max )
		{
			Max=Cmp;
			Num_Max=pEliza->num;
		}
	}

	if(Max==0)
	{	
		return 0;
	}
	fseek(fq, (Num_Max-1)*sizeof(TELIZA), SEEK_SET);
	fread(pEliza,sizeof(TELIZA),1,fq);
	strcpy(rep, pEliza->phrase);
	
	return 1;
}

//****************************************************************************************************************************************
//********************Fonction int fchatterbot(char *UTIL, char *ficnameCHAT)*************************************************************
//****************************************************************************************************************************************
int fchatterbot(char *UTIL, char *ficnameCHAT)
{
	char Reponse[MLIGNE]="";
	char Question[MLIGNE]="";
	FILE *fq=NULL;
	FILE *Conv=NULL;

	fq=fopen(NAMEFIC,"r");
	if(fq==NULL)
	{
		printf("\nErreur d'ouverture (fchatterbot)");
		return -1;
	}

	Conv=fopen(UTIL,"w");
	if(Conv==NULL)
	{
		printf("\nErreur de creation du fichier conv (fchatterbot)");
	}

	system(CLEAN);
	printf("\n\tSession chatterbot ouverte\n\n");
	printf("// Saisissez 'STOP' pour arreter la session\n");
	fprintf(Conv,"\n\tSession chatterbot ouverte\n\n");
	fprintf(Conv,"// Saisissez 'STOP' pour arreter la session\n");
	
	do
	{
		printf("\n%s : ",UTIL);
		fgets(Question,MLIGNE,stdin);	
		fprintf(Conv,"\n%s : %s", UTIL, Question);
		if( strcmp(Question, "STOP\n") == 0 )
		{
			break;
		}
		if( fdialogue(fq, Question, Reponse)==0 )
		{
			printf("\nChatterBot : Il faut m'en dire plus\n");
			fprintf(Conv,"\nChatterBot : Il faut m'en dire plus\n");
		}
		else
		{
			printf("\nChatterBot : %s",Reponse);
			fprintf(Conv,"\nChatterBot : %s",Reponse);
		}
		
		
	}while(1);

	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	printf("\n\tSession chatterbot fermee\n");
	fprintf(Conv,"\n\tSession chatterbot fermee\n");

	if (Conv!=NULL)
	{
		fclose(Conv);
		Conv=NULL;
	}
	
	return 0;
}

//****************************************************************************************************************************************
//********************Fonction int ftrace(char *UTIL)*************************************************************************************
//****************************************************************************************************************************************
int ftrace(char *UTIL)					//Lit, caractere par caractere, le fichier contenant la conversation de l'utilisateur actuel
{
	FILE *Conv;
	char Buffer;

	Conv=fopen(UTIL,"r");
	if(Conv==NULL)
	{
		printf("\nPas de session pour cet utilisateur\n");
		return 0;
	}
	
	system(CLEAN);
	printf("\n____________________Affichage de la session %s____________________\n",UTIL);

	while(fread(&Buffer,sizeof(Buffer),1,Conv)!=0 && !feof(Conv))
	{
		printf("%c",Buffer);
	}

	if (Conv!=NULL)
	{
		fclose(Conv);
		Conv=NULL;
	}

	return 1;		
}

//****************************************************************************************************************************************
//********************Fonction char menu_util(void)***************************************************************************************
//****************************************************************************************************************************************
char menu_util(void)
{
	char Etat;
	char Poubelle;

	do
	{
		system(CLEAN);
		printf("\n\tMenu utilisateur\n\n");
		printf("\t(a) Afficher sa derniere session\n");
		printf("\t(b) Nouvelle session\n");
		printf("\t(c) Quitter\n");
		printf("\nVotre choix : ");
		scanf("%c",&Etat);
		scanf("%c",&Poubelle);

	}while( Etat!='a' && Etat!='b' && Etat!='c' );

	return Etat;
}

//****************************************************************************************************************************************
//********************Fonction int gestion_util(void)*************************************************************************************
//****************************************************************************************************************************************
int gestion_util(void)
{
	char Util[MLIGNE]="";
	char Etat;
	int i=0;
	FILE *Conv=NULL;	

	system(CLEAN);

	do
	{
		
		printf("\nEntrez votre nom d'utilisateur : ");

		do
		{
			
			
			if( i < MLIGNE )
			{
				Util[i]=getc(stdin);
				i++;
			}
			else
			{
				break;
			}
		}while( Util[i-1]!='\n' );

		Util[i-1]='\0';

		Conv=fopen(Util,"r");
		if(Conv==NULL)
		{
		
		}
		else
		{
			printf("\nL'utilisateur existe deja\n");
			i=0;
			memset(Util,0,MLIGNE);
		}

	}while(Conv!=NULL);

	if (Conv!=NULL)
	{
		fclose(Conv);
		Conv=NULL;
	}

	do
	{
		Etat=menu_util();
		switch(Etat)
		{
			case 'a':
			ftrace(Util);
			printf("\nTAPEZ ENTRER");
			getc(stdin);
			break;
	
			case 'b':
			fchatterbot(Util, NAMEFIC);
			printf("\nTAPEZ ENTRER");
			getc(stdin);
			break;
	
			case 'c':
			return 0;
			break;
	
			default :
			break;
		}
	}while(1);

}

//****************************************************************************************************************************************
//********************Fonction int CreateChatterFile(char *nomfic)*************************************************************************************
//****************************************************************************************************************************************
int CreateChatterFile(char *nomfic)
{
	FILE *fq=NULL;

	fq=fopen(nomfic,"a+");
	if(fq==NULL)
	{
		printf("\nErreur d'ouverture (Fonction CreateChatterFile)");
		return 0;
	}

	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	return 1;
}

//****************************************************************************************************************************************
//****************************************************************************************************************************************
//****************************************************************************************************************************************

//****************************************************************************************************************************************
//****************************************************************************************************************************************
//****************************************************************************************************************************************
