#include "ofApp.h"
#include "ofStarryUtils.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("Starry Night");
    ofSetEscapeQuitsApp(false);

    m_MousePrevPos = ofVec3f(0.f, 0.f, 0.f);
    m_MouseDragged = false;

    ofStarryPixel::DefaultColors();

    m_File = std::make_unique<ofStarryFile>();

    m_Gui = std::make_unique<ofStarryGui>();
    m_Gui->setup();
    m_Gui->SetupCallbacks(this, &ofApp::LoadPixels, &ofApp::SavePixels, &ofApp::ConnectPixelPusher);

    m_View = std::make_unique<ofStarryView>();

    m_PixelPusher.reset();
    m_SelectedPixel.reset();
}

void ofApp::exit()
{
    m_View.reset();
    m_Gui.reset();
    m_File.reset();
}

void ofApp::LoadPixels()
{
    m_File->LoadPixels(m_Strips);
}

void ofApp::SavePixels()
{
    m_File->SavePixels(m_Strips);
    m_File->SavePixelsCanned(m_Strips);
}

void ofApp::ConnectPixelPusher()
{
    m_DiscoveryService = ofxPixelPusher::DiscoveryService::getInstance();
    auto callback = std::bind(&ofApp::OnPixelPusherConnected, this, std::placeholders::_1);
    m_DiscoveryService->addRegistrationCallback(callback);

    m_Gui->SetStatus("Connecting", ofColor::yellow);

    m_File->LogInfo("Connecting to Pixel Pusher");
}

void ofApp::OnPixelPusherConnected(std::shared_ptr<ofxPixelPusher::PixelPusher> _pusher)
{
    if (!m_PixelPusher.expired() && m_PixelPusher.lock()->getControllerId() == _pusher->getControllerId())
    {
        return;
    }

    _pusher->setPowerScale(1.0);
    _pusher->setColorCorrection(ofxPixelPusher::Pixel::UNCORRECTED);
    _pusher->setColorTemperature(ofxPixelPusher::Pixel::UNCORRECTED_TEMPERATURE);
    _pusher->setAntilog(false);
    
    m_PixelPusher = _pusher;

    m_Gui->SetStatus("Connected", ofColor::lightGreen);

    m_File->LogInfo("Pixel Pusher connected at address " + _pusher->getIpAddress());
}

void ofApp::SetupPixels(int _strips, int _pixels)
{
    if (m_NumberOfStrips == _strips && m_PixelsPerStrip == _pixels)
    {
        return;
    }

    // Setup Pixels

	// Size of the rectangle in the world is 30 units
    static const float rectDim = 30.f;
	// Distance between rectangles is 2 units
    static const float rectDist = 2.f;

    m_View->SetWorldSize((rectDim + rectDist) * (_pixels), (rectDim + rectDist) * (_strips));

    // Initialize the new 2D vector
    std::vector<std::vector<std::shared_ptr<ofStarryPixel> > > newStrips(_strips, std::vector<std::shared_ptr<ofStarryPixel> >(_pixels));

    // Copy everything from the old 2D vector until the new is full or old is done
    for (int s = 0; s < m_NumberOfStrips; ++s)
    {
        if (s < _strips)
        {
            for (int p = 0; p < m_PixelsPerStrip; ++p)
            {
                if (p < _pixels)
                {
                    newStrips.at(s).at(p) = m_Strips.at(s).at(p);
                }
            }
        }
    }
    m_Strips.clear();
    m_Strips = newStrips;

    // Add new elements if the new 2D vector is bigger
    float px = rectDist;
    float py = rectDist;
    for (int s = 0; s < _strips; ++s)
    {
        for (int p = 0; p < _pixels; ++p)
        {
            if (p >= m_PixelsPerStrip || s >= m_NumberOfStrips)
            {
                ofStarryPixel* sp = new ofStarryPixel(s, p);
                sp->SetRect(px, py, rectDim, rectDim);
                m_Strips.at(s).at(p) = std::shared_ptr<ofStarryPixel>(sp);
            }
            px += (rectDim + rectDist);
        }
        py += (rectDim + rectDist);
        px = rectDist;
    }

    // Update the number of strips and pixels
    m_NumberOfStrips = _strips;
    m_PixelsPerStrip = _pixels;
}

