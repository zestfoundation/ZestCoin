// Copyright (c) 2018 The Zest Foundation developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "invalid.h"
#include "invalid_outpoints.json.h"

#include "util.h"

namespace invalid_out
{
    std::set<COutPoint> setInvalidOutPoints;

    UniValue read_json(const std::string& jsondata)
    {
        UniValue v;

        if (!v.read(jsondata) || !v.isArray())
            return UniValue(UniValue::VARR);

        return v.get_array();
    }

    bool LoadOutpoints()
    {
        LogPrintf("%s\n", LoadInvalidOutPoints());

        UniValue v = read_json(LoadInvalidOutPoints());

        if (v.empty()) {
            LogPrintf("LoadOutpoints(): EMPTY!!!!");
            return false;
        }

        for (unsigned int idx = 0; idx < v.size(); idx++) {
            const UniValue &val = v[idx];
            const UniValue &o = val.get_obj();

            const UniValue &vTxid = find_value(o, "txid");
            if (!vTxid.isStr())
                return false;

            uint256 txid = uint256(vTxid.get_str());
            if (txid == 0)
                return false;

            const UniValue &vN = find_value(o, "n");
            if (!vN.isNum())
                return false;

            auto n = static_cast<uint32_t>(vN.get_int());
            COutPoint out(txid, n);
            setInvalidOutPoints.insert(out);
            LogPrintf("LoadOutpoints(): %s\n", out.ToString());
        }
        return true;
    }

    bool ContainsOutPoint(const COutPoint& out)
    {
        return static_cast<bool>(setInvalidOutPoints.count(out));
    }
}

