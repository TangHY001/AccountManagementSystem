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


int saveCard(const Card* pCard, const char* pPath) //���濨��Ϣ���ļ���
{
	FILE* fp = NULL;
	char startTime[20] = { '\0' }; //����ʱ��
	char endTime[20] = { '\0' }; //��ֹʱ��
	char lastTime[20] = { '\0' }; //�ϴ�ʹ��ʱ��

	if ((fp = fopen(pPath, "ab")) == NULL)
	{
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("��ӿ���Ϣ���ļ�ʧ�ܣ�\n");
			return FALSE;
		}
			
	}
	//������Ϣ���浽�ļ���
	fwrite(pCard, sizeof(Card), 1, fp);

	//�ر��ļ�
	fclose(fp);
	printf("����Ϣ�ɹ������ļ�\n\n");

	return TRUE;


	////ת��ʱ���ʽ
	//timeToString(pCard->tStart, startTime);
	//timeToString(pCard->tEnd, endTime); ////
	//timeToString(pCard->tLast, lastTime);
	////fwrite(pCard, sizeof(Card), 1, fp);

	////������Ϣд���ļ������� ���� ��״̬ ����ʱ�� ��ֹʱ�� �ۼƽ�� ���ʹ��ʱ�� ʹ�ô��� ��ǰ��� ɾ����ʶ
	//fprintf(fp, "%s\t%s\t%d\t%s\t%s\t%.1lf\t%s\t%d\t%.1lf\t%d\n", pCard->aName, pCard->aPassword, pCard->nStatus,
	//	startTime, endTime, pCard->fTotalUse, lastTime, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//fclose(fp);

	//return TRUE;
}

int readCard(Card* pCard, const char* pPath)
{
	FILE* fp;
	int nIndex = 0;

	//��ֻ����ʽ�򿪶������ļ�
	
	if ((fp = fopen(pPath, "rb")) == NULL) {
		if (printf("��ȡ����Ϣ���ļ�ʧ�ܣ�\n"));
		return FALSE;

	}

	//���ļ������ж�ȡ����Ϣ
	while (!feof(fp)) {
		if (fread(&pCard[nIndex], sizeof(Card), 1, fp) != 0) {
			nIndex++;
		}
	}

	//�ر��ļ�
	fclose(fp);

	return TRUE;
	

	//FILE* fp = NULL;
	//char aBuf[256] = { '\0' };  ////////////////////////��*****��
	//int i = 0;
	//if ((fp = fopen(pPath, "r")) == NULL)
	//	return FALSE;

	////��ȡ�ļ�
	//while (!feof(fp))
	//{
	//	memset(aBuf, 0, 256); //��������ǰ���
	//	if (fgets(aBuf, 256, fp))
	//	{
	//		if (strlen(aBuf)) //�ж��Ƿ�ɹ���ȡ����
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
//	char flag[200][200] = { '\0' }; //����ָ����ַ���
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

	//��ֻ��ģʽ���ļ�
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("��ȡ����Ϣ�������ļ�ʧ�ܣ�\n");
		return 0;
	}
	
	//���ж�ȡ�ļ����ݣ� ��ȡ���ļ��������ǿ���Ϣ��
	while (!feof(fp)) {
		if (fread(pCard, sizeof(Card), 1, fp) != 0) {
			nIndex++;
		}
	}

	//�ر��ļ�
	fclose(fp);
	free(pCard);
	return nIndex;



	//FILE* fp = NULL;
	//char aBuf[256] = { '\0' };   
	//int nCount = 0;
	//if ((fp = fopen(pPath, "r")) == NULL)
	//	return FALSE;

	////��ȡ�ļ�
	//while (!feof(fp))
	//{
	//	memset(aBuf, 0, 256); //��������ǰ���
	//	if (fgets(aBuf, 256, fp))
	//	{
	//		if (strlen(aBuf)) //�ж��Ƿ�ɹ���ȡ����
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
	//�Զ�дģʽ���ļ������ʧ�ܣ� ����FALSE
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		printf("���¿���Ϣ���ļ�ʧ�ܣ�\n");
		return FALSE;
	}
	//�����ļ�����ȡ�����ļ��е�λ��
	//ע��nIndex��doLogon�����л�õĽ���������ļ�������ţ���0��ʼ���
	while ((!feof(fp)) && (nLine < nIndex)) {

		//���ж�ȡ�ļ�����
		if (fread(&bBuf, sizeof(Card), 1, fp) != 0) {
			//��ȡ�ļ���ʶλ�ã�ѭ�������һ�����ҵ���λ��
			//��Ҫ���µĿ���Ϣ��ʼ��λ��
			lPosition = ftell(fp);
			nLine++;
		}
	}

	//�ƶ��ļ���ʶ��λ��
	fseek(fp, lPosition, 0);
	//����Ϣ���µ��ļ�
	fwrite(pCard, sizeof(Card), 1, fp);
	printf("����Ϣ��ӵ��ļ��ɹ���\n\n");

	//�ر��ļ�
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


	////��ֻ����ʽ���ļ�
	//if ((fp = fopen(pPath, "r+")) == NULL) {
	//	printf("���¿���Ϣ�ļ���ʧ��");
	//	return FALSE;

	//}

	////�����ļ����ҵ�������¼

	//while (!feof(fp) && nLine < nIndex) {

	//	memset(aBuf, 0, CARDCHARNUM);

	//	if (fgets(aBuf, CARDCHARNUM, fp) != NULL) {
	//		lPosition = ftell(fp);
	//		nLine++;
	//	}
	//}
	////����ļ���ʶλ��
	//fseek(fp, lPosition, 0);
	//
	////������д���ļ�
	//isSaveScs = fprintf(fp, "%s\t%s\t%d\t%s\t%s\t%.1lf\t%s\t%d\t%.1lf\t%d\n", pCard->aName, pCard->aPassword, pCard->nStatus,
	//	aStart, aEnd, pCard->fTotalUse, aLast, pCard->nUseCount, pCard->fBalance, pCard->nDel);

	//if (isSaveScs > 0) {
	//	printf("����Ϣ���³ɹ���");
	//}
	////�ر��ļ�
	//fclose(fp);

	//return TRUE;

}