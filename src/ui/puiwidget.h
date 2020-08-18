#ifndef __pandasdl_puiwidget_h__
#define __pandasdl_puiwidget_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "puiframe.h"
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
    
    class PUIWidget
    {
        public:
            PUIWidget();
            ~PUIWidget();
            
            virtual void Load(std::shared_ptr<PUIFrame> parent, pugi::xml_node node);
            virtual void Update(const PandaSDL::Timer &gameTimer) = 0;
            
        protected:
            pugi::xml_node _template;
            pugi::xml_node _node;
            std::shared_ptr<PUIFrame> _parent;
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