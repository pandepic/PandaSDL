#include "game.h"

PandaSDL::DynamicTextureManager PandaSDL::Game::DynamicTextureManager = PandaSDL::DynamicTextureManager();
PandaSDL::AssetManager PandaSDL::Game::AssetManager = PandaSDL::AssetManager();
PandaSDL::AnimationManager PandaSDL::Game::AnimationManager = PandaSDL::AnimationManager();
PandaSDL::InputManager PandaSDL::Game::InputManager = PandaSDL::InputManager();
PandaSDL::SettingsManager PandaSDL::Game::SettingsManager = PandaSDL::SettingsManager();
PandaSDL::GameControlManager PandaSDL::Game::GameControlManager = PandaSDL::GameControlManager();
PandaSDL::Game* PandaSDL::Game::GameInstance = nullptr;

PandaSDL::Game::Game()
    : CurrentGamestate(nullptr), NextGamestate(nullptr), _quit(false), _depthEnabled(false), _blendEnabled(false), _clearFlags(GL_COLOR_BUFFER_BIT)
{
}

PandaSDL::Game::~Game()
{
    if (_glContext != NULL)
        SDL_GL_DeleteContext(_glContext);
}

void PandaSDL::Game::SetGamestate(PandaSDL::Gamestate* NewState)
{
    if (NewState == nullptr)
        return;
    
    if (CurrentGamestate != nullptr)
        CurrentGamestate->Close();
    
    CurrentGamestate = NewState;
    CurrentGamestate->Setup(PandaSDL::Game::GameInstance);
}

void PandaSDL::Game::SetNextGamestate(PandaSDL::Gamestate* NewState)
{
    if (NewState == nullptr)
        return;
    
    NextGamestate = NewState;
}

void PandaSDL::Game::Quit()
{
    _quit = true;
}

void PandaSDL::Game::Setup(PandaSDL::Game* gameInstance, std::string windowName, PandaSDL::Rectangle windowRect, std::string assetsPath, std::string animationsAssetName, bool vsync, bool glDebug)
{
    _windowName = windowName;
    _windowRect = windowRect;
    _windowFlags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SDL_INIT_FAILED);

    _window = PandaSDL::SDLSharedPtr(SDL_CreateWindow(_windowName.c_str(), _windowRect.X, _windowRect.Y, _windowRect.Width, _windowRect.Height, _windowFlags));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    _glContext = SDL_GL_CreateContext(_window.get());
    
    ResetViewport();
    
    SetClearColor(PandaSDL::Color(0, 0, 0, 255));
    
    SDL_GL_SetSwapInterval(vsync);
    
    _frameTimer.Start();
    
    if (glewInit() != GLEW_OK)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::GLEW_INIT_FAILED);

    if (glDebug)
        PandaSDL::GraphicsPlatform::EnableDebugOutput();
    
    PandaSDL::GraphicsPlatform::SetPixelStorageUnpackAlignment(1);
    
    //std::cout << "GL Version: " << PandaSDL::GraphicsPlatform::GetVersion() << std::endl;

    PandaSDL::Game::GameInstance = gameInstance;
    PandaSDL::Game::AssetManager.LoadAssets(assetsPath);
    PandaSDL::Game::AnimationManager.LoadAnimations(PandaSDL::Game::AssetManager.GetAssetPath(animationsAssetName));

    Load();
}

