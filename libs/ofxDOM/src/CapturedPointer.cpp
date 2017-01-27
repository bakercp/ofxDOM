//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/DOM/CapturedPointer.h"
#include "ofx/DOM/Element.h"


namespace ofx {
namespace DOM {


CapturedPointer::CapturedPointer(std::size_t id):
    _id(id),
    _start(Position()),
    _offset(Position()),
    _position(Position()),
    _velocity(Position()),
    _lastUpdate(std::numeric_limits<uint64_t>::lowest()),
    _timestamp(std::numeric_limits<uint64_t>::lowest())
{
}


CapturedPointer::~CapturedPointer()
{
}


void CapturedPointer::update(Element* element, const PointerUIEventArgs& e)
{
    uint64_t now = ofGetElapsedTimeMillis();

    if (_timestamp != std::numeric_limits<uint64_t>::lowest())
    {
        uint64_t dt = now - _lastUpdate;
        Point ds = _position - e.pointer().point();

        _velocity = ds / dt;
        _position = e.pointer().point();
        _lastUpdate = now;
    }
    else
    {
        _id = e.pointer().id();
        _start = e.pointer().point();
        _offset = _start - element->getScreenPosition();
        _position = e.pointer().point();
        _velocity = Position();
        _lastUpdate = now;
        _timestamp = now;
    }
}


std::size_t CapturedPointer::id() const
{
    return _id;
}


const Position& CapturedPointer::start() const
{
    return _start;
}


const Position& CapturedPointer::offset() const
{
    return _offset;
}


const Position& CapturedPointer::position() const
{
    return _position;
}


const Position& CapturedPointer::velocity() const
{
    return _velocity;
}


uint64_t CapturedPointer::lastUpdate() const
{
    return _lastUpdate;
}


uint64_t CapturedPointer::timestamp() const
{
    return _timestamp;
}


} } // namespace ofx::DOM
