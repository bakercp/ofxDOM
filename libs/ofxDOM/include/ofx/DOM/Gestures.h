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


#include "ofx/DOM/Types.h"
#include "ofx/DOM/Events.h"


namespace ofx {
namespace DOM {


class Element;


class GestureRecognizer
{
public:
    enum class State
	{
        POSSIBLE,
        BEGAN,
        CHANGED,
        ENDED,
        CANCELLED,
        FAILED,
        RECOGNIZED = ENDED
    };

    GestureRecognizer(Element* element);

    virtual ~GestureRecognizer();

    State state() const;

    void reset();

    void onPointerEvent(PointerEvent& e);

private:
    State _state = State::POSSIBLE;

    Element* _element;

};


class PressGestureRecognizer: public GestureRecognizer
{
public:

private:
    uint64_t _flags = 0;
    Point _location;
    uint64_t _buttonMask = 0;
    uint64_t _numberOfClicksRequired = 1;
    uint64_t _currentButtonCount = 0;
    uint64_t _activeButtonCount = 0;
    uint64_t _currentClickCount = 0;

    uint64_t _minimumPressDuration = 500;

    float _allowableMovement = 5;

};


class MagnificationGestureRecognizer: public GestureRecognizer
{
public:

private:
    Point _location;
    float _magnification = 1;

};


class PanGestureRecognizer: public GestureRecognizer
{
public:

private:
    Point _location;
    Point _translation;
    uint64_t _buttonMask;
    uint64_t _buttonCount;
    // id _velocityFilter;

};


class RotationGestureRecognizer: public GestureRecognizer
{
public:

private:
    Point _location;
    float _rotation;

};





} } // namespace ofx::DOM
