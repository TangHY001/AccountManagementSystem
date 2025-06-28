#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "model.h"
#include "global.h"

int saveMoney(const Money* pMoney, const char* pPath) {
	FILE* fp = NULL;

	//以追加模式打开文件，如果打开失败，则以只写的模式打开
	if ((fp = fopen(pPath, "ab")) == NULL) {
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("写入充值退费信息打开文件失败！\n");
			return FALSE;
		}
	}
	//将充值退费信息保存到文件
	fwrite(pMoney, sizeof(Money), 1, fp);
	
	//关闭文件
	fclose(fp);
	printf("充值退费信息成功保存到文件！\n");
	return TRUE;
}