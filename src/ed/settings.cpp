#include "settings.h"
#include "../gbin/gfile.h"
#include "inicpp.h"

void Settings::Load()
{
    if (GFile::FileExists("settings.ini"))
    {
        ini::IniFile f;
        f.load("settings.ini");
        width = f["Settings"]["Width"].as<int>();
        height = f["Settings"]["Height"].as<int>();
        style.name = f["Settings"]["Style"].as<std::string>();
    }
    else
    {
        style.name = "Luigis Mansion";
    }
}

void Settings::LoadTheme()
{
    style.Load(style.name);
}

void Settings::Save()
{
    ini::IniFile f;
    f["Settings"]["Width"] = width;
    f["Settings"]["Height"] = height;
    f["Settings"]["Style"] = style.name;
    f.save("settings.ini");
}