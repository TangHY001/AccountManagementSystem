

#ifndef CARD_FILE_H
#define CARD_FILE_H

#include "card_service.h"

int saveCard(const Card* pCard, const char* pPath);
int readCard(Card* pCard, const char* pPath);
//Card praseCard(char* pBuf);
int getCardCount(const char* pPath);
void showCard(Card card);
int updateCard(const Card* pCard, const char* pPath, int nIndex);

#endif// CARD_FILE_H


