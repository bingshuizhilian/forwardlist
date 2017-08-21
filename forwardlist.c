/********************************************************************************

      **** Copyright (C), 2017, bingshuizhilian@yeah.net               ****

 ********************************************************************************
 * File Name     : forwardlist.c
 * Author        : bingshuizhilian@yeah.net
 * Date          : 2017-03-21
 * Description   : 前向链表容器的实现，含有丰富的操作支持
 * Version       : 1.0
 * Function List :
 * 
 * Record        :
 * Others        : 某些操作(e.g. pushback)对于单向链表来说时间复杂度为O(n)，
 				   请视数据容量考量某方法的使用与否
 * 1.Date        : 2017-03-21
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created file
*************************************************************************************************************/
#include "forwardlist.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

/*****************************************************************************
 * Function      : CreateList
 * Description   : 创建链表容器，需传入链表的大小，以及节点所挂数据的大小
 * Input         : unsigned int max_size         
                unsigned int carry_data_size  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
list_t* CreateList( unsigned int max_size, unsigned int carry_data_size )
{
	list_t* pList = NULL;
	
	pList = (list_t*)LIST_MALLOC(sizeof(list_t));
	
	if( NULL == pList )
	{
		return OPERATE_FAIL;
	}
	
	pList->Size = 0;
	pList->MaxSize = max_size;
	pList->CarryDataSize = carry_data_size;
	pList->pHead = NULL;

	pList->front = OperateFront;
	pList->back = OperateBack;
	pList->pushfront = OperatePushFront;
	pList->pushback = OperatePushBack;
	pList->popfront = OperatePopFront;
	pList->popback = OperatePopBack;
	pList->get = OperateGet;
	pList->findif = OperateFindIf;
	pList->findif2 = OperateFindIf2;
	pList->findindexif = OperateFindIndexIf;
	pList->remove = OperateRemove;
	pList->assign = OperateAssign;
	pList->insert = OperateInsert;
	pList->insertafter = OperateInsertAfter;
	pList->swap = OperateSwap;
	pList->sortif = OperateSortIf;
	pList->size = OperateSize;
	pList->capacity = OperateCapacity;
	pList->resize = OperateResize;
	pList->empty = OperateEmpty;
	pList->full = OperateFull;
	pList->clear = OperateClear;
	pList->destroy = OperateDestroy;
	pList->foreach = OperateForeach;

	return pList;
}

/*****************************************************************************
 * Function      : CopyItemToNode
 * Description   : 内部实现接口，更新元素index，容器内部在各种添加/删除/插入操
 				   作后始终通过UpdateAllItemIndex保证index是按从0开始有序的分配，
 				   如pushfront一个A元素后，get(0)就一定得到A
 * Input         : list_t* pList
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int UpdateAllItemIndex(list_t* pList)
{
	node_t* pScan = pList->pHead;
	unsigned int counter = 0;

	for( ; NULL != pScan; pScan = pScan->pNext )
	{
		pScan->item.Index = counter++;
	}

	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : PackageDataWithAutoIndex
 * Description   : 将用户数据组装，并自动分配index，分配原则见UpdateAllItemIndex
 * Input         : node_t* pNode        
                const void* pData    
                list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static item_t* PackageDataWithAutoIndex(node_t* pNode, const void* pData, list_t* pList)
{
	item_t* pItem = &pNode->item;

	pItem->Index = pList->Size;
	pItem->pData = (void*)(pNode + 1);
	memcpy(pItem->pData, pData, pList->CarryDataSize);

	return pItem;
}

/*****************************************************************************
 * Function      : CopyItemToNode
 * Description   : 内部实现接口，拷贝数据到节点
 * Input         : node_t* pNode        
                const item_t* pItem  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void CopyItemToNode(node_t* pNode, const item_t* pItem)
{
	memcpy( &pNode->item, pItem, sizeof( item_t ) );
}

/*****************************************************************************
 * Function      : OperateFront
 * Description   : 返回链表容器首元素
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void* OperateFront(list_t* pList)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return NULL;
	}
	
	return pScan->item.pData;
}

/*****************************************************************************
 * Function      : OperateBack
 * Description   : 返回链表容器尾元素
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void* OperateBack(list_t* pList)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return NULL;
	}

	while(NULL != pScan->pNext)
	{
		pScan = pScan->pNext;
	}

	return pScan->item.pData;
}

/*****************************************************************************
 * Function      : OperatePushFront
 * Description   : 添加一个元素到链表容器头部
 * Input         : list_t* pList      
                const void* pData  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperatePushFront(list_t* pList, const void* pData)
{
	node_t* pNewNode = (node_t*)LIST_MALLOC(sizeof(node_t) + pList->CarryDataSize);
	item_t* pItem = NULL;

	if( NULL == pNewNode )
	{
		return OPERATE_FAIL;
	}

	if( pList->full(pList) )
	{
		return OPERATE_FAIL;
	}

	pItem = PackageDataWithAutoIndex( pNewNode, pData, pList );
	CopyItemToNode(pNewNode, pItem);
	pNewNode->pNext = pList->pHead;
	pList->pHead = pNewNode;
	pList->Size++;
	UpdateAllItemIndex(pList);
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperatePushBack
 * Description   : 添加一个元素到链表容器尾部
 * Input         : list_t* pList      
                const void* pData  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperatePushBack(list_t* pList, const void* pData)
{
	node_t* pNewNode = (node_t*)LIST_MALLOC(sizeof(node_t) + pList->CarryDataSize);
	item_t* pItem = NULL;
	node_t* pScan = pList->pHead;

	if( NULL == pNewNode )
	{
		return OPERATE_FAIL;
	}

	if( pList->full(pList) )
	{
		return OPERATE_FAIL;
	}

	pItem = PackageDataWithAutoIndex( pNewNode, pData, pList );
	CopyItemToNode(pNewNode, pItem);
	pNewNode->pNext = NULL;
	
	if( NULL == pScan )
	{
		pList->pHead = pNewNode;
	}
	else
	{
		while(NULL != pScan->pNext)
		{
			pScan = pScan->pNext;
		}
		
		pScan->pNext = pNewNode;
	}
	
	pList->Size++;
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperatePopFront
 * Description   : 删除链表容器首元素
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperatePopFront(list_t* pList)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return OPERATE_FAIL;
	}

	pList->pHead = pScan->pNext;
	pList->Size--;
	LIST_FREE(pScan);
	UpdateAllItemIndex(pList);
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperatePopBack
 * Description   : 删除链表容器尾元素
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperatePopBack(list_t* pList)
{
	node_t* pScan = pList->pHead;
	node_t* pPrev = pScan;
	
	if(pList->empty(pList))
	{
		return OPERATE_FAIL;
	}

	if( NULL == pScan->pNext )
	{
		pList->pHead = pScan->pNext;
		pList->Size--;
		LIST_FREE(pScan);
		
		return OPERATE_SUCC;
	}

	while(NULL != pScan->pNext)
	{
		pPrev = pScan;
		pScan = pScan->pNext;
	}

	pPrev->pNext = NULL;
	pList->Size--;
	LIST_FREE(pScan);
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperateGet
 * Description   : 获取链表容器指定index处的元素，index从0开始
 * Input         : list_t* pList       
                unsigned int index  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void* OperateGet(list_t* pList, unsigned int index)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList) || pList->Size <= index)
	{
		return NULL;
	}

	do
	{
		if(index == pScan->item.Index)
		{		
			return pScan->item.pData;
		}
		
		pScan = pScan->pNext;
		
	}while(NULL != pScan);
	
	return NULL;
}

/*****************************************************************************
 * Function      : OperateFindIf
 * Description   : 按用户传入的函数指针的匹配条件查找并返回元素,未找到返回NULL
 * Input         : list_t* pList               
                unsigned int(*exec)(const void*)  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void* OperateFindIf(list_t* pList, unsigned int(*exec)(const void*))
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return NULL;
	}

	do
	{
		if(OPERATE_TRUE == exec( pScan->item.pData ))
		{		
			return pScan->item.pData;
		}
		
		pScan = pScan->pNext;
		
	}while(NULL != pScan);
	
	return NULL;
}

/*****************************************************************************
 * Function      : OperateFindIf2
 * Description   : 按用户传入的函数指针与待查找内容的匹配条件查找并返回元素,若
 				未找到返回NULL
 * Input         : list_t* pList
 				unsigned int(*exec)(const void*, const void*)
 				const void* pRef                 
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function

*****************************************************************************/
static void* OperateFindIf2(list_t* pList, unsigned int(*exec)(const void*, const void*), const void* pRef)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return NULL;
	}

	do
	{
		if(OPERATE_TRUE == exec( pScan->item.pData, pRef ))
		{		
			return pScan->item.pData;
		}
		
		pScan = pScan->pNext;
		
	}while(NULL != pScan);
	
	return NULL;
}

