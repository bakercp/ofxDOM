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
    _lastUpdate(std::numeric_limits<uint64_t>::min()),
    _timestamp(std::numeric_limits<uint64_t>::min())
{
}


CapturedPointer::~CapturedPointer()
{
}


void CapturedPointer::update(Element* element, const PointerEvent& e)
{
    uint64_t now = ofGetElapsedTimeMillis();

    if (_timestamp != std::numeric_limits<uint64_t>::min())
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
        _offset = _start - element->screenPosition();
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
