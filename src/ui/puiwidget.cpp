#include "puiwidget.h"

PandaSDL::PUIWidget::PUIWidget()
    : _widgetRect()
{
}

PandaSDL::PUIWidget::~PUIWidget()
{
}

void PandaSDL::PUIWidget::Load(std::shared_ptr<PUIFrame> parent, pugi::xml_node node)
{
    // _node = node;
    
    // auto name = node.attribute("Name").value();
    // PandaSDL::Vector2 position = { node.child("Position").attribute("X").as_int() };
}

void PandaSDL::PUIWidget::LoadTemplate()
{
}

// pugi::xml_node PandaSDL::PUIWidget::GetXMLNode(std::string name)
// {
// }

// pugi::xml_node PandaSDL::PUIWidget::GetXMLNode(std::string parent, std::string child)
// {
// }

// pugi::xml_node PandaSDL::PUIWidget::GetXMLAttribute(std::string name)
// {
// }

// pugi::xml_node PandaSDL::PUIWidget::GetXMLAttribute(std::string parent, std::string child)
// {
// }

void PandaSDL::PUIWidget::AnchorTop()
{
}

void PandaSDL::PUIWidget::AnchorBottom()
{
}

void PandaSDL::PUIWidget::AnchorLeft()
{
}

void PandaSDL::PUIWidget::AnchorRight()
{
}

void PandaSDL::PUIWidget::CenterX()
{
}

void PandaSDL::PUIWidget::CenterY()
{
}
