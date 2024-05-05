#include "hack.h"

// Font for printing to screen
GL::Font font;

// Offsets
uintptr_t offsets::dwcrosshair = 0x607c0;
uintptr_t offsets::dwentityListPtr = 0x10f4f8;
uintptr_t offsets::dwlocalPlayerPtr = 0x10F4F4;
uintptr_t offsets::iHealth = 0xF8;
uintptr_t offsets::iArmor = 0xFC;
uintptr_t offsets::recoil = 0x463786;
uintptr_t offsets::iNumOfPlayers = 0x50f500;
uintptr_t offsets::iGameMode = 0x50f49c;
uintptr_t offsets::vMatrix = 0x501AE8;

uintptr_t offsets::showCursorAddr = NULL;

// Global Hack state variables
uintptr_t hack::modBase = 0;
uintptr_t* hack::localPlayerPtr = 0;
uintptr_t* hack::entityListPtr = 0;
ent* hack::localPlayer = 0;
entList* hack::entityList = nullptr;
HMODULE hack::showCursorMod = NULL;


// Hack statuses
bool hack::bHealth = false;
bool hack::bAmmo = false;
bool hack::bRecoil = false;
bool hack::bAimbot = false;
int hack::aimBotMode = 0;
bool hack::bESP = false;
bool hack::bMenu = false;
bool hack::bPlayerNames = false;
bool hack::bSnapLines = false;

// For Drawing
int hack::fontWidth = 8;
int hack::fontHeight = 15;

ImVec4 hack::rgb::playerBoxColor = ImVec4(255, 0, 0, 1);
ImVec4 hack::rgb::playerNameColor = ImVec4(255, 255, 255, 1);
ImVec4 hack::rgb::playerSnapColor = ImVec4(255, 0, 0, 1);

void hack::init() {
	// Get Module Base Address
	hack::modBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	hack::localPlayerPtr = (uintptr_t*)(hack::modBase + offsets::dwlocalPlayerPtr);
	hack::localPlayer = (ent*)*localPlayerPtr;
	hack::entityListPtr = (uintptr_t*)(hack::modBase + offsets::dwentityListPtr);
	hack::entityList = (entList*)(*entityListPtr);
	hack::showCursorMod = GetModuleHandle(L"SDL.dll");
	offsets::showCursorAddr = (uintptr_t)GetProcAddress(hack::showCursorMod, "SDL_ShowCursor");
}

void hack::doESP() {

	GL::SetupOrtho();
	drawESPBoxes();
	GL::RestoreGL();
}

void hack::doAmmo() {
	*(hack::localPlayer->weapon->ammo) = 1337;
}

void hack::doHealth() {
	hack::localPlayer->health = 1337;
}

void hack::doRecoil() {
	mem::Nop((BYTE*)0x463786, 10);
}

void hack::fixRecoil() {
	mem::Patch((BYTE*)0x463786, (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
}

void hack::doAimbot() {
	if (hack::aimBotMode == 0 || hack::aimBotMode == 1 && GetAsyncKeyState(VK_RBUTTON)) {
		ent* target = getClosestEnt(hack::localPlayer, hack::entityList);
		if (target) {
			// Calculate Angle
			Vector3 aimAngles = calcAngle(hack::localPlayer->head, target->head);
			// Set yaw/pitch
			hack::localPlayer->angle.x = aimAngles.x;
			hack::localPlayer->angle.y = aimAngles.y;
		}
	}
}


void hack::drawESPBoxes() {
	for (int i = 0; i < *(int*)offsets::iNumOfPlayers; i++) {
		if ((isValidEnt(hack::entityList->ents[i]) && isTeamGameMode() && hack::localPlayer->team != hack::entityList->ents[i]->team && hack::entityList->ents[i]->health > 0) ||
			(isValidEnt(hack::entityList->ents[i]) && !isTeamGameMode() && hack::entityList->ents[i]->health > 0)) {
			ent* currentEnt = hack::entityList->ents[i];

			Vector3 headPos = currentEnt->head;
			Vector3 feetPos = currentEnt->pos;

			Vector3 headScreenPos, feetScreenPos;

			// Makes box slightly above player
			headPos.z != 0.8f;

			if (currentEnt->bCrouching)
				headPos.z -= 0.5625;

			// Get Screen Head/Feet POS
			if (WorldToScreen(headPos, headScreenPos, (float*)offsets::vMatrix, (int)GL::getScreenx(), (int)GL::getScreenY()) &&
				WorldToScreen(feetPos, feetScreenPos, (float*)offsets::vMatrix, (int)GL::getScreenx(), (int)GL::getScreenY())) {
				float height = feetScreenPos.y - headScreenPos.y;
				float width = height / 2.0f;
				
				// Draw Outline
				GL::DrawOutline(headScreenPos.x - width / 2.0f, headScreenPos.y, headScreenPos.x + width / 2.0f, headScreenPos.y + height, 2.0f, hack::rgb::playerBoxColor);

				// Print Player Name
				if (hack::bPlayerNames) {
					std::string name = currentEnt->name;
					int tWidth = name.length() * hack::fontWidth;
					float diff = width - tWidth;
					font.Print((headScreenPos.x - width / 2.0f) + diff / 2, headScreenPos.y - hack::fontHeight / 2, hack::rgb::playerNameColor, "%s", currentEnt->name);
				}
				
				// Draw Snap Lines
				if (hack::bSnapLines) {
					GL::DrawLine(GL::getScreenx() / 2.f, GL::getScreenY() / 2.f, feetScreenPos.x, feetScreenPos.y, 2.0f, hack::rgb::playerSnapColor);
				}
			}
		}
	}
}