#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "menu.h"



int main() {
	int nSelection = 0;
	printf("\n欢迎进入计费管理系统\n");
	printf("Created by THY001\n");
	printf("\n");
	do {
		outputMenu();
		printf("\n请输入您的选择：");
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
			printf("输入的菜单编号错误！\n");
			break;
		}
	} while (nSelection != 0);
	printf("\n谢谢使用计费管理系统！");

	return 0;
	
}