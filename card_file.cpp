#define _CRT_SECURE_NO_WARNINGS
#include"card_file.h"
#include"menu.h"
#include"tool.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
//#define CARDCHARNUM 256


int saveCard(const Card* pCard, const char* pPath) //保存卡信息到文件中
{
	FILE* fp = NULL;
	char startTime[20] = { '\0' }; //开卡时间
	char endTime[20] = { '\0' }; //截止时间
	char lastTime[20] = { '\0' }; //上次使用时间

	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("添加卡信息打开文件失败！\n");
			return FALSE;
		}
			
	}
	//将卡信息保存到文件中
	fwrite(pCard, sizeof(Card), 1, fp);

	//关闭文件
	fclose(fp);
	printf("卡信息成功存入文件\n\n");

	return TRUE;


	////转换时间格式
	//timeToString(pCard->tStart, startTime);
	//timeToString(pCard->tEnd, endTime); ////
	//timeToString(pCard->tLast, lastTime);
	////fwrite(pCard, sizeof(Card), 1, fp);

	////将卡信息写入文件：卡号 密码 卡状态 开卡时间 截止时间 累计金额 最后使用时间 使用次数 当前余额 删除标识
	//fprintf(fp, "%s\t%s\t%d\t%s\t%s\t%.1lf\t%s\t%d\t%.1lf\t%d\n", pCard->aName, pCard->aPassword, pCard->nStatus,
	//	startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//fclose(fp);

	//return TRUE;
}

int readCard(Card* pCard, const char* pPath)
{
	FILE* fp;
	int nIndex = 0;

	//以只读方式打开二进制文件
	
	if ((fp = fopen(pPath, "rb")) == NULL) {
		if (printf("读取卡信息打开文件失败！\n"));
		return FALSE;

	}

	//从文件中逐行读取卡信息
	while (!feof(fp)) {
		if (fread(&pCard[nIndex], sizeof(Card), 1, fp) != 0) {
			nIndex++;
		}
	}

	//关闭文件
	fclose(fp);

	return TRUE;
	

	//FILE* fp = NULL;
	//char aBuf[256] = { '\0' };  ////////////////////////“*****”
	//int i = 0;
	//if ((fp = fopen(pPath, "r")) == NULL)
	//	return FALSE;

	////读取文件
	//while (!feof(fp))
	//{
	//	memset(aBuf, 0, 256); //将数组提前清空
	//	if (fgets(aBuf, 256, fp))
	//	{
	//		if (strlen(aBuf)) //判断是否成功读取数据
	//		{
	//			pCard[i] = praseCard(aBuf);
	//			i++;
	//		}
	//	}
	//}

	//return TRUE;
}

//Card praseCard(char* pBuf)
//{
//	Card card;
//	const char* delims = "\t";
//	char* buf = NULL;
//	char* str = NULL;
//	char flag[200][200] = { '\0' }; //保存分割后的字符串
//	int index = 0;
//
//	buf = pBuf;
//
//	str = strtok(pBuf, delims);
//	while (str)
//	{
//		strcpy(flag[index], str);
//		index++;
//		str = strtok(NULL, delims);
//	}
//
//	strncpy(card.aName, flag[0], sizeof(card.aName) - 1);
//	card.aName[sizeof(card.aName) - 1] = '\0';
//	strncpy(card.aPassword, flag[1], sizeof(card.aPassword) - 1);
//	card.aPassword[sizeof(card.aPassword) - 1] = '\0';
//	card.nStatus = atoi(flag[2]);
//	card.tStart = stringToTime(flag[3]);
//	card.tEnd = stringToTime(flag[4]);
//	card.fTotalUse = (double)atof(flag[5]);
//	card.tLast = stringToTime(flag[6]);
//	card.nUseCount = atoi(flag[7]);
//	card.fBalance = (double)atof(flag[8]);
//	card.nDel = atoi(flag[9]);
//
//	return card;
//}

int getCardCount(const char* pPath)
{

	FILE* fp = NULL;
	int nIndex = 0;
	Card* pCard = (Card*)malloc(sizeof(Card));

	//以只读模式打开文件
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("获取卡信息数量打开文件失败！\n");
		return 0;
	}
	
	//逐行读取文件内容， 获取的文件行数就是卡信息数
	while (!feof(fp)) {
		if (fread(pCard, sizeof(Card), 1, fp) != 0) {
			nIndex++;
		}
	}

	//关闭文件
	fclose(fp);
	free(pCard);
	return nIndex;



	//FILE* fp = NULL;
	//char aBuf[256] = { '\0' };   
	//int nCount = 0;
	//if ((fp = fopen(pPath, "r")) == NULL)
	//	return FALSE;

	////读取文件
	//while (!feof(fp))
	//{
	//	memset(aBuf, 0, 256); //将数组提前清空
	//	if (fgets(aBuf, 256, fp))
	//	{
	//		if (strlen(aBuf)) //判断是否成功读取数据
	//		{
	//			nCount++;
	//		}
	//	}
	//}

	//fclose(fp);
	//return nCount;
}

int updateCard(const Card* pCard, const char* pPath, int nIndex) {
	 
	FILE* fp = NULL;
	int nLine = 0;
	long lPosition = 0;
	Card bBuf;
	//以读写模式打开文件，如果失败， 返回FALSE
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		printf("更新卡信息打开文件失败！\n");
		return FALSE;
	}
	//遍历文件，获取卡在文件中的位置
	//注意nIndex是doLogon函数中获得的结点在链表（文件）中序号，从0开始编号
	while ((!feof(fp)) && (nLine < nIndex)) {

		//逐行读取文件内容
		if (fread(&bBuf, sizeof(Card), 1, fp) != 0) {
			//获取文件标识位置，循环的最后一次是找到的位置
			//在要更新的卡信息开始的位置
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//移动文件标识的位置
	fseek(fp, lPosition, 0);
	//将信息更新到文件
	fwrite(pCard, sizeof(Card), 1, fp);
	printf("卡信息添加到文件成功！\n\n");

	//关闭文件
	fclose(fp);
	return TRUE;

	//FILE* fp = NULL;
	//int nLine = 0;
	//char aBuf[CARDCHARNUM] = { 0 };
	//char aStart[30] = { 0 };
	//char aEnd[30] = { 0 };
	//char aLast[30] = { 0 };
	//
	//long lPosition = 0;
	//int isSaveScs;

	//timeToString(pCard->tStart, aStart);
	//timeToString(pCard->tEnd, aEnd);
	//timeToString(pCard->tLast, aLast);


	////以只读方式打开文件
	//if ((fp = fopen(pPath, "r+")) == NULL) {
	//	printf("更新卡信息文件打开失败");
	//	return FALSE;

	//}

	////遍历文件，找到该条记录

	//while (!feof(fp) && nLine < nIndex) {

	//	memset(aBuf, 0, CARDCHARNUM);

	//	if (fgets(aBuf, CARDCHARNUM, fp) != NULL) {
	//		lPosition = ftell(fp);
	//		nLine++;
	//	}
	//}
	////添加文件标识位置
	//fseek(fp, lPosition, 0);
	//
	////将数据写进文件
	//isSaveScs = fprintf(fp, "%s\t%s\t%d\t%s\t%s\t%.1lf\t%s\t%d\t%.1lf\t%d\n", pCard->aName, pCard->aPassword, pCard->nStatus,
	//	aStart, aEnd, pCard->fTotalUse, aLast, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//if (isSaveScs > 0) {
	//	printf("卡信息更新成功！");
	//}
	////关闭文件
	//fclose(fp);

	//return TRUE;

}