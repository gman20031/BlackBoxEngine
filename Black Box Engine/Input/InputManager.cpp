#include "InputManager.h"

#include <SDL.h>

#include "../Interface/UserInterface.h"

namespace BlackBoxEngine
{
    InputManager::InputManager()
    {
        m_keyCodes.reserve(static_cast<size_t>(KeyCode::kCount));
        m_pInputTarget = &m_gameInputTarget;
    }

    bool InputManager::IsKeyDown(KeyCode key) const
    {
        if (!m_inputCanOccur)
            return false;
        return m_keyCodes.contains(key);
    }

    const MouseState& InputManager::GetMouseState()
    {
        SDL_GetMouseState( &m_currentMouseState.xScreenPos, &m_currentMouseState.yScreenPos);
        return m_currentMouseState;
    }

    InputManager::CallBackId InputManager::SubscribeToKey( KeyCode key, InputType type , Callback&& function)
    {
        switch (type)
        {
        case InputType::kKeyDown:
            return m_pInputTarget->m_keyDown.RegisterListener( key, std::forward<Callback>( function ) ); break;
        case InputType::kKeyUp:
            return m_pInputTarget->m_keyUp.RegisterListener(key, std::forward<Callback>(function) ); break;
        case InputType::kKeyHeld:
            return m_pInputTarget->m_keyHeld.RegisterListener(key, std::forward<Callback>(function) ); break;
        default:
            return 0;
        }
    }

    InputManager::CallBackId InputManager::SubscribeToMouse( MouseEvent event, MouseCallback&& function )
    {
        return m_pInputTarget->m_mouseEvent.RegisterListener(
            event, std::forward<MouseCallback>( function )
        );
    }

    InputManager::CallBackId InputManager::SubscribeToRelativeMouse( RelativeMouseCallback&& function )
    {
        return m_pInputTarget->m_relativeMouseMotionEvent.RegisterListener( 
            MouseEvent::kMouseMotion, std::forward< RelativeMouseCallback>(function)
        );
    }

    void InputManager::UnsubscribeInput(CallBackId id)
    {
        m_pInputTarget->m_keyDown.RemoveListener( id );
        m_pInputTarget->m_keyUp.RemoveListener( id );
        m_pInputTarget->m_keyHeld.RemoveListener( id );
        m_pInputTarget->m_mouseEvent.RemoveListener( id );
    }

    void InputManager::UnsubscribeMouseInput( CallBackId id )
    {
        UnsubscribeInput( id, InputType::kMouse );
    }

    void InputManager::UnsubscribeInput(CallBackId id, InputType type)
    {
        switch (type)
        {
        case InputType::kKeyDown:   m_pInputTarget->m_keyDown.RemoveListener( id );     return;
        case InputType::kKeyUp:     m_pInputTarget->m_keyUp.RemoveListener( id );       return;
        case InputType::kKeyHeld:   m_pInputTarget->m_keyHeld.RemoveListener( id );     return;
        case InputType::kMouse:     m_pInputTarget->m_mouseEvent.RemoveListener( id );  return;
        default:
            return;
        }
    }

    void InputManager::UnsubscribeKeyWithCode(CallBackId id, InputType type, KeyCode key)
    {
        switch (type)
        {
        case InputType::kKeyDown:
            m_pInputTarget->m_keyDown.RemoveListenerWithEvent( id, key ); return;
        case InputType::kKeyUp:
            m_pInputTarget->m_keyUp.RemoveListenerWithEvent(id, key); return;
        case InputType::kKeyHeld:
            m_pInputTarget->m_keyHeld.RemoveListenerWithEvent(id, key); return;
        default:
            return;
        }
    }

    void InputManager::Update()
    {
        if (!m_inputCanOccur)
            return;

        for (KeyCode code : m_keyCodes)
        {
            m_pInputTarget->m_keyHeld.PushEvent(code);
            if constexpr (kLogInputData)
                BB_LOG(LogType::kMessage, "Key held : ", code);
        }
    }

    void InputManager::SwapInputTargetToInterface(UserInterface* pInterface)
    {
        m_pInputTarget = pInterface->GetInputTarget();
    }

    void InputManager::SwapInputToGame()
    {
        m_pInputTarget = &m_gameInputTarget;
    }

    void InputManager::EnableShowClickDebug( bool enable, float radius )
    {
        m_debugClick = enable;
        m_debugClickCircleRadius = radius;
    }

    void InputManager::PushMouseDown( float xPos, float yPos )
    {
        if ( m_currentMouseState.mouseDown )
            return;
        m_currentMouseState.mouseDown = true;
        m_currentMouseState.xScreenPos = xPos;
        m_currentMouseState.yScreenPos = yPos;

        m_pInputTarget->m_mouseEvent.PushEvent( MouseEvent::kMouseDown , m_currentMouseState );
    }

    void InputManager::PushMouseUp( float xPos, float yPos )
    {
        if ( !m_currentMouseState.mouseDown )
            return;
        m_currentMouseState.mouseDown = false;
        m_currentMouseState.xScreenPos = xPos;
        m_currentMouseState.xScreenPos = yPos;
        
        m_pInputTarget->m_mouseEvent.PushEvent( MouseEvent::kMouseUp , m_currentMouseState );
    }

    void InputManager::PushMouseMotion( float newX, float newY, float relX, float relY )
    {
        m_currentMouseState.xScreenPos = newX;
        m_currentMouseState.yScreenPos = newY;

        m_pInputTarget->m_mouseEvent.PushEvent( MouseEvent::kMouseMotion, m_currentMouseState );
        m_pInputTarget->m_relativeMouseMotionEvent.PushEvent( MouseEvent::kMouseMotion, relX, relY );
    }

    void InputManager::AddKeyDown( KeyCode key )
    {
        if ( m_keyCodes.contains( key ) )
            return;

        m_keyCodes.emplace( key );
        if ( !m_inputCanOccur )
            return;

        m_pInputTarget->m_keyDown.PushEvent( key );
        if constexpr ( kLogInputData )
            BB_LOG( LogType::kMessage, "Key down : ", key );
    }

    void InputManager::RemoveKeyDown( KeyCode key )
    {
        if ( !m_keyCodes.contains( key ) )
        {
            BB_LOG( LogType::kError, "Attempting to remove key that was not pressed" );
            return;
        }

        m_keyCodes.erase( key );
        if ( !m_inputCanOccur )
            return;

        m_pInputTarget->m_keyUp.PushEvent( key );
        if constexpr ( kLogInputData )
            BB_LOG( LogType::kMessage, "Key up : ", key );
    }

}

