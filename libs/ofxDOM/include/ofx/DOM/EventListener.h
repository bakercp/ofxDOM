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

#pragma once


#include <string>
#include <vector>
#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


class EventListener
{
public:
    EventListener();

    virtual ~EventListener();

    bool isEventListener(const std::string& event, bool useCapture = false) const;
    void addEventListener(const std::string& event, bool useCapture = false);
    void removeEventListener(const std::string& event, bool useCapture = false);

//    template<typename EventType>
//    bool dispatchEvent(EventType& event)
//    {
//        return false;
//    }

    void handleEvent(PointerEvent& e);
    void handleEvent(KeyboardEvent& e);
    void handleEvent(FocusEvent& e);

    virtual void onSetup();
    virtual void onUpdate();
    virtual void onDraw();
    virtual void onExit();

    virtual void onPointerOver(PointerEvent& e);
    virtual void onPointerEnter(PointerEvent& e);
    virtual void onPointerDown(PointerEvent& e);
    virtual void onPointerMove(PointerEvent& e);
    virtual void onPointerUp(PointerEvent& e);
    virtual void onPointerCancel(PointerEvent& e);
    virtual void onPointerOut(PointerEvent& e);
    virtual void onPointerLeave(PointerEvent& e);

    virtual void onPointerScroll(PointerEvent& e);

    virtual void onGotPointerCapture(PointerEvent& e);
    virtual void onLostPointerCapture(PointerEvent& e);

    virtual void onKeyDown(KeyboardEvent& e);
    virtual void onKeyUp(KeyboardEvent& e);

    virtual void onBlur(FocusEvent& e);
    virtual void onFocusIn(FocusEvent& e);
    virtual void onFocus(FocusEvent& e);
    virtual void onFocusOut(FocusEvent& e);

    virtual void onParentSet(Element* element);
    virtual void onChildAdded(Element* element);
    virtual void onChildRemoved(Element* element);
    virtual void onSiblingAdded(Element* element);
    virtual void onSiblingRemoved(Element* element);

    virtual void onChildMovedForward(Element* element);
    virtual void onChildMovedToFront(Element* element);
    virtual void onChildMovedBackward(Element* element);
    virtual void onChildMovedToBack(Element* element);

    virtual void onMovedForward();
    virtual void onMovedToFront();
    virtual void onMovedBackward();
    virtual void onMovedToBack();

    virtual void onAttributeSet(const std::string& name, const std::string& value);
    virtual void onAttributeCleared(const std::string& name);

    virtual void onMoved(float x, float y);
    virtual void onResized(float width, float height);

    virtual void onEnabled();
    virtual void onDisabled();

    virtual void onLocked();
    virtual void onUnlocked();

    virtual void onHidden();
    virtual void onUnhidden();

protected:
    std::vector<std::string> _bubbleListeners;
    std::vector<std::string> _captureListeners;

};


} } // namespace ofx::DOM
