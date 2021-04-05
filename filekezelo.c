#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include "filekezelo.h"
#include "debugmalloc.h"

static bool hossz_van(ListaSzo *szavak, int hossz){ //megnezi van-e ilyen hosszu szo a szavak kozott
    ListaSzo *mozgo=szavak;
    while(mozgo!=NULL){
        if(strlen(mozgo->szo)==hossz) return true;
        mozgo=mozgo->kov;
    }
    return false;
}
static void kisbetu(char* szo){ //a nagybetuket kicsereli kicsikre
    for(int i=0; i<strlen(szo); i++){
        szo[i]=tolower(szo[i]);
    }
}

static int maxhossz(ListaSzo *szavak){ //megkeresi milyen hosszu a leghosszabb szo
    int max=0;
    ListaSzo *mozgo;
    for(mozgo=szavak; mozgo!=NULL; mozgo=mozgo->kov){
        if(max<strlen(mozgo->szo)) max = strlen(mozgo->szo);
    }
    return max;
}
static ListaSzo *johossz(ListaSzo *eleje, int *hossz){ //kitorli a nem jo hosszusagu szavakat
    ListaSzo *lemarado = NULL;
    ListaSzo * mozgo =eleje;
    while(mozgo!=NULL){
        if(strlen(mozgo->szo) != *hossz){
            ListaSzo *torlo=mozgo;
            mozgo=mozgo->kov;
            if(lemarado==NULL){
                eleje =torlo->kov;
            }
            else{
                lemarado->kov = torlo->kov;
            }
            free(torlo->szo);
            free(torlo);
        }
        else{
            lemarado = mozgo;
            mozgo = mozgo->kov;
        }
    }
    return eleje;
}
ListaSzo *filekezeles(int *hossz){
    //filekezelés
    FILE *fp;
    fp=fopen("magyar_ascii.txt", "r");
    if(fp==NULL){
        perror("Nem sikerult megnyitni a fajlt\n");
        return NULL;
    }

    //szavak beolvasasa lancolt listaba
    ListaSzo *szavak=NULL;
    char ujszo[50];
    while(fgets(ujszo, 50, fp)){
            if(strlen(ujszo)!=2){ //egy betus szavak
                ujszo[strlen(ujszo)-1]='\0';
                kisbetu(ujszo);
                ListaSzo *uj;
                uj=(ListaSzo*)malloc(sizeof(ListaSzo));
                    if(uj==NULL){
                        printf("Nem sikerult memoriat foglalni!");
                        return;
                    }
                uj->szo=(char*)malloc(sizeof(char)*(strlen(ujszo)+1));
                    if(uj->szo==NULL){
                        printf("Nem sikerult memoriat foglalni!");
                        return;
                    }
                strcpy(uj->szo, ujszo);
                uj->kov=szavak;
                szavak=uj;
            }
    }
    fclose(fp);

    //leghosszabb szo megkeresese
    int max;
    max=maxhossz(szavak);

    //random hosszusagu szo kivalasztasa
    srand(time(0));
    do{
        *hossz=rand()%max+1;
    }while(*hossz==1 && !hossz_van(szavak, *hossz));


    //jo hosszusagu szavak kivalogatasa
    szavak=johossz(szavak, hossz);


    return szavak;
}

