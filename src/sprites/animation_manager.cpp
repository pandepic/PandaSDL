#include "animation_manager.h"

PandaSDL::AnimationManager::AnimationManager()
{
}

PandaSDL::AnimationManager::~AnimationManager()
{
}

void PandaSDL::AnimationManager::LoadAnimations(std::string animationsFile)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(animationsFile.c_str());

    if (!result)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::LOADING_ANIMATIONS_FAILED, result.description());
    
    for (auto node: doc.child("Animations").children("Animation"))
    {
        auto animation = std::make_shared<PandaSDL::Animation>();

        animation->Name = node.attribute("Name").value();
        animation->Flip = StringToSpriteFlip[node.attribute("Flip").value()];
        animation->Duration = std::stod(node.attribute("Duration").value());

        auto endFrame = PANDASDL_ANIMATION_NOENDFRAME;
        auto endFrameAttr = node.attribute("EndFrame");

        if (endFrameAttr)
            endFrame = std::stoi(endFrameAttr.value());
        
        animation->EndFrame = endFrame;

        std::string frames = node.attribute("Frames").value();

        if (frames.find(",") == std::string::npos)
        {
            if (frames.find(">") == std::string::npos)
            {
                animation->Frames.push_back(std::stoi(frames));
            }
            else
            {
                auto rangeSplit = PandaSDL::SplitString(frames, ">");
                auto start = std::stoi(rangeSplit[0]);
                auto end = std::stoi(rangeSplit[1]);
                animation->FrameAddRange(start, end);
            }
        }
        else
        {
            auto frameSplit = PandaSDL::SplitString(frames, ",");

            for (const auto &frameInfo : frameSplit)
            {
                if (frameInfo.find(">") == std::string::npos)
                {
                    animation->Frames.push_back(std::stoi(frameInfo));
                }
                else
                {
                    auto rangeSplit = PandaSDL::SplitString(frameInfo, ">");
                    auto start = std::stoi(rangeSplit[0]);
                    auto end = std::stoi(rangeSplit[1]);
                    animation->FrameAddRange(start, end);
                }
            }
        }

        _animations.insert({ animation->Name, animation });
        //std::cout << "Loaded animation " << animation->Name << " - " << animationsFile << std::endl;
    }
}

std::shared_ptr<PandaSDL::Animation> PandaSDL::AnimationManager::GetAnimation(std::string name)
{
    if (_animations.count(name) == 0)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::ANIMATION_MANAGER_MISSING, name + " not found in the AnimationManager");
    
    return _animations[name];
}
