#include "ofStarryPixel.h"
#include "ofStarryView.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

std::vector<ofColor> ofStarryPixel::s_Colors;
std::vector<float> ofStarryPixel::s_Correction = { 0.f, 0.f, 0.f };

// As seen in ofxPixelPusher::Pixel.h; PIXEL_STRING Color Correction
// But we want to have it locally and not relying on ofxPixelPusher to do the correction for us
const int ofStarryPixel::s_ColorCorrection = 0xFFE08C;
const int ofStarryPixel::s_ColorTemperature = 0xFFFFFF;

void ofStarryPixel::DefaultColors()
{
	// Make sure we do this only once
	if (s_Colors.size() > 0)
	{
		return;
	}

    s_Colors.push_back(ofColor(0, 0, 140));     // Dark blue
    s_Colors.push_back(ofColor(100, 150, 255)); // Light blue
    s_Colors.push_back(ofColor(255, 255, 200)); // Yellow
    s_Colors.push_back(ofColor(255, 215, 0));   // Orange
    s_Colors.push_back(ofColor(0, 0, 0));       // Black

    // As seen in ofxPixelPusher::Pixel.cpp
    s_Correction[0] = float((((s_ColorCorrection >> 16) & 0xFF)) * (((s_ColorTemperature >> 16) & 0xFF))) / 65535;
    s_Correction[1] = float((((s_ColorCorrection >> 8)  & 0xFF)) * (((s_ColorTemperature >> 8)  & 0xFF))) / 65535;
    s_Correction[2] = float((((s_ColorCorrection >> 0)  & 0xFF)) * (((s_ColorTemperature >> 0)  & 0xFF))) / 65535;

}

ofStarryPixel::ofStarryPixel()
    : m_Strip(0)
    , m_Index(0)
    , m_IsSelected(false)
{
    Reset();
}

ofStarryPixel::ofStarryPixel(int _strip, int _index)
    : m_Strip(_strip)
    , m_Index(_index)
    , m_IsSelected(false)
{ 
    Reset();
}

ofStarryPixel::~ofStarryPixel()
{
}

void ofStarryPixel::GetColor(const StarryColor _color)
{
    if (_color < StarryColor::Count)
    {
        m_ColorIndex = _color;
    }
}

const ofColor& ofStarryPixel::GetColor() const
{
    return s_Colors[m_ColorIndex];
}

ofColor ofStarryPixel::CorrectColor(const ofColor& _uncorrected)
{
    unsigned char red   = s_Correction[0] * _uncorrected.r;
    unsigned char green = s_Correction[1] * _uncorrected.g;
    unsigned char blue  = s_Correction[2] * _uncorrected.b;
    return ofColor(red, green, blue);
}

void ofStarryPixel::Draw(const ofStarryView* _view, const int _numberOfStrips)
{
    ofRectangle rectToDraw = _view->ToScreen(GetRect());
    ofRectangle rectWindow(0.f, 0.f, ofGetWindowWidth(), ofGetWindowHeight());
    bool isInWindow = rectToDraw.intersects(rectWindow);
    // If the pixel is visible draw it
    if (isInWindow)
    {
        // Draw the pixel
        ofSetColor(GetColor());
        ofFill();
        ofDrawRectangle(rectToDraw);
        // If the pixel is selected give it the red border
        if (IsSelected())
        {
            ofSetColor(ofColor::red);
            ofNoFill();
            ofSetLineWidth(5.f);
            ofDrawRectangle(rectToDraw);
        }
        // Adjust the pixel count color to the pixel color for clarity
        GetColor().getBrightness() > 140.f ? ofSetColor(ofColor::black) : ofSetColor(ofColor::white);
        // If the pixels are big put the count on each pixel
        if (_view->GetZoom() > 0.65)
        {
            ofDrawBitmapString(ofToString(GetIndex() + 1), rectToDraw.getBottomLeft());
        }
        // If the pixels are small count just the rows which are multiple of 20
        else if (((GetIndex() + 1) % 20 == 0) && (GetStrip() == (_numberOfStrips - 1)))
        {
            ofDrawBitmapString(ofToString(GetIndex() + 1), rectToDraw.getBottomLeft() + glm::vec3(0.f, 10.f, 0.f));
        }
    }
}
