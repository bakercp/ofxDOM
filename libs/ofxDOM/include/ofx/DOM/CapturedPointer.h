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


#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


/// \brief A class for keeping track of captured pointers.
class CapturedPointer
{
public:
    /// \brief Create a CapturedPointer with the given id.
    /// \param id The id of the captured pointer.
    CapturedPointer(std::size_t id);

    /// \brief Destroy the captured pointer.
    ~CapturedPointer();

    /// \brief Update a given Captured Pointer from an Element and PointerEvent.
    ///
    /// This method updates offset, position, velocity, lastUpdate and other
    /// parameters of the CapturedPointer.
    ///
    /// \param element The element that is currently handling the Pointer.
    /// \param e The PointerEvent used to update the CapturedPointer's data.
    void update(Element* element, const PointerEvent& e);

    /// \returns the captured pointer's id.
    std::size_t id() const;

    /// \returns the start position of the captured pointer.
    const Position& start() const;

    /// \returns the start()-position() of the captured pointer.
    const Position& offset() const;

    /// \returns the current position of the captured pointer.
    const Position& position() const;

    /// \returns the velocity of the captured pointer (lastPosition() - position()).
    const Position& velocity() const;

    /// \returns the last update time.
    uint64_t lastUpdate() const;

    /// \returns the initial timestamp when the pointer was captured.
    uint64_t timestamp() const;

    friend std::ostream& operator << (std::ostream& os,
                                      const CapturedPointer& pointer);

private:
    /// \brief The captured pointer's id.
    std::size_t _id;

    /// \brief The captured pointer's start position.
    Position _start;

    /// \brief The captured pointer's current offset from its start position.
    Position _offset;

    /// \brief The captured pointer's current position.
    Position _position;

    /// \brief The captured pointer's current velocity.
    Position _velocity;

    /// \brief The timestamp of the last update.
    uint64_t _lastUpdate;

    /// \brief The timestamp when the pointer was captured.
    uint64_t _timestamp;

};


inline std::ostream& operator<<(std::ostream& os,
                                const CapturedPointer& pointer)
{
    os << pointer.id() << " " << pointer.lastUpdate();
    return os;
}


} } // namespace ofx::DOM
