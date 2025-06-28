#define _CRT_SECURE_NO_WARNINGS
#include "billing_file.h"
#include <stdio.h>
#include "model.h"
#include "global.h"
#include <stdlib.h>

//���Ʒ���Ϣ���浽billing.ams�ļ���
int saveBilling(const Billing* pBilling, const char* pPath) {

	FILE* fp = NULL;

	//�򿪶������ļ�
	fp = fopen(pPath, "ab");
	if (((fp = fopen(pPath, "ab")) == NULL) ){
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("��ӼƷ���Ϣ���ļ���ʧ�ܣ�");
			return FALSE;
		}
	}

	

	//������д���������ļ���
	fwrite(pBilling, sizeof(Billing), 1, fp);

	//�ر��ļ�
	fclose(fp);
	printf("�Ʒ���Ϣ�ɹ������ļ�");
	return TRUE;

}

int readBilling(Billing* pBilling, const char* pPath) {
	FILE* fp = NULL;
	int nIndex = 0;

	//��ֻ����ʽ�򿪶������ļ��������ʧ�ܣ����ȡʧ��
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("�ļ��޷��򿪣����ܶ�ȡ�Ʒ���Ϣ��");
		return FALSE;
	}

	//��ȡ�ļ�����
	while (!feof(fp)) {
		if (fread(&pBilling[nIndex], sizeof(Billing), 1, fp) != 0) {
			nIndex++;
		}
	}

	fclose(fp);

	return TRUE;
}

int getBillingCount(const char* pPath) {
	FILE* fp = NULL;
	int nCount = 0;
	Billing* pBilling = (Billing*)malloc(sizeof(Billing));

	//��ֻ����ʽ�򿪶������ļ��������ʧ�ܣ����ȡʧ��
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("�ļ��޷��򿪣�����ͳ�ƼƷ���Ϣ������");
		return 0;
	}

	//��ȡ�ļ���ÿ��һ������һ��
	while (!feof(fp)) {
		if (fread(pBilling, sizeof(Billing), 1, fp) != 0) {
			nCount++;
		}
	}

	fclose(fp);
	free(pBilling);
	return nCount;

}

int updateBilling(const Billing* pBilling, const char* pPath, int nIndex) {
	FILE* fp = NULL;
	int nCount = 0;
	long lPosition = 0;
	Billing pbBuf;

	//�Զ�дģʽ�򿪶������ļ�
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		printf("�ļ��޷��򿪣�\n");
		return FALSE;
	}

	//�����ļ�����ȡ������Ϣ���ļ��е�λ��
	while ((!feof(fp)) && (nCount < nIndex)) {
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0) {
			//��ȡ�ļ���ʶ��λ�ã����һ�����ҵ���λ��

			lPosition = ftell(fp);
		}

		nCount++;
	}

	//�ƶ��ļ����λ�ã���Ҫ���µļƷ���Ϣ��¼��ͷ
	fseek(fp, lPosition, 0);

	//���¼Ʒ���Ϣ���ļ�
	fwrite(pBilling, sizeof(Billing), 1, fp);
	printf("�Ʒ���Ϣ���µ��ļ��ɹ���");

	fclose(fp);


	return TRUE;
}