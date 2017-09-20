//////////////////////////////////////////////////////////////////////
//                             North Carolina State University
//
//
//
//                             Copyright 2016
//
////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////
//
//   Author:  Hung-Wei Tseng
//
//   Description:
//     Skeleton of NPHeap Pseudo Device
//
////////////////////////////////////////////////////////////////////////

#include "npheap.h"

#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>
#include <linux/mutex.h>

extern struct miscdevice npheap_dev;

typedef struct node node;

typedef struct mutex p_lock;

// Structure is ready
struct node {
    __u64 objectId;
    __u64 size;
    void* k_virtual_addr;
    //pthread_mutex_t lock;
    p_lock *lock;
    node *next;
}*k_head_list = NULL;



node createObject(__u64 offset)
{
    printk("Starting createObject function."); 
    struct node *newNode = kmalloc(sizeof(struct node));

    newNode->objectId = offset;
    newNode->size = 0;
    newNode->lock = PTHREAD_MUTEX_INITIALIZER;
    newNode->k_virtual_addr = NULL;
    return newNode;
}



node getObject(__u64 inputOffset)
{
    printk("Starting getObject function.");    
    struct node* temp = k_head_list;
    while(temp->next!=k_head_list)
    {
        if(temp->offset==inputOffset)
            return temp;
    }
    return NULL;    
}

p_lock getMutex(__u64 inputOffset)
{
    printk("Starting getMutex function.");
    struct node* temp = k_head_list;
    while(temp->next!=k_head_list)
    {
        if(temp->offset==inputOffset)
            return temp->lock;
    }
    printk("Object does not exist");
    return NULL;    
}

__u64 getSize(__u64 inputOffset)
{
    printk("Starting getSize function.");
    struct node* temp = k_head_list;
    while(temp->next!=k_head_list)
    {
        if(temp->offset==inputOffset)
            return temp->size;
    }
    printk("Object does not exist");
    return -1;    
}


void resetAddress(__u64 inputOffset)
{
    struct node* temp = k_head_list;
    while(temp->next!=k_head_list)
    {
        if(temp->offset==inputOffset)
            {
                temp->k_virtual_addr=NULL;
                temp->size=0;
            }
    }  
}

// Memory Allocation is ready
int npheap_mmap(struct file *filp, struct vm_area_struct *vma)
{
    __u64 offset = vma->vm_pgoff;
    struct node *object;

    *object = getObject(offset);
    if(object == NULL){
        *object = createObject(offset);
    }

    if(object->size == 0){
        __u64 size = vma->vm_end - vma->vm_start;
        object->k_virtual_addr = kmalloc(size, GFP_KERNEL);
        object->size = size;
        //__virt_to_phys
        if(remap_pfn_range(vma, vma->vm_start, __pa(object->k_virtual_addr)>>PAGE_SHIFT, size, vma->vm_page_prot) < 0)
            return -EAGAIN;
    }else{
        if(remap_pfn_range(vma, vma->vm_start, __pa(object->k_virtual_addr)>>PAGE_SHIFT, object->size, vma->vm_page_prot) < 0)
            return -EAGAIN;
    }
    return 0;
}

int npheap_init(void)
{
    int ret;
    struct mutex *linked_list_lock;
    if ((ret = misc_register(&npheap_dev)))
        printk(KERN_ERR "Unable to register \"npheap\" misc device\n");
    else
        printk(KERN_ERR "\"npheap\" misc device installed\n");
    return ret;
}

void npheap_exit(void)
{
    misc_deregister(&npheap_dev);
}