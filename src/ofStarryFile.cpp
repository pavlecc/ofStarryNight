#include "ofStarryFile.h"

#include "ofGraphics.h"
#include "ofVec3f.h"
#include "ofFileUtils.h"
#include "ofUtils.h"

const std::string ofStarryFile::s_FileName = "pixels";

ofStarryFile::ofStarryFile()
{
    m_LogFile.open(ofGetTimestampString("%Y%m%d_%H%M%S") + ".log", ofFile::WriteOnly);
}

ofStarryFile::~ofStarryFile()
{
    m_LogFile.close();
}

void ofStarryFile::LoadPixels(std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips)
{
    size_t numberOfStrips = _strips.size();
    size_t pixelsPerStrip = _strips.size() ? _strips.at(0).size() : 0;
    if (numberOfStrips == 0 || pixelsPerStrip == 0)
    {
        return;
    }

    m_TextFile.open(s_FileName + ".txt", ofFile::ReadOnly);
    ofBuffer buff = m_TextFile.readToBuffer();
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    for (auto line : buff.getLines())
    {
        pos = line.find(delimiter);
        size_t stripIdx = ofToInt(line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
        pos = line.find(delimiter);
        size_t pixelIdx = ofToInt(line.substr(0, pos));
        line.erase(0, pos + delimiter.length());
        int colorIndex = ofToInt(line);
        if ((stripIdx < numberOfStrips) && (pixelIdx < pixelsPerStrip))
        {
            _strips.at(stripIdx).at(pixelIdx)->GetColor(static_cast<StarryColor>(colorIndex));
        }
    }

    LogInfo("Pixels loaded");
}

void ofStarryFile::SavePixels(const std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips)
{
    m_TextFile.open(s_FileName + ".txt", ofFile::WriteOnly);

    for (auto strip : _strips)
    {
        for (auto pixel : strip)
        {
            m_TextFile << ofToString(pixel->GetStrip()) + "; " + ofToString(pixel->GetIndex()) + "; " + ofToString(pixel->GetColorIndex()) << std::endl;
        }
    }

    m_TextFile.close();

    LogInfo("Pixels saved");
}

void ofStarryFile::SavePixelsCanned(const std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips)
{
    size_t numberOfStrips = _strips.size();
    size_t pixelsPerStrip = _strips.size() ? _strips.at(0).size() : 0;
    if (numberOfStrips == 0 || pixelsPerStrip == 0)
    {
        return;
    }

    m_CannedFile.open(s_FileName + ".bin", ofFile::WriteOnly, true);

    // Allocate buffer - for each stip three bytes for pixel colors, strip number and timestamp
    static int numColors = 3;
    ofBuffer cannedBuffer;
    cannedBuffer.reserve(numberOfStrips * (pixelsPerStrip * numColors + sizeof(char) + sizeof(int)));

    for (int s = 0; s < numberOfStrips; ++s)
    {
        auto strip = _strips.at(s);
        // Timestamp
        int timeStamp = 0;
        for (int b = 0; b < sizeof(int); ++b)
        {
            char timeStampByte = (char)((timeStamp >> (b * 8)) & 0xffL);
            cannedBuffer.append(&timeStampByte, sizeof(char));
        }

        // Strip number
        cannedBuffer.append((char*)&s, sizeof(char));

        for (auto pixel : strip)
        {
            char colorByte;
            colorByte = (char)ofStarryPixel::CorrectColor(pixel->GetColor()).r;
            cannedBuffer.append(&colorByte, sizeof(char));
            colorByte = (char)ofStarryPixel::CorrectColor(pixel->GetColor()).g;
            cannedBuffer.append(&colorByte, sizeof(char));
            colorByte = (char)ofStarryPixel::CorrectColor(pixel->GetColor()).b;
            cannedBuffer.append(&colorByte, sizeof(char));

        }
    }

    cannedBuffer.writeTo(m_CannedFile);
    m_CannedFile.close();
    cannedBuffer.clear();

    LogInfo("Pixels saved canned");
}

void ofStarryFile::Log(const std::string& _string)
{
    m_Log.push_back(_string);
    if (m_Log.size() > 20) m_Log.erase(m_Log.begin());
    m_LogFile << "[" << ofGetTimestampString("%Y/%m/%d %H:%M:%S") << "]\t" << _string << std::endl;
}

void ofStarryFile::LogInfo(const std::string& _string)
{
    Log("[Info] " + _string);
}

void ofStarryFile::LogColor(const std::shared_ptr<ofStarryPixel>& _pixel)
{
    std::string color;

    switch (_pixel->GetColorIndex())
    {
    case StarryColor::DarkBlue:  color = "Dark Blue";  break;
    case StarryColor::LightBlue: color = "Light Blue"; break;
    case StarryColor::Yellow:    color = "Yellow";     break;
    case StarryColor::Orange:    color = "Orange";     break;
    case StarryColor::Black:     color = "Black";      break;
    default: return;
    }

    Log("[Color] " + ofToString(static_cast<char>(_pixel->GetStrip() + 'A')) + " " + ofToString(_pixel->GetIndex() + 1) + " " + color);
}

void ofStarryFile::LogSelect(const std::shared_ptr<ofStarryPixel>& _pixel, bool _selection)
{
    if (_selection)
    {
        Log("[Select] " + ofToString(static_cast<char>(_pixel->GetStrip() + 'A')) + " " + ofToString(_pixel->GetIndex() + 1));
    }
    else
    {
        Log("[Select] off");
    }
}

void ofStarryFile::DrawLog(float _x, float _y)
{
    ofSetColor(ofColor::white);
    ofVec2f drawLogPos(_x, _y);
    for (std::vector<std::string>::reverse_iterator ri = m_Log.rbegin(); ri != m_Log.rend(); ++ri)
    {
        ofDrawBitmapString(*ri, drawLogPos);
        drawLogPos.y -= 15.f;
    }
}
