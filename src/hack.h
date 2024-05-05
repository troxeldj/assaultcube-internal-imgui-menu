#include <Windows.h>
#include <iostream>
#include "game.h"
#include "mem.h"
#include "openglDraw.h"
#include "util.hpp"
#include "imgui195/imgui.h"

namespace offsets {
	extern uintptr_t dwcrosshair;
	extern uintptr_t dwentityListPtr;
	extern uintptr_t dwlocalPlayerPtr;

	extern uintptr_t iHealth;
	extern uintptr_t iArmor;
	extern uintptr_t recoil;
	extern uintptr_t iNumOfPlayers;
	extern uintptr_t iGameMode;
	extern uintptr_t vMatrix;

	extern uintptr_t showCursorAddr;
}

namespace hack {
	extern uintptr_t modBase;
	extern uintptr_t* localPlayerPtr;
	extern uintptr_t* entityListPtr;
	extern ent* localPlayer;
	extern entList* entityList;
	extern HMODULE showCursorMod;
	
	extern bool bAmmo;
	extern bool bHealth;
	extern bool bRecoil;
	extern bool bAimbot;
	extern int aimBotMode; // 0 = auto, 1 = hold
	extern bool bESP;
	extern bool bMenu;
	extern bool bPlayerNames;

	extern int fontWidth;
	extern int fontHeight;

	namespace rgb {
		extern ImVec4 playerBoxColor;
		extern ImVec4 playerNameColor;
	}


	void init();
	
	// Hack Functions
	void doESP();
	void doAmmo();
	void doHealth();
	void doRecoil();
	void fixRecoil();
	void doAimbot();
	
	// Helpers
	void drawESPBoxes();
}