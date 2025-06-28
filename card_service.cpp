#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"model.h"
#include"global.h"
#include"card_file.h"
#include"tool.h"
#include <time.h>
#include "billing_service.h"









Card aCard[50]; //����Ϣ�ṹ����
int nCount = 0; //����Ϣ��Ŀ

lpCardNode cardList = NULL;  //����Ϣ����
lpCardNode head = NULL;


int initCardList() //��������г�ʼ��
{
	//IpCardNode head = NULL;
	head = (lpCardNode)malloc(sizeof(CardNode)); //Ϊͷ�������ڴ�

	if (head != NULL)
	{
		head->next = NULL;
		cardList = head;
		return TRUE;
	}
	return FALSE;
}

int addCard(Card card) //�������Ŀ���Ϣ��ӵ��������п��Ľṹ��������
{
	//aCard[nCount] = card;
	//nCount++;


	/*IpCardNode current = NULL;

	if (cardList == NULL)
		initCardList();
	current = (IpCardNode)malloc(sizeof(CardNode));

	if (current != NULL)
	{
		current->data = card;
		current->next = NULL;

		while (cardList->next) //���������ҵ����һ�����
			cardList = cardList->next;
		cardList->next = current;
		nCount++;
		return TRUE;
	}

	return FALSE;*/
	return saveCard(&card, CARDPATH);
}

void releaseCardList() //������Ŀռ��ͷ�
{
	lpCardNode current;
	if (cardList != NULL)
	{
		while (cardList->next)
		{
			//current = cardList->next;
			//free(current); ///
			//current = NULL;
			current = cardList;
			cardList = cardList->next;
			free(current);
		}
	}
}

Card* queryCards(const char* pName, int* pIndex)
{
	lpCardNode current = NULL;

	if (!getCard())
		return NULL;

	Card* pCard = (Card*)malloc(sizeof(Card));
	if (pCard == NULL)
		return NULL;

	current = cardList->next;

	while (current != NULL)
	{
		if (strstr(current->data.aName, pName))
		{
			pCard[*pIndex] = current->data;
			(*pIndex)++;

			// ʹ����ʱָ����� realloc �ķ���ֵ
			Card* temp = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
			if (temp == NULL)
			{
				// �ڴ����ʧ�ܣ����������Ϣ
				printf("Memory allocation failed in queryCards!\n");
				// ����ԭָ�룬����֮ǰ������ڴ�
				return pCard;
			}
			// �ڴ����ɹ�������ԭָ��
			pCard = temp;
		}
		current = current->next;
	}

	return pCard;
}

Card* queryCard(const char* pName) {
	if (getCard() == FALSE) {
		return NULL;
	}
	lpCardNode cur = NULL;
	if (cardList != NULL) {
		cur = cardList->next;
		while (cur != NULL) {
			if (strcmp(cur->data.aName, pName) == 0) {
				return &cur->data;
			}
			cur = cur->next;
		}
	}
	return NULL;
}

void Show()
{
	int n = getCardCount(CARDPATH);
	int i = 0, j;
	char aTime1[20] = { '\0' };
	char aTime2[20] = { '\0' };
	lpCardNode current = NULL;
	Card* pCard = (Card*)malloc(sizeof(Card) * (n + 1));

	if (n == 0)
	{
		printf("δ¼���κο���Ϣ��\n");
		return;
	}

	if (pCard == NULL)
		return;


	current = cardList->next;

	while (current)
	{
		pCard[i] = current->data;
		i++;

		current = current->next;
	}

	printf("\n------------------------------------------��ѯ��Ϣ����------------------------------------------\n");
	printf("����\t��״̬\t���\t�ۼ�ʹ�ý��\tʹ�ô���\t�ϴ�ʹ��ʱ��\t\t������ʱ��\n");
	for (j = 0; j < n; j++)
	{
		//ת��ʱ�������ʽ
		timeToString(pCard[j].tLast, aTime1);
		timeToString(pCard[j].tEnd, aTime2);

		printf("%s\t%d\t%.1lf\t%.1lf\t\t%d\t\t%s\t%s\n", pCard[j].aName, pCard[j].nStatus, pCard[j].fBalance, pCard[j].fTotalUse, pCard[j].nUseCount, aTime1, aTime2);
	}
}

