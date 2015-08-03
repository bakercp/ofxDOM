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


#include "ofx/DOM/Types.h"
#include "ofx/DOM/Element.h"
#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


CapturedPointer::CapturedPointer(std::size_t id):
    id(id),
    start(Position()),
    offset(Position()),
    position(Position()),
    velocity(Position()),
    lastUpdate(std::numeric_limits<uint64_t>::min()),
    timestamp(std::numeric_limits<uint64_t>::min())
{
}


CapturedPointer::~CapturedPointer()
{
}


void CapturedPointer::update(Element* element, const PointerEvent& e)
{
    uint64_t now = ofGetElapsedTimeMillis();

    if (timestamp != std::numeric_limits<uint64_t>::min())
    {
        uint64_t dt = now - lastUpdate;
        Point ds = position - e.pointer().point();

        velocity = ds / dt;
        position = e.pointer().point();
        lastUpdate = now;
    }
    else
    {
        id = e.pointer().id();
        start = e.pointer().point();
        offset = start - element->screenPosition();
        position = e.pointer().point();
        velocity = Position();
        lastUpdate = now;
        timestamp = now;
    }
}


} } // namespace ofx::DOM
