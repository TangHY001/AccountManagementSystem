#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "model.h"
#include "global.h"

int saveMoney(const Money* pMoney, const char* pPath) {
	FILE* fp = NULL;

	//��׷��ģʽ���ļ��������ʧ�ܣ�����ֻд��ģʽ��
	if ((fp = fopen(pPath, "ab")) == NULL) {
		if ((fp = fopen(pPath, "wb")) == NULL) {
			printf("д���ֵ�˷���Ϣ���ļ�ʧ�ܣ�\n");
			return FALSE;
		}
	}
	//����ֵ�˷���Ϣ���浽�ļ�
	fwrite(pMoney, sizeof(Money), 1, fp);
	
	//�ر��ļ�
	fclose(fp);
	printf("��ֵ�˷���Ϣ�ɹ����浽�ļ���\n");
	return TRUE;
}