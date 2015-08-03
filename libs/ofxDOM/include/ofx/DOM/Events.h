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
#include <type_traits>
#include <utility>
#include <ctime>
#include "Poco/Any.h"
#include "ofEvents.h"
#include "ofx/PointerEvents.h"
#include "ofx/DOM/Exceptions.h"
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Element;


/// \brief The base type describing a named Layout Event.
///
/// \sa http://www.w3.org/TR/DOM-Level-3-Events/
class Event
{
public:
    /// \brief Create an Event with a type.
    /// \param type The event type string.
    /// \param source The source of the event.
    /// \param target The target element.
    /// \param bubbles True iff the argument bubbles after AT_TARGET phase.
    /// \param cancelable True iff the event can be cancelled by a listener.
    /// \param timestamp The timestamp of the event.
    Event(const std::string& type,
          Element* source,
          Element* target,
          bool bubbles,
          bool cancelable,
          uint64_t timestamp);

    /// \brief Destroy the event type.
    virtual ~Event();

    /// \brief Get the event type.
    /// \returns the event type string.
    const std::string& type() const;

    enum class Phase
    {
        /// \brief Events not currently dispatched are in this phase.
        NONE,
        /// \brief When an event is dispatched to an object that participates in a tree it will be in this phase before it reaches its target attribute value.
        CAPTURING_PHASE,
        /// \brief When an event is dispatched it will be in this phase on its target attribute value.
        AT_TARGET,
        /// \brief When an event is dispatched to an object that participates in a tree it will be in this phase after it reaches its target attribute value.
        BUBBLING_PHASE
    };


    /// \brief Stop the propagation of the event.
    ///
    /// The stopPropagation method is used prevent further propagation of an
    /// event during event flow. If this method is called by any EventListener
    /// the event will cease propagating through the tree. The event will
    /// complete dispatch to all listeners on the current EventTarget before
    /// event flow stops. This method may be used during any stage of event
    /// flow.
    void stopPropagation();

    // \brief Stop the propagation of the event, including all other events at the target.
    //
    // If successful this event throws ofEventAttendedException(). This exception
    // should be handled gracefully by the ofEvent dispatching this event.
    //
    // The same effect can be achieved by returning "true" from any event
    // callback with a boolean return type.
    //
    // \throws ofEventAttendedException if the event is cancelable.
    void stopImmediatePropagation();

    /// \brief Prevent any default actions associated with the event.
    ///
    /// For some events, there are default actions.  Calling preventDefault will
    /// tell the event dispatcher that it should not call the default action
    /// after the event has been dispatched.  This is different from
    /// stopPropagation.  Calling stopPropagation does not prevent the default
    /// action from taking place.  To stop propagation and prevent the default,
    /// both methods must be called.
    void preventDefault();

    bool isCancelled() const;

    bool isDefaultPrevented() const;

    void setPhase(Phase phase);

    Phase getPhase() const;

    /// \brief Determine if the event has a bubbling phase.
    /// \returns true iff the event should bubble.
    bool bubbles() const;

    bool isCancelable() const;

    Element* source() const;

    Element* target() const;

    Element* relatedTarget() const;

    Element* getCurrentTarget() const;

    void setCurrentTarget(Element* target);

protected:
    /// \brief The name of the event (case-insensitive).
    std::string _type;

    /// \brief The source of the event.
    Element* _source = nullptr;

    /// \brief The event's target.
    Element* _target = nullptr;

    Element* _relatedTarget = nullptr;

    /// \brief Used to indicate whether or not an event is a bubbling event.
    /// If the event can bubble the value is true, else the value is false.
    bool _bubbles = true;

    /// \brief Used to indicate whether propgation was stopped.  The currentTarget
    /// will indicate which target stopped propagation.
    bool _cancelable = true;

    /// \brief Used to indicated
    bool _defaultPrevented = false;

    /// \brief Used to indicate the EventTarget whose EventListeners are currently being processed.
    /// This is particularly useful during capturing and bubbling.
    Element* _currentTaget  = nullptr;

    /// \brief Used to indicate which phase of event flow is currently being evaluated.
    Phase _phase = Phase::NONE;

    /// \brief Used to indicate if an event is canceled.
    bool _canceled = false;

    /// \brief Used to specify the time (in milliseconds relative to the epoch) at which the event was created.
    /// Due to the fact that some systems may not provide this information the
    /// value of timeStamp may be not available for all events. When not
    /// available, a value of 0 will be returned. Examples of epoch time are
    /// the time of the system start or 0:0:0 UTC 1st January 1970.
    uint64_t _timestamp = 0;

