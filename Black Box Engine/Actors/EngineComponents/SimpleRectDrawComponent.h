#pragma once

#include "../Component.h"
#include "TransformComponent.h"
#include "../../Graphics/Renderer.h"

namespace BlackBoxEngine
{
    class SimpleRectDrawComponent final : public Component
    {
        GENERATE_ID( "SimpleRectDrawComponent" );

        BB_Renderer* m_pRenderer = nullptr;
        TransformComponent* m_pTransform = nullptr;
        
        BB_FRectangle m_rect{};
        ColorValue m_color = ColorPresets::green;
        bool m_filled = true;
    public:
        SimpleRectDrawComponent( Actor* pOwner ) : Component( pOwner ) { /*EMPTY*/ }

        void SetRect( const BB_FRectangle& rect ) { m_rect = rect; }
        void SetColor( const ColorValue& color ) { m_color = color; }
        void SetFilled( bool filled ) { m_filled = filled; }

        const BB_FRectangle& GetRect() const { return m_rect; }
        const ColorValue& GetColor() const { return m_color; }
        bool GetIsFilled() const { return m_filled; }

        virtual void Load( const XMLElementParser parser ) override;
        virtual void Save( XMLElementParser parser ) override;
        virtual void Render() override;
        virtual void Start() override;
    };
}