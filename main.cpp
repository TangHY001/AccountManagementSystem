#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "menu.h"



int main() {
	int nSelection = 0;
	printf("\n��ӭ����Ʒѹ���ϵͳ\n");
	printf("Created by THY001\n");
	printf("\n");
	do {
		outputMenu();
		printf("\n����������ѡ��");
		scanf("%d", &nSelection);
		switch (nSelection) {
		case 1:
			add();
			break;
			
		case 2:
			query();
			break;
			
		case 3:
			logon();
			break;
		case 4:
			settle();
			break;
		
		case 5:
			addMoney();
			break;
		case 6:
			refundMoney();
			break;

		case 8:
			annul();
			break;
		case 0:
			exitApp();
			break;
		default:
			printf("����Ĳ˵���Ŵ���\n");
			break;
		}
	} while (nSelection != 0);
	printf("\nллʹ�üƷѹ���ϵͳ��");

	return 0;
	
}