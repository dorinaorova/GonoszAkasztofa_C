#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tipp.h"
#include "filekezelo.h"
#include "betuk.h"
#include "debugmalloc.h"

static void Hely_lista_felszabadit(Hely *eleje){
    Hely *mozgo = eleje;
    while(mozgo!=NULL){
        Hely *kov = mozgo->kov;
        free(mozgo->hely);
        free(mozgo);
        mozgo=kov;
    }
}
static void Szavak_lista_felszabadit(ListaSzo *eleje){
    ListaSzo *mozgo = eleje;
    while(mozgo!=NULL){
        ListaSzo *kov = mozgo->kov;
        free(mozgo->szo);
        free(mozgo);
        mozgo=kov;
    }
}
static void Betuk_lista_felszabadit(ListaBetuk *eleje){
    ListaBetuk *mozgo = eleje;
    while(mozgo!=NULL){
        ListaBetuk *kov = mozgo->kov;
        free(mozgo);
        mozgo=kov;
    }
}

//------------------------------------------------------------------------------------------------------

static ListaSzo *joszavak(ListaSzo *eleje, int jodb, char betu){ //kitorli azokat a szavakat, amik nem lehetnek megoldasok a tippelesek utan, mert tobbszor szerepel bennuk az adott betu
    ListaSzo *lemarado = NULL;
    ListaSzo * mozgo =eleje;
    while(mozgo!=NULL){
        int db=0;
        for(int i=0; i<strlen(mozgo->szo); i++){
            if(mozgo->szo[i]==betu) db++;
        }
        if(db!=jodb){
            ListaSzo *torlo=mozgo;
            mozgo=mozgo->kov;
            if(lemarado==NULL){
                eleje=torlo->kov;
            }
            else{
                lemarado->kov = torlo->kov;
            }
            free(torlo->szo);
            free(torlo);
        }
        else{
            lemarado=mozgo;
            mozgo=mozgo->kov;
        }
    }
    return eleje;
}
static int betu_legkevesebbszer(char betu, ListaSzo *eleje){ //megszamolja h a szavak kozott melyikekben van benne a legkevesebbszer a betu
    ListaSzo *mozgo =eleje;
    int mindb=strlen(eleje->szo);
    while(mozgo!=NULL){
        int db=0;
        for(int i=0; i<strlen(mozgo->szo); i++){
            if(mozgo->szo[i]==betu) db++;
        }
        if(mindb>db) mindb=db;
        mozgo=mozgo->kov;
    }
    return mindb;
}

//------------------------------------------------------------------------------------------------------

static Hely* elrendezes_keres(char *elrendezes, Hely *helyek){ //megkeres egy szamot a helyek nevu binaris faban
    Hely *mozgo=helyek;
    while(mozgo!=NULL){
        if(strcmp(mozgo->hely, elrendezes)==0){
            return mozgo;
        }
        mozgo=mozgo->kov;
    }
    return NULL;
}
static Hely *elrendezes_beszur(char *elrendezes, Hely *helyek){ //beszur egy uj szamot a helyek nevu listaba
    Hely *uj=(Hely*)malloc(sizeof(Hely));
    uj->hely=(char*)malloc(sizeof(char)*(strlen(elrendezes)+1));
    strcpy(uj->hely, elrendezes);
    uj->hely[strlen(elrendezes)]='\0';
    uj->db=0;
    uj->kov = helyek;
    helyek=uj;
    return helyek;
}
static Hely* hely_maxdb(Hely *helyek){ //megkeresi, hogy a betuk milyen elrendezesben szerepelnek a legtobbszor
    Hely *mozgo=helyek->kov;
    Hely *max=helyek;
    while(mozgo!=NULL){
        if(max->db<mozgo->db) max=mozgo;
        mozgo=mozgo->kov;
    }
    return max;
}
static void *helyek(int db, ListaSzo *eleje, char betu, char *vissza){
    Hely *helyek=NULL;
    ListaSzo *mozgo;
    for(mozgo=eleje; mozgo!=NULL; mozgo=mozgo->kov){
        char *elrendezes=(char*)malloc(sizeof(char)*strlen(eleje->szo)+1);
        for(int i=0; i<strlen(mozgo->szo); i++){
            if(mozgo->szo[i]==betu){
                elrendezes[i] = betu;
            }
            else elrendezes[i] = '.';
        }
        elrendezes[strlen(eleje->szo)]='\0';

        Hely *hol=elrendezes_keres(elrendezes, helyek);
        if(hol!= NULL){
            hol->db+=1;
        }
        else{
            helyek=elrendezes_beszur(elrendezes, helyek);
        }
        free(elrendezes);
    }
    Hely *max=hely_maxdb(helyek);
    strcpy(vissza, max->hely);
    Hely_lista_felszabadit(helyek);
}

