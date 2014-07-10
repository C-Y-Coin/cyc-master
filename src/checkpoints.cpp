// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (      0, uint256("0x00000b1903bc3f863147577a730dfe7f1ed9e71f873995d74e758387185a669e"))    
        ( 1, uint256("0x0000084b66d8bf4807cd2e7b1bea73b33c176d1131d20f994038d1e34b674eb8"))
        ( 2, uint256("0x000003c835eb5d4af79dc72684ff1b39bea9f9915d7dbf8663cda8e4a49dc34e"))
        ( 500, uint256("0x0000005609bb8a59ceec8db10c95cf75605a83f7ef289b680812a3c50099eebb"))        
        ( 5000, uint256("0x00000003ea93fe6603c2a1563f051322663faab3f10324050dc99ea4901d2097"))
       	( 10000, uint256("0x0000000000e6f9bcca768c07c8da905a4e74e796241c505aa01cfb3b795c3e7a"))
       	( 12000, uint256("0x000000000ee8a0bded874105a0bfb77aa7d8f6d0f0834883ed1778169c3915ba"))
        ( 15700, uint256("0x000000001d9a33b92d1eed808517d6bba7a88e714f38fcd37eeb79d682667dc5"))        
        ( 18500, uint256("0x000000028b3bf0e637f2c46359b9acd34e94fa692fbf1199985b815d68ea7a6"))
       	( 24200, uint256("0x00000000160e400d1b6468b026d8d60becf42aed895071861b509168e987cb78"))       
       	( 28200, uint256("0x0000000003cfbd3440a095a5ac61a0a5f497f33dbc6a823e63d472c95348e958"))
        ( 31000, uint256("0x00000000029dd9d5a1b6ad5b8895ceb4151ffff5479b9391c96a460d33cc8191"))
        ( 32700, uint256("0x00000000017021f4c8975de50a4874c98e86268f9f2669a284cf656946071068"))        
        ( 34250, uint256("0x0000000000c4d71c5763e4c10c11fa7545297e03f669a39ed44a8de96f14fd2b"))
       	( 35800, uint256("0x000000000b31771e2a5cc11723d56c2079363587e4fb847a3e5c250459e1d13"))
       	( 37500, uint256("0x00000000002cd33387fc56110026bda3f84199d98027361d3dd8a71410a73c3c"))
       	
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1390701333, // * UNIX timestamp of last checkpoint block
        1000,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        8000.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
        (   0, uint256("0x"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1389241455,
        0,
        300
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
    	      
        if (fTestNet) return true; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        
    	    return true;
    	    if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
    	    
	return 0;    	    
        if (fTestNet) return 0; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
    	    
    	return NULL;    
        if (fTestNet) return NULL; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}

