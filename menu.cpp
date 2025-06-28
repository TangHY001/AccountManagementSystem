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
	printf("\n------------------�˵�------------------");
	printf("\n1. ��ӿ�");
	printf("\n2. ��ѯ��");
	printf("\n3. �ϻ�");
	printf("\n4. �»�");
	printf("\n5. ��ֵ");
	printf("\n6. �˷�");
	printf("\n7. ��ѯͳ��");
	printf("\n8. ע��");
	printf("\n0. �˳�");
}




void add() {
	Card card;
	int nNameSize = 0;
	int nPwdSize = 0;

	char aName[32] = { '\0' };
	char aPwd[32] = {'\0'};
	printf("------------��ӿ�-----------\n");
	printf("�����뿨�ţ�");
	scanf("%s", aName);
	printf("���������룺");
	scanf("%s", aPwd);

	//�жϿ��ź����볤��
	nNameSize = getSize(aName);
	nPwdSize = getSize(aPwd);

	if (nNameSize > 18 || nPwdSize > 8) {
		printf("���ź����볬���涨����");
		return;
	}

	strcpy(card.aPassword, aPwd);
	strcpy(card.aName, aName);
	printf("�����뿪����");
	scanf("%f", &card.fBalance);
	card.fTotalUse = card.fBalance;
	card.nUseCount = 0;
	card.nStatus = 0;
	card.nDel = 0;

	card.tStart = card.tEnd = card.tLast = time(NULL);
	if (queryCardInfo(aName) != NULL) {
		printf("����Ŀ����Ѿ����ڣ����������룡\n");
		return;
	}
	

	

	printf("------------��ӿ���Ϣ����-----------\n");

	printf("����\t����\t��״̬\t���\n");
	printf("%s\t%s\t%d\t%0.1f\n", card.aName, card.aPassword, card.nStatus, card.fBalance
	);

	if (FALSE == addCardInfo(card)) {
		printf("��ӿ���Ϣ�ļ�ʧ�ܣ�");
	}
	else {
		printf("��ӿ���Ϣ�ļ��ɹ���");
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

void query() //��ѯ��
{
	printf("\n----------2.��ѯ��----------\n");

	char aName[18] = { 0 };
	char aTime1[20] = { '\0' };
	char aTime2[20] = { '\0' };
	int nIndex = 0;
	int i;
	Card* pCard = NULL;
	

	int icha;
	
	

	printf("������Ҫ��ѯ�Ŀ��ţ�����1~18)");
	scanf("%s", aName);

	printf("1.��ȷ��ѯ�� 2.ģ����ѯ������1��2��");
	scanf("%d", &icha);
	if (icha == 1) {
		pCard = queryCardInfo(aName);
		nIndex = 1;
	}
	else {
		pCard = queryCardsInfo(aName, &nIndex);
	}

	
	if (pCard == NULL || nIndex == 0) //����δ��ѯ��ʱ����� 
	{
		printf("�ÿ���Ϣ�����ڣ�\n");
		return;
	}

	printf("\n------------------------------------------��ѯ��Ϣ����------------------------------------------\n");
	printf("����\t��״̬\t���\t�ۼ�ʹ�ý��\tʹ�ô���\t�ϴ�ʹ��ʱ��\t\t������ʱ��\n");
	
	if (icha == 1) {
		timeToString(pCard->tLast, aTime1);
		timeToString(pCard->tEnd, aTime2);
		printf("%s\t%d\t%.1lf\t%.1lf\t\t%d\t\t%s\t%s\n", pCard->aName, pCard->nStatus, pCard->fBalance, pCard->fTotalUse, pCard->nUseCount, aTime1, aTime2);

	}

	else {
		for (i = 0; i < nIndex; i++)
		{
			//ת��ʱ�������ʽ
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

	char aName[30] = { '\0'};		//�ϻ�����
	char aPwd[20] = { '\0' };		//�ϻ�������
	//Card* pCard = NULL;
	char aLastTime[30] = { '\0'};		//���ָ����ʽ���ַ���ʱ��
	LogonInfo* pInfo = NULL;
	int nResult = 0;




	

	//��ʾ�������ϻ�����
	printf("�������ϻ����ţ�����Ϊ1-17����");
	scanf("%s", aName);
	rewind(stdin);		//��ֹ���������쳣

	//�ж�����Ŀ����Ƿ����Ҫ��
	if (strlen(aName) >= 18) {
		printf("���뿨�ų��ȳ������ֵ��\n");
		system("pause");
		
		return;
	}

	//��ʾ�������ϻ����룻
	printf("�������ϻ����루����Ϊ1-7��:");
	scanf("%s", aPwd);
	rewind(stdin);		//��ֹ���������쳣
	system("cls");

	//�ж�����Ŀ��������Ƿ����Ҫ��
	if (strlen(aPwd) >= 8) {
		printf("���뿨�ų��ȳ������ֵ��\n");
		system("pause");
		system("cls");
		return;
	}

	//�����ϻ�
	//pCard = doLogon(aName, aPwd);
	pInfo = (LogonInfo*)malloc(sizeof(LogonInfo));
	nResult = doLogon(aName, aPwd, pInfo);

	
	switch (nResult)
	{
		case 0:
			printf("�ϻ�ʧ�ܣ�");
			break;
		case 1:
			printf("�ϻ�����Ϣ���£�\n");

			printf("����\t���\t�ϻ�ʱ��\n");

			timeToString(pInfo->tLogon, aLastTime);

			printf("%s\t%0.1f\t%s\n\n", pInfo->aCardName, pInfo->fBalance, aLastTime);
			printf("�ϻ��ɹ���");
			break;

		case 2:
			printf("�ÿ�����ʹ�ã�\n");
			break;

		case 3:
			printf("����!\n");
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

	//Ϊ�»���Ϣ��̬�����ڴ�
	pInfo = (SettleInfo*)malloc(sizeof(SettleInfo));

	printf("�������»�����");
	scanf("%s", aName);

	printf("�������»����룺");
	scanf("%s", aPwd);

	//�����»�
	nResult = doSettle(aName, aPwd, pInfo);

	//�����»��������ʾ��ͬ��Ϣ
	printf("\n �»���Ϣ���£�\n");

	switch (nResult)
	{
	case 0:
		printf("�»�ʧ��!\n");
		break;

	case 1:
		printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");


		timeToString(pInfo->tStart, aStartTime);
		timeToString(pInfo->tEnd, aEndTime);

		//����»���ʱ��
		printf("%s\t%0.1f\t%0.1f\t%s\t%s\n\n", pInfo->aCardName, pInfo->fAmount, pInfo->fBalance, aStartTime, aEndTime);
		printf("�»��ɹ���");

		break;
	case 2:
		printf("�ÿ�δ��ʹ��\n");
		break;

	case 3:
		printf("�����㣡\n");

		break;
	default:
		break;
	}

	free(pInfo);
	
	
}
void annul() {
	Card card;

	printf("������ע�����ţ�����1~18����");
	scanf("%s", card.aName);

	printf("���������루����1~8����");
	scanf("%s", card.aPassword);

	//��ʾע������Ϣ
	if (FALSE == annualCard(&card)) {
		printf("ע����ʧ�ܣ�\n");
		return;
	}
	else {
		printf("ע����Ϣ���£�\n");
		//�����ͷ
		printf("����\t�˿���\n");

	
		//���ע������Ϣ
		printf("%s\t%0.1f\n", card.aName, card.fBalance);

		return;
	}
}
void addMoney() {
	char aName[18] = { 0 };
	char aPwd[18] = { 0 };
	float fAmount = 0;
	MoneyInfo sMoneyInfo;

	printf("�������ֵ���ţ�����1~18��:");
	scanf("%s", aName);

	printf("�������ֵ���루����1~8) :");
	scanf("%s", aPwd);

	printf("�������ֵ��RMB): ");
	scanf("%f", &fAmount);

	//�����ֵ���
	sMoneyInfo.fMoney = fAmount;

	//�жϳ�ֵ�Ƿ�ɹ�
	if (TRUE == doAddMoney(aName, aPwd, &sMoneyInfo)) {
		//��ʾ��ֵ��Ϣ
		printf("��ֵ��Ϣ���£�\n");
		//�����ͷ
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%0.1f\t\t%0.1f\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
	}
	else {
		printf("��ֵʧ�ܣ�\n");
	}
}
void refundMoney() {
	char aName[18] = { 0 };
	char aPwd[8] = { 0 };
	int nResult = -1;
	MoneyInfo sMoneyInfo;

	printf("�������˷ѿ��ţ�����1~10����");
	scanf("%s", aName);

	printf("�������˷����루����1~8����");
	scanf("%s", aPwd);

	//�����˷�

	nResult = doRefundMoney(aName, aPwd, &sMoneyInfo);

	//�����˷ѽ������ʾ��ͬ��Ϣ
	switch (nResult)
	{
	case 0:
		printf("�˷�ʧ�ܣ�\n");
		break;

	case 1:
		printf("�˷���Ϣ���£�\n");

		//�����ͷ
		printf("����\t�˷���Ϣ\t���\n");
		//����˷���Ϣ
		printf("%s\t%0.1f\t\t%0.1f\n", sMoneyInfo.aCardName, sMoneyInfo.fMoney, sMoneyInfo.fBalance);
		break;

	case 2:
		printf("�ÿ�����ʹ�û����Ѿ�ע����\n");
		break;

	case 3:
		printf("������\n");
		break;
	default:
		break;
	}
}