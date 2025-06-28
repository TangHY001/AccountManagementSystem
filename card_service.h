#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
#include "model.h"
#include "card_file.h"

int addCard(Card card);
void releaseCardList();
Card* queryCard(const char* pName);
Card* queryCards(const char* pName, int* pIndex);
void Show();
void Update();
void check_exist();
int getCard();

Card* checkCard(const char* pName, const char* pPwd, int* pIndex);


#endif // CARD_SERVICE_H
	
