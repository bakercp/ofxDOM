//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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

    void onPointerEvent(PointerUIEventArgs& e);

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
