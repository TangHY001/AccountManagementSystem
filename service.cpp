#define _CRT_SECURE_NO_WARNINGS
#include "model.h"
#include "global.h"
#include "card_file.h"
#include "service.h"
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "card_service.h"
#include "billing_file.h"
#include "billing_service.h"
#include "money_file.h"
#include <time.h>

int addCardInfo(Card card) {
	if (TRUE == saveCard(&card, CARDPATH)) {
		return TRUE;
	}

	return FALSE;
}

//��ȷ��ѯ����Ϣ
Card* queryCardInfo(const char* pName) {
	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;
}

//ģ����ѯ����Ϣ
Card* queryCardsInfo(const char* pName, int* pIndex) {
	Card* pCard = NULL;
	pCard = queryCards(pName, pIndex);
	return pCard;
}

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo) {
	//��ȡ�ļ��п�����Ϣ
	Card* pCard = NULL;
	int nIndex = 0;

	Billing billing;

	//���ݿ��ź����룬�������л�ȡ����Ϣ�Ϳ���Ϣ�������е�����
	pCard = checkCard(pName, pPwd, &nIndex);


	if (pCard == NULL) {
		return FALSE;
	}

	//�����״̬��Ϊ0����ʾ�ÿ������ϻ�
	if (pCard->nStatus != 0) {
		return UNUSE;
	}

	//����������Ϊ0�� ��ʾ�����ϻ�
	if (pCard->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//��������ϻ������¿���Ϣ
	pCard->nStatus = 1;
	pCard->nUseCount++;
	pCard->tLast = time(NULL);

	//�����ļ��п���Ϣ

	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//������Ѽ�¼
	strcpy(billing.aCardName, pName);
	billing.tStart = time(NULL);
	billing.tEnd = 0;
	billing.nStatus = 0;
	billing.fAmount = 0;
	billing.nDel = 0;

	//�Ƚ��Ʒ���Ϣ���浽�ļ���
	if (TRUE == saveBilling(&billing, BILLINGPATH) ){
		//��װ�ϻ���Ϣ
		strcpy(pInfo -> aCardName, pName);
		pInfo -> fBalance = pCard -> fBalance;
		pInfo -> tLogon = billing.tStart;
		return TRUE;
	}
	//if (FALSE == getCard()) {
	//	return FALSE;
	//}

	//if (cardList != NULL) {
	//	node = cardList->next;
	//}

	//while (node != NULL) {

	//	if (strcmp(node->data.aName, pName) == 0 && (strcmp(node->data.aPassword, pPwd) == 0)) {
	//		if (node->data.nStatus != 0) {
	//			printf("�ÿ������ϻ���");
	//			return UNUSE;
	//		}

	//		if (node->data.fBalance <= 0) {
	//			printf("����");
	//			return ENOUGHMONEY;
	//		}



	//		node->data.nStatus = 1;
	//		node->data.nUseCount++;
	//		node->data.tLast = time(NULL);

	//		//�����ļ��еĿ���Ϣ
	//		if (FALSE == updateCard(&node->data, CARDPATH, nIndex)) {
	//			return FALSE;
	//		}

	//		//������Ѽ�¼���ļ�
	//		pBilling = (Billing*)malloc(sizeof(Billing));
	//		if (TRUE == addBilling(node->data.aName, pBilling)) {


	//			//�ɹ��ϻ����ϻ���Ϣ���浽�ϻ���Ϣ�ṹ��
	//			strcpy(pInfo->aCardName, node->data.aName);
	//			pInfo->tLogon = pBilling->tStart;
	//			pInfo->fBalance = node->data.fBalance;


	//			free(pBilling);

	//			//������Ϣ����ɹ������ʾ�ϻ��ɹ�
	//			return TRUE;
	//		}
	//		else {
	//			return FALSE;
	//		}
	//	}
	//	node = node->next;
	//	nIndex++;

	//}

	//if (node == NULL) {
	//	printf("�޸ÿ�����Ϣ������������");

	//}

	//return FALSE;




}

void releaseList() {
	releaseCardList();
	releaseBillingList();
}

int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo) {

	Card* pCard = NULL;
	Billing* pBilling = NULL;
	int nIndex = 0;
	int nPosition = 0;
	double dbAmount = 0.0;
	float fBalance = 0.0;

	//��ѯ�ϻ���
	pCard = checkCard(pName, pPwd, &nIndex);

	//���Ϊ�գ���ʾû�п���Ϣ�� ����FALSE

	if (pCard == NULL) {
		printf("�޸ÿ���Ϣ���������룡");
		return FALSE;
	}

	//�жϸÿ��Ƿ������ϻ��� ֻ�����ϻ��Ŀ����ܽ����»�����
	if (pCard->nStatus != 1) {
		printf("�ÿ�δ�ϻ�");
		return UNUSE;
	}

	//���ݿ��ţ���ѯ�Ʒ���Ϣ
	pBilling = queryBilling(pName, &nPosition);

	//�����ѯ�Ʒ���ϢΪ�գ� ��ʾ�»�ʧ��

	if (pBilling == NULL) {
		printf("�޸ÿ���Ϣ������������");
		return FALSE;
	}

	//�������ѽ��
	dbAmount = getAmount(pBilling->tStart);

	//������С�����ѽ� �����»�
	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance < 0) {
		return ENOUGHMONEY;
	}

	//�����ļ��еĿ���Ϣ
	pCard->fBalance = fBalance;
	pCard->nStatus = 0;
	pCard->tLast = time(NULL);

	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//���¼Ʒ���Ϣ
	pBilling->fAmount = (float)dbAmount;
	pBilling->nStatus = 1;
	pBilling->tEnd = time(NULL);

	//�����ļ��еļƷ���Ϣ
	if (FALSE == updateBilling(pBilling, BILLINGPATH, nPosition)) {
		return FALSE;
	}

	//��װ�»���Ϣ
	strcpy(pInfo->aCardName, pName);
	pInfo->fAmount = (float)dbAmount;
	pInfo->fBalance = fBalance;
	pInfo->tStart = pBilling->tStart;
	pInfo->tEnd = time(NULL);

	return TRUE;
}

