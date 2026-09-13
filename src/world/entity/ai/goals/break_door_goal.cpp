#include "world/entity/ai/goals/break_door_goal.h"
#include "world/entity/mob.h"
#include "world/level/level.h"
#include "world/level/world.h"
#include "world/level/chunk/chunk.h"
#include "world/difficulty.h"

extern World g_world;

bool BreakDoorGoal::canUse() {
    if (!DoorInteractGoal::canUse()) return false;
    return !isDoorOpen();
}

bool BreakDoorGoal::canContinueToUse() {
    if (ticksLeft < 0) return false;
    if (isDoorOpen()) return false;
    return mob->distanceToSqr((float)doorX, (float)doorY, (float)doorZ) < 4.0f;
}

void BreakDoorGoal::start() {
    DoorInteractGoal::start();
    ticksLeft = 240;
}

void BreakDoorGoal::tick() {
    DoorInteractGoal::tick();
    if (--ticksLeft != 0) return;

    if (mob->level->getDifficulty() != Difficulty::HARD) return;

    unsigned char d = (unsigned char)mob->level->getData(doorX, doorY, doorZ);
    int lowerY = (d & 8) ? doorY - 1 : doorY;
    worldSetBlockAndData(&g_world, doorX, lowerY, doorZ, 0, 0);
    worldNotifyNeighborsChanged(&g_world, doorX, lowerY, doorZ);
    worldRebuildAroundNow(&g_world, doorX, lowerY, doorZ);
}
