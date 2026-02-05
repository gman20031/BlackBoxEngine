#include "RenderingStructs.h"

using namespace BlackBoxEngine;

void StructHelpers::ConvertOffsetToAnchorPoint( float& newX, float& newY, BB_AnchorPoint anchor, const BB_FRectangle& rect )
{
    using enum BB_AnchorPoint;

    switch ( anchor )
    {
    case kTopLeft:  break;
    case kTopMiddle: 
        newX -= rect.w / 2;
        break;
    case kTopRight:
        newX -= rect.w;
        break;
    case kCenterLeft:
        newY -= rect.h / 2;
        break;
    case kCenterTrue:
        newX -= rect.w / 2;
        newY -= rect.h / 2;
        break;
    case kCenterRight:
        newX -= rect.w;
        newY -= rect.h / 2;
        break;
    case kBotLeft:
        newY -= rect.h;
        break;
    case kBotmiddle:
        newY -= rect.h;
        newX -= rect.w / 2;
        break;
    case kBotRight:
        newY -= rect.h;
        newX -= rect.w;
        break;
    default:
        break;
    }

    return;
}

bool BlackBoxEngine::StructHelpers::RectangleContainsPoint( const BB_FRectangle& rect, const BB_FPoint& point )
{                                      // point CANT BE CONTAINED IF:                       |
    if ( point.x < rect.x )            // point is more left than leftmost edge of box      |    
        return false;                  //                                                 | AAAAAAAAAAAAA
    if ( point.x > rect.x + rect.w )   // point is more right than rightmost edge of box    | A           A
        return false;                  //                                                 | A   B       A
    if ( point.y > rect.y + rect.h )   // point is farther down than bottommost edge of box | A           A
        return false;                  //                                                 | A           A
    if ( point.y < rect.y )            // point is higher up than topmost edge of box       | A           A
        return false;                  //                                                 | AAAAAAAAAAAAA
    return true;                       //                                                 |
}
