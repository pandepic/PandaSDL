#include "puiwidget_builder.h"

PandaSDL::PUIWidgetBuilder::PUIWidgetBuilder()
{
}

PandaSDL::PUIWidgetBuilder::~PUIWidgetBuilder()
{
}

std::shared_ptr<PandaSDL::PUIWidget> PandaSDL::PUIWidgetBuilder::BuildWidget(const pugi::xml_node &node) const
{
    auto nodeName = node.name();
    
    if (nodeName == "ImageBox")
    {
    }
    else if (nodeName == "Button")
    {
    }
    
    return nullptr;
}
