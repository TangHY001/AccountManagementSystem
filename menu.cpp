#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#include "model.h"
#include "service.h"
#include "menu.h"
#include "global.h"
#include "tool.h"
#include "card_file.h"
#include "card_service.h"
#include "service.h"

int getSize(const char* pInfo);
void copy(const char aInput[], char aOutput[], int nSize);

void outputMenu() {
	printf("\n------------------菜单------------------");
	printf("\n1. 添加卡");
	printf("\n2. 查询卡");
	printf("\n3. 上机");
	printf("\n4. 下机");
	printf("\n5. 充值");
	printf("\n6. 退费");
	printf("\n7. 查询统计");
	printf("\n8. 注销");
	printf("\n0. 退出");
}




void add() {
	Card card;
	int nNameSize = 0;
	int nPwdSize = 0;

	char aName[32] = { '\0' };
	char aPwd[32] = {'\0'};
	printf("------------添加卡-----------\n");
	printf("请输入卡号：");
	scanf("%s", aName);
	printf("请输入密码：");
	scanf("%s", aPwd);

	//判断卡号和密码长度
	nNameSize = getSize(aName);
	nPwdSize = getSize(aPwd);

	if (nNameSize > 18 || nPwdSize > 8) {
		printf("卡号和密码超过规定长度");
		return;
	}

	strcpy(card.aPassword, aPwd);
	strcpy(card.aName, aName);
	printf("请输入开卡金额：");
	scanf("%f", &card.fBalance);
	card.fTotalUse = card.fBalance;
	card.nUseCount = 0;
	card.nStatus = 0;
	card.nDel = 0;

	card.tStart = card.tEnd = card.tLast = time(NULL);
	if (queryCardInfo(aName) != NULL) {
		printf("输入的卡号已经存在！请重新输入！\n");
		return;
	}
	

	

	printf("------------添加卡信息如下-----------\n");

	printf("卡号\t密码\t卡状态\t余额\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPassword, card.nStatus, card.fBalance
	);

	if (FALSE == addCardInfo(card)) {
		printf("添加卡信息文件失败！");
	}
	else {
		printf("添加卡信息文件成功！");
	}
		
}

void copy(const char aInput[], char aOutput[], int nSize) {
	int i;
	for (i = 0; i < nSize && aInput[i] != '\0'; i++) {
		aOutput[i] = aInput[i];
	}
	aOutput[i] = '\0';

}

int getSize(const char* pInfo ) 
{
	int nSize = 0;
	while (*(pInfo + nSize) != '\0') {
		nSize++;
	}

	return nSize;
}

