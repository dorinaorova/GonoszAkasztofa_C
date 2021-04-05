#ifndef BETUK_H_INCLUDED
#define BETUK_H_INCLUDED

typedef struct ListaBetuk{
 char betu;
 struct ListaBetuk *kov;
}ListaBetuk;

ListaBetuk *betu_beszur(ListaBetuk *eleje, char betu);
char ujbetu(ListaBetuk *jo, ListaBetuk *rossz);


#endif // BETUK_H_INCLUDED
