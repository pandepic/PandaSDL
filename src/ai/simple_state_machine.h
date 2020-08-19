#ifndef __pandasdl_simplestatemachine_h__
#define __pandasdl_simplestatemachine_h__

#include "../general/timer.h"

namespace PandaSDL
{
    struct SimpleStateBase
    {
        std::string Name;
        SimpleStateMachine *Parent;
        
        virtual void Begin() = 0;
        virtual void Update(const PandaSDL::Timer &gameTimer) = 0;
        virtual void End() = 0;
    };
    
    struct SimpleStateTimedBase : public SimpleStateBase
    {
        int Duration;
        
        virtual void Begin() override {}
        virtual void Update(const PandaSDL::Timer &gameTimer) override;
        virtual void EndDuration() = 0;
        virtual void End() override {}
    };
    
    class SimpleStateMachine
    {
        public:
            SimpleStateMachine();
            ~SimpleStateMachine();
            
            std::string GetCurrentStateName();
            std::shared_ptr<SimpleStateBase> GetCurrentState();
            
            void RegisterState(std::shared_ptr<PandaSDL::SimpleStateBase> state);
            void RemoveState(std::string name);
            void RemoveState(std::shared_ptr<PandaSDL::SimpleStateBase> state);
            
            void SetState(std::string name, bool allowSame = false);
            void SetState(std::shared_ptr<PandaSDL::SimpleStateBase> state, bool allowSame = false);
            void SetNextState(std::string name, bool allowSame = false);
            void SetNextState(std::shared_ptr<PandaSDL::SimpleStateBase> state, bool allowSame = false);
            
            void Start(std::string initialState);
            void Update(const PandaSDL::Timer &gameTimer);
            
        protected:
            std::map<std::string, std::shared_ptr<SimpleStateBase>> _states;
            std::shared_ptr<SimpleStateBase> _currentState;
            std::shared_ptr<SimpleStateBase> _nextState;
            std::shared_ptr<SimpleStateBase> _prevState;
            
            void SetCurrentStateInternal(std::shared_ptr<SimpleStateBase> state);
    };
}

#endif