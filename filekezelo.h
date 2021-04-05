#ifndef FILEKEZELO_H_INCLUDED
#define FILEKEZELO_H_INCLUDED

typedef struct ListaSzo{
    char *szo;
    struct ListaSzo *kov;
}ListaSzo;

ListaSzo *filekezeles(int *h);

#endif // FILEKEZELO_H_INCLUDED
