#pragma once

/*
* I forgot why I used this to be honest
*/

__declspec(align(8))struct GlobalVars
{
	int gameModule;
	int plantRecharge;
	int plantHealth;
	int sunCounter;
	int zombieHeadgear;
	int zombieShields;
	int zombieHealth;
	int instantUseTimer;
	int noplantLocation;
	int economyCrasher;
};

// This is to help create the features of the hack
__declspec(align(8)) struct Offsets
{
	int dwRechargeOffset = 0x91E55;
	int dwWriteSunValueOffset = 0x94445;
	int dwPlantHealthOffset = 0x140680;
	int dwZombieHeadgearOffset = 0x1419FA;
	int dwZombieShieldOffset = 0x141651;
	int dwZombieHealthOffset = 0x141CDA;
	int dwInstantUseTimerOffset = 0x66E22;
	int dwPlantLocationOffset = 0x127EF;
	int dwAddCoinsOffset = 0x3375F;
};