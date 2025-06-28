#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include <stdio.h>
#include "billing_service.h"
#include "billing_file.h"
#include "global.h"
#include <string.h>
#include <stdlib.h>

lpBillingNode billingList = NULL;

int addBilling(const char* pName, Billing* pBilling) {

	//消费信息写入结构体
	strcpy(pBilling->aCardName, pName);
	pBilling->tStart = time(NULL);
	pBilling->tEnd = 0;
	pBilling->fAmount = 0;
	pBilling->nStatus = 0;
	pBilling->nDel;

	//消费信息结构体写入文件
	if (FALSE == saveBilling(pBilling, BILLINGPATH)) {
		return FALSE;
	}
	return TRUE;
}

void initBillingList() {
	lpBillingNode head = NULL;
	if (billingList == NULL) {

		head = (lpBillingNode)malloc(sizeof(BillingNode));
		head->next = NULL;
		billingList = head;
	}
}

void releaseBillingList() {
	lpBillingNode cur = billingList;
	lpBillingNode next = NULL;

	//销毁链表
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;

	}
	billingList = NULL;
}

int getBilling() {
	int nCount = 0;
	Billing* pBilling = NULL;
	lpBillingNode node = NULL;
	int i = 0;
	lpBillingNode cur = NULL;

	//如果链表不为空，释放链表
	if (billingList != NULL) {
		releaseBillingList();
	}

	//初始化链表
	initBillingList();

	//获取计费信息数量
	nCount = getBillingCount(BILLINGPATH);
	//动态内存分配
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL) {

		//获取计费信息
		if (FALSE == readBilling(pBilling, BILLINGPATH)) {
			free(pBilling);
			return FALSE;
		}

		//将信息保存到链表中

		for (i = 0, node = billingList; i < nCount; i++) {
			//为当前结点分配内存
			cur = (lpBillingNode)malloc(sizeof(BillingNode));
			//若分配失败，则在返回FALSE 前，需要释放pBilling的内存
			if (cur == NULL) {
				free(pBilling);
				return FALSE;
			}


			//初始化新的空间， 全部赋值为0

			memset(cur, 0, sizeof(BillingNode));
			//将数据添加到链表中
			cur->data = pBilling[i];
			cur->next = NULL;

			//将结点添加到链表中
			node->next = cur;
			node = cur;
		}

		free(pBilling);
		return TRUE;
	}
	return FALSE;

}

Billing* queryBilling(const char* pName, int* pIndex) {
	lpBillingNode node = NULL;
	int nIndex = 0;

	if (FALSE == getBilling()) {
		return NULL;
	}

	//遍历链表
	node = billingList->next;
	while (node != NULL) {
		//查询到卡号相同，并且没有结算的计费信息
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0) {
			return &node->data;
		}

		node = node->next;
		nIndex++;
		*pIndex = nIndex;


	}
	return NULL;

}