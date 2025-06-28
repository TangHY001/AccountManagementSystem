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









Card aCard[50]; //卡信息结构数组
int nCount = 0; //卡信息数目

lpCardNode cardList = NULL;  //卡信息链表
lpCardNode head = NULL;


int initCardList() //对链表进行初始化
{
	//IpCardNode head = NULL;
	head = (lpCardNode)malloc(sizeof(CardNode)); //为头结点分配内存

	if (head != NULL)
	{
		head->next = NULL;
		cardList = head;
		return TRUE;
	}
	return FALSE;
}

int addCard(Card card) //将新增的卡信息添加到管理所有卡的结构体数组中
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

		while (cardList->next) //遍历链表找到最后一个结点
			cardList = cardList->next;
		cardList->next = current;
		nCount++;
		return TRUE;
	}

	return FALSE;*/
	return saveCard(&card, CARDPATH);
}

void releaseCardList() //将分配的空间释放
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

			// 使用临时指针接收 realloc 的返回值
			Card* temp = (Card*)realloc(pCard, ((*pIndex) + 1) * sizeof(Card));
			if (temp == NULL)
			{
				// 内存分配失败，输出错误信息
				printf("Memory allocation failed in queryCards!\n");
				// 返回原指针，保留之前分配的内存
				return pCard;
			}
			// 内存分配成功，更新原指针
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
		printf("未录入任何卡信息！\n");
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

	printf("\n------------------------------------------查询信息如下------------------------------------------\n");
	printf("卡号\t卡状态\t余额\t累计使用金额\t使用次数\t上次使用时间\t\t卡到期时间\n");
	for (j = 0; j < n; j++)
	{
		//转换时间输出格式
		timeToString(pCard[j].tLast, aTime1);
		timeToString(pCard[j].tEnd, aTime2);

		printf("%s\t%d\t%.1lf\t%.1lf\t\t%d\t\t%s\t%s\n", pCard[j].aName, pCard[j].nStatus, pCard[j].fBalance, pCard[j].fTotalUse, pCard[j].nUseCount, aTime1, aTime2);
	}
}

//int updateCard(const Card* pCard, const char* pPath, int nIndex)
//{
//	FILE* fp = NULL;
//	int nLine = 0;      // 文件卡信息数
//	long lPosition = 0; // 文件位置标记
//	Card bBuf;
//	if ((fp = fopen(pPath, "rb+")) == NULL)
//		return FALSE;
//
//	while ((!feof(fp)) && (nLine < nIndex - 1))
//	{
//		if (fread(&bBuf, sizeof(Card), 1, fp) != 0)  // 获取文件标识位置
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

	printf("请输入要更新文件第几条记录：");
	scanf("%d", &nIndex);
	printf("请输入新卡号(长度为1~18)：");
	scanf("%s", card.aName);
	printf("请输入新密码(长度为1~8)：");
	scanf("%s", card.aPassword);
	printf("请输入开卡金额(RMB)：");
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
	char aName[18] = { '\0' };   // 存放读取出的卡号
	if ((fp = fopen(pPath, "rb")) == NULL)
		return FALSE;

	while (!feof(fp))  // 读取卡号，并比较卡号是否为当前查找的卡号
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
	printf("请输入要检索的卡号(长度为1~18)：");
	scanf("%s", temp);

	if (TRUE == isExsit(temp, CARDPATH))
		printf("卡信息文件中，搜索到有卡号为%s的卡信息。\n", temp);
	else
		printf("卡信息文件中，未搜索到有卡号为%s的卡信息。\n", temp);
}

int getCard() //将文件中的卡信息保存至链表
{
	int nCount = 0;
	int i = 0;
	lpCardNode node = NULL;
	lpCardNode current = NULL;
	Card* pCard = NULL;

	if (cardList)
		releaseCardList();

	initCardList();

	nCount = getCardCount(CARDPATH); //获取卡信息数量

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

		memset(current, 0, sizeof(CardNode)); //初始化新的空间
		current->data = pCard[i]; //将卡信息保存到结点中
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
		// 正确比较卡号和密码
		if (strcmp(cardNode->data.aName, pName) == 0 && strcmp(cardNode->data.aPassword, pPwd) == 0) {
			*pIndex = nIndex;
			return &cardNode->data;
		}
		cardNode = cardNode->next;
		nIndex++;
	}
	return NULL;
}