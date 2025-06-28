

#ifndef BILLING_FILE_H
#define BILLING_FILE_H
#include <stdio.h>


#include"model.h"
//º¯ÊýÉùÃ÷
int saveBilling(const Billing* pBilling, const char* pPath);
int readBilling(Billing* pBilling, const char* pPath);
int getBillingCount(const char* pPath);
int updateBilling(const Billing* pBilling, const char* pPath, int nIndex);




#endif // BILLING_FILE_H