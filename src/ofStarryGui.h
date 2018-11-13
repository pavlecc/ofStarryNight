#pragma once

#include "ofxGui.h"

class ofStarryGui : public ofxPanel
{
public:
    ofStarryGui();
    ~ofStarryGui();

    ofxPanel* setup();

    template<class ListenerClass, typename ListenerMethod>
    void SetupCallbacks(ListenerClass * listener, ListenerMethod _load, ListenerMethod _save, ListenerMethod _connect)
    {
        m_LoadButton.addListener(listener, _load);
        m_SaveButton.addListener(listener, _save);
        m_ConnectButton.addListener(listener, _connect);
    }

    void UpdateColors(std::vector<ofColor>& _colors);

    int GetStrips() { return m_PixelPusherStrips; }
    int GetPixels() { return m_PixelPusherPixels; }

    void SetStatus(std::string _status, ofColor _color = ofColor::black);

private:
    ofxGuiGroup m_Buttons;
    ofxButton m_LoadButton;
    ofxButton m_SaveButton;
    ofxButton m_ConnectButton;

    ofxGuiGroup m_PixelPusherGroup;
    ofxLabel    m_PixelPusherStatus;
    ofxIntField m_PixelPusherStrips;
    ofxIntField m_PixelPusherPixels;

    ofxGuiGroup    m_ColorGroup;
    ofxColorSlider m_ColorDarkBlue;
    ofxColorSlider m_ColorLightBlue;
    ofxColorSlider m_ColorYellow;
    ofxColorSlider m_ColorOrange;
};

