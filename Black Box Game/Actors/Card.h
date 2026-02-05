#pragma once

#include <Actors/ActorManager.h>
#include <Graphics/Text Rendering/TextRenderer.h>
#include <Graphics/Text Rendering/Text.h>
#include <Actors/EngineComponents/TransformComponent.h>


class CardLetteringComponent : public BlackBoxEngine::Component
{
    GENERATE_ID( "CardLetteringComponent" );

    BlackBoxEngine::BB_TextRenderer* m_pTextRenderer = nullptr;
    BlackBoxEngine::TransformComponent* m_pTransform = nullptr;

    std::shared_ptr<BlackBoxEngine::BB_Font> m_pFont = nullptr;
    std::shared_ptr<BlackBoxEngine::BB_Text> m_pNameText = nullptr;
    std::shared_ptr<BlackBoxEngine::BB_Text> m_pBodyText = nullptr;

public:
    CardLetteringComponent( BlackBoxEngine::Actor* pOwner );

    std::shared_ptr<BlackBoxEngine::BB_Text> GetNameText() { return m_pNameText; }
    std::shared_ptr<BlackBoxEngine::BB_Text> GetBodyText() { return m_pBodyText; }

    void SetHeaderPosition( float x, float y );
    void SetBodyPosition( float x, float y );

    void SetFont( const char* pFile , float size);
    void SetNameText( const char* pText );
    void SetBodyText( const char* pText );

    virtual void Render();
    virtual void Start();
};

const BlackBoxEngine::ActorManager::ActorPtr& MakeCard();




