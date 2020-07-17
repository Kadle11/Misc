#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#endif

#define SIZE 10

struct arrayList
{
  int val[SIZE];
  int next[SIZE];
  int head;
  int fHead;
};

typedef struct arrayList arrayList_t;

struct iterator
{
  arrayList_t *ptr_list;
  int current;
};

typedef struct iterator iterator_t;

void Init(arrayList_t *ptr_list);
iterator_t* InitIterator(arrayList_t *ptr_list, int current);
void Insert(arrayList_t *ptr_list, int key);
void Delete(arrayList_t *ptr_list, int key);
void Disp(arrayList_t *ptr_list);
iterator_t* Search(arrayList_t *ptr_list, int key);


