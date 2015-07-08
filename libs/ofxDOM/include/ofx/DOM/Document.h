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


#include "ofx/DOM/Element.h"


namespace ofx {
namespace DOM {


class Document: public Element
{
public:
    Document();
    virtual ~Document();

    void setup(ofEventArgs& e);
    void update(ofEventArgs& e);
    void draw(ofEventArgs& e);
    void exit(ofEventArgs& e);
    void windowResized(ofResizeEventArgs& e);

    // UIEvents
    bool fileDragEvent(ofDragInfo& e);
    bool onKeyEvent(ofKeyEventArgs& e);
    bool onPointerEvent(PointerEventArgs& e);

    void addPointerDeviceFilter(const std::string& type);
    void removePointerDeviceFilter(const std::string& type);

    bool setPointerCapture(Element* element, PointerEventArgs::PointerID id);
    bool releasePointerCapture(PointerEventArgs::PointerID id);

    /// \brief Dispatch an event.
    ///
    /// This will return true if the default action should be called.
    /// It will return false if at least one of the event targets called
    /// event.preventDefault().
    ///
    /// \returns true if the default action should be called.
    template<typename EventType, typename EventTarget>
    bool dispatchEvent(EventType& event, std::vector<EventTarget*> targets);

protected:
    typedef std::map<PointerEventArgs::PointerID, Element*> PointerMap;

    PointerMap _capturedPointers;

//    PointerMap _pointerMap;

    std::set<std::string> _pointerDeviceFilter;

};


template<typename EventType, typename EventTarget>
bool Document::dispatchEvent(EventType& event, std::vector<EventTarget*> targets)
{
    // Capture phase.
    event.setPhase(Event::Phase::CAPTURING_PHASE);

    auto iter = targets.begin();

    while (iter != targets.end())
    {
        if ((*iter)->isEventListener(event.type(), true))
        {
            event.setCurrentTarget(*iter);
            (*iter)->handleEvent(event);
        }

        if (event.isCancelled())
        {
            return !event.isDefaultPrevented();
        }

        ++iter;
    }

    // Target phase.
    event.setPhase(Event::Phase::AT_TARGET);
    event.setCurrentTarget(targets.back());
    targets.back()->handleEvent(event);

    if (event.isCancelled())
    {
        return !event.isDefaultPrevented();
    }

    // Bubble phase.
    event.setPhase(Event::Phase::BUBBLING_PHASE);

    if (targets.size() > 1 && event.bubbles())
    {
        // Begin with the parent of the target element.
        auto riter = targets.rbegin() + 1;

        while (riter != targets.rend())
        {
            if ((*riter)->isEventListener(event.type(), false))
            {
                event.setCurrentTarget(*iter);
                (*riter)->handleEvent(event);
            }

            if (event.isCancelled())
            {
                return !event.isDefaultPrevented();
            }

            ++riter;
        }
    }

    return event.isDefaultPrevented();
}


} } // namespace ofx::DOM
