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

#ifndef _CUSTOM_H
#define _CUSTOM_H

#include "Common.h"
#include "Config/Config.h"

enum CustomConfigUint32Values
{
    CUSTOMCONFIG_UINT32_EMPTY = 0,  // overwrite with new conf
    CUSTOMCONFIG_UINT32_VALUE_COUNT
};

enum CustomConfigInt32Values
{
    CUSTOMCONFIG_INT32_EMPTY = 0,  // overwrite with new conf
    CUSTOMCONFIG_INT32_VALUE_COUNT
};

enum CustomConfigFloatValues
{
    CUSTOMCONFIG_FLOAT_EMPTY = 0,  // overwrite with new conf
    CUSTOMCONFIG_FLOAT_VALUE_COUNT
};

enum CustomConfigBoolValues
{
    CUSTOMCONFIG_BOOL_EMPTY = 0, 
    CUSTOMCONFIG_BOOL_VALUE_COUNT
};

class MANGOS_DLL_SPEC CustomClass
{
    public:
        CustomClass();
        ~CustomClass();

        void CustomInit();
        void LoadCustomConfSettings(bool reload = false);

        // Getters and Setters
        void setCustomConfig(CustomConfigUint32Values index, char const* fieldname, uint32 defvalue) { m_customConfigUint32Values[index] = CustomConfig.GetIntDefault(fieldname, defvalue); }
        uint32 getCustomConfig(CustomConfigUint32Values index) const { return m_customConfigUint32Values[index]; }

        void setCustomConfig(CustomConfigInt32Values index, char const* fieldname, int32 defvalue) { m_customConfigInt32Values[index] = CustomConfig.GetIntDefault(fieldname, defvalue); }
        int32 getCustomConfig(CustomConfigInt32Values index) const { return m_customConfigInt32Values[index]; }

        void setCustomConfig(CustomConfigFloatValues index, char const* fieldname, float defvalue) { m_customConfigFloatValues[index] = CustomConfig.GetFloatDefault(fieldname, defvalue); }
        float getCustomConfig(CustomConfigFloatValues rate) const { return m_customConfigFloatValues[rate]; }

        void setCustomConfig(CustomConfigBoolValues index, char const* fieldname, bool defvalue) { m_customConfigBoolValues[index] = CustomConfig.GetBoolDefault(fieldname, defvalue); }
        bool getCustomConfig(CustomConfigBoolValues index) const { return m_customConfigBoolValues[index]; }

    protected:

    private:
        Config CustomConfig;

        uint32 m_customConfigUint32Values[CUSTOMCONFIG_UINT32_VALUE_COUNT];
        int32 m_customConfigInt32Values[CUSTOMCONFIG_INT32_VALUE_COUNT];
        float m_customConfigFloatValues[CUSTOMCONFIG_FLOAT_VALUE_COUNT];
        bool m_customConfigBoolValues[CUSTOMCONFIG_BOOL_VALUE_COUNT];
};


#define sCustom MaNGOS::Singleton<CustomClass>::Instance()

#endif