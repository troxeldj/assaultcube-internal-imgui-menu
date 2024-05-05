#include "hack.h"
#include "game.h"

bool isValidEnt(ent* ent) {
	if (ent) {
		if (ent->vTable == 0x004e4ac0)
			return true;
	}
	return false;
}


bool isTeamGameMode() {
	int gameMode = *(int*)offsets::iGameMode;

	return (gameMode == 0 || gameMode == 4 || gameMode == 5 || 
		gameMode == 7 || gameMode == 11 || gameMode == 13 || gameMode == 14 || 
		gameMode == 16 || gameMode == 17 || gameMode == 20 || gameMode == 21);
}


ent* getClosestEnt(ent* localPlayer, entList* entList) {
	float closest_distance = FLT_MAX;
	float currentDistance = 0;
	ent* target = nullptr;

	for (int i = 0; i < *(int*)offsets::iNumOfPlayers; i++) {
		if ((isValidEnt(entList->ents[i]) && entList->ents[i]->health > 0 && isTeamGameMode() && entList->ents[i]->team != hack::localPlayer->team) ||
			(isValidEnt(entList->ents[i]) && !isTeamGameMode() && entList->ents[i]->health > 0)) {
			currentDistance = vec3Distance(localPlayer->pos, entList->ents[i]->pos);
				if (currentDistance < closest_distance) {
					closest_distance = currentDistance;
					target = entList->ents[i];
				}
		}
	}
	return target;
}


Vector3 calcAngle(Vector3 src, Vector3 dst) {
	Vector3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / (float)PI * 180 + 180;
	angle.y = asinf((dst.z - src.z) / vec3Distance(src, dst)) * 180 / (float)PI;
	angle.z = 0;

	return angle;
}