//int updateCard(const Card* pCard, const char* pPath, int nIndex)
//{
//	FILE* fp = NULL;
//	int nLine = 0;      // �ļ�����Ϣ��
//	long lPosition = 0; // �ļ�λ�ñ��
//	Card bBuf;
//	if ((fp = fopen(pPath, "rb+")) == NULL)
//		return FALSE;
//
//	while ((!feof(fp)) && (nLine < nIndex - 1))
//	{
//		if (fread(&bBuf, sizeof(Card), 1, fp) != 0)  // ��ȡ�ļ���ʶλ��
//		{
//			lPosition = ftell(fp);
//			nLine++;
//		}
//	}
//	fseek(fp, lPosition, 0);
//	fwrite(pCard, sizeof(Card), 1, fp);
//	fclose(fp);
//	return TRUE;
//}

void Update()
{
	struct tm* endTime;
	struct tm* startTime;
	int nIndex;
	Card card;

	printf("������Ҫ�����ļ��ڼ�����¼��");
	scanf("%d", &nIndex);
	printf("�������¿���(����Ϊ1~18)��");
	scanf("%s", card.aName);
	printf("������������(����Ϊ1~8)��");
	scanf("%s", card.aPassword);
	printf("�����뿪�����(RMB)��");
	scanf("%lf", &card.fBalance);

	card.fTotalUse = card.fBalance;
	card.nDel = 0;
	card.nStatus = 0;
	card.nUseCount = 0;
	card.tStart = card.tEnd = card.tLast = time(NULL);

	startTime = localtime(&card.tStart);
	endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);
	updateCard(&card, CARDPATH, nIndex);
}

int isExsit(const char* pNum, const char* pPath)
{
	FILE* fp;
	char aName[18] = { '\0' };   // ��Ŷ�ȡ���Ŀ���
	if ((fp = fopen(pPath, "rb")) == NULL)
		return FALSE;

	while (!feof(fp))  // ��ȡ���ţ����ȽϿ����Ƿ�Ϊ��ǰ���ҵĿ���
	{
		if (fread(aName, sizeof(aName), 1, fp) != 0)
		{
			if (strcmp(aName, pNum) == 0)
			{
				fclose(fp);
				return TRUE;
			}
			else
				fseek(fp, sizeof(Card) - sizeof(aName), SEEK_CUR);
		}
	}
	fclose(fp);
	return FALSE;
}

void check_exist()
{
	char temp[20];
	printf("������Ҫ�����Ŀ���(����Ϊ1~18)��");
	scanf("%s", temp);

	if (TRUE == isExsit(temp, CARDPATH))
		printf("����Ϣ�ļ��У��������п���Ϊ%s�Ŀ���Ϣ��\n", temp);
	else
		printf("����Ϣ�ļ��У�δ�������п���Ϊ%s�Ŀ���Ϣ��\n", temp);
}

int getCard() //���ļ��еĿ���Ϣ����������
{
	int nCount = 0;
	int i = 0;
	lpCardNode node = NULL;
	lpCardNode current = NULL;
	Card* pCard = NULL;

	if (cardList)
		releaseCardList();

	initCardList();

	nCount = getCardCount(CARDPATH); //��ȡ����Ϣ����

	pCard = (Card*)malloc(sizeof(Card) * nCount);
	if (pCard == NULL)
		return FALSE;

	if (!readCard(pCard, CARDPATH))
	{
		free(pCard);
		pCard = NULL;
		return FALSE;
	}

	for (i = 0, node = cardList; i < nCount; i++)
	{
		current = (lpCardNode)malloc(sizeof(CardNode));

		if (!current)
		{
			free(pCard);
			return FALSE;
		}

		memset(current, 0, sizeof(CardNode)); //��ʼ���µĿռ�
		current->data = pCard[i]; //������Ϣ���浽�����
		current->next = NULL;

		node->next = current;
		node = current;
	}

	free(pCard);
	pCard = NULL;

	return TRUE;
}

Card* checkCard(const char* pName, const char* pPwd, int* pIndex) {
	lpCardNode cardNode = NULL;
	int nIndex = 0;

	if (FALSE == getCard()) {
		return NULL;
	}
	cardNode = cardList->next;
	while (cardNode != NULL) {
		// ��ȷ�ȽϿ��ź�����
		if (strcmp(cardNode->data.aName, pName) == 0 && strcmp(cardNode->data.aPassword, pPwd) == 0) {
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return NULL;
}