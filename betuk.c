#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "betuk.h"
#include "debugmalloc.h"

ListaBetuk *betu_beszur(ListaBetuk *eleje, char betu){ //már ellenõrzõtt betûk beszúrása: jo és roszs lista bõvítése
    ListaBetuk *uj=(ListaBetuk*)malloc(sizeof(ListaBetuk));
        if(uj==NULL){
            printf("Nem sikerult memoriat foglalni!");
            return;
        }
    uj->betu=betu;
    uj->kov =eleje;
    eleje=uj;
    return eleje;
}
static bool betu_ellenoriz(char uj, ListaBetuk *eleje){ //új betûk ellenõrzése: szerepel-e már a jo/rossz listában
    ListaBetuk *mozgo=eleje;
    while(mozgo!=NULL){
        if(uj==mozgo->betu){
                return true;
        }
        mozgo=mozgo->kov;
    }
    return false;
}
char ujbetu(ListaBetuk *jo, ListaBetuk *rossz){ //tippeles egy beture
    char beolvas[200];
    char uj;
    scanf("%s", beolvas);
    if(strlen(beolvas)!=1){
        printf("Csak egy beture tippelhetsz, adj meg egy masik betut: ");
        uj= ujbetu(jo, rossz);
    }
    else{
        uj = beolvas[0];
        if(!betu_ellenoriz(uj, jo) && !betu_ellenoriz(uj, rossz)){
            return uj;
        }
        else{
            printf("Erre mar tippeltel, adj meg egy masik betut: ");
            uj= ujbetu(jo, rossz);
        }
    }
    return uj;
}