double getAmount(time_t tStart) {
	double dbAmount = 0;
	int nCount = 0;
	int nSec = 0;
	int nMinutes = 0;
	time_t tEnd = time(NULL);

	//��������ʱ��
	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;

	//��������ʱ��ʱ�䵥Ԫ��
	if (nMinutes % UNIT == 0) {
		nCount = nMinutes / UNIT; // ����

	}
	else {
		nCount = nMinutes / UNIT + 1; //������
	}

	//�������ѽ��
	dbAmount = nCount * CHARGE;
	return dbAmount;
}

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo) {
	Card* pCard = NULL;
	int nIndex = 0;
	Money sMoney;


	//��ѯ��ֵ��
	pCard = checkCard(pName, pPwd, &nIndex);

	//�������ϢΪ�գ���ʾû�п���Ϣ����ֵʧ��
	if (pCard == NULL) {
		printf("�޸ÿ���Ϣ�����ܳ�ֵ��\n");
		return FALSE;
	}

	//�жϸÿ��Ƿ�δʹ�û������ϻ���
	//ֻ��δʹ�ú��������ϻ��Ŀ����ܽ��г�ֵ����
	if (pCard->nStatus != 0 && pCard->nStatus != 1) {
		return FALSE;
	}

	//������Գ�ֵ�����¿���Ϣ
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;

	//�����ļ��еĿ���Ϣ
	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//��װ��ֵ��Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 0;
	sMoney.fMoney = pMoneyInfo->fMoney;
	sMoney.nDel = 0;

	//����ֵ��¼���浽�ļ���
	if (TRUE == saveMoney(&sMoney, MONEYPATH)) {
		//��װ������ʾ��ֵ��Ϣ
		strcpy(pMoneyInfo->aCardName, sMoney.aCardName);
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}


	return FALSE;

}

int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo) {

	Card* pCard = NULL;
	int nIndex = 0;
	float fBalance = 0.0;
	Money sMoney;

	//��ѯ�˷ѿ�
	pCard = checkCard(pName, pPwd, &nIndex);

	//���Ϊ�գ���ʾû�иÿ���Ϣ������FALSE
	if (pCard == NULL) {
		printf("�޸ÿ���Ϣ�������˷ѣ�\n");
		return FALSE;

	}
	//�жϸÿ��Ƿ�δʹ�ã�ֻ��δʹ�õĿ����ܽ����˷Ѳ���
	if (pCard->nStatus != 0) {
		return UNUSE;
	}

	//���������0�������˷�
	fBalance = pCard->fBalance;
	if (fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//���¿���Ϣ
	pCard->fBalance = 0;
	pCard->fTotalUse -= fBalance;

	//���¿���Ϣ���ļ�
	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//����˷���Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 1;
	sMoney.fMoney = fBalance;
	sMoney.nDel = 0;

	//�����ļ��еĳ�ֵ�˷���Ϣ
	if (TRUE == saveMoney(&sMoney, MONEYPATH)) {
		//��װ�˷���Ϣ
		strcpy(pMoneyInfo->aCardName, sMoney.aCardName);
		pMoneyInfo->fMoney = sMoney.fMoney;
		pMoneyInfo->fBalance = pCard->fBalance;

		return TRUE;
	}

	return FALSE;
}

int annualCard(Card* pCard) {
	Card* pCurCard = NULL;
	int nIndex = -1;

	if (pCard == NULL) {
		return FALSE;
	}

	//���ݿ��ź����룬��ѯ����Ϣ
	pCurCard = checkCard(pCard->aName, pCard->aPassword, &nIndex);

	if (pCurCard == NULL) {
		return FALSE;
	}

	//ֻ��δ�ϻ��Ŀ�����ע��
	if (pCurCard->nStatus != 0) {
		return FALSE;
	}

	//����ע������Ϣ
	pCard->fBalance = pCurCard->fBalance;

	//����ע������Ϣ
	pCurCard->nStatus = 2;
	pCurCard->nDel = 1;
	pCurCard->fBalance = 0;
	pCurCard->tLast = time(NULL);



	//���¿����ļ��е���Ϣ
	if (FALSE == updateCard(pCurCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	return TRUE;
}