#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct Node{
int objID;
long int *k_la_ptr;
bool mutex;
int semaphore;
struct Node* next;
struct Node* prev;
};
struct Node* head;

struct Node* GetNewNode(__u64 id, long int *ptr, bool mutex, int sem)
{
struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
newNode->objID = id;
newNode->*k_la_ptr = *ptr;
newNode->mutex = mutex;
newNode->semaphore = sem;
newNode->next = NULL;
newNode->prev = NULL;
} 
void Insert(__u64 id, long int *ptr, bool mut, int sem)
{
struct Node* temp = head;
struct Node* newNode = GetNewNode(id, ptr, mut, sem);
if(head==NULL)
	{
	head = newNode;
	return;
	}
while(temp->next!=NULL||temp->next!=head)
{
temp = temp->next;
}
temp->next = newNode;
newNode->prev = temp;
newNode->next = head;
}

void delete(int id)
{
struct Node* temp = head;
struct Node* temp1 = NULL;
while(temp->id!=id)
{
temp = temp->next;
}
if(temp==head)
{
	if(head->next == NULL)
	{
	 free(head);
	 return;
	}
	else
	{
	 temp = head->next;
     temp->prev = head->prev;
     free(head);
     head = temp;
	 return;		
	}
}

else
{
	temp1 = temp->prev;
  	temp1->next = temp->next;
	temp1 = temp->next;
	temp1->prev = temp->prev;
}
}

struct Node* search(__u64 id)
{
    struct Node* temp = head;
    while(temp->id!=id)
    {
        temp = temp->next;
    }
    return temp;
}




