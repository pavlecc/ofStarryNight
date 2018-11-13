#pragma once

#include "ofStarryPixel.h"
#include "ofFileUtils.h"

class ofStarryFile
{
public:
    ofStarryFile();
    ~ofStarryFile();

    void LoadPixels(std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips);
    void SavePixels(const std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips);
    void SavePixelsCanned(const std::vector<std::vector<std::shared_ptr<ofStarryPixel> > >& _strips);

    void Log(const std::string& _string);
    void LogInfo(const std::string& _string);
    void LogColor(const std::shared_ptr<ofStarryPixel>& _pixel);
    void LogSelect(const std::shared_ptr<ofStarryPixel>& _pixel, bool _selection);
    void DrawLog(float _x, float _y);

    static const std::string s_FileName;

private:
    ofFile m_TextFile;
    ofFile m_LogFile;
    ofFile m_CannedFile;

    std::vector<std::string> m_Log;


};

