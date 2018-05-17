// Copyright (c) 2018 The Zest Foundation developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODE_DEVBUDGET_H
#define MASTERNODE_DEVBUDGET_H

#include "main.h"
#include "masternode.h"
#include <base58.h>

class CDevBudget;

extern CDevBudget devbudget;

//
// Dev Budget
//
class CDevBudget
{
private:
    CScript payee;
    double nPercentage;

public:
    void PrepareBudget();
    bool IsTransactionValid(const CTransaction& txNew, int nBlockHeight);
    void FillBlockPayee(CMutableTransaction& txNew, int64_t nFees, bool fProofOfStake);
};

#endif // MASTERNODE_DEVBUDGET_H