/*****************************************************************************
 * Function      : OperateFindIndexIf
 * Description   : 按用户传入的函数指针与待查找内容的匹配条件查找并返回所查找
 				元素的index, 若未找到返回OPERATE_INVALID
 * Input         : list_t* pList              
                unsigned int(*exec)(const void*, const void*)                     
                const void* pRef                 
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170323
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static unsigned int OperateFindIndexIf(list_t* pList, unsigned int(*exec)(const void*, const void*), const void* pRef)
{
	node_t* pScan = pList->pHead;
	
	if(pList->empty(pList))
	{
		return OPERATE_FAIL;
	}

	do
	{
		if(OPERATE_TRUE == exec( pScan->item.pData, pRef ))
		{		
			return pScan->item.Index;
		}
		
		pScan = pScan->pNext;
		
	}while(NULL != pScan);
	
	return OPERATE_INVALID;
}

/*****************************************************************************
 * Function      : OperateRemove
 * Description   : 删除链表容器指定index处的元素
 * Input         : list_t* pList       
                unsigned int index  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateRemove(list_t* pList, unsigned int index)
{
	node_t* pScan = pList->pHead;
	node_t* pPrev = NULL;
	
	if(pList->empty(pList) || pList->Size <= index)
	{
		return OPERATE_FAIL;
	}

	if(index == pScan->item.Index)
	{
		pList->pHead = pScan->pNext;
		pList->Size--;
		LIST_FREE(pScan);
		UpdateAllItemIndex(pList);
		
		return OPERATE_SUCC;
	}

	while(NULL != pScan->pNext)
	{
		pPrev = pScan;
		pScan = pScan->pNext;

		if(index == pScan->item.Index)
		{
			pPrev->pNext = pScan->pNext;
			pList->Size--;
			LIST_FREE(pScan);
			UpdateAllItemIndex(pList);

			return OPERATE_SUCC;
		}
	}
	
	return OPERATE_FAIL;
}

/*****************************************************************************
 * Function      : OperateAssign
 * Description   : 用新元素对链表容器指定index处的元素进行替换
 * Input         : list_t* pList       
                unsigned int index  
                const void* pData   
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateAssign(list_t* pList, unsigned int index, const void* pData)
{
	node_t* pScan = pList->pHead;

	if(pList->empty(pList) || pList->Size <= index)
	{
		return OPERATE_FAIL;
	}

	while( index != pScan->item.Index )
	{
		if(NULL == pScan->pNext)
		{
			return OPERATE_FAIL;
		}

		pScan = pScan->pNext;
	}

	memcpy( pScan->item.pData, pData, pList->CarryDataSize );
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperateInsert
 * Description   : 在链表容器指定index处的元素之前插入一个元素
 * Input         : list_t* pList       
                unsigned int index  
                const void* pData   
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateInsert(list_t* pList, unsigned int index, const void* pData)
{
	node_t* pNewNode = (node_t*)LIST_MALLOC(sizeof(node_t) + pList->CarryDataSize);
	item_t* pItem = NULL;
	node_t* pScan = pList->pHead;
	node_t* pPrev = NULL;

	if( NULL == pNewNode )
	{
		return OPERATE_FAIL;
	}

	if( pList->full(pList) || pList->empty(pList) || pList->Size <= index)
	{
		return OPERATE_FAIL;
	}

	pItem = PackageDataWithAutoIndex( pNewNode, pData, pList );
	CopyItemToNode(pNewNode, pItem);
	pNewNode->pNext = NULL;

	while( index != pScan->item.Index )
	{
		if(NULL == pScan->pNext)
		{
			return OPERATE_FAIL;
		}

		pPrev = pScan;
		pScan = pScan->pNext;
	}

	if( NULL == pPrev )
	{
		pList->pushfront(pList, pData);
	}
	else
	{
		pList->insertafter(pList, pPrev->item.Index, pData);
	}
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperateInsertAfter
 * Description   : 在链表容器指定index处的元素之后插入一个元素
 * Input         : list_t* pList       
                unsigned int index  
                const void* pData   
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateInsertAfter(list_t* pList, unsigned int index, const void* pData)
{
	node_t* pNewNode = (node_t*)LIST_MALLOC(sizeof(node_t) + pList->CarryDataSize);
	item_t* pItem = NULL;
	node_t* pScan = pList->pHead;

	if( NULL == pNewNode )
	{
		return OPERATE_FAIL;
	}

	if( pList->full(pList) || pList->empty(pList) || pList->Size <= index)
	{
		return OPERATE_FAIL;
	}

	pItem = PackageDataWithAutoIndex( pNewNode, pData, pList );
	CopyItemToNode(pNewNode, pItem);
	pNewNode->pNext = NULL;

	while( index != pScan->item.Index )
	{
		if(NULL == pScan->pNext)
		{
			return OPERATE_FAIL;
		}

		pScan = pScan->pNext;
	}
	
	pNewNode->pNext = pScan->pNext;
	pScan->pNext = pNewNode;
	
	pList->Size++;
	UpdateAllItemIndex(pList);
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperateSwap
 * Description   : 交换链表容器指定的两个index处的元素的内容，以交换节点携带的
 				   数据而非交换节点本身的方式实现
 * Input         : list_t* pList        
                unsigned int index1  
                unsigned int index2  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170322
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateSwap(list_t* pList, unsigned int index1, unsigned int index2)
{
	node_t* pScan1 = pList->pHead;
	node_t* pScan2 = pList->pHead;
	char* pTmpData = (char*)LIST_MALLOC(pList->CarryDataSize);

	if((index1 == index2) || (pList->Size <= index1) || (pList->Size <= index2) || (pList->Size <= 1))
	{
		return OPERATE_FAIL;
	}

	while( index1 != pScan1->item.Index )
	{
		if(NULL == pScan1->pNext)
		{
			return OPERATE_FAIL;
		}

		pScan1 = pScan1->pNext;
	}
	
	memcpy(pTmpData, pScan1->item.pData, pList->CarryDataSize);

	while( index2 != pScan2->item.Index )
	{
		if(NULL == pScan2->pNext)
		{
			return OPERATE_FAIL;
		}

		pScan2 = pScan2->pNext;
	}

	memcpy(pScan1->item.pData, pScan2->item.pData, pList->CarryDataSize);
	memcpy(pScan2->item.pData, pTmpData, pList->CarryDataSize);
	LIST_FREE(pTmpData);
	
	return OPERATE_SUCC;
}

/*****************************************************************************
 * Function      : OperateSortIf
 * Description   : 按用户传入的函数指针指定的比较方法来重新排列链表容器
 * Input         : list_t* pList              
                unsigned int(*exec)(const void*, const void*)                     
 * Output        : None
 * Return        : static
 * Others        : 节点携带数据容量较大时，本sort方法效率不高，因受制于swap方法
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void OperateSortIf(list_t* pList, unsigned int(*exec)(const void*, const void*))
{
	node_t* pScan1 = pList->pHead;
	node_t* pScan2 = NULL;

	if( 1 >= pList->size(pList) )
	{
		return;
	}

	for( ; NULL != pScan1->pNext; pScan1 = pScan1->pNext )
	{
		for( pScan2 = pScan1->pNext; NULL != pScan2; pScan2 = pScan2->pNext )
		{
			if( OPERATE_TRUE == exec( pScan1->item.pData, pScan2->item.pData ) )
			{
				pList->swap(pList, pScan1->item.Index, pScan2->item.Index);
			}
		}
	}
}

/*****************************************************************************
 * Function      : OperateSize
 * Description   : 链表容器当前含有的元素个数
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static unsigned int OperateSize(list_t* pList)
{
	return pList->Size;
}

/*****************************************************************************
 * Function      : OperateCapacity
 * Description   : 链表容器最多可以携带的元素个数
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static unsigned int OperateCapacity(list_t* pList)
{
	return pList->MaxSize;
}

/*****************************************************************************
 * Function      : OperateResize
 * Description   : 重新设置链表容器最多可以携带的元素个数
 * Input         : list_t* pList          
                unsigned int new_size  
 * Output        : None
 * Return        : static
 * Others        : 从链表容器后面截断，此操作可能会导致元素丢失
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void OperateResize(list_t* pList, unsigned int new_size)
{
	pList->MaxSize = new_size;

	while( pList->Size > pList->MaxSize )
	{
		pList->popback(pList);
	}
}

/*****************************************************************************
 * Function      : OperateEmpty
 * Description   : 链表容器当前是否为空
 * Input         : const list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateEmpty(list_t* pList)
{
	return ( ( 0 == pList->Size ) ? 1 : 0 );
}

/*****************************************************************************
 * Function      : OperateFull
 * Description   : 链表容器当前是否为满
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateFull(list_t* pList)
{
	return ( ( pList->size(pList) < pList->capacity(pList) ) ? 0 : 1 );
}

/*****************************************************************************
 * Function      : OperateClear
 * Description   : 清空链表容器数据，但不销毁链表
 * Input         : list_t* pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateClear(list_t* pList)
{
	node_t* pScan = pList->pHead;
	node_t* pErase = NULL;
	
	if(NULL == pList)
	{
		return OPERATE_SUCC;
	}

	while(NULL != pScan)
	{
		pErase = pScan;
		pScan = pScan->pNext;
		LIST_FREE(pErase);
	}

	pList->Size = 0;
	pList->pHead = NULL;
	
	return OPERATE_SUCC;	
}

/*****************************************************************************
 * Function      : OperateDestroy
 * Description   : 销毁链表容器
 * Input         : list_t** pList  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static int OperateDestroy(list_t** ppList)
{
	node_t* pScan = (*ppList)->pHead;
	node_t* pErase = NULL;
	
	if(NULL == *ppList)
	{
		return OPERATE_SUCC;
	}

	while(NULL != pScan)
	{
		pErase = pScan;
		pScan = pScan->pNext;
		LIST_FREE(pErase);
	}

	(*ppList)->Size = 0;
	(*ppList)->MaxSize = 0;
	(*ppList)->CarryDataSize = 0;
	(*ppList)->pHead = NULL;
	LIST_FREE(*ppList);
	*ppList = NULL;
	
	return OPERATE_SUCC;	
}

/*****************************************************************************
 * Function      : OperateForeach
 * Description   : 将链表容器的每个用户数据依次传递传递至用户传入的函数指针
 * Input         : list_t* pList       
                void(*exec)(void*)  
 * Output        : None
 * Return        : static
 * Others        : 
 * Record
 * 1.Date        : 20170321
 *   Author      : bingshuizhilian@yeah.net
 *   Modification: Created function
*****************************************************************************/
static void OperateForeach(list_t* pList, void(*exec)(void*))
{
	node_t* pScan = pList->pHead;

	for( ; NULL != pScan; pScan = pScan->pNext )
	{
		exec(pScan->item.pData);
	}
}
