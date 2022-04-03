#pragma once

#include "../gbin/gtypes.h"

// A door that can be edited. See https://wiki.lmhack.in/Documentation:Doors
struct Door : GReadable, GWriteable
{
    u8 orientation;
    u8 type;
    u16 jmpDoorNum;
    u8 modelId;
    s8 doorIndex;
    s32 xPos;
    s32 yPos;
    s32 zPos;
    s16 viewportX;
    s16 viewportY;
    s16 viewportZ;
    u8 nextRoomIndex;
    u8 currRoomIndex;

    // V-Functions.
    void Read(GFile* f);
    void Write(GFile* f);

};