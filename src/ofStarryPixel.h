#pragma once

#include "ofRectangle.h"
#include "ofColor.h"

class ofStarryView;

enum StarryColor : int
{
    DarkBlue,
    LightBlue,
    Yellow,
    Orange,
    Black,
    Count
};

class ofStarryPixel
{
public:

    ofStarryPixel();
    ofStarryPixel(int _strip, int _index);
    ~ofStarryPixel();

    static std::vector<ofColor> s_Colors;
    static void DefaultColors();

    static std::vector<float> s_Correction;
    static const int s_ColorCorrection;
    static const int s_ColorTemperature;
    static ofColor CorrectColor(const ofColor& _uncorrected);

    inline int GetStrip() const { return m_Strip; }
    inline const int GetIndex() const { return m_Index; }

    inline int GetColorIndex() const { return m_ColorIndex; }
    void GetColor(const StarryColor _color);
    const ofColor& GetColor() const;
    inline void Reset() { m_ColorIndex = StarryColor::Black; }

    inline bool IsSelected() { return m_IsSelected; }
    inline void Select() { m_IsSelected = true; }
    inline void Deselect() { m_IsSelected = false; }

    inline const ofRectangle& GetRect() const { return m_Rect; }
    inline void SetRect(float _x, float _y, float _w, float _h) { m_Rect = ofRectangle(_x, _y, _w, _h); }

    void Draw(const ofStarryView* _view, const int _numberOfStrips);

private:
    int m_Strip;
    int m_Index;
    int m_ColorIndex;
    bool m_IsSelected;
    ofRectangle m_Rect;
};

