// Copyright (c) 2018 The Zest Foundation developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <masternode-devbudget.h>

CDevBudget devbudget;

void CDevBudget::PrepareBudget() {
    CBitcoinAddress address;

    if (Params().NetworkID() == CBaseChainParams::MAIN)
        address = CBitcoinAddress("ZHyGD8z6vPbdPJrrz6AoYdE3LdZJCqwiTb");
    else
        address = CBitcoinAddress("zAmjFKyYP6pSuncbYX9XprpVp6AQrzgK83");

    // Parse ZEST address
    payee = GetScriptForDestination(address.Get());
    nPercentage = 0.05;

    LogPrintf("CDevBudget::PrepareBudget(): %s\n", address.ToString());
    LogPrintf("CDevBudget::PrepareBudget(): %f\n", nPercentage);

}

bool CDevBudget::IsTransactionValid(const CTransaction& txNew, int nBlockHeight)
{
    PrepareBudget();

    bool found = false;
    int i = 0;
    BOOST_FOREACH (CTxOut out, txNew.vout) {
        if (i > 0 && payee == out.scriptPubKey) {
            CAmount budgetPayment = txNew.vout[i - 1].nValue * nPercentage;
            if (out.nValue >= budgetPayment) {
                found = true;
                LogPrint("masternode", "CDevBudget::IsTransactionValid - Found valid Dev Budget Payment of %d\n", out.nValue);
            }
        }
        i++;
    }

    if (!found) {
        LogPrint("masternode", "CDevBudget::IsTransactionValid - Missing required payment\n");
    }

    return found;
}

void CDevBudget::FillBlockPayee(CMutableTransaction& txNew, int64_t nFees, bool fProofOfStake)
{
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (!pindexPrev) return;

    PrepareBudget();

    unsigned int i = txNew.vout.size();
    if (txNew.vout[i - 1].nValue > 0) {
        CAmount budgetPayment = txNew.vout[i - 1].nValue * nPercentage;

        txNew.vout.resize(i + 1);
        txNew.vout[i].scriptPubKey = payee;
        txNew.vout[i].nValue = budgetPayment;

        //subtract budget payment from mn reward
        txNew.vout[i - 1].nValue -= budgetPayment;
    }
}