//------------------------------------------------------------------------------------------------------

static bool helyek_ellenoriz(char *szo, char *jo, int betu){ //megnezi, hogy a szoban jo elrendezesben vannak-e a betuk
    char elrendezes[strlen(jo)+1];
    for(int i=0; i<strlen(szo); i++){
        if(szo[i]==betu){
            elrendezes[i]=betu;
        }
        else elrendezes[i]= '.';
    }
    elrendezes[strlen(jo)]='\0';
    if (strcmp(elrendezes, jo)!=0) return true;
    else return false;
}
static ListaSzo *joszavak_hely(ListaSzo *eleje, char *elrendezes, char betu){ //kitorli azokat a szavakat, amikben nem jo elrendezesben vannak a betuk
    ListaSzo *lemarado = NULL;
    ListaSzo * mozgo =eleje;
    while(mozgo!=NULL){
        if(helyek_ellenoriz(mozgo->szo, elrendezes, betu)){
            ListaSzo *torlo=mozgo;
            mozgo=mozgo->kov;
            if(lemarado==NULL){
                eleje=torlo->kov;
            }
            else{
                lemarado->kov = torlo->kov;
            }
            free(torlo->szo);
            free(torlo);
        }
        else{
            lemarado=mozgo;
            mozgo=mozgo->kov;
        }
    }
    return eleje;
}

//------------------------------------------------------------------------------------------------------

static ListaSzo* tipp(int *elet, int *maradek, ListaSzo *szavak, char *szo, ListaBetuk **jo, ListaBetuk **rossz){
    char betu;
    betu=ujbetu(*jo, *rossz); //uj betu beolvasasa
    int db;
    db=betu_legkevesebbszer(betu, szavak);
    szavak=joszavak(szavak, db, betu); //kitorli azokat a szavakat, amikben tobbszor szerepel a betu
    if(db==0){ //ha van olyan szo, amiben nincs meg ez a betu
        *elet-=1;
        *rossz=betu_beszur(*rossz, betu);
        printf("Nincs benne ilyen betu. Meg %d eleted maradt. %s\n", *elet, szo);
    }
    else{
        *maradek-=db;
        *jo=betu_beszur(*jo, betu);
        char *elrendezes=(char*)malloc(sizeof(char)*strlen(szavak->szo)+1);
        if(elrendezes==NULL){
            printf("Nem sikerult memoriat foglalni!");
            return szavak;
        }
        helyek(db, szavak, betu, elrendezes);
        szavak=joszavak_hely(szavak, elrendezes, betu);
        int k=0;
        for(int i=0; i<strlen(elrendezes); i++){ //felepiti a szot
            if(elrendezes[i]==betu){
                szo[k]=betu;
            }
            k+=2;
        }
        printf("%d darab ilyen betu van benne. Meg %d eleted maradt. %s\n",db, *elet, szo);
        free(elrendezes);
    }
    return szavak;
}
int jatek(){
    //letrehozza a szukseges változokat
    ListaBetuk *jo=NULL;
    ListaBetuk *rossz = NULL;
    ListaSzo *szavak=NULL;
    int hossz;
    int elet =11;
    szavak = filekezeles(&hossz);
    int maradek=hossz;
    char *szo=(char*)malloc(sizeof(char)*(2*hossz)+1); //szo kinezete, ez fog megjelenni a kepernyon: ismert-iseretlen betuk
        if(szo==NULL){
            printf("Nem sikerult memoriat foglalni!");
            return;
        }
    printf("Gondoltam egy %d betus szora: ", hossz);
    int k=0;
    for(int i=0; i<hossz; i++){
        szo[k]='_';
        k++;
        szo[k]=' ';
        k++;
    }
    szo[2*hossz]='\0';
    printf("%s\n", szo);

    while(elet >0 && maradek >0){
        printf("\nMire tippelsz? ");
        szavak=tipp(&elet, &maradek, szavak, szo, &jo, &rossz);
    }

    if(elet==0){
        printf("\nA %s szora gondoltam.\n", szavak->szo);
    }
    //lefoglalt teruletek felszabaditasa
    free(szo);
    Szavak_lista_felszabadit(szavak);
    Betuk_lista_felszabadit(jo);
    Betuk_lista_felszabadit(rossz);

    if(elet==0){ //elfogyott a jatekos elete, vesztett
        return 1;
    }
    else if(maradek == 0){ //kitalalta a jatekos a szot, nyert
        return 0;
    }

}
