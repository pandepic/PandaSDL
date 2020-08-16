#ifndef __pandasdl_settings_manager_h__
#define __pandasdl_settings_manager_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "../general/extensions.h"
#include "../general/globals.h"

namespace PandaSDL
{
    struct Setting
    {
        std::string Name;
        std::string Value;
        std::map<std::string, std::string> Attributes;
    };
    
    struct SettingsSection
    {
        std::string Name;
        std::map<std::string, PandaSDL::Setting> Settings;
    };
    
    class SettingsManager
    {
        public:
            SettingsManager();
            ~SettingsManager();
            
            void Load(std::string settingsFile);
            
            std::string GetSettingValue(std::string sectionName, std::string settingName);
            int GetSettingValueInt(std::string sectionName, std::string settingName);
            std::string GetSettingAttribute(std::string sectionName, std::string settingName, std::string attributeName);
            std::vector<PandaSDL::Setting> GetSettings(std::string sectionName);
            
        protected:
            std::map<std::string, PandaSDL::SettingsSection> _settingsSections;
    };
}

#endif