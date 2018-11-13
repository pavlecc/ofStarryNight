#pragma once

#include "ofVec3f.h"
#include "ofRectangle.h"

class ofStarryView
{
public:
    ofStarryView();
    ~ofStarryView();

    inline ofVec2f GetWorldSize() const { return m_WorldSize; }
    inline void SetWorldSize(float _sizeX, float _sizeY) { m_WorldSize.x = _sizeX; m_WorldSize.y = _sizeY; }

    inline ofVec3f GetPosition() const { return m_Position; }
    inline void SetPosition(ofVec3f _pos) { m_Position = _pos; ClampPosition(); }
    inline void Translate(ofVec3f _offset) { m_Position += _offset; ClampPosition(); }

    void UpdatePosition(bool _zoomOut);
    inline float GetZoom() const { return (s_ZoomMax - m_Position.z) / (s_ZoomMax - s_ZoomMin); }

    ofVec3f ToScreen(const ofVec3f& _pos) const;
    ofRectangle ToScreen(const ofRectangle& _rect) const;
    ofVec3f ToWorld(const ofVec3f& _pos) const;
    ofRectangle ToWorld(const ofRectangle& _rect) const;

private:
    ofVec3f m_Position;
    ofVec2f m_WorldSize;

    static float s_ZoomMin;
    static float s_ZoomMax;
    static float s_ZoomInc;

    //  View always needs to have the World visible
    void ClampPosition();
};
