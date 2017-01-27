//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofx/DOM/Gestures.h"
#include "ofx/DOM/Element.h"


namespace ofx {
namespace DOM {


GestureRecognizer::GestureRecognizer(Element* element): _element(element)
{
    ofAddListener(_element->pointerDown.event(), this, &GestureRecognizer::onPointerEvent);
}


GestureRecognizer::~GestureRecognizer()
{
    ofRemoveListener(_element->pointerDown.event(), this, &GestureRecognizer::onPointerEvent);
}


GestureRecognizer::State GestureRecognizer::state() const
{
    return _state;
}


void GestureRecognizer::reset()
{
    // reset
}


void GestureRecognizer::onPointerEvent(PointerUIEventArgs& e)
{

}


} } // namespace ofx::DOM
