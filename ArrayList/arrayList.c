#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arrayList.h"

void Init(arrayList_t *ptr_list)
{
  int i;
  for(i = 0; i<SIZE-1; i++)
    {
      ptr_list->next[i] = i+1;
    }
  ptr_list->next[i] = -1;
  ptr_list->head = -1;
  ptr_list->fHead = 0;
}

iterator_t* InitIterator(arrayList_t *ptr_list, int current)
{
  iterator_t *ptr_it = (iterator_t*)malloc(sizeof(iterator_t));
  ptr_it->ptr_list = ptr_list;
  ptr_it->current = current;
  return ptr_it;
}

void Disp(arrayList_t *ptr_list)
{
  int curr = ptr_list->head;
  while(curr != -1)
    {
      printf("%d ", ptr_list->val[curr]);
      curr = ptr_list->next[curr];
    }
  printf("\n");
}

iterator_t* Search(arrayList_t *ptr_list, int key)
{
  int curr = ptr_list->head;
  while(curr != -1 && ptr_list->val[curr]!=key)
    {
      curr = ptr_list->next[curr];
    }
  
  return InitIterator(ptr_list, curr);
}

void Insert(arrayList_t *ptr_list, int key)
{
  if(ptr_list->fHead == -1)
    {
      printf("ArrayList is Full. Unable to Insert\n");
      return;
    }

  int freeIdx = ptr_list->fHead;
  ptr_list->fHead = ptr_list->next[freeIdx];
  
  if(ptr_list->head == -1)
    {
      ptr_list->head++;
      ptr_list->val[freeIdx] = key;
      ptr_list->next[ptr_list->head] = -1;
      return;
    }
 
  int prev = -1;
  int curr = ptr_list->head;
  while(curr!=-1 && ptr_list->val[curr] < key)
    {
      prev = curr;
      curr = ptr_list->next[curr];
    }

  if(prev != -1)
    {
      ptr_list->next[prev] = freeIdx;
      ptr_list->next[freeIdx] = curr;
      ptr_list->val[freeIdx] = key;
    }
  else
    {
      ptr_list->val[freeIdx] = key;
      ptr_list->next[freeIdx] = curr;
      ptr_list->head = freeIdx;
    }
}

void Delete(arrayList_t *ptr_list, int key)
{
  if(ptr_list->head == -1)
    {
      printf("Empty List, Unable to Delete\n");
      return;
    }

  int prev = -1;
  int curr = ptr_list->head;
  int freeIdx = ptr_list->fHead;

  while(curr!=-1 && ptr_list->val[curr] != key)
    {
      prev = curr;
      curr = ptr_list->next[curr];
    }

  if(curr == -1)
    {
      printf("Element not found. Unable to Delete\n");
      return;
    }
  
  if(prev == -1)
    {
      ptr_list->head = ptr_list->next[curr];
      ptr_list->fHead = curr;
      ptr_list->next[curr] = freeIdx;
    }
  else
    {
      ptr_list->next[prev] = ptr_list->next[curr];
      ptr_list->fHead = curr;
      ptr_list->next[curr] = freeIdx;
    }
}

int main()
{
  arrayList_t *ptr_list = (arrayList_t*)malloc(sizeof(arrayList_t));
  Init(ptr_list);

  srand(time(0));
  int x = 0;
  for(int i=0; i < 10; i++)
    {
      x = rand()%10;
      printf("Inserting %d\n", x);
      Insert(ptr_list, x);
      Disp(ptr_list);
      x = rand()%10;
      printf("Deleting %d\n", x);
      Delete(ptr_list, x);
      Disp(ptr_list);
      x = rand()%10;
      printf("Inserting %d\n", x);
      Insert(ptr_list, x);
      Disp(ptr_list);
    }
  
  free(ptr_list);
}
