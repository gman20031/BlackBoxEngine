#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <bitset>
#include <unordered_set>
#include <mutex>

#include "KeyCodes.h"
#include "MouseInputEvents.h"
#include "../System/Observer.h"
#include "../Math/FVector2.h"

namespace BlackBoxEngine
{
    class UserInterface;

    class InputManager
    {
    public:
        friend class BlackBoxManager;
        friend class UserInterface;
        using CallBackId = uint64_t;

        enum class InputType
        {
            kKeyDown,
            kKeyUp,
            kKeyHeld,
            kMouse,
            kCount,
        };

        enum class MouseInputCode;
    private:
        using Callback              = std::function<void()>;
        using MouseCallback         = std::function<void( const MouseState& )>;
        using RelativeMouseCallback = std::function<void( float , float )>;

        using InputObserver = BB_Observer< Callback, KeyCode , CallBackId>;
        using MouseObserver = BB_Observer< MouseCallback, MouseEvent, CallBackId>;
        using RelativeMouseObserver = BB_Observer< RelativeMouseCallback, MouseEvent , CallBackId>;

        struct InputTarget
        {
            InputObserver m_keyDown;
            InputObserver m_keyUp;
            InputObserver m_keyHeld;
            MouseObserver m_mouseEvent;
            RelativeMouseObserver m_relativeMouseMotionEvent;
        };
        static constexpr bool kLogInputData = false;

        std::recursive_mutex m_inputMutex;
        std::unordered_set<KeyCode> m_keyCodes;
        InputTarget m_gameInputTarget;
        InputTarget* m_pInputTarget;

        MouseState m_currentMouseState;

        float m_debugClickCircleRadius = 2.0f;

        bool m_inputCanOccur = true;
        bool m_debugClick = false;

    public:
        InputManager();

        bool IsKeyDown(KeyCode key) const;
        const MouseState& GetMouseState();

        [[nodiscard]] CallBackId SubscribeToKey( KeyCode key, InputType type, Callback&& function );
        [[nodiscard]] CallBackId SubscribeToMouse( MouseEvent event, MouseCallback&& function );
        [[nodiscard]] CallBackId SubscribeToRelativeMouse( RelativeMouseCallback&& function );
        void UnsubscribeInput(CallBackId id);
        void UnsubscribeMouseInput(CallBackId id);
        void UnsubscribeInput( CallBackId id, InputType type );
        void UnsubscribeKeyWithCode(CallBackId id, InputType type, KeyCode key);
        void Update();

        void StopAllInput() { std::unique_lock lock(m_inputMutex); m_inputCanOccur = false; }
        void ResumeInput()  { std::unique_lock lock(m_inputMutex); m_inputCanOccur = true; } 
        void SwapInputTargetToInterface( UserInterface* pInterface);
        void SwapInputToGame();
    
        void EnableShowClickDebug( bool enable, float radius );

    private:
        void PushMouseDown( float xPos, float yPos);
        void PushMouseUp( float xPos, float yPos);
        void PushMouseMotion( float newX, float newY, float relX, float relY);
        void AddKeyDown( KeyCode key );
        void RemoveKeyDown( KeyCode key );
    };

};