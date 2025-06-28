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

//精确查询卡信息
Card* queryCardInfo(const char* pName) {
	Card* pCard = NULL;
	pCard = queryCard(pName);
	return pCard;
}

//模糊查询卡信息
Card* queryCardsInfo(const char* pName, int* pIndex) {
	Card* pCard = NULL;
	pCard = queryCards(pName, pIndex);
	return pCard;
}

int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo) {
	//获取文件中卡的信息
	Card* pCard = NULL;
	int nIndex = 0;

	Billing billing;

	//根据卡号和密码，从链表中获取卡信息和卡信息在链表中的索引
	pCard = checkCard(pName, pPwd, &nIndex);


	if (pCard == NULL) {
		return FALSE;
	}

	//如果卡状态不为0，表示该卡不能上机
	if (pCard->nStatus != 0) {
		return UNUSE;
	}

	//如果卡的余额为0， 表示不能上机
	if (pCard->fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//如果可以上机，更新卡信息
	pCard->nStatus = 1;
	pCard->nUseCount++;
	pCard->tLast = time(NULL);

	//更新文件中卡信息

	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//添加消费记录
	strcpy(billing.aCardName, pName);
	billing.tStart = time(NULL);
	billing.tEnd = 0;
	billing.nStatus = 0;
	billing.fAmount = 0;
	billing.nDel = 0;

	//先将计费信息保存到文件中
	if (TRUE == saveBilling(&billing, BILLINGPATH) ){
		//组装上机信息
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
	//			printf("该卡正在上机！");
	//			return UNUSE;
	//		}

	//		if (node->data.fBalance <= 0) {
	//			printf("余额不足");
	//			return ENOUGHMONEY;
	//		}



	//		node->data.nStatus = 1;
	//		node->data.nUseCount++;
	//		node->data.tLast = time(NULL);

	//		//更新文件中的卡信息
	//		if (FALSE == updateCard(&node->data, CARDPATH, nIndex)) {
	//			return FALSE;
	//		}

	//		//添加消费记录到文件
	//		pBilling = (Billing*)malloc(sizeof(Billing));
	//		if (TRUE == addBilling(node->data.aName, pBilling)) {


	//			//成功上机，上机信息保存到上机信息结构体
	//			strcpy(pInfo->aCardName, node->data.aName);
	//			pInfo->tLogon = pBilling->tStart;
	//			pInfo->fBalance = node->data.fBalance;


	//			free(pBilling);

	//			//消费信息保存成功后，则表示上机成功
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
	//	printf("无该卡的信息！请重新输入");

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

	//查询上机卡
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果为空，表示没有卡信息， 返回FALSE

	if (pCard == NULL) {
		printf("无该卡信息，重新输入！");
		return FALSE;
	}

	//判断该卡是否正在上机， 只有在上机的卡才能进行下机操作
	if (pCard->nStatus != 1) {
		printf("该卡未上机");
		return UNUSE;
	}

	//根据卡号，查询计费信息
	pBilling = queryBilling(pName, &nPosition);

	//如果查询计费信息为空， 表示下机失败

	if (pBilling == NULL) {
		printf("无该卡信息，请重新输入");
		return FALSE;
	}

	//计算消费金额
	dbAmount = getAmount(pBilling->tStart);

	//如果余额小于消费金额， 则不能下机
	fBalance = pCard->fBalance - (float)dbAmount;
	if (fBalance < 0) {
		return ENOUGHMONEY;
	}

	//更新文件中的卡信息
	pCard->fBalance = fBalance;
	pCard->nStatus = 0;
	pCard->tLast = time(NULL);

	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//更新计费信息
	pBilling->fAmount = (float)dbAmount;
	pBilling->nStatus = 1;
	pBilling->tEnd = time(NULL);

	//更新文件中的计费信息
	if (FALSE == updateBilling(pBilling, BILLINGPATH, nPosition)) {
		return FALSE;
	}

	//组装下机信息
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

	//计算消费时长
	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;

	//计算消费时长时间单元数
	if (nMinutes % UNIT == 0) {
		nCount = nMinutes / UNIT; // 整除

	}
	else {
		nCount = nMinutes / UNIT + 1; //不整除
	}

	//计算消费金额
	dbAmount = nCount * CHARGE;
	return dbAmount;
}

int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo) {
	Card* pCard = NULL;
	int nIndex = 0;
	Money sMoney;


	//查询充值卡
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果卡信息为空，表示没有卡信息，充值失败
	if (pCard == NULL) {
		printf("无该卡信息，不能充值！\n");
		return FALSE;
	}

	//判断该卡是否未使用或正在上机，
	//只有未使用和在正在上机的卡才能进行充值操作
	if (pCard->nStatus != 0 && pCard->nStatus != 1) {
		return FALSE;
	}

	//如果可以充值，更新卡信息
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;

	//更新文件中的卡信息
	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//组装充值信息
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 0;
	sMoney.fMoney = pMoneyInfo->fMoney;
	sMoney.nDel = 0;

	//将充值记录保存到文件中
	if (TRUE == saveMoney(&sMoney, MONEYPATH)) {
		//组装界面显示充值信息
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

	//查询退费卡
	pCard = checkCard(pName, pPwd, &nIndex);

	//如果为空，表示没有该卡信息，返回FALSE
	if (pCard == NULL) {
		printf("无该卡信息，不能退费！\n");
		return FALSE;

	}
	//判断该卡是否未使用，只有未使用的卡才能进行退费操作
	if (pCard->nStatus != 0) {
		return UNUSE;
	}

	//如果余额等于0，则不能退费
	fBalance = pCard->fBalance;
	if (fBalance <= 0) {
		return ENOUGHMONEY;
	}

	//更新卡信息
	pCard->fBalance = 0;
	pCard->fTotalUse -= fBalance;

	//更新卡信息的文件
	if (FALSE == updateCard(pCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	//组合退费信息
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 1;
	sMoney.fMoney = fBalance;
	sMoney.nDel = 0;

	//更新文件中的充值退费信息
	if (TRUE == saveMoney(&sMoney, MONEYPATH)) {
		//组装退费信息
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

	//根据卡号和密码，查询卡信息
	pCurCard = checkCard(pCard->aName, pCard->aPassword, &nIndex);

	if (pCurCard == NULL) {
		return FALSE;
	}

	//只有未上机的卡才能注销
	if (pCurCard->nStatus != 0) {
		return FALSE;
	}

	//保存注销卡信息
	pCard->fBalance = pCurCard->fBalance;

	//更新注销卡信息
	pCurCard->nStatus = 2;
	pCurCard->nDel = 1;
	pCurCard->fBalance = 0;
	pCurCard->tLast = time(NULL);



	//更新卡在文件中的信息
	if (FALSE == updateCard(pCurCard, CARDPATH, nIndex)) {
		return FALSE;
	}

	return TRUE;
}