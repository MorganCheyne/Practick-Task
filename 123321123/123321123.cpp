#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

struct z {
	char name[20];
	char team[20];
	long points;
	long position;
	char engine[11];
	};

struct sp {
	char fio[20];
	long points;
	struct sp* sled;
	struct sp* pred;
	} *spisok;

int menu(int);
void more_points(struct z*, int NC);
void last_pos (struct z*, int NC);
void listing (struct z*, int NC);
void srvn(struct z*, int NC);
void diagr(struct z*, int NC, struct sp **spisok);
void alfalist(struct z* pilots, int NC, struct sp **spisok);
void vstavka(struct z* pilots, char* fio, int NC, struct sp **spisok);

void vstavka(struct z* pilots, char* fio, int NC, struct sp **spisok)
{
	int i;
	struct sp *nov,*nt,*z=0;
	for(nt=*spisok; nt!=0 && strcmp(nt->fio,fio)<0; z=nt, nt=nt->sled);
		if(nt && strcmp(nt->fio,fio)==0) return;
			nov=(struct sp *) malloc(sizeof(struct sp));
			strcpy(nov->fio,fio);
			nov->sled=nt;
			nov->points=0;
			nov->pred=z;
			for(i=0;i<NC;i++)
				if(strcmp(pilots[i].name,fio)==0)
					nov->points+=pilots[i].points;
					if(!z) *spisok=nov;
					else z->sled=nov;
					if(nt) nt->pred=nov;
					return;
}

void alfalist(struct z* pilots, int NC, struct sp **spisok)
{
	int i;
	struct sp *nt, *z;
	Console::BufferHeight=720;
	Console::BufferWidth=1280;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::DarkGreen;
	Console::Clear();
	printf("\n     Алфавитный список пилотов");
	printf(" \t\tОбратный алфавитный список пилотов");
	printf("\n =================================");
	printf(" \t===================================\n");
	if(!*spisok)
		for(i=0;i<NC;i++)
			vstavka(pilots, pilots[i].name, NC, spisok);
	for(nt=*spisok; nt!=0; nt=nt->sled)
		printf("\n %-20s %ld",nt->fio,nt->points);
	for(nt=*spisok, z=0; nt!=0; z=nt, nt=nt->sled);
		Console::CursorTop=4;
		for(nt=z, i=0; nt!=0; i++, nt=nt->pred){
			Console::CursorLeft=41;
			printf("%-20s %ld" ,nt->fio,nt->points);
			Console::CursorTop+=1;}
getch();
}

int main(array<System::String ^> ^args)
{
	int i, n, NC;
	FILE *in;
	struct z *pilots;
	struct sp *spisok =0;
	char dan[7][55] = {
	"У какого пилота максимальное количество очков?    ",
	"Какой пилот находится на последнем месте?         ",
	"Пилоты с двигателем Mercedes и 100+ баллами       ",
	"Вопрос сложный                                    ",
	"Диаграма                                          ",
	"Алфавитный и обратный списки пилотов              ",
	"Выход                                             "
	};
	char BlankLine[ ]= "                                                        ";
	setlocale(LC_CTYPE,"Russian");
	Console::CursorVisible::set(false);
	getch();
	if((in=fopen("dok.dat","r"))==NULL)
		{
		printf("\nФайл dok.dat не открыт !");
		getch(); exit(1);
		}
		fscanf(in,"%d",&NC);
		pilots=(struct z*)malloc(NC*sizeof(struct z));
		for(i=0;i<NC;i++)
			fscanf(in,"%s%s%ld%ld%s",pilots[i].name, pilots[i].team, &pilots[i].points, &pilots[i].position, pilots[i].engine);
		for(i=0;i<NC;i++)
			printf("\n%-20s %-20s %7ld %5d %s",
		pilots[i].name, pilots[i].team, pilots[i].points, pilots[i].position, pilots[i].engine);
		getch();
		
		while(1)
		{
			Console::ForegroundColor=ConsoleColor::DarkGreen;
			Console::BackgroundColor=ConsoleColor::DarkGreen;
			Console::Clear();
			Console::ForegroundColor=ConsoleColor::Black;
			Console::BackgroundColor=ConsoleColor::DarkGreen;
			Console::CursorLeft=10;
			Console::CursorTop=5;
			printf(BlankLine);
			for(i=0;i<7;i++)
				{
				Console::CursorLeft=10;
				Console::CursorTop=i+5;
				printf(" %s ",dan[i]);
				}
			Console::CursorLeft=10;
			Console::CursorTop=15;
			printf(BlankLine);
			n=menu(7);
			switch(n){
						case 1: more_points(pilots, NC); break;
						case 2: last_pos(pilots, NC); break;
						case 3: listing(pilots, NC); break;
						case 4: srvn(pilots, NC); break;
						case 5: diagr(pilots, NC, &spisok); break;
						case 6: alfalist(pilots, NC, &spisok); break;
						case 7: exit(0);
						}
		} 
		return 0;
}

