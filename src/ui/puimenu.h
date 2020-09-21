#ifndef __pandasdl_puimenu_h__
#define __pandasdl_puimenu_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "puiframe.h"
#include "puiwidget_builder.h"

#include "../general/globals.h"
#include "../graphics/sprite_font.h"

namespace PandaSDL
{
    class PUIFrame;
    
    class PUIMenu
    {
        public:
            PUIMenu();
            ~PUIMenu();
            
            std::map<std::string, std::shared_ptr<SpriteFont>> Fonts;
            std::vector<std::shared_ptr<PUIFrame>> Frames;
            std::map<std::string, pugi::xml_node> Templates;
            
            void Load(std::string assetName, std::string templatesName = "");
            void Load(std::string assetName, PUIWidgetBuilder widgetBuilder, std::string templatesName = "");
            
            std::shared_ptr<PandaSDL::PUIFrame> GetFrame(std::string name);
            std::shared_ptr<PandaSDL::PUIWidget> GetWidget(std::string name);
            
            template <class T>  
            T GetWidget(std::string name);
            
        protected:
            bool _focused;
            std::vector<std::shared_ptr<PUIFrame>> _frames;
    };
}

#endif
