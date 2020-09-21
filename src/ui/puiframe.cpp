#include "puiframe.h"

PandaSDL::PUIFrame::PUIFrame(PUIMenu *parent, pugi::xml_node &node, const PUIWidgetBuilder &widgetBuilder)
{
    Parent = parent;
    
    auto widgetsRoot = node.child("Widgets");
    
    _widgetContainer.Load(widgetsRoot, widgetBuilder);
}

PandaSDL::PUIFrame::~PUIFrame()
{
}

std::shared_ptr<PandaSDL::PUIWidget> PandaSDL::PUIFrame::GetWidget(std::string name)
{
    return _widgetContainer.GetWidget(name);
}
