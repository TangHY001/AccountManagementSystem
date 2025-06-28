#define _CRT_SECURE_NO_WARNINGS
#include "billing_file.h"
#include <stdio.h>
#include "model.h"
#include "global.h"
#include <stdlib.h>

//将计费信息保存到billing.ams文件中
int saveBilling(const Billing* pBilling, const char* pPath) {

	FILE* fp = NULL;

	//打开二进制文件
	fp = fopen(pPath, "ab");
	if (((fp = fopen(pPath, "ab")) == NULL) ){
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("添加计费信息打开文件夹失败！");
			return FALSE;
		}
	}

	

	//将数据写进二进制文件中
	fwrite(pBilling, sizeof(Billing), 1, fp);

	//关闭文件
	fclose(fp);
	printf("计费信息成功存入文件");
	return TRUE;

}

int readBilling(Billing* pBilling, const char* pPath) {
	FILE* fp = NULL;
	int nIndex = 0;

	//以只读方式打开二进制文件，如果打开失败，则读取失败
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("文件无法打开！不能读取计费信息！");
		return FALSE;
	}

	//读取文件内容
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

	//以只读方式打开二进制文件，如果打开失败，则读取失败
	if ((fp = fopen(pPath, "rb")) == NULL) {
		printf("文件无法打开！不能统计计费信息数量！");
		return 0;
	}

	//读取文件，每读一条计数一次
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

	//以读写模式打开二进制文件
	if ((fp = fopen(pPath, "rb+")) == NULL) {
		printf("文件无法打开！\n");
		return FALSE;
	}

	//遍历文件，获取消费信息在文件中的位置
	while ((!feof(fp)) && (nCount < nIndex)) {
		if (fread(&pbBuf, sizeof(Billing), 1, fp) != 0) {
			//获取文件标识的位置，最后一次是找到的位置

			lPosition = ftell(fp);
		}

		nCount++;
	}

	//移动文件标记位置，到要更新的计费信息记录开头
	fseek(fp, lPosition, 0);

	//更新计费信息到文件
	fwrite(pBilling, sizeof(Billing), 1, fp);
	printf("计费信息更新到文件成功！");

	fclose(fp);


	return TRUE;
}