int PandaSDL::Game::Run()
{
    _gameTimer.Start();

    while (!_quit)
    {
        if (NextGamestate != nullptr)
        {
            CurrentGamestate->Close();
            CurrentGamestate = NextGamestate;
            CurrentGamestate->Setup(PandaSDL::Game::GameInstance);
            NextGamestate = nullptr;
        }
        
        // Begin input events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
                {
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                        Quit();
                }
                break;
                
                case SDL_KEYDOWN:
                {
                    PandaSDL::Game::InputManager.HandleKeyDown(event.key, _gameTimer);
                }
                break;
                
                case SDL_KEYUP:
                {
                    PandaSDL::Game::InputManager.HandleKeyUp(event.key, _gameTimer);
                }
                break;
                
                case SDL_MOUSEMOTION:
                {
                    PandaSDL::Game::InputManager.HandleMouseMotion(event.motion, _gameTimer);
                }
                break;
                
                case SDL_MOUSEBUTTONDOWN:
                {
                    PandaSDL::Game::InputManager.HandleMouseButtonDown(event.button, _gameTimer);
                }
                break;
                
                case SDL_MOUSEBUTTONUP:
                {
                    PandaSDL::Game::InputManager.HandleMouseButtonUp(event.button, _gameTimer);
                }
                break;
                
                case SDL_MOUSEWHEEL:
                {
                    PandaSDL::Game::InputManager.HandleMouseWheel(event.wheel, _gameTimer);
                }
                break;
                
                case SDL_QUIT:
                {
                    Quit();
                }
                break;
            }
        }
        // End input events

        // todo : look at if this is the best way to support closing the game long term
        if (_quit)
            return EXIT_SUCCESS;
        
        // Begin Update
        PandaSDL::Game::InputManager.Update(_gameTimer);
        Update(_gameTimer);
        
        if (CurrentGamestate != nullptr)
            CurrentGamestate->Update(_gameTimer);
        
        _gameTimer.Tick();
        // End Update

        //Begin Draw
        Clear();
        
        Draw(_gameTimer);
        
        if (CurrentGamestate != nullptr)
            CurrentGamestate->Draw(_gameTimer);
        
        SDL_GL_SwapWindow(_window.get());
        // End Draw

        if (_targetFPS > PANDASDL_UNLIMITED_FPS)
        {
            auto targetTicks = 1000.0 / (double)_targetFPS;
            auto ticks = _frameTimer.GetTicks();

            if (ticks < targetTicks)
            {
                SDL_Delay(targetTicks - ticks);
            }
        }

        _frameCounter++;
        _frameCounterTicks += _frameTimer.GetTicks();

        if (_frameCounterTicks >= 1000)
        {
            std::string newTitle = _windowName + " [PandaSDL Game Engine] - " + std::to_string(_frameCounter) + " fps";
            SDL_SetWindowTitle(_window.get(), newTitle.c_str());
            _frameCounter = 0;
            _frameCounterTicks -= 1000;
        }

        _frameTimer.Tick();
    }

    return EXIT_SUCCESS;
}

void PandaSDL::Game::ResetViewport()
{
    PandaSDL::GraphicsPlatform::SetViewport(0, 0, _windowRect.Width, _windowRect.Height);
}

void PandaSDL::Game::SetClearColor(PandaSDL::Color color)
{
    _windowClearColor = color;
    PandaSDL::GraphicsPlatform::SetClearColor(_windowClearColor.R, _windowClearColor.G, _windowClearColor.B, _windowClearColor.A);
}

void PandaSDL::Game::Clear()
{
    PandaSDL::GraphicsPlatform::Clear(_clearFlags);
}

void PandaSDL::Game::Clear(PandaSDL::Color color)
{
    auto prevColor = _windowClearColor;

    SetClearColor(color);
    Clear();
    SetClearColor(prevColor);
}

PandaSDL::Rectangle PandaSDL::Game::GetWindowRect()
{
    return _windowRect;
}

void PandaSDL::Game::EnableBlend(GLenum sfactor, GLenum dfactor)
{
    if (_blendEnabled)
        return;
    
    PandaSDL::GraphicsPlatform::EnableFeature(GL_BLEND);
    PandaSDL::GraphicsPlatform::SetBlendFunc(sfactor, dfactor);
    
    _blendEnabled = true;
}

void PandaSDL::Game::EnableDepth(GLenum func)
{
    if (_depthEnabled)
        return;
    
    PandaSDL::GraphicsPlatform::EnableFeature(GL_DEPTH_TEST);
    PandaSDL::GraphicsPlatform::SetDepthFunc(func);
    PandaSDL::GraphicsPlatform::Clear(GL_DEPTH_BUFFER_BIT);
    
    _clearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    _depthEnabled = true;
}

bool PandaSDL::Game::GetDepthEnabled()
{
    return _depthEnabled;
}

bool PandaSDL::Game::GetBlendEnabled()
{
    return _blendEnabled;
}
