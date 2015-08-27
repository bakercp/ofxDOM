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


#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


/// \brief A class for keeping track of captured pointers.
class CapturedPointer
{
public:
    CapturedPointer(std::size_t id);
    ~CapturedPointer();

    void update(Element* element, const PointerEvent& e);

	std::size_t id() const;
	const Position& start() const;
	const Position& offset() const;
	const Position& position() const;
	const Position& velocity() const;
	uint64_t lastUpdate() const;
	uint64_t timestamp() const;

	friend std::ostream& operator << (std::ostream& os,
									  const CapturedPointer& vec);

private:
    std::size_t _id;
    Position _start;
    Position _offset;
    Position _position;
    Position _velocity;
    uint64_t _lastUpdate;
    uint64_t _timestamp;

};


inline std::ostream& operator<<(std::ostream& os,
								const CapturedPointer& pointer)
{
	os << pointer.id() << " " << pointer.lastUpdate();
	return os;
}


} } // namespace ofx::DOM
