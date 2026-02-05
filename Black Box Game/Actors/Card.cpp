#include "Card.h"

#include <BlackBoxManager.h>
#include <Actors/ActorManager.h>
#include <Actors/Component.h> 
#include <Actors/EngineComponents/TransformComponent.h>
#include <Actors/EngineComponents/DragAndDropComponent.h>
#include <Actors/EngineComponents/SimpleRectDrawComponent.h>
#include <Actors/Collision/AABBColliderComponent.h>

#include <Resources/ResourceManager.h>

using namespace BlackBoxEngine;

CardLetteringComponent::CardLetteringComponent( Actor* pOwner )
    : Component( pOwner )
{
}

void CardLetteringComponent::SetHeaderPosition( float x, float y )
{
    if ( !m_pNameText )
        return;
    m_pNameText->SetPosition( {x, y} );
}

void CardLetteringComponent::SetBodyPosition( float x, float y )
{
    if ( !m_pBodyText )
        return;
    m_pBodyText->SetPosition( {x, y} );
}

void CardLetteringComponent::SetFont( const char* pFile, float size )
{
    m_pFont = ResourceManager::GetFont( pFile, size );
    if ( m_pBodyText )
        m_pBodyText = ResourceManager::GetText(m_pBodyText->GetString(), m_pFont );
    if ( m_pNameText )
        m_pNameText = ResourceManager::GetText( m_pNameText->GetString(), m_pFont );
}

void CardLetteringComponent::SetNameText( const char* pText )
{
    if ( !m_pFont )
    {
        BB_LOG( LogType::kWarning, "No font file set" );
        return;
    }
    m_pNameText = ResourceManager::GetText( pText, m_pFont );
}

void CardLetteringComponent::SetBodyText( const char* pText )
{
    if ( !m_pFont )
    {
        BB_LOG( LogType::kWarning, "No font file set" );
        return;
    }
    m_pBodyText = ResourceManager::GetText( pText, m_pFont );
}

void CardLetteringComponent::Render( )
{
    float x = m_pTransform->m_position.x;
    float y = m_pTransform->m_position.y;
    if ( m_pNameText )
        m_pTextRenderer->RenderText( m_pNameText, x, y );
    if ( m_pBodyText )
        m_pTextRenderer->RenderText( m_pBodyText, x, y );
}

void CardLetteringComponent::Start()
{
    m_pTextRenderer = BlackBoxManager::Get()->GetWindow()->GetTextRenderer();
    m_pTransform = m_pOwner->GetComponent<TransformComponent>();
    if ( !m_pTransform )
    {
        BB_LOG( LogType::kWarning, "No transform component found, making new one" );
        m_pTransform = m_pOwner->AddComponent<TransformComponent>();
    }
}

// Original/Test function to create initial card
const ActorManager::ActorPtr& MakeCard()
{
    auto& pActor = BlackBoxManager::Get()->m_pActorManager->NewActor();

    pActor->AddComponent<TransformComponent>()->m_position = {100, 200};

    auto* pCardRect = pActor->AddComponent<SimpleRectDrawComponent>();
    pCardRect->SetColor( ColorPresets::green );
    pCardRect->SetFilled( true );
    pCardRect->SetRect( {0,0,25,50} );

    auto* pDragAndDrop = pActor->AddComponent<DragAndDropComponent>();
    pDragAndDrop->SetClickDimensions( 25 , 50 );
    pDragAndDrop->ToggleDebugDrag();

    auto* pLettering = pActor->AddComponent<CardLetteringComponent>();
    pLettering->SetFont( "../Assets/Fonts/arial.ttf", 12 );
    pLettering->SetNameText( "Card Name" );
    pLettering->SetBodyText( "Card Body" );
    pLettering->SetBodyPosition( 0, 15 );

    return pActor;
}

