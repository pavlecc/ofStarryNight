#pragma once

#include "ofMain.h"
#include "DiscoveryService.h"

#include "ofStarryView.h"
#include "ofStarryGui.h"
#include "ofStarryPixel.h"
#include "ofStarryFile.h"

class ofApp : public ofBaseApp{

public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(ofKeyEventArgs & key) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(ofMouseEventArgs & mouse) override;
    void mouseDragged(ofMouseEventArgs & mouse) override;

private:
    void LoadPixels();
    void SavePixels();
    void ConnectPixelPusher();

    void OnPixelPusherConnected(std::shared_ptr<ofxPixelPusher::PixelPusher> _pusher);
    void SetupPixels(int _strips, int _pixels);
    void MoveSelectedPixel(std::shared_ptr<ofStarryPixel>& _selectedPixel, int _direction);

    std::shared_ptr<ofxPixelPusher::DiscoveryService> m_DiscoveryService;
    std::weak_ptr<ofxPixelPusher::PixelPusher> m_PixelPusher;
    std::vector<std::vector<std::shared_ptr<ofStarryPixel> > > m_Strips;
    std::weak_ptr<ofStarryPixel> m_SelectedPixel;

    int m_NumberOfStrips;
    int m_PixelsPerStrip;

    std::unique_ptr<ofStarryGui>  m_Gui;
    std::unique_ptr<ofStarryView> m_View;
    std::unique_ptr<ofStarryFile> m_File;

    ofVec3f m_MousePrevPos;
    bool m_MouseDragged;
};
