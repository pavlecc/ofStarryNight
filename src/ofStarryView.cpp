#include "ofStarryView.h"

#include "ofAppRunner.h"
#include "ofMath.h"

// We can zoom out 10x
float ofStarryView::s_ZoomMin = 0.f;
float ofStarryView::s_ZoomMax = 10.f;
float ofStarryView::s_ZoomInc = 1.f;

ofStarryView::ofStarryView()
    : m_Position(0.f, 0.f, 0.f)
    , m_WorldSize(0.f, 0.f)
{
}

ofStarryView::~ofStarryView()
{
}

void ofStarryView::UpdatePosition(bool _zoomOut)
{
    float newZoom = m_Position.z + (_zoomOut ? s_ZoomInc : -s_ZoomInc);
    m_Position.z = ofClamp(newZoom, s_ZoomMin, s_ZoomMax - 1.f);
    m_Position.x *= GetZoom();
    m_Position.y *= GetZoom();
    ClampPosition();
}

void ofStarryView::ClampPosition()
{
    if ((m_WorldSize.x  * GetZoom()) > ofGetWindowWidth())
    {
        m_Position.x = ofClamp(m_Position.x, 0.f, ABS(m_WorldSize.x * GetZoom() - ofGetWindowWidth()));
    }
    else
    {
        m_Position.x = ofClamp(m_Position.x, -ABS(m_WorldSize.x * GetZoom() - ofGetWindowWidth()), 0.f);
    }
    if ((m_WorldSize.y * GetZoom()) > ofGetWindowHeight())
    {
        m_Position.y = ofClamp(m_Position.y, 0.f, ABS(m_WorldSize.y * GetZoom() - ofGetWindowHeight()));
    }
    else
    {
        m_Position.y = ofClamp(m_Position.y, -ABS(m_WorldSize.y * GetZoom() - ofGetWindowHeight()), 0.f);
    }
}

ofVec3f ofStarryView::ToScreen(const ofVec3f& _pos) const
{
    ofVec3f screenPos = _pos * GetZoom() - m_Position;
    // The screen is flat
    screenPos.z = 0.f;
    return screenPos;
}

ofRectangle ofStarryView::ToScreen(const ofRectangle& _rect) const
{
    ofRectangle screenRect = _rect;
    screenRect.scale(GetZoom());
    screenRect.setPosition(ToScreen(screenRect.getPosition()));
    return screenRect;
}

ofVec3f ofStarryView::ToWorld(const ofVec3f& _pos) const
{
    ofVec3f worldPos = (_pos + m_Position) / GetZoom();
    // This world is flat
    worldPos.z = 0.f;
    return worldPos;
}

ofRectangle ofStarryView::ToWorld(const ofRectangle& _rect) const
{
    ofRectangle worldRect = _rect;
    worldRect.scale(1.f / GetZoom());
    worldRect.setPosition(ToWorld(worldRect.getPosition()));
    return worldRect;
}