int menu(int n)
{
		int y1=0,y2=n-1;
		char c=1;
		char dan[7][55] = {
		"У какого пилота максимальное количество очков?    ",
		"Какой пилот находится на последнем месте?         ",
		"Пилоты с двигателем Mercedes и 100+ баллами       ",
		"Вопрос сложный                                    ",
		"Диаграма                                          ",
		"Алфавитный и обратный списки пилотов              ",
		"Выход                                             "
		};
		while (c!=ESC)
		{
			switch(c) {
				case DOWN: y2=y1; y1++; break;
				case UP: y2=y1; y1--; break;
				case ENTER: return y1+1;
				case HOME: y2=y1; y1=0; break;
				case END: y2=y1; y1=n-1; break;
				}
		if(y1>n-1){y2=n-1;y1=0;}
		if(y1<0) {y2=0;y1=n-1;}
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::White;
		Console::CursorLeft=11;
		Console::CursorTop=y1+5;
		printf("%s",dan[y1]);
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::DarkGreen;
		Console::CursorLeft=11;
		Console::CursorTop=y2+5;
		printf("%s",dan[y2]);
		c=getch();
		}
	exit(0);
}

void more_points(struct z* pilots, int NC)
{
	int i=0; struct z best;
	strcpy(best.name, pilots[0].name);
	best.points = pilots[0].points;
	for(i=1;i<NC;i++)
		if (pilots[i].points>best.points)
			{
				strcpy(best.name, pilots[i].name);
				best.points = pilots[i].points;
			}
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::White;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Максимально - %ld очков ",best.points);
Console::CursorLeft=34;
Console::CursorTop=15;
printf("имеет пилот %s",best.name);
getch();
}

void last_pos(struct z* pilots, int NC)
{
	int i=0; struct z best;
	strcpy(best.name, pilots[0].name);
	best.position = pilots[0].position;
	for(i=1;i<NC;i++)
		if (pilots[i].points>best.points)
			{
				strcpy(best.name, pilots[i].name);
				best.position = pilots[i].position;
			}
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::White;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Последнее %ld место ",best.position);
Console::CursorLeft=29;
Console::CursorTop=15;
printf("занимает пилот %s",best.name);
getch();
}

void listing(struct z* pilots, int NC)
{
int i;
struct z* nt;
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::DarkGreen;
Console::Clear();
printf("\n\r Список пилотов, использующих двигатели Mercedes и набравших больше 100 очков");
printf("\n\r ============================================================================\n\r");
for(i=0,nt=pilots;i<NC;nt++,i++)
	if (nt->points>100 && strcmp(nt->engine,"Mercedes")==0)
		printf("\n\r %-20s %ld б.",nt->name,nt->points);
		getch();
}
void srvn(struct z* pilots, int NC)
{
int i, j;
int srvn=0;
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::DarkGreen;
Console::Clear();
for (i=0; i<NC; i++){
	for (j=i+1; j<NC; j++){
		if ((pilots[i].points == pilots[j].points) && (strcmp(pilots[i].engine, pilots[j].engine)==0)){
			srvn=1;
			break;
		}
	}
	if (srvn == 1) {
		printf("\n ФИО: %s, Двигатель: %s, Набранные очки: %ld \n ФИО: %s, Двигатель: %s, Набранные очки: %ld", pilots[i].name, pilots[i].engine, pilots[i].points, pilots[j].name, pilots[j].engine, pilots[j].points);
		srvn = 0;
	}
	if (srvn == 0) continue;
}
if (srvn == 0)
printf("\n ");
_getch();
}

void diagr(struct z* pilots, int NC, struct sp **spisok)
{
struct sp *nt;
int len, i, NColor;
long sum = 0;
char str1[20];
char str2[20];
System::ConsoleColor Color;
Console::ForegroundColor=ConsoleColor::Black;
Console::BackgroundColor=ConsoleColor::DarkGreen;
Console::Clear();
for (i=0; i<NC; i++)
	sum = sum+pilots[i].points;
if(!*spisok)
	for(i=0; i<NC; i++)
		vstavka(pilots, pilots[i].name, NC, spisok);
Color=ConsoleColor::DarkRed;
NColor=0;
for(nt=*spisok, i=0; nt!=0; nt = nt->sled, i++)
{
	sprintf(str1, "%s", nt->fio);
	sprintf(str2, "%3.lf%%", (nt->points*100./sum));
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::DarkGreen;
	Console::CursorLeft=5;
	Console::CursorTop=i+1;
	printf(str1);
	Console::CursorLeft=30;
	printf("%s", str2);
	Console::BackgroundColor=++Color;
	NColor++;
	Console::CursorLeft=40;
	for(len=0; len<nt->points*100./sum; len++) printf(" ");
	if (NColor==6){
		Color=ConsoleColor::DarkRed;
		NColor=0;
	}
}
_getch();
return;
}