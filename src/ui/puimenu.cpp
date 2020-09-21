#include "puimenu.h"

PandaSDL::PUIMenu::PUIMenu()
    : _focused(false)
{
}

PandaSDL::PUIMenu::~PUIMenu()
{
    Fonts.clear();
    _frames.clear();
}

void PandaSDL::PUIMenu::Load(std::string assetName, std::string templatesName)
{
    Load(assetName, PUIWidgetBuilder(), templatesName);
}

void PandaSDL::PUIMenu::Load(std::string assetName, PUIWidgetBuilder widgetBuilder, std::string templatesName)
{
    auto assetPath = PandaSDL::Game::AssetManager.GetAssetPath(assetName);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(assetPath.c_str());
    
    if (!result)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::MENU_LOADING_FAILED, result.description());
    
    auto menuRoot = doc.child("Menu");
    
    for (auto fontElement : menuRoot.children("Font"))
    {
        auto fontAsset = fontElement.attribute("AssetName").value();
        auto font = PandaSDL::Game::AssetManager.LoadSpritefont(fontAsset);
        Fonts.insert({ fontAsset, font });
    }
    
    for (auto frameElement : menuRoot.children("Frame"))
    {
        auto newFrame = std::make_shared<PUIFrame>(this, frameElement, widgetBuilder);
        _frames.push_back(newFrame);
    }
} // Load

std::shared_ptr<PandaSDL::PUIFrame> PandaSDL::PUIMenu::GetFrame(std::string name)
{
    for (const auto &frame : _frames)
    {
        if (frame->Name == name)
            return frame;
    }
    
    return nullptr;
}

std::shared_ptr<PandaSDL::PUIWidget> PandaSDL::PUIMenu::GetWidget(std::string name)
{
    std::shared_ptr<PUIWidget> widget = nullptr;
    
    for (const auto &frame : _frames)
    {
        if (widget == nullptr)
            widget = frame->GetWidget(name);
    }
    
    return widget;
}

template <class T>  
T PandaSDL::PUIMenu::GetWidget(std::string name)
{
    std::shared_ptr<PUIWidget> widget = nullptr;
    
    for (const auto &frame : _frames)
    {
        if (widget == nullptr)
            widget = frame->GetWidget(name);
    }
    
    return static_cast<std::shared_ptr<T>>(widget);
}
