#pragma once
#include <Windows.h>
#include "util.hpp"

#define PI 3.14159265358979


enum gameModes
{
	TEAMDEATHMATCH = 0,
	COOPEDIT,
	DEATHMATCH,
	SURVIVOR,
	TEAMSURVIVOR,
	CTF,
	PISTOLFRENZY,
	BOTTEAMDEATHMATCH,
	BOTDEATHMATCH,
	LASTSWISSSTANDING,
	ONESHOTONEKILL,
	TEAMONESHOTONEKILL,
	BOTONESHOTONEKILL,
	HUNTTHEFLAG,
	TEAMKEEPTHEFLAG,
	KEEPTHEFLAG,
	TEAMPF,
	TEAMLSS,
	BOTPISTOLFRENZY,
	BOtlSS,
	BOTTEAMSURVIVOR,
	BOTTEAMONESHOTONEKILL,
	NUM
};

class ent;

class weapon
{
public:
	char pad_0x0000[0x4]; //0x0000
	__int32 ID; //0x0004
	ent* owner; //0x0008
	uintptr_t* guninfo; //0x000C
	int* ammo2; //0x0010
	int* ammo; //0x0014
	int* gunWait;
	int shots;
	int breload;
};

class ent
{
public:
	DWORD vTable; //0x0
	Vector3 head; //0x0004
	char _0x0010[36];
	Vector3 pos; //0x0034
	Vector3 angle; //0x0040
	char _0x004C[34]; //0x004C
	BYTE bCrouching;
	char _0x0072[137];
	__int32 health; //0x00F8
	__int32 armor; //0x00FC
	char _0x0100[292];
	BYTE bAttacking; //0x0224
	char name[16]; //0x0225
	char _0x0235[247];
	BYTE team; //0x032C
	char _0x032D[11];
	BYTE state; //0x0338
	char _0x0339[59];
	weapon* weapon; //0x0374
	char _0x0378[520];
};

class entList
{
public:
	ent* ents[31];
};

bool isValidEnt(ent* ent);
bool isTeamGameMode();
ent* getClosestEnt(ent* localPlayer, entList* entList);
Vector3 calcAngle(Vector3 src, Vector3 dst);