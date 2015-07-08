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


#include "ofx/DOM/EventListener.h"
#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


EventListener::EventListener()
{
}


EventListener::~EventListener()
{
}


bool EventListener::isEventListener(const std::string& event, bool useCapture) const
{
    const auto& a = (useCapture ? _captureListeners : _bubbleListeners);
    return std::find(a.begin(), a.end(), event) != a.end();
}


void EventListener::addEventListener(const std::string& event, bool useCapture)
{
    removeEventListener(event, useCapture);
    (useCapture ? _captureListeners : _bubbleListeners).push_back(event);
}


void EventListener::removeEventListener(const std::string& event, bool useCapture)
{
auto& a = useCapture ? _captureListeners : _bubbleListeners;
a.erase(std::remove(a.begin(), a.end(), event), a.end());
}


void EventListener::handleEvent(PointerEvent& e)
{
    if (e.type() == PointerEventArgs::POINTER_OVER)
    {
        onPointerOver(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_ENTER)
    {
        onPointerEnter(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_DOWN)
    {
        onPointerDown(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_MOVE)
    {
        onPointerMove(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_UP)
    {
        onPointerUp(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_CANCEL)
    {
        onPointerCancel(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_OUT)
    {
        onPointerOut(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_LEAVE)
    {
        onPointerLeave(e);
    }
    else if (e.type() == PointerEventArgs::POINTER_SCROLL)
    {
        onPointerScroll(e);
    }
    else
    {
        ofLogWarning("Element::Element::handleEvent") << "Unhandled PointerEvent type: " << e.type();
    }
}


void EventListener::handleEvent(KeyboardEvent& e)
{
    if (e.type() == KeyboardEvent::KEY_DOWN)
    {
        onKeyDown(e);
    }
    else if (e.type() == KeyboardEvent::KEY_UP)
    {
        onKeyUp(e);
    }
    else
    {
        ofLogWarning("Element::Element::handleEvent") << "Unhandled KeyboardEvent type: " << e.type();
    }
}


void EventListener::onSetup()
{
}


void EventListener::onUpdate()
{
}


void EventListener::onDraw()
{
}


void EventListener::onExit()
{
}


void EventListener::onPointerOver(PointerEvent& e)
{
}


void EventListener::onPointerEnter(PointerEvent& e)
{
}


void EventListener::onPointerDown(PointerEvent& e)
{
}


void EventListener::onPointerMove(PointerEvent& e)
{
}


void EventListener::onPointerUp(PointerEvent& e)
{
}


void EventListener::onPointerCancel(PointerEvent& e)
{
}


void EventListener::onPointerOut(PointerEvent& e)
{
}


void EventListener::onPointerLeave(PointerEvent& e)
{
}


void EventListener::onPointerScroll(PointerEvent& e)
{
}


void EventListener::onGotPointerCapture(PointerEvent& e)
{
}


void EventListener::onLostPointerCapture(PointerEvent& e)
{
}


void EventListener::onKeyDown(KeyboardEvent& e)
{
}


void EventListener::onKeyUp(KeyboardEvent& e)
{
}


void EventListener::onBlur(FocusEvent& e)
{
}


void EventListener::onFocusIn(FocusEvent& e)
{
}


void EventListener::onFocus(FocusEvent& e)
{
}


void EventListener::onFocusOut(FocusEvent& e)
{
}


void EventListener::onParentSet(Element* element)
{
}


void EventListener::onChildAdded(Element* element)
{
}


void EventListener::onChildRemoved(Element* element)
{
}


void EventListener::onSiblingAdded(Element* element)
{
}


void EventListener::onSiblingRemoved(Element* element)
{
}


void EventListener::onChildMovedForward(Element* element)
{
}


void EventListener::onChildMovedToFront(Element* element)
{
}


void EventListener::onChildMovedBackward(Element* element)
{
}


void EventListener::onChildMovedToBack(Element* element)
{
}


void EventListener::onMovedForward()
{
}


void EventListener::onMovedToFront()
{
}


void EventListener::onMovedBackward()
{
}


void EventListener::onMovedToBack()
{
}


void EventListener::onAttributeSet(const std::string& name, const std::string& value)
{
}


void EventListener::onAttributeCleared(const std::string& name)
{
}


void EventListener::onMoved(float x, float y)
{
}

    
void EventListener::onResized(float width, float height)
{
}


void EventListener::onEnabled()
{
}


void EventListener::onDisabled()
{
}


void EventListener::onLocked()
{
}


void EventListener::onUnlocked()
{
}


void EventListener::onHidden()
{
}


void EventListener::onUnhidden()
{
}


} } // namespace ofx::DOM
