#include "DragAndDropComponent.h"

#include "../../Input/InputManager.h"
#include "../../System/Debugging.h"
#include "../../BlackBoxManager.h"

using namespace BlackBoxEngine;

DragAndDropComponent::DragAndDropComponent( Actor* pOwner )
    : Component(pOwner)
{
    
}

BlackBoxEngine::DragAndDropComponent::~DragAndDropComponent()
{
    auto* pInput = BlackBoxManager::Get()->m_pInputManager;
    for ( auto& id : m_callbackIds )
        pInput->UnsubscribeInput( id , InputManager::InputType::kMouse );
}

void DragAndDropComponent::SetClickDimensions( float width, float height )
{
    m_clickRectangle.w = width;
    m_clickRectangle.h = height;
}

void DragAndDropComponent::SetOffset( float xOff, float yOff, BB_AnchorPoint anchorPoint )
{
    StructHelpers::ConvertOffsetToAnchorPoint( xOff, yOff, anchorPoint, m_clickRectangle );
    m_clickRectangle.x = xOff;
    m_clickRectangle.y = yOff;
}

void BlackBoxEngine::DragAndDropComponent::ToggleDebugDrag()
{
    m_debugDraw = !m_debugDraw;
}

void DragAndDropComponent::Start()
{
    m_pTransform = m_pOwner->GetComponent<TransformComponent>();
    if ( !m_pTransform )
    {
        BB_LOG( LogType::kWarning, "No transform component found, making new one" );
        m_pTransform = m_pOwner->AddComponent<TransformComponent>();
    }

    auto* pInput = BlackBoxManager::Get()->m_pInputManager;
    m_callbackIds[0] = pInput->SubscribeToMouse( MouseEvent::kMouseDown, [this]( const MouseState& state ) { OnMouseDown( state ); } );
    m_callbackIds[1] = pInput->SubscribeToMouse( MouseEvent::kMouseUp,  [this]( const MouseState& state ) { OnMouseUp( state ); } );
    m_callbackIds[2] = pInput->SubscribeToMouse( MouseEvent::kMouseMotion, [this]( const MouseState& state ) { OnMouseMoved( state ); } );
}

void BlackBoxEngine::DragAndDropComponent::Render()
{
    if ( !kDebug || !m_debugDraw )
        return;

    auto rect = m_clickRectangle;
    rect.x += m_pTransform->m_position.x;
    rect.y += m_pTransform->m_position.y;

    BlackBoxManager::Get()->GetWindow()->GetRenderer()->DrawRectGame(
        rect, ColorPresets::red
    );
}

void BlackBoxEngine::DragAndDropComponent::OnMouseDown( const MouseState& state )
{
    auto rect = GetScreenRect();
    if ( !StructHelpers::RectangleContainsPoint( rect, {state.xScreenPos, state.yScreenPos} ) )
        return;
    BB_LOG( LogType::kMessage, "Mouse clicked inside drag and drop component\n" );
    m_mouseGrabOffset.x = (m_clickRectangle.x + m_pTransform->m_position.x) - state.xScreenPos;
    m_mouseGrabOffset.y = (m_clickRectangle.y + m_pTransform->m_position.y) - state.yScreenPos;
    m_isHeld = true;
}

void BlackBoxEngine::DragAndDropComponent::OnMouseUp( [[maybe_unused]]const MouseState& state )
{
    m_isHeld = false;
}

void BlackBoxEngine::DragAndDropComponent::OnMouseMoved( const MouseState& state )
{
    if ( !m_isHeld )
        return;
    m_pTransform->m_position.x = m_mouseGrabOffset.x += state.xScreenPos;
    m_pTransform->m_position.y = m_mouseGrabOffset.y += state.yScreenPos;
}

BB_FRectangle BlackBoxEngine::DragAndDropComponent::GetScreenRect() const
{
    BB_FRectangle rect = m_clickRectangle;
    rect.x += m_pTransform->m_position.x;
    rect.y += m_pTransform->m_position.y;
    
    auto* pCamera = BlackBoxManager::Get()->m_pMainCamera;
    rect = pCamera->ConvertGameToScreen( rect );
    rect = pCamera->ZoomScreenRect( rect );

    return rect;
}
