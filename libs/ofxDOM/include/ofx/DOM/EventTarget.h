// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
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


/// \brief A class representing an EventTarget.
///
/// EventTargets know how to handle events. This class is usually inherited
/// using the curiously-recurring template pattern.
///
/// \tparam EventTargetType The type of the Tvent target.
template<class EventTargetType>
class EventTarget
{
public:
    /// \brief Create an EventTarget.
    EventTarget();

    /// \brief Destroy the EventTarget.
    virtual ~EventTarget();

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

    /// \brief Dispatch the given event.
    ///
    /// This will return true if the default action for this event should be
    /// prevented.
    ///
    /// \param event The Event to dispatch.
    /// \tparam EventType The Event type to dispatch.
    /// \returns true iff one of the responders called Event::preventDefault().
    template<class EventType>
    bool dispatchEvent(EventType& event)
    {
        EventTargetType* target = dynamic_cast<EventTargetType*>(this);

        // Create the path from the target to the document.
        std::vector<EventTargetType*> targets;

        do
        {
            targets.push_back(target);
            target = target->parent();
        }
        while (target != nullptr);

        // Capture and Target phase.
        auto riter = targets.rbegin();

        while (riter != targets.rend())
        {
            event.setPhase(event.target() == *riter ? EventArgs::Phase::AT_TARGET : EventArgs::Phase::CAPTURING_PHASE);
            event.setCurrentTarget(*riter);

            (*riter)->handleEvent(event);

            if (event.isCancelled())
            {
                return !event.isDefaultPrevented();
            }

            ++riter;
        }

        // Bubble phase if needed.
        if (targets.size() > 1 && event.bubbles())
        {
            // Begin with the parent of the target element.
            auto bubbleIter = targets.begin() + 1;

            while (bubbleIter != targets.end())
            {
                event.setPhase(EventArgs::Phase::BUBBLING_PHASE);

                event.setCurrentTarget(*bubbleIter);
                (*bubbleIter)->handleEvent(event);

                if (event.isCancelled())
                {
                    return !event.isDefaultPrevented();
                }

                ++bubbleIter;
            }
        }

        return event.isDefaultPrevented();
    }


    /// \brief Handle the given event.
    ///
    /// This will return true if the default action for this event should be
    /// prevented.
    ///
    /// \param event The Event to dispatch.
    /// \tparam EventArgsType The EventArgs type to dispatch.
    /// \returns true iff one of the responders called Event::preventDefault().
    template <class EventArgsType>
    void handleEvent(EventArgsType& e)
    {
        auto iter = _eventRegistry.find(e.type());

        if (iter != _eventRegistry.end())
        {
            DOMEvent<EventArgsType>* _event = dynamic_cast<DOMEvent<EventArgsType>*>(iter->second);

            if (nullptr != _event)
            {
                _event->notify(e);
            }
            else
            {
                throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "EventTarget::handleEvent");
            }
        }
        else
        {
            throw DOMException(DOMException::UNREGISTERED_EVENT + ": " + "EventTarget::handleEvent");
        }
    }

    /// \brief Determine if the EventTarget is a listener for an event.
    /// \param event The Event name.
    /// \param useCapture true if the EventTarget listens for event during capture phase.
    /// \returns true if it is a listener to the named event and phase.
    bool isEventListener(const std::string& event, bool useCapture) const;

    virtual void onSetup()
    {
    }

    virtual void onUpdate()
    {
    }

    virtual void onDraw() const
    {
    }

    virtual void onExit()
    {
    }

    DOMEvent<PointerUIEventArgs> pointerOver;
    DOMEvent<PointerUIEventArgs> pointerEnter;
    DOMEvent<PointerUIEventArgs> pointerDown;
    DOMEvent<PointerUIEventArgs> pointerMove;
    DOMEvent<PointerUIEventArgs> pointerUp;
    DOMEvent<PointerUIEventArgs> pointerCancel;
    DOMEvent<PointerUIEventArgs> pointerOut;
    DOMEvent<PointerUIEventArgs> pointerLeave;
    DOMEvent<PointerUIEventArgs> pointerScroll;

    DOMEvent<PointerCaptureUIEventArgs> gotPointerCapture;
    DOMEvent<PointerCaptureUIEventArgs> lostPointerCapture;

    DOMEvent<KeyboardUIEventArgs> keyDown;
    DOMEvent<KeyboardUIEventArgs> keyUp;

    DOMEvent<FocusEventArgs> blur;
    DOMEvent<FocusEventArgs> focusIn;
    DOMEvent<FocusEventArgs> focus;
    DOMEvent<FocusEventArgs> focusOut;

    ofEvent<ElementEventArgs> addedTo;
    ofEvent<ElementEventArgs> removedFrom;
    ofEvent<ElementOrderEventArgs> reordered;

    ofEvent<ElementEventArgs> siblingAdded;
    ofEvent<ElementEventArgs> siblingRemoved;
    ofEvent<ElementOrderEventArgs> siblingReordered;

    ofEvent<ElementEventArgs> childAdded;
    ofEvent<ElementEventArgs> childRemoved;
    ofEvent<ElementOrderEventArgs> childReordered;

    ofEvent<MoveEventArgs> move;
    ofEvent<ResizeEventArgs> resize;

    ofEvent<AttributeEventArgs> attributeSet;
    ofEvent<AttributeEventArgs> attributeCleared;

    ofEvent<EnablerEventArgs> enabled;
    ofEvent<EnablerEventArgs> locked;
    ofEvent<EnablerEventArgs> hidden;

protected:
    std::unordered_map<std::string, BaseDOMEvent*> _eventRegistry;

};


template<class EventTargetType>
EventTarget<EventTargetType>::EventTarget()
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

        { KeyboardUIEventArgs::KEY_DOWN, &keyDown },
        { KeyboardUIEventArgs::KEY_UP, &keyUp }
    };
}


template<class EventTargetType>
EventTarget<EventTargetType>::~EventTarget()
{
}


template<class EventTargetType>
bool EventTarget<EventTargetType>::isEventListener(const std::string& event,
                                                   bool useCapture) const
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


} } // namespace ofx::DOM
