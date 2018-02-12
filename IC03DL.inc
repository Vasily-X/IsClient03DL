/*

*/

#if defined A_IC03DL_
    #endinput
#endif
#define A_IC03DL_

#if ! defined PAWNRAKNET_INC_
	#error You must have Pawn.Raknet include in order to use this include.
#endif

/*Variables*/
new IC03DL_IsClient03DL[MAX_PLAYERS];
	
/*Callbacks*/
public OnIncomingRPC(playerid, rpcid, BitStream:bs)
{
	if(rpcid == 0x19)
	{
		new iVersion,
			byteMod,
			byteNameLen,
			NickName[24],
			uiClientChallengeResponse,
			byteAuthBSLen,
			auth_bs[4*16],
			iClientVerLen,
			ClientVersion[30];

		BS_ReadValue(
			bs,
			PR_INT32, iVersion,
			PR_UINT8, byteMod,
			PR_UINT8, byteNameLen,
			PR_STRING, NickName, byteNameLen,
			PR_UINT32, uiClientChallengeResponse,
			PR_UINT8, byteAuthBSLen,
			PR_STRING, auth_bs, byteAuthBSLen,
			PR_UINT8, iClientVerLen,
			PR_STRING, ClientVersion, iClientVerLen
		);

		IC03DL_IsClient03DL[playerid] = false;
		
		if(strcmp(ClientVersion, "0.3.DL-R1") == 0)
		{
			IC03DL_IsClient03DL[playerid] = true;

			new uiChallenge = uiClientChallengeResponse ^ iVersion;
			iVersion = 4057;

			//fix and solve challenge
			uiClientChallengeResponse = uiChallenge ^ iVersion;
			

			iClientVerLen = 5;
			new ClientVersionP[6] = "0.3.7"; //

			BS_Reset(bs);

			BS_WriteValue(
				bs,
				PR_INT32, iVersion,
				PR_UINT8, byteMod,
				PR_UINT8, byteNameLen,
				PR_STRING, NickName,
				PR_UINT32, uiClientChallengeResponse,
				PR_UINT8, byteAuthBSLen,
				PR_STRING, auth_bs,
				PR_UINT8, iClientVerLen,
				PR_STRING, ClientVersionP
			);
		}

		BS_ResetReadPointer(bs);
		BS_ResetWritePointer(bs);
		
	}
	
	#if defined IC03DL_OnIncomingRPC
		return IC03DL_OnIncomingRPC(playerid, rpcid, BitStream:bs);
	#else
		return 1;
	#endif
}

/*Functions*/
stock IsClient03DL(playerid) return IC03DL_IsClient03DL[playerid];

/*Hooks*/
#if defined _ALS_OnIncomingRPC
#undef OnIncomingRPC
#else
#define _ALS_OnIncomingRPC
#endif
#define OnIncomingRPC IC03DL_OnIncomingRPC

#if defined IC03DL_OnIncomingRPC
forward IC03DL_OnIncomingRPC(playerid, rpcid, BitStream:bs);
#endif
