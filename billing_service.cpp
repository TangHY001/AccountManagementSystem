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

	//������Ϣд��ṹ��
	strcpy(pBilling->aCardName, pName);
	pBilling->tStart = time(NULL);
	pBilling->tEnd = 0;
	pBilling->fAmount = 0;
	pBilling->nStatus = 0;
	pBilling->nDel;

	//������Ϣ�ṹ��д���ļ�
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

	//��������
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

	//�������Ϊ�գ��ͷ�����
	if (billingList != NULL) {
		releaseBillingList();
	}

	//��ʼ������
	initBillingList();

	//��ȡ�Ʒ���Ϣ����
	nCount = getBillingCount(BILLINGPATH);
	//��̬�ڴ����
	pBilling = (Billing*)malloc(sizeof(Billing) * nCount);

	if (pBilling != NULL) {

		//��ȡ�Ʒ���Ϣ
		if (FALSE == readBilling(pBilling, BILLINGPATH)) {
			free(pBilling);
			return FALSE;
		}

		//����Ϣ���浽������

		for (i = 0, node = billingList; i < nCount; i++) {
			//Ϊ��ǰ�������ڴ�
			cur = (lpBillingNode)malloc(sizeof(BillingNode));
			//������ʧ�ܣ����ڷ���FALSE ǰ����Ҫ�ͷ�pBilling���ڴ�
			if (cur == NULL) {
				free(pBilling);
				return FALSE;
			}


			//��ʼ���µĿռ䣬 ȫ����ֵΪ0

			memset(cur, 0, sizeof(BillingNode));
			//��������ӵ�������
			cur->data = pBilling[i];
			cur->next = NULL;

			//�������ӵ�������
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

	//��������
	node = billingList->next;
	while (node != NULL) {
		//��ѯ��������ͬ������û�н���ļƷ���Ϣ
		if (strcmp(node->data.aCardName, pName) == 0 && node->data.nStatus == 0) {
			return &node->data;
		}

		node = node->next;
		nIndex++;
		*pIndex = nIndex;


	}
	return NULL;

}