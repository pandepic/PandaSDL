#ifndef __pandasdl_widgetcontainer_h__
#define __pandasdl_widgetcontainer_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "../math/vector2.h"
#include "puiwidget.h"
#include "puiwidget_builder.h"

namespace PandaSDL
{
    // forward declarations
    class PUIWidget;
    class PUIWidgetBuilder;
    
    class PUIWidgetContainer
    {
        public:
            PUIWidgetContainer();
            ~PUIWidgetContainer();
            
            Vector2 Position;
            std::vector<std::shared_ptr<PUIWidget>> Widgets;
            
            void Load(pugi::xml_node &widgetsRoot, const PUIWidgetBuilder &widgetBuilder);
            std::shared_ptr<PandaSDL::PUIWidget> GetWidget(std::string name);
            
        protected:
    };
}

#endif
