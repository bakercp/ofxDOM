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
#include "ofx/DOM/Exceptions.h"


namespace ofx {
namespace DOM {


EventListener::EventListener()
{
    _eventRegistry = {
        { PointerEventArgs::POINTER_OVER, &pointerOver },
        { PointerEventArgs::POINTER_ENTER, &pointerEnter },
        { PointerEventArgs::POINTER_DOWN, &pointerDown },
        { PointerEventArgs::POINTER_MOVE, &pointerMove },
        { PointerEventArgs::POINTER_UP, &pointerUp },
        { PointerEventArgs::POINTER_CANCEL, &pointerCancel },
        { PointerEventArgs::POINTER_OUT, &pointerOut },
        { PointerEventArgs::POINTER_LEAVE, &pointerLeave },
        { PointerEventArgs::POINTER_SCROLL, &pointerScroll },

        { PointerEventArgs::GOT_POINTER_CAPTURE, &gotPointerCapture },
        { PointerEventArgs::LOST_POINTER_CAPTURE, &lostPointerCapture },

        { KeyboardEvent::KEY_DOWN, &keyDown },
        { KeyboardEvent::KEY_UP, &keyUp }
    };
}


EventListener::~EventListener()
{
}


bool EventListener::isEventListener(const std::string& event, bool useCapture) const
{
    auto iter = _eventRegistry.find(event);

    if (iter != _eventRegistry.end())
    {
        return iter->second->hasListeners();
    }
    else
    {
        return false;
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


} } // namespace ofx::DOM
