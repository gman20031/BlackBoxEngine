#pragma once

#include "TransformComponent.h"

#include "../Component.h"
#include "../../Input/InputManager.h"
#include "../../Input/MouseInputEvents.h"

namespace BlackBoxEngine
{

    class DragAndDropComponent final : public Component
    {
        GENERATE_ID( "DragAndDropComponent" );

        TransformComponent* m_pTransform = nullptr;

        BB_FRectangle m_clickRectangle {};
        InputManager::CallBackId m_callbackIds[3]{};
        BB_FPoint m_mouseGrabOffset {};
        bool m_isHeld = false;
        bool m_debugDraw = false;

    public:
        DragAndDropComponent( Actor* pOwner );
        virtual ~DragAndDropComponent();

        void SetClickDimensions( float width, float height );
        void SetOffset( float xOff, float yOff, BB_AnchorPoint anchorPoint = BB_AnchorPoint::kCenterTrue );
        const BB_FRectangle& GetClickAreaRectangle() const { return m_clickRectangle; }

        bool IsBeingHeld() const { return m_isHeld; }
        void ToggleDebugDrag();

        virtual void Start() override;
        virtual void Render() override;
    private:
        void OnMouseDown( const MouseState& state );
        void OnMouseUp( const MouseState& state );
        void OnMouseMoved( const MouseState& state );

        BB_FRectangle GetScreenRect() const;
    };

}