void ofApp::MoveSelectedPixel(std::shared_ptr<ofStarryPixel>& _selectedPixel, int _direction)
{
    int strip = _selectedPixel->GetStrip();
    int index = _selectedPixel->GetIndex();

    _selectedPixel->Deselect();
    if (_direction == OF_KEY_UP && strip > 0)
    {
        m_SelectedPixel = m_Strips.at(strip - 1).at(index);
    }
    if (_direction == OF_KEY_DOWN && strip < (m_NumberOfStrips - 1))
    {
        m_SelectedPixel = m_Strips.at(strip + 1).at(index);
    }
    if (_direction == OF_KEY_LEFT && index > 0)
    {
        m_SelectedPixel = m_Strips.at(strip).at(index - 1);
    }
    if (_direction == OF_KEY_RIGHT && index < (m_PixelsPerStrip - 1))
    {
        m_SelectedPixel = m_Strips.at(strip).at(index + 1);
    }
    _selectedPixel = m_SelectedPixel.lock();
    _selectedPixel->Select();

	// If the new selected pixel is out of the window, move the view so that the pixel is centered
    ofRectangle rectToDraw = m_View->ToScreen(m_SelectedPixel.lock()->GetRect());
    ofRectangle rectWindow(0.f, 0.f, ofGetWindowWidth(), ofGetWindowHeight());
    bool isRectInWindow = rectToDraw.intersects(rectWindow);
    if (!isRectInWindow)
    {
        if (rectToDraw.getLeft() > rectWindow.getRight())
        {
            m_View->Translate(ofVec3f(rectWindow.getWidth() / 2.f, 0.f, 0.f));
        }
        else if (rectToDraw.getRight() < rectWindow.getLeft())
        {
            m_View->Translate(ofVec3f(-rectWindow.getWidth() / 2.f, 0.f, 0.f));
        }
        else if (rectToDraw.getBottom() < rectWindow.getTop())
        {
            m_View->Translate(ofVec3f(0.f, -rectWindow.getHeight() / 2.f, 0.f));
        }
        else if (rectToDraw.getTop() > rectWindow.getBottom())
        {
            m_View->Translate(ofVec3f(0.f, rectWindow.getHeight() / 2.f, 0.f));
        }
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    // Update colors
    m_Gui->UpdateColors(ofStarryPixel::s_Colors);

    // Update number of strips and pixels
    SetupPixels(m_Gui->GetStrips(), m_Gui->GetPixels());

    if (!m_PixelPusher.expired())
    {
        auto pusher = m_PixelPusher.lock();
        if ((pusher->getNumberOfStrips() != m_NumberOfStrips) || (pusher->getPixelsPerStrip(0) != m_PixelsPerStrip))
        {
            m_Gui->SetStatus("Setup mismatch", ofColor::red);
        }
        for (int s = 0; s < m_NumberOfStrips; ++s)
        {
            auto pusherPixels = pusher->getStrip(s)->getPixels();
            for (int p = 0; p < m_PixelsPerStrip; ++p)
            {
                auto pixel = m_Strips.at(s).at(p);
                ofColor pixelColor = pixel->IsSelected() ? ofColor::red : pixel->GetColor();
                ofColor pixelCorrectedColor = ofStarryPixel::CorrectColor(pixelColor);
                pusherPixels[p]->setColor(pixelCorrectedColor.r, pixelCorrectedColor.g, pixelCorrectedColor.b);
            }
            pusher->setStripValues(s, pusherPixels);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(ofColor::darkGray);

    // Draw all pixels
    for (auto strip : m_Strips)
    {
        for (auto pixel : strip)
        {
            pixel->Draw(m_View.get(), m_NumberOfStrips);
        }
    }

    m_Gui->draw();

    // Logging in the window
    m_File->DrawLog(220.f, ofGetWindowHeight() - 5.f);
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs & key)
{
    if (key.hasModifier(PLATFORM_SWITCH(OF_KEY_CONTROL, OF_KEY_COMMAND)))
    {
        switch (key.keycode)
        {
        case 83: SavePixels(); break; // ctrl + s
        case 76: LoadPixels(); break; // ctrl + l
        case 67: ConnectPixelPusher(); break; // ctrl + c
        default: break;
        }
    }
    else
    {
        if (!m_SelectedPixel.expired())
        {
            auto selectedPixel = m_SelectedPixel.lock();
            switch (key.key)
            {
            case OF_KEY_LEFT: case OF_KEY_RIGHT: case OF_KEY_UP: case OF_KEY_DOWN:
                MoveSelectedPixel(selectedPixel, key.key);
                m_File->LogSelect(selectedPixel, true);
                break;
            case '1': case '2': case '3': case '4':
                selectedPixel->GetColor(static_cast<StarryColor>(key.key - '1'));
                m_File->LogColor(selectedPixel);
                break;
            case OF_KEY_DEL: case OF_KEY_BACKSPACE:
                selectedPixel->Reset();
                m_File->LogColor(selectedPixel);
                break;
            case OF_KEY_ESC:
                selectedPixel->Deselect();
                m_SelectedPixel.reset();
                m_File->LogSelect(selectedPixel, false);
                break;
            default:
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (button == OF_MOUSE_BUTTON_RIGHT)
    {
        m_MousePrevPos = ofVec3f(x, y, 0.f);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (m_MouseDragged)
    {
        m_MouseDragged = false;
        return;
    }

    if (button == OF_MOUSE_BUTTON_LEFT)
    {
        if (!m_SelectedPixel.expired())
        {
            auto selectedPixel = m_SelectedPixel.lock();
            ofRectangle selectedRect = m_View->ToScreen(selectedPixel->GetRect());
            if (selectedRect.inside(x, y))
            {
                selectedPixel->Deselect();
                m_SelectedPixel.reset();
                m_File->LogSelect(selectedPixel, false);
                return;
            }
            selectedPixel->Deselect();
        }

        m_SelectedPixel.reset();
        for (auto strip : m_Strips)
        {
            for (auto pixel : strip)
            {
                ofRectangle pixelRect = m_View->ToScreen(pixel->GetRect());
                if (pixelRect.inside(x, y))
                {
                    m_SelectedPixel = pixel;
                    auto selectedPixel = m_SelectedPixel.lock();
                    selectedPixel->Select();
                    m_File->LogSelect(selectedPixel, true);
                    break;
                }
            }
        }
    }
}

void ofApp::mouseScrolled(ofMouseEventArgs & mouse)
{
    std::weak_ptr<ofStarryPixel> hoveredPixel;
    for (auto strip : m_Strips)
    {
        for (auto pixel : strip)
        {
            ofRectangle pixelRect = m_View->ToScreen(pixel->GetRect());
            ofRectangle rectWindow(0.f, 0.f, ofGetWindowWidth(), ofGetWindowHeight());
            if (pixelRect.intersects(rectWindow) && pixelRect.inside(mouse.x, mouse.y))
            {
                hoveredPixel = pixel;
                break;
            }
        }
    }

    if (!hoveredPixel.expired())
    {
        bool zoomOut = mouse.scrollY < 0.f;
        m_View->UpdatePosition(zoomOut);
        ofVec3f mousePos(mouse.x, mouse.y, 0.f);
        ofVec3f hoveredPixelCenter = m_View->ToScreen(hoveredPixel.lock()->GetRect().getCenter());
        m_View->Translate(hoveredPixelCenter - mousePos);
    }
}

void ofApp::mouseDragged(ofMouseEventArgs & mouse)
{
    if (mouse.button == OF_MOUSE_BUTTON_RIGHT)
    {
        ofVec3f mouseNewPos(mouse.x, mouse.y, 0.f);
        m_View->Translate(m_MousePrevPos - mouseNewPos);
        m_MousePrevPos = mouseNewPos;
        m_MouseDragged = true;
    }
}