void query() //查询卡
{
	printf("\n----------2.查询卡----------\n");

	char aName[18] = { 0 };
	char aTime1[20] = { '\0' };
	char aTime2[20] = { '\0' };
	int nIndex = 0;
	int i;
	Card* pCard = NULL;
	

	int icha;
	
	

	printf("请输入要查询的卡号（长度1~18)");
	scanf("%s", aName);

	printf("1.精确查询， 2.模糊查询（输入1或2）");
	scanf("%d", &icha);
	if (icha == 1) {
		pCard = queryCardInfo(aName);
		nIndex = 1;
	}
	else {
		pCard = queryCardsInfo(aName, &nIndex);
	}

	
	if (pCard == NULL || nIndex == 0) //处理未查询到时的情况 
	{
		printf("该卡信息不存在！\n");
		return;
	}

	printf("\n------------------------------------------查询信息如下------------------------------------------\n");
	printf("卡号\t卡状态\t余额\t累计使用金额\t使用次数\t上次使用时间\t\t卡到期时间\n");
	
	if (icha == 1) {
		timeToString(pCard->tLast, aTime1);
		timeToString(pCard->tEnd, aTime2);
		printf("%s\t%d\t%.1lf\t%.1lf\t\t%d\t\t%s\t%s\n", pCard->aName, pCard->nStatus, pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, aTime1, aTime2);

	}

	else {
		for (i = 0; i < nIndex; i++)
		{
			//转换时间输出格式
			timeToString(pCard[i].tLast, aTime1);
			timeToString(pCard[i].tEnd, aTime2);

			printf("%s\t%d\t%.1lf\t%.1lf\t\t%d\t\t%s\t%s\n", pCard[i].aName, pCard[i].nStatus, pCard[i].fBalance, pCard[i].fTotalUse, pCard[i].nUseCount, aTime1, aTime2);
		}
	}
	
	
	

}
void exitApp() {
	releaseList();

}
void logon() {

	char aName[30] = { '\0'};		//上机卡号
	char aPwd[20] = { '\0' };		//上机卡密码
	//Card* pCard = NULL;
	char aLastTime[30] = { '\0'};		//存放指定格式的字符串时间
	LogonInfo* pInfo = NULL;
	int nResult = 0;




	

	//提示并接受上机卡号
	printf("请输入上机卡号（长度为1-17）：");
	scanf("%s", aName);
	rewind(stdin);		//防止填入数据异常

	//判断输入的卡号是否符合要求
	if (strlen(aName) >= 18) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		
		return;
	}

	//提示并接受上机密码；
	printf("请输入上机密码（长度为1-7）:");
	scanf("%s", aPwd);
	rewind(stdin);		//防止填入数据异常
	system("cls");

	//判断输入的卡号密码是否符合要求
	if (strlen(aPwd) >= 8) {
		printf("输入卡号长度超过最大值！\n");
		system("pause");
		system("cls");
		return;
	}

	//进行上机
	//pCard = doLogon(aName, aPwd);
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));
	nResult = doLogon(aName, aPwd, pInfo);

	
	switch (nResult)
	{
		case 0:
			printf("上机失败！");
			break;
		case 1:
			printf("上机卡信息如下：\n");

			printf("卡号\t余额\t上机时间\n");

			timeToString(pInfo->tLogon, aLastTime);

			printf("%s\t%0.1f\t%s\n\n", pInfo->aCardName, pInfo->fBalance, aLastTime);
			printf("上机成功！");
			break;

		case 2:
			printf("该卡不能使用！\n");
			break;

		case 3:
			printf("余额不足!\n");
			break;



	
	}
	printf("--------------------------------");

	free(pInfo);
	
	
	
}
void settle() {

	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = -1;
	SettleInfo* pInfo = NULL;
	char aStartTime[30] = { 0 };
	char aEndTime[30] = { 0 };

	//为下机信息动态分配内存
	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	printf("请输入下机卡号");
	scanf("%s", aName);

	printf("请输入下机密码：");
	scanf("%s", aPwd);

	//进行下机
	nResult = doSettle(aName, aPwd, pInfo);

	//根据下机结果，提示不同信息
	printf("\n 下机信息如下：\n");

	switch (nResult)
	{
	case 0:
		printf("下机失败!\n");
		break;

	case 1:
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");


		timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);

		//输出下机卡时间
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, aStartTime, aEndTime);
		printf("下机成功！");

		break;
	case 2:
		printf("该卡未在使用\n");
		break;

	case 3:
		printf("卡余额不足！\n");

		break;
	default:
		break;
	}

	free(pInfo);
	
	
}
void annul() {
	Card card;

	printf("请输入注销卡号（长度1~18）：");
	scanf("%s", card.aName);

	printf("请输入密码（长度1~8）：");
	scanf("%s", card.aPassword);

	//提示注销的信息
	if (FALSE == annualCard(&card)) {
		printf("注销卡失败！\n");
		return;
	}
	else {
		printf("注销信息如下：\n");
		//输出表头
		printf("卡号\t退款金额\n");

	
		//输出注销卡信息
		printf("%s\t%0.1f\n", card.aName, card.fBalance);

		return;
	}
}
void addMoney() {
	char aName[18] = { 0 };
	char aPwd[18] = { 0 };
	float fAmount = 0;
	MoneyInfo sMoneyInfo;

	printf("请输入充值卡号（长度1~18）:");
	scanf("%s", aName);

	printf("请输入充值密码（长度1~8) :");
	scanf("%s", aPwd);

	printf("请输入充值金额（RMB): ");
	scanf("%f", &fAmount);

	//保存充值金额
	sMoneyInfo.fMoney = fAmount;

	//判断充值是否成功
	if (TRUE == doAddMoney(aName, aPwd, &sMoneyInfo)) {
		//提示充值信息
		printf("充值信息如下：\n");
		//输出表头
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%0.1f\t\t%0.1f\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
	}
	else {
		printf("充值失败！\n");
	}
}
void refundMoney() {
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = -1;
	MoneyInfo sMoneyInfo;

	printf("请输入退费卡号（长度1~10）：");
	scanf("%s", aName);

	printf("请输入退费密码（长度1~8）：");
	scanf("%s", aPwd);

	//进行退费

	nResult = doRefundMoney(aName, aPwd, &sMoneyInfo);

	//根据退费结果，提示不同信息
	switch (nResult)
	{
	case 0:
		printf("退费失败！\n");
		break;

	case 1:
		printf("退费信息如下：\n");

		//输出表头
		printf("卡号\t退费信息\t余额\n");
		//输出退费信息
		printf("%s\t%0.1f\t\t%0.1f\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
		break;

	case 2:
		printf("该卡正在使用或者已经注销！\n");
		break;

	case 3:
		printf("卡余额不足\n");
		break;
	default:
		break;
	}
}