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


struct CapturedPointer
{
    std::size_t id;
    Point start;
    Point offset;
    Point position;
    Point velocity;
    uint64_t lastUpdate;
};


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
            throw DOMException(DOMException::UNREGISTERED_EVENT);
        }
    }

    bool isEventListener(const std::string& event, bool useCapture) const;

    virtual void onSetup();
    virtual void onUpdate();
    virtual void onDraw();
    virtual void onExit();

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

    ofEvent<ElementEvent> addedTo;
    ofEvent<ElementEvent> removedFrom;
    ofEvent<ElementOrderEvent> reordered;

    ofEvent<ElementEvent> siblingAdded;
    ofEvent<ElementEvent> siblingRemoved;
    ofEvent<ElementOrderEvent> siblingReordered;

    ofEvent<ElementEvent> childAdded;
    ofEvent<ElementEvent> childRemoved;
    ofEvent<ElementOrderEvent> childReordered;

    ofEvent<MoveEvent> move;
    ofEvent<ResizeEvent> resize;

    ofEvent<AttributeEvent> attributeSet;
    ofEvent<AttributeEvent> attributeCleared;

    ofEvent<EnablerEvent> enabled;
    ofEvent<EnablerEvent> locked;
    ofEvent<EnablerEvent> hidden;

protected:
    std::unordered_map<std::string, BaseDOMEvent*> _eventRegistry;

//    std::string

};


} } // namespace ofx::DOM
