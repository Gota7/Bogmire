#include "door.h"

void Door::Read(GFile* f)
{
    orientation = f->ReadU8();
    type = f->ReadU8();
    f->ReadU16();
    jmpDoorNum = f->ReadU16();
    modelId = f->ReadU8();
    doorIndex = f->ReadS8();
    xPos = f->ReadS32();
    yPos = f->ReadS32();
    zPos = f->ReadS32();
    viewportX = f->ReadS16();
    viewportY = f->ReadS16();
    viewportZ = f->ReadS16();
    nextRoomIndex = f->ReadU8();
    currRoomIndex = f->ReadU8();
}

void Door::Write(GFile* f)
{
    f->Write(orientation);
    f->Write(type);
    f->Write((u16)0);
    f->Write(jmpDoorNum);
    f->Write(modelId);
    f->Write(doorIndex);
    f->Write(xPos);
    f->Write(yPos);
    f->Write(zPos);
    f->Write(viewportX);
    f->Write(viewportY);
    f->Write(viewportZ);
    f->Write(nextRoomIndex);
    f->Write(currRoomIndex);
}