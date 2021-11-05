//***********************Jonathan CASSAING****************************
//***********************16/01/2009***********************************
//***********************ChatterBot_v0.2b******************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MNAME 256
#define MLIGNE 256
#define NAMEFIC "./Eliza_File.txt"
#define CLEAN "clear" //ecrire "cls" a la place de "clear" pour une utilisation sous windows
#define CHATTERBOT	"__________ChatterBot_v0.2b__________"

int Cpt=1; //Variable globale de numerotation des structures (Num)

typedef struct eliza{
	char cle1[MNAME];
	char cle2[MNAME];
	char cle3[MNAME];
	char cle4[MNAME];
	char phrase[MLIGNE];
	int num;
}TELIZA;

TELIZA tsaisie_mcle(void);
void fenr_mcle(TELIZA *E, FILE * fq);
int fcreer_chatterbot(char *nomfic);
void faff_mcle(FILE *faff, TELIZA *E);
int faff_chatterbot(char *nomfic);
int frech_mcle(TELIZA *Eret, FILE * fq, int num1);
void fmodif_mcle(TELIZA *Qamodif);
int fmodif_chatterbot(char *nomfic);

char menu_admin(void);
void gestion_admin();

int str_inclus(char *mot, char *phrase);
int trouve_reponse(char *phUTIL, TELIZA *E);
int fdialogue(FILE *fq, char *phUTIL, char *rep);

int main(void)
{		
	char Etat;
	char Poubelle;
	char Reponse[MLIGNE]="";
	char Question[MLIGNE]="";
	FILE *fq;
	
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
			menu_admin();
			printf("\nTAPEZ ENTRER");
			getc(stdin);
			break;

			case 'b':
			fq=fopen(NAMEFIC,"r");
			if(fq==NULL)
			{
				printf("\nErreur d'ouverture (Main)");
				return 0;
			}
			printf("\nPosez une question : ");
			fgets(Question,MLIGNE,stdin);
			if( fdialogue(fq, Question, Reponse)==-1 )
			{
				printf("\nIl faut m'en dire plus\n");
			}
			else
			{
				printf("\n%s",Reponse);
			}
			printf("\nTapez ENTRER");
			getc(stdin);
			if (fq!=NULL)
			{
				fclose(fq);
				fq=NULL;
			}
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

	Cpt++;		//Incremente la numerotation pour le mode admin
		
	return *pEliza;
}

void fenr_mcle(TELIZA *E, FILE * fq)
{
	fseek(fq, 0, SEEK_END);	
	fwrite(E,sizeof(TELIZA),1,fq);
}

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

void faff_mcle(FILE *faff, TELIZA *E)
{
	fseek(faff, 0, SEEK_SET);	//peut etre à enlever	
	fprintf(faff,"\n%s",E->cle1);
	fprintf(faff,"\n%s",E->cle2);
	fprintf(faff,"\n%s",E->cle3);
	fprintf(faff,"\n%s",E->cle4);
	fprintf(faff,"\n%s",E->phrase);
	fprintf(faff,"\n%d",E->num);
	
}

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

	fseek(fq, 0, SEEK_SET);
	printf("\n____________________Affichage____________________\n");

	//li dans le fichier rep, 1 fois, la taille de Personne et le range dans l'endroit pointé par p
	while(fread(pEliza,sizeof(TELIZA),1,fq)!=0 && !feof(fq))
	{
		printf("\n%s",pEliza->cle1);
		printf("%s",pEliza->cle2);
		printf("%s",pEliza->cle3);
		printf("%s",pEliza->cle4);
		printf("%s",pEliza->phrase);
		/***********A DELETE A LA FIN**********/
		printf("%d",pEliza->num);
		/**************************************/
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

void fmodif_mcle(TELIZA *Qamodif)
{
	int Test=-1;
	int Num;
	char Poubelle;
		
	do
	{
	printf("\nVoulez vous modifier les mots clef de la structure ?");
	printf("\n(0) NON");
	printf("\n(1) OUI\n");	
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

	}
}

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
		printf("\nQuestion non trouvee");

	}
	else
	{
		fmodif_mcle(pEret);
		fseek(fq, (Num-1)*sizeof(TELIZA), SEEK_SET);
		fwrite(pEret,sizeof(TELIZA),1,fq);
		printf("\nQuestion modifiee");
	}

	if (fq!=NULL)
	{
		fclose(fq);
		fq=NULL;
	}

	return 0;
}

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
	
			break;
	
			case 'd':
				return Etat;	
			break;

			default :

			break;
		}

	}while(1);

}

int str_inclus(char *mot, char *phrase)
{
	int i=0;
	int j=0;

	while( phrase[i]!='\0' ) //Mettre en majuscule
	{
		phrase[i]=toupper(phrase[i]);
		i++;
	}

	i=0;

	while( mot[i]!='\0' ) //Mettre en majuscule
	{
		mot[i]=toupper(mot[i]);
		i++;
	}

	i=0;

	while( phrase[i]!='\0' )
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

int trouve_reponse(char *phUTIL, TELIZA *E)	//La fonction retourne le nombre de mot cle trouve
{
	int Cpt=0;

	if( str_inclus(E->cle1, phUTIL) )
	{
		Cpt++;
	}
	if( str_inclus(E->cle2, phUTIL) )
	{
		Cpt++;
	}
	if( str_inclus(E->cle3, phUTIL) )
	{
		Cpt++;
	}
	if( str_inclus(E->cle4, phUTIL) )
	{
		Cpt++;
	}
	return Cpt;
}

int fdialogue(FILE *fq, char *phUTIL, char *rep)
{
	TELIZA *pEliza;
	TELIZA Eliza;
	int Cpt=0;
	int Max=0;
	int Num_Max=0;

	pEliza=&Eliza;

	memset(pEliza,0,sizeof(TELIZA));
	fseek(fq, 0, SEEK_SET);

	while(fread(pEliza,sizeof(TELIZA),1,fq)!=0 && !feof(fq))
	{	
		Cpt=trouve_reponse(phUTIL, pEliza);
		if( Cpt>Max )
		{
			Max=Cpt;
			Num_Max=pEliza->num;
		}
	}

	if(Max==0)
	{	
		return -1;
	}
	fseek(fq, (Num_Max-1)*sizeof(TELIZA), SEEK_SET);
	fread(pEliza,sizeof(TELIZA),1,fq);
	strcpy(rep, pEliza->phrase);
	
	return 1;
}
