#include "settings_manager.h"

PandaSDL::SettingsManager::SettingsManager()
{
}

PandaSDL::SettingsManager::~SettingsManager()
{
}

void PandaSDL::SettingsManager::Load(std::string settingsFile)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(settingsFile.c_str());
    
    if (!result)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::LOADING_SETTINGS_FAILED, result.description());
    
    for (auto sectionNode : doc.child("Settings").children("Section"))
    {
        PandaSDL::SettingsSection newSection;
        newSection.Name = sectionNode.attribute("Name").value();
        
        for (auto settingNode : sectionNode.children("Setting"))
        {
            PandaSDL::Setting newSetting;
            newSetting.Name = settingNode.attribute("Name").value();
            newSetting.Value = settingNode.attribute("Value").value();
            
            for (auto settingAttr : settingNode.attributes())
            {
                std::string attrName = settingAttr.name();
                
                if (attrName != "Name" && attrName != "Value")
                    newSetting.Attributes.insert(std::pair<std::string, std::string>(attrName, settingAttr.value()));
            }
            
            newSection.Settings.insert(std::pair<std::string, PandaSDL::Setting>(newSetting.Name, newSetting));
        }
        
        _settingsSections.insert(std::pair<std::string, PandaSDL::SettingsSection>(newSection.Name, newSection));
    }
}

std::string PandaSDL::SettingsManager::GetSettingValue(std::string sectionName, std::string settingName)
{
    return _settingsSections[sectionName].Settings[settingName].Value;
}

int PandaSDL::SettingsManager::GetSettingValueInt(std::string sectionName, std::string settingName)
{
    return std::stoi(GetSettingValue(sectionName, settingName));
}

std::string PandaSDL::SettingsManager::GetSettingAttribute(std::string sectionName, std::string settingName, std::string attributeName)
{
    return _settingsSections[sectionName].Settings[settingName].Attributes[attributeName];
}

std::vector<PandaSDL::Setting> PandaSDL::SettingsManager::GetSettings(std::string sectionName)
{
    std::vector<PandaSDL::Setting> result;
    auto section = _settingsSections[sectionName];
    
    for (const auto &[key, val] : section.Settings)
    {
        result.push_back(val);
    }
    
    return result;
}