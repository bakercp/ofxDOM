// =============================================================================
//
// Copyright (c) 2009-2015 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


namespace ofx {
namespace DOM {


Document::Document(): Element("document", 0, 0, ofGetWidth(), ofGetHeight())
{
    ofAddListener(ofEvents().setup, this, &Document::setup);
    ofAddListener(ofEvents().update, this, &Document::update);
    ofAddListener(ofEvents().draw, this, &Document::draw);
    ofAddListener(ofEvents().exit, this, &Document::exit);
    ofAddListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::min());

    ofAddListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::min());

    ofAddListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::min());
    ofAddListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::min());

    ofAddListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
}


Document::~Document()
{
    ofRemoveListener(ofEvents().setup, this, &Document::setup);
    ofRemoveListener(ofEvents().update, this, &Document::update);
    ofRemoveListener(ofEvents().draw, this, &Document::draw);
    ofRemoveListener(ofEvents().exit, this, &Document::exit);
    ofRemoveListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::min());

    ofRemoveListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::min());

    ofRemoveListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::min());
    ofRemoveListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::min());

    ofRemoveListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
}


void Document::setup(ofEventArgs& e)
{
}

void Document::update(ofEventArgs& e)
{
}


void Document::draw(ofEventArgs& e)
{
    Element::draw();
}


void Document::exit(ofEventArgs& e)
{
//    doExit();
}


void Document::windowResized(ofResizeEventArgs& e)
{
    setSize(e.width, e.height);
}


bool Document::fileDragEvent(ofDragInfo& e)
{
    return false;
}


bool Document::onKeyEvent(ofKeyEventArgs& e)
{
    return false;
}


bool Document::onPointerEvent(PointerEventArgs& e)
{
    if (_pointerDeviceFilter.find(e.getDeviceType()) != _pointerDeviceFilter.end())
    {
        return false;
    }

    auto capturedPointerIter = _capturedPointers.find(e.getID());

    if (capturedPointerIter != _capturedPointers.end())
    {
        PointerEvent event(e, this, capturedPointerIter->second);
        event.setPhase(Event::Phase::AT_TARGET);
        capturedPointerIter->second->handleEvent(event);

        // Release capture if appropriate.
        if (event.type() == PointerEventArgs::POINTER_UP ||
            event.type() == PointerEventArgs::POINTER_CANCEL)
        {
            // TODO Fire document-wide lost pointer capture event.
            _capturedPointers.erase(capturedPointerIter);
        }

        return true;
    }
    else
    {
        std::vector<Element*> path;

        recursiveHitTest(e.getEventType(),
                         e.getPoint(),
                         screenToLocal(e.getPoint()),
                         path);

        if (!path.empty())
        {
            PointerEvent event(e, this, nullptr);

            if (dispatchEvent(event, path))
            {
                // TODO call default action.
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}


void Document::addPointerDeviceFilter(const std::string& type)
{
    _pointerDeviceFilter.insert(type);
}


void Document::removePointerDeviceFilter(const std::string& type)
{
    _pointerDeviceFilter.erase(type);
}


bool Document::setPointerCapture(Element* element, PointerEventArgs::PointerID id)
{
    cout << "================================== Calling Document::setPointerCapture " << endl;

    if (nullptr != element)
    {
        if (_capturedPointers.find(id) == _capturedPointers.end())
        {
            _capturedPointers[id] = element;
            return true;
        }
        else
        {
            ofLogWarning("Document:setPointerCapture") << "Pointer id << " << std::get<0>(id) << std::get<1>(id) << std::get<2>(id) << " is already captured.";
            return false;
        }
    }
    else
    {
        ofLogError("Document:setPointerCapture") << "Target element is nullptr.";
        return false;
    }
}


bool Document::releasePointerCapture(PointerEventArgs::PointerID id)
{
    auto iter = _capturedPointers.find(id);

    if (iter != _capturedPointers.end())
    {
        _capturedPointers.erase(iter);
        return true;
    }
    else
    {
        return false;
    }
}


} } // namespace ofx::DOM
