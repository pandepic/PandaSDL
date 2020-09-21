#include "puiwidget_container.h"

PandaSDL::PUIWidgetContainer::PUIWidgetContainer()
{
}

PandaSDL::PUIWidgetContainer::~PUIWidgetContainer()
{
    Widgets.clear();
}

void PandaSDL::PUIWidgetContainer::Load(pugi::xml_node &widgetsRoot, const PUIWidgetBuilder &widgetBuilder)
{
    for (const auto &widget : widgetsRoot.children())
    {
        auto newWidget = widgetBuilder.BuildWidget(widget);
        
        if (newWidget != nullptr)
            Widgets.push_back(newWidget);
    }
}

std::shared_ptr<PandaSDL::PUIWidget> PandaSDL::PUIWidgetContainer::GetWidget(std::string name)
{
    for (const auto &widget : Widgets)
    {
        if (widget->Name == name)
            return widget;
    }
    
    return nullptr;
}
