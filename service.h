#pragma once
#ifndef SERVICE_H
#define SERVICE_H

int addCardInfo(Card card);
Card* queryCardInfo(const char* pName);
Card* queryCardsInfo(const char* pName, int* pIndex);
int doLogon(const char* pName, const char* pPwd, LogonInfo* pInfo);
void releaseList();
int doSettle(const char* pName, const char* pPwd, SettleInfo* pInfo);
double getAmount(time_t tStart);
int doAddMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);
int doRefundMoney(const char* pName, const char* pPwd, MoneyInfo* pMoneyInfo);
int annualCard(Card* pCard);
#endif// SERVICE_H
