#include "SimpleRectDrawComponent.h"

#include <SDL.h>

#include "../../BlackBoxManager.h"
#include "../../System/Log.h"
#include "../../Actors/Actor.h"

namespace BlackBoxEngine
{

void SimpleRectDrawComponent::Load( const XMLElementParser parser )
{
    parser.GetChildVariable( "isFilled", &m_filled);
    auto rect = parser.GetChildElement( "rect" );
    rect.GetChildVariable( "x", &m_rect.x );
    rect.GetChildVariable( "y", &m_rect.y );
    rect.GetChildVariable( "w", &m_rect.w );
    rect.GetChildVariable( "h", &m_rect.h);
    auto color = parser.GetChildElement( "color" );
    color.GetChildVariable( "r", &m_color.r );
    color.GetChildVariable( "g", &m_color.g );
    color.GetChildVariable( "b", &m_color.b );
    color.GetChildVariable( "a", &m_color.a );
}

void SimpleRectDrawComponent::Save( XMLElementParser parser )
{
    parser.NewChildVariable( "isFilled", m_filled );
    auto rect = parser.InsertNewChild( "rect" );
    rect.NewChildVariable( "x", m_rect.x );
    rect.NewChildVariable( "y", m_rect.y );
    rect.NewChildVariable( "w", m_rect.w );
    rect.NewChildVariable( "h", m_rect.h );
    auto color = parser.InsertNewChild( "color" );
    color.NewChildVariable( "r", m_color.r );
    color.NewChildVariable( "g", m_color.g );
    color.NewChildVariable( "b", m_color.b );
    color.NewChildVariable( "a", m_color.a );
}

void SimpleRectDrawComponent::Render()
{
    assert( m_pRenderer );
    if ( m_rect.w == 0 || m_rect.h == 0 )
    {
        BB_LOG( LogType::kError, "Width or height is zero" );
        return;
    }
    BB_FRectangle dest = m_rect;
    dest.x += m_pTransform->m_position.x;
    dest.y += m_pTransform->m_position.y;
    
    bool good = true;
    if ( m_filled )
        good = m_pRenderer->DrawRectGameFilled( dest );
    else
        good = m_pRenderer->DrawRectGame( dest );

    if ( !good )
        BB_LOG( LogType::kError, SDL_GetError() );
}

void SimpleRectDrawComponent::Start()
{
    m_pTransform = m_pOwner->GetComponent<TransformComponent>();
    if ( !m_pTransform )
    {
        BB_LOG( LogType::kWarning, "No transform component found, making new one" );
        m_pTransform = m_pOwner->AddComponent<TransformComponent>();
    }
    m_pRenderer = BlackBoxManager::Get()->GetWindow()->GetRenderer();
}

}