    friend class Document;
};


/// can we handle this named ui event at this coordinate?
class UIEvent: public Event
{
public:
    using Event::Event;
    
    virtual ~UIEvent()
    {
    }

};


class PointerCaptureEvent: public UIEvent
{
public:
    PointerCaptureEvent(std::size_t id, bool wasCaptured, Element* source, Element* target);
    virtual ~PointerCaptureEvent();

    std::size_t id() const;

private:
    std::size_t _id;

};


class PointerEvent: public UIEvent
{
public:
    PointerEvent(const PointerEventArgs& args, Element* source, Element* target);
    virtual ~PointerEvent();

    const PointerEventArgs& pointer() const;

protected:
    static bool eventBubbles(const std::string& event);
    static bool eventCancelable(const std::string& event);

    PointerEventArgs _pointer;
    
};


class KeyboardEvent: public UIEvent
{
public:
    KeyboardEvent(const ofKeyEventArgs& args, Element* source, Element* target);
    virtual ~KeyboardEvent();

    const ofKeyEventArgs& key() const;

    static const std::string KEY_DOWN;
    static const std::string KEY_UP;

protected:
    ofKeyEventArgs _key;

};


/// \sa http://www.w3.org/TR/DOM-Level-3-Events/#event-type-focus
class FocusEvent: public Event
{
public:
    FocusEvent(const std::string& type,
               Element* source,
               Element* target,
               Element* relatedTarget = nullptr);

    virtual ~FocusEvent();

    static const std::string FOCUS_IN;
    static const std::string FOCUS;
    static const std::string FOCUS_OUT;
    static const std::string BLUR;

};


class DragDropEvent: public Event
{
public:
    
};


class AbstractDOMEvent {
public:
    virtual ~AbstractDOMEvent()
    {
    }
};


class BaseDOMEvent: public AbstractDOMEvent
{
public:
    virtual ~BaseDOMEvent()
    {
    }

    virtual bool hasListeners() const = 0;

};


template<typename EventArgs>
class DOMEvent: public BaseDOMEvent
{
public:
    virtual ~DOMEvent()
    {
    }

    ofEvent<EventArgs>& event(bool useCapture = false)
    {
        return useCapture ? capture : bubble;
    }

    bool hasListeners() const
    {
        return bubble.size() > 0 || capture.size() > 0;
    }

    void notify(EventArgs& e)
    {
        switch (e.getPhase())
        {
            case Event::Phase::NONE:
                throw DOMException(DOMException::INVALID_STATE_ERROR);
            case Event::Phase::CAPTURING_PHASE:
                capture.notify(e.source(), e);
                return;
            case Event::Phase::AT_TARGET:
                capture.notify(e.source(), e);
                bubble.notify(e.source(), e);
                return;
            case Event::Phase::BUBBLING_PHASE:
                capture.notify(e.source(), e);
                return;
        }
    }

private:
    ofEvent<EventArgs> bubble;
    ofEvent<EventArgs> capture;

};


class MoveEvent: public ofEventArgs
{
public:
    MoveEvent(const Position& position);
    virtual ~MoveEvent();

    const Position& position() const;

protected:
    Position _position;

};


class ResizeEvent: public ofEventArgs
{
public:
    ResizeEvent(const Geometry& geometry);
    virtual ~ResizeEvent();

    const Geometry& geometry() const;

protected:
    Geometry _geometry;

};


class AttributeEvent: public ofEventArgs
{
public:
    AttributeEvent(const std::string& key, const Poco::Any& value = Poco::Any());
    virtual ~AttributeEvent();

    const std::string& key() const;
    const Poco::Any& value() const;

protected:
    std::string _key;
    Poco::Any _value;
    
};


class EnablerEvent: public ofEventArgs
{
public:
    EnablerEvent(bool value);
    virtual ~EnablerEvent();
    
    bool value() const;
    
protected:
    bool _value;
    
};


class ElementEvent: public ofEventArgs
{
public:
    ElementEvent(Element* element);
    virtual ~ElementEvent();

    Element* element();

protected:
    Element* _element;

};


class ElementOrderEvent: public ElementEvent
{
public:
    enum Type
    {
        FORWARD,
        BACKWARD,
        TO_FRONT,
        TO_BACK
    };

    ElementOrderEvent(Element* element, Type type);
    virtual ~ElementOrderEvent();

    Type type() const;

protected:
    Type _type;

};


} } // namespace ofx::DOM
