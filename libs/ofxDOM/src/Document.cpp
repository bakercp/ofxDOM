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
    bool wasEventHandled = false;

    // If there's an event for it, it's active.
    _activePointers[e.id()] = e;

    auto capturedPointerIter = _capturedPointers.find(e.id());

    PointerEvent event(e, this, nullptr);

    if (capturedPointerIter != _capturedPointers.end())
    {
        Element* targetElement = capturedPointerIter->second;
        event._target = targetElement;
        event.setPhase(Event::Phase::AT_TARGET);
        targetElement->handleEvent(event);

        // Release capture if it wasn't explicitly released by the target.
        if (event.type() == PointerEventArgs::POINTER_UP ||
            event.type() == PointerEventArgs::POINTER_CANCEL)
        {
            releasePointerCapture(targetElement, e.id());
        }

        wasEventHandled = true;
    }
    else
    {
        std::vector<Element*> path;

        recursiveHitTest(e.eventType(),
                         e.point(),
                         screenToLocal(e.point()),
                         path);

        if (!path.empty())
        {
            event._target = path.back();

            if (dispatchEvent(event, path))
            {
                // TODO call default action.
            }

            wasEventHandled = true;
        }
        else
        {
            wasEventHandled = false;
        }
    }

    // Mouse events are always active.
    // Touch and pen events are no longer active if up or cancelled.
    if (e.deviceType() != PointerEventArgs::TYPE_MOUSE &&
        (event.type() == PointerEventArgs::POINTER_UP ||
         event.type() == PointerEventArgs::POINTER_CANCEL))
    {
        _activePointers.erase(e.id());
    }

    return wasEventHandled;
}


void Document::setPointerCapture(Element* element, std::size_t id)
{
    if (nullptr == element || this != element->document())
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR);
    }
    else
    {
        auto activePointersIter = _activePointers.find(id);

        if (activePointersIter == _activePointers.end())
        {
            throw DOMException(DOMException::INVALID_POINTER_ID);
        }
        else if (activePointersIter->second.buttons() > 0)
        {
            cout << "POINTER CAPTURE .... " << endl;
            _capturedPointers[id] = element;
            GotPointerEvent evt(id, this, element);
            std::vector<Element*> path = { element };
            dispatchEvent(evt, path);
        }
        else
        {
            cout << "POINTER CAPTURE FAILED.... " << endl;
        }
    }
}


void Document::releasePointerCapture(Element* element, std::size_t id)
{
    // TODO: release if the target element is removed from the document.

    if (nullptr == element)
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR);
    }
    else
    {
        auto activePointersIter = _activePointers.find(id);

        if (activePointersIter == _activePointers.end())
        {
            throw DOMException(DOMException::INVALID_POINTER_ID);
        }
        else
        {
            auto iter = _capturedPointers.find(id);

            if (iter != _capturedPointers.end())
            {
                _capturedPointers.erase(iter);
                LostPointerEvent evt(id, this, element);
                std::vector<Element*> path = { element };
                dispatchEvent(evt, path);
            }
        }
    }
}


} } // namespace ofx::DOM
