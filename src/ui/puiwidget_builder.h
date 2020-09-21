#ifndef __pandasdl_puiwidget_builder_h__
#define __pandasdl_puiwidget_builder_h__

//#include "widgets/puiwimage_box.h"
#include "puiwidget.h"

namespace PandaSDL
{
    class PUIWidget;
    
    class PUIWidgetBuilder
    {
        public:
            PUIWidgetBuilder();
            ~PUIWidgetBuilder();
            
            std::shared_ptr<PandaSDL::PUIWidget> BuildWidget(const pugi::xml_node &node) const;
            
        protected:
    };
}

#endif
