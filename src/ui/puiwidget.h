#ifndef __pandasdl_puiwidget_h__
#define __pandasdl_puiwidget_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "puiwidget_container.h"
#include "../math/rectangle.h"
#include "../general/timer.h"

#define PANDASDL_WIDGET_NOSETPOS -1

namespace PandaSDL
{
    struct PUIWidgetPositionFlags
    {
        PUIWidgetPositionFlags() : SetX(PANDASDL_WIDGET_NOSETPOS), SetY(PANDASDL_WIDGET_NOSETPOS) {}
        
        bool CenterX, CenterY;
        bool AnchorLeft, AnchorRight, AnchorTop, AnchorBottom;
        int SetX, SetY;
    };
    
    // forward declarations
    class PUIWidgetContainer;
    
    class PUIWidget
    {
        public:
            PUIWidget();
            ~PUIWidget();
            
            std::string Name;
            
            virtual void Load(PUIWidgetContainer *parent, pugi::xml_node &node);
            virtual void Update(const PandaSDL::Timer &gameTimer) = 0;
            
        protected:
            PUIWidgetContainer *_parent;
            pugi::xml_node _template;
            Rectangle _widgetRect;
            
            void LoadTemplate();
            bool PointInsideWidget(PandaSDL::Vector2 point);
            void UpdatePositionFromFlags();
            
            void AnchorTop();
            void AnchorBottom();
            void AnchorLeft();
            void AnchorRight();
            void CenterX();
            void CenterY();
            
            // get nodes and attributes from a template where available, otherwise from the widget node
            pugi::xml_node GetXMLNode(std::string name);
            pugi::xml_node GetXMLNode(std::string parent, std::string child);
            pugi::xml_node GetXMLAttribute(std::string name);
            pugi::xml_node GetXMLAttribute(std::string parent, std::string child);
    };
}

#endif