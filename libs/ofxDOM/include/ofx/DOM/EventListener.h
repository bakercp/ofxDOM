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

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void addEventListener(EventType& event,
                          void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
                          bool useCapture = false,
                          int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void addEventListener(EventType& event,
                          void (ListenerClass::*listenerMethod)(ArgumentsType&),
                          bool useCapture = false,
                          int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void addEventListener(EventType& event,
                          bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
                          bool useCapture = false,
                          int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void addEventListener(EventType& event,
                          bool (ListenerClass::*listenerMethod)(ArgumentsType&),
                          bool useCapture = false,
                          int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofAddListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void removeEventListener(EventType& event,
                             void (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
                             bool useCapture = false,
                             int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }

    template <class EventType, typename ArgumentsType, class ListenerClass>
    void removeEventListener(EventType& event,
                             void (ListenerClass::*listenerMethod)(ArgumentsType&),
                             bool useCapture = false,
                             int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }


    template <class EventType, typename ArgumentsType, class ListenerClass>
    void removeEventListener(EventType& event,
                             bool (ListenerClass::*listenerMethod)(const void*, ArgumentsType&),
                             bool useCapture = false,
                             int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }


    template <class EventType, typename ArgumentsType, class ListenerClass>
    void removeEventListener(EventType& event,
                             bool (ListenerClass::*listenerMethod)(ArgumentsType&),
                             bool useCapture = false,
                             int priority = OF_EVENT_ORDER_AFTER_APP)
    {
        ofRemoveListener(event.event(useCapture), dynamic_cast<ListenerClass*>(this), listenerMethod, priority);
    }


    template <typename EventType>
    void handleEvent(EventType& e)
    {
        auto iter = _eventRegistry.find(e.type());

        if (iter != _eventRegistry.end())
        {
            DOMEvent<EventType>* event = dynamic_cast<DOMEvent<EventType>*>(iter->second);

            if (nullptr != event)
            {
                event->notify(e);
            }
            else
            {
                throw DOMException(DOMException::INVALID_STATE_ERROR);
            }
        }
        else
        {
            ofLogWarning("Element::handleEvent") << "Unhandled event type: " << e.type();
        }
    }

    bool isEventListener(const std::string& event, bool useCapture) const;

    virtual void onSetup();
    virtual void onUpdate();
    virtual void onDraw();
    virtual void onExit();

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

    DOMEvent<PointerEvent> pointerOver;
    DOMEvent<PointerEvent> pointerEnter;
    DOMEvent<PointerEvent> pointerDown;
    DOMEvent<PointerEvent> pointerMove;
    DOMEvent<PointerEvent> pointerUp;
    DOMEvent<PointerEvent> pointerCancel;
    DOMEvent<PointerEvent> pointerOut;
    DOMEvent<PointerEvent> pointerLeave;
    DOMEvent<PointerEvent> pointerScroll;

    DOMEvent<PointerCaptureEvent> gotPointerCapture;
    DOMEvent<PointerCaptureEvent> lostPointerCapture;

    DOMEvent<KeyboardEvent> keyDown;
    DOMEvent<KeyboardEvent> keyUp;

    DOMEvent<FocusEvent> blur;
    DOMEvent<FocusEvent> focusIn;
    DOMEvent<FocusEvent> focus;
    DOMEvent<FocusEvent> focusOut;


protected:
    std::unordered_map<std::string, BaseDOMEvent*> _eventRegistry;

};


} } // namespace ofx::DOM
