// =============================================================================
//
// Copyright (c) 2010-2015 Christopher Baker <http://christopherbaker.net>
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


#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


Event::Event(const std::string& type,
             Element* source,
             Element* target,
             bool bubbles,
             bool cancelable,
             uint64_t timestamp):
    _type(type),
    _source(source),
    _target(target),
    _bubbles(bubbles),
    _cancelable(cancelable),
    _timestamp(timestamp)
{
}


Event::~Event()
{
}


const std::string& Event::type() const
{
    return _type;
}


void Event::stopPropagation()
{
    if (_cancelable)
    {
        _canceled = true;
    }
}


void Event::stopImmediatePropagation()
{
    if (_cancelable)
    {
        _canceled = true;
        throw ofEventAttendedException();
    }
}


void Event::preventDefault()
{
    _defaultPrevented = true;
}


bool Event::isCancelled() const
{
    return _canceled;
}


bool Event::isDefaultPrevented() const
{
    return _defaultPrevented;
}


void Event::setPhase(Phase phase)
{
    _phase = phase;
}


Event::Phase Event::getPhase() const
{
    return _phase;
}


bool Event::bubbles() const
{
    return _bubbles;
}


bool Event::isCancelable() const
{
    return _cancelable;
}


Element* Event::source() const
{
    return _source;
}


Element* Event::target() const
{
    return _target;
}


Element* Event::relatedTarget() const
{
    return _relatedTarget;
}


Element* Event::getCurrentTarget() const
{
    return _currentTaget;
}


void Event::setCurrentTarget(Element* target)
{
    _currentTaget = target;
}


PointerCaptureEvent::PointerCaptureEvent(std::size_t id,
                                         bool wasCaptured,
                                         Element* source,
                                         Element* target):
    UIEvent((wasCaptured ? PointerEventArgs::GOT_POINTER_CAPTURE : PointerEventArgs::LOST_POINTER_CAPTURE),
            source,
            target,
            true,
            false,
            ofGetElapsedTimeMillis()),
    _id(id)
{
}



PointerCaptureEvent::~PointerCaptureEvent()
{
}


std::size_t PointerCaptureEvent::id() const
{
    return _id;
}


PointerEvent::PointerEvent(const PointerEventArgs& pointer,
                           Element* target,
                           Element* source):
    UIEvent(pointer.eventType(),
            source,
            target,
            eventBubbles(pointer.eventType()),
            eventCancelable(pointer.eventType()),
            _pointer.timestamp()),
    _pointer(pointer)
{
}


PointerEvent::~PointerEvent()
{
}


const PointerEventArgs& PointerEvent::pointer() const
{
    return _pointer;
}


bool PointerEvent::eventBubbles(const std::string& event)
{
    return !(event == PointerEventArgs::POINTER_ENTER
          || event == PointerEventArgs::POINTER_LEAVE);
}


bool PointerEvent::eventCancelable(const std::string& event)
{
    return !(event == PointerEventArgs::POINTER_ENTER
          || event == PointerEventArgs::POINTER_CANCEL
          || event == PointerEventArgs::POINTER_LEAVE
          || event == PointerEventArgs::GOT_POINTER_CAPTURE
          || event == PointerEventArgs::LOST_POINTER_CAPTURE);
}


const std::string KeyboardEvent::KEY_DOWN = "keydown";
const std::string KeyboardEvent::KEY_UP = "keyup";


KeyboardEvent::KeyboardEvent(const ofKeyEventArgs& args,
                             Element* source,
                             Element* target):
    UIEvent(args.type == ofKeyEventArgs::Pressed ? KEY_DOWN : KEY_UP,
            source,
            target,
            true,
            true,
            ofGetElapsedTimeMillis())
{
}


KeyboardEvent::~KeyboardEvent()
{
}


const ofKeyEventArgs& KeyboardEvent::key() const
{
    return _key;
}


const std::string FocusEvent::FOCUS_IN = "focusin";
const std::string FocusEvent::FOCUS = "focus";
const std::string FocusEvent::FOCUS_OUT = "focusout";
const std::string FocusEvent::BLUR = "blur";


FocusEvent::FocusEvent(const std::string& type,
                       Element* source,
                       Element* target,
                       Element* relatedTarget):
    Event(type,
          source,
          target,
          (type != FOCUS),
          false,
          ofGetElapsedTimeMillis())
{
    // TODO: better Event constructor.
    _relatedTarget = relatedTarget;
}


FocusEvent::~FocusEvent()
{
}


MoveEvent::MoveEvent(const Position& position):
    _position(position)
{
}


MoveEvent::~MoveEvent()
{
}


const Position& MoveEvent::position() const
{
    return _position;
}


ResizeEvent::ResizeEvent(const Geometry& geometry):
    _geometry(geometry)
{
}


ResizeEvent::~ResizeEvent()
{
}


const Geometry& ResizeEvent::geometry() const
{
    return _geometry;
}



AttributeEvent::AttributeEvent(const std::string& key, const std::string& value):
    _key(key),
    _value(value)
{
}


AttributeEvent::~AttributeEvent()
{
}


const std::string& AttributeEvent::key() const
{
    return _key;
}


const std::string& AttributeEvent::value() const
{
    return _value;
}


EnablerEvent::EnablerEvent(bool value):
    _value(value)
{
}


EnablerEvent::~EnablerEvent()
{
}


bool EnablerEvent::value() const
{
    return _value;
}


ElementEvent::ElementEvent(Element* element):
    _element(element)
{
}


ElementEvent::~ElementEvent()
{
}


Element* ElementEvent::element()
{
    return _element;
}


ElementOrderEvent::ElementOrderEvent(Element* element, Type type):
    ElementEvent(element),
    _type(type)
{
}


ElementOrderEvent::~ElementOrderEvent()
{
}


ElementOrderEvent::Type ElementOrderEvent::type() const
{
    return _type;
}


} } // namespace ofx::DOM
