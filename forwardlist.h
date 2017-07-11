#ifndef __FORWARDLIST_H__
#define __FORWARDLIST_H__

#if WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#define LIST_MALLOC malloc
#define LIST_FREE free
#else //else of WIN32
#include "limits.h"
//insert your essential includes
#endif //end of WIN32

#define OPERATE_SUCC 1
#define OPERATE_FAIL 0

#define OPERATE_TRUE 1
#define OPERATE_FALSE 0

#define OPERATE_INVALID UINT_MAX

typedef struct data
{
	unsigned int Index;
	void* pData;
}item_t;

typedef struct node
{
	item_t item;
	struct node* pNext;
}node_t;

typedef struct list
{
//private:
	unsigned int Size;
	unsigned int MaxSize;
	unsigned int CarryDataSize;
	node_t* pHead;
//public:
	void* (*front)(struct list*);
	void* (*back)(struct list*);
	int (*pushfront)(struct list*, const void*);
	int (*pushback)(struct list*, const void*);
	int (*popfront)(struct list*);
	int (*popback)(struct list*);
	void* (*get)(struct list*, unsigned int);
	void* (*findif)(struct list*, unsigned int(*)(const void*));
	void* (*findif2)(struct list*, unsigned int(*)(const void*, const void*), const void*);
	unsigned int (*findindexif)(struct list*, unsigned int(*)(const void*, const void*), const void*);
	int (*remove)(struct list*, unsigned int);
	int (*assign)(struct list*, unsigned int, const void*);
	int (*insert)(struct list*, unsigned int, const void*);
	int (*insertafter)(struct list*, unsigned int, const void*);
	int (*swap)(struct list*, unsigned int, unsigned int);
	void (*sortif)(struct list*, unsigned int(*)(const void*, const void*));
	unsigned int (*size)(struct list*);
	unsigned int (*capacity)(struct list*);
	void (*resize)(struct list*, unsigned int);
	int (*empty)(struct list*);
	int (*full)(struct list*);
	int (*clear)(struct list*);
	int (*destroy)(struct list**);
	void (*foreach)(struct list*, void(*)(void*));
}list_t;

list_t* CreateList( unsigned int max_size, unsigned int carry_data_size );
static int UpdateAllItemIndex(list_t* pList);
static item_t* PackageDataWithAutoIndex(node_t* pNode, const void* pData, list_t* pList);
static void CopyItemToNode(node_t* pNode, const item_t* pItem);
static void* OperateFront(list_t* pList);
static void* OperateBack(list_t* pList);
static int OperatePushFront(list_t* pList, const void* pData);
static int OperatePushBack(list_t* pList, const void* pData);
static int OperatePopFront(list_t* pList);
static int OperatePopBack(list_t* pList);
static void* OperateGet(list_t* pList, unsigned int index);
static void* OperateFindIf(list_t* pList, unsigned int(*exec)(const void*));
static void* OperateFindIf2(list_t* pList, unsigned int(*exec)(const void*, const void*), const void* pRef);
static unsigned int OperateFindIndexIf(list_t* pList, unsigned int(*exec)(const void*, const void*), const void* pRef);
static int OperateRemove(list_t* pList, unsigned int index);
static int OperateAssign(list_t* pList, unsigned int index, const void* pData);
static int OperateInsert(list_t* pList, unsigned int index, const void* pData);
static int OperateInsertAfter(list_t* pList, unsigned int index, const void* pData);
static int OperateSwap(list_t* pList, unsigned int index1, unsigned int index2);
static void OperateSortIf(list_t* pList, unsigned int(*exec)(const void*, const void*));
static unsigned int OperateSize(list_t* pList);
static unsigned int OperateCapacity(list_t* pList);
static void OperateResize(list_t* pList, unsigned int new_size);
static int OperateEmpty(list_t* pList);
static int OperateFull(list_t* pList);
static int OperateClear(list_t* pList);
static int OperateDestroy(list_t** ppList);
static void OperateForeach(list_t* pList, void(*exec)(void*));

#endif //end of __FORWARDLIST_H__
