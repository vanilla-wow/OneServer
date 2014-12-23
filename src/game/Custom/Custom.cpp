/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Custom.h"

#include "Log.h"
#include "SystemConfig.h"
#include "Policies/Singleton.h"


INSTANTIATE_SINGLETON_1(CustomClass);

CustomClass::CustomClass()
{
    for (int i = 0; i < CUSTOMCONFIG_UINT32_VALUE_COUNT; ++i)
        m_customConfigUint32Values[i] = 0;

    for (int i = 0; i < CUSTOMCONFIG_INT32_VALUE_COUNT; ++i)
        m_customConfigInt32Values[i] = 0;

    for (int i = 0; i < CUSTOMCONFIG_FLOAT_VALUE_COUNT; ++i)
        m_customConfigFloatValues[i] = 0.0f;

    for (int i = 0; i < CUSTOMCONFIG_BOOL_VALUE_COUNT; ++i)
        m_customConfigBoolValues[i] = false;
}

CustomClass::~CustomClass()
{
}

void CustomClass::CustomInit()
{
    LoadCustomConfSettings();
}

void CustomClass::LoadCustomConfSettings(bool reload)
{
    bool loaded = true;
    if (reload)
    {
        if (!CustomConfig.Reload())
        {
            sLog.outError("Custom settings reload fail: can't read settings from %s.", CustomConfig.GetFilename().c_str());
            loaded = false;
            return;
        }
    }
    else
    {
        // Get configuration file
        if (!CustomConfig.SetSource(_CUSTOM_CONFIG))
        {
            loaded = false;
            error_log("CUSTOM: Unable to open configuration file. Configuration values will use default.");
        }
        else
            outstring_log("CUSTOM: Using configuration file %s", _CUSTOM_CONFIG);
    }

    ///- Read the version of the configuration file and warn the user in case of emptiness or mismatch

    if (loaded)
    {
        if (CustomConfig.GetIntDefault("ConfVersion", 0) < _CUSTOMCONFVERSION)
        {
            sLog.outError("*****************************************************************************");
            sLog.outError(" WARNING: Your custom.conf version indicates your conf file is out of date!");
            sLog.outError("          Please check for updates, as your current default values may cause");
            sLog.outError("          unexpected behavior.");
            sLog.outError("*****************************************************************************");
            Log::WaitBeforeContinueIfNeed();
        }
    }
}