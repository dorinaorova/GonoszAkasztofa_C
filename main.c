#include <stdio.h>
#include "tipp.h"
#include "debugmalloc.h"

int main()
{
    printf("Gonosz akasztofa\nJatekszabalyok:\nA jatek soran 11 eleted van, ha mind elfogyott, vesztettel.\nTippelj betukre a billentyuezet segitsegevel!\n\n");
    char ujjatek = '1';
    do{
                int eredmeny=jatek();
        if(eredmeny==1){
            printf("Vesztettel!\n");
        }
        else{
            printf("Nyertel!\n");
        }
        printf("Akarsz uj jatekot kezdeni?\n1 - igen\t2 - nem\n");
        scanf("%c", &ujjatek);
        printf("\n");
    }while(ujjatek=='1' || ujjatek == '\n');


    return 0;
}
