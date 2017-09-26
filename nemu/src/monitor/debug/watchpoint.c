#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 320

static WP wp_pool[NR_WP];
static WP *head, *free_;
WP* new_wp()
{
    WP *temp,*temp2,*result;
    if(free_==NULL)
    {
	    assert(0);
    }
    temp2 = NULL;
    temp = free_;
    while(temp->next!=NULL)
    {
	 temp2 = temp;
         temp = temp->next;
    }
    if(temp==free_)
    {
	  free_ = NULL;
    }
    else
    {
	    temp2->next = NULL;
    }
    result = temp;
    temp2 = NULL;
    temp = head;
    if(temp==NULL)
    {
	   head = result;
	   result->next = NULL;
	   result->empty = false;
	   result->NO = 1;
    }
    else
    {
	    while(temp->next!=NULL)
	    {
		    temp2 = temp;
		    temp = temp->next;
	    }
	    temp->next = result;
	    result->next = NULL;
	    result->empty = false;
	    result->NO = temp->NO + 1;
    }


    return result;
}
void print_wp()
{
	if(head==NULL)
	{
           printf("no watchpoint\n");
	}
	else
	{
           WP *temp;
	   temp = head;
	   while(temp!=NULL)
	   {
	      printf("%d : %s\n",temp->NO,temp->expr);
	      temp = temp->next;
	   }
	}
	return;
}
void free_wp(WP* wp)
{   
    if(wp->empty )
    {
	    assert(0);
    }
    WP *parent,*temp;
    parent = head;
    if(wp==head)
    {
	    head = head->next;
	    temp = head;
	    while(temp!=NULL)
	    {
		 
		    temp->NO = temp->NO -1;
	    }
    }
    else
    {
  
        while(parent->next!=wp)
       {
	 parent = parent->next;
       }
       parent->next = wp->next;
       temp = parent->next;
       while(temp!=NULL )
       {
	       temp->NO = temp->NO -1;
       }
    }
    temp = free_;
    if(temp==NULL)
    {
	    free_ = wp;
	    wp->next = NULL;
	    wp->empty = true;
    }
    else
    {
	    while(temp->next!=NULL)
	    {
		    temp = temp->next;
	    }
	    temp->next = wp;
	    wp->next = NULL;
	    wp->empty = true;
    }
    return;

}
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i].empty = true;
    wp_pool[i].expr = NULL;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


