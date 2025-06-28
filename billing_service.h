#pragma once
#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H
#include "model.h"
#include <stdio.h>
#


int addBilling(const char* pName, Billing* billing);
void releaseBillingList();
void initBillingList();
int getBilling();
Billing* queryBilling(const char* pName, int* pIndex);
#endif // BILLING_SERVICE_H
