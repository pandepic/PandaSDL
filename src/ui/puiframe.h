#ifndef __pandasdl_puiframe_h__
#define __pandasdl_puiframe_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "puiwidget_container.h"
#include "puimenu.h"

namespace PandaSDL
{
    class PUIMenu;
    
    class PUIFrame
    {
        public:
            PUIFrame(PUIMenu *parent, pugi::xml_node &node, const PUIWidgetBuilder &widgetBuilder);
            ~PUIFrame();
            
            std::string Name;
            PUIMenu *Parent;
            
            std::shared_ptr<PandaSDL::PUIWidget> GetWidget(std::string name);
            
        protected:
            PUIWidgetContainer _widgetContainer;
    };
}

#endif
