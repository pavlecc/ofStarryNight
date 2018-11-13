#include "ofStarryGui.h"

#include "ofStarryPixel.h"
#include "ofStarryUtils.h"

ofStarryGui::ofStarryGui()
{
}

ofStarryGui::~ofStarryGui()
{
}

ofxPanel* ofStarryGui::setup()
{
    ofxPanel::setup("Starry Night", "setup.xml", 2.f, ofGetWindowHeight() - 305.f);

    m_Buttons.setup("Main");
    m_Buttons.add(m_LoadButton.setup(PLATFORM_SWITCH("Load (ctrl + l)", "Load (cmd + l)")));
    m_Buttons.add(m_SaveButton.setup(PLATFORM_SWITCH("Save (ctrl + s)", "Load (cmd + s)")));
    m_Buttons.add(m_ConnectButton.setup(PLATFORM_SWITCH("Connect (ctrl + c)", "Load (cmd + c)")));
    add(&m_Buttons);

    m_PixelPusherGroup.setup("Pixel Pusher");
    m_PixelPusherGroup.add(m_PixelPusherStrips.setup("Stripes", 6));
    m_PixelPusherGroup.add(m_PixelPusherPixels.setup("Pixels", 300));
    m_PixelPusherGroup.add(m_PixelPusherStatus.setup("Status", "Disconnected"));
    add(&m_PixelPusherGroup);

    m_ColorGroup.setup("Colors");
    m_ColorGroup.add(m_ColorDarkBlue.setup("Dark Blue", ofStarryPixel::s_Colors[StarryColor::DarkBlue], ofColor(0, 0), ofColor(255, 255)));
    m_ColorGroup.add(m_ColorLightBlue.setup("Light Blue", ofStarryPixel::s_Colors[StarryColor::LightBlue], ofColor(0, 0), ofColor(255, 255)));
    m_ColorGroup.add(m_ColorYellow.setup("Yellow", ofStarryPixel::s_Colors[StarryColor::Yellow], ofColor(0, 0), ofColor(255, 255)));
    m_ColorGroup.add(m_ColorOrange.setup("Orange", ofStarryPixel::s_Colors[StarryColor::Orange], ofColor(0, 0), ofColor(255, 255)));
    add(&m_ColorGroup);

    return this;
}

void ofStarryGui::UpdateColors(std::vector<ofColor>& _colors)
{
    _colors[0] = m_ColorDarkBlue;
    _colors[1] = m_ColorLightBlue;
    _colors[2] = m_ColorYellow;
    _colors[3] = m_ColorOrange;
}

void ofStarryGui::SetStatus(std::string _status, ofColor _color /* = ofColor::black */)
{
    m_PixelPusherStatus = _status;
    m_PixelPusherStatus.setBackgroundColor(_color);
}
