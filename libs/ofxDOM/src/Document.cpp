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


#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


namespace ofx {
namespace DOM {


Document::Document():
    Element("document", 0, 0, 1024, 768)
{
    ofAddListener(ofEvents().setup, this, &Document::setup);
    ofAddListener(ofEvents().update, this, &Document::update);
    ofAddListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().exit, this, &Document::exit);
    ofAddListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::lowest());

    ofAddListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::lowest());

    ofAddListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
    ofAddListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());

    ofAddListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofAddListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofAddListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofAddListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
}


Document::~Document()
{
    ofRemoveListener(ofEvents().setup, this, &Document::setup);
    ofRemoveListener(ofEvents().update, this, &Document::update);
    ofRemoveListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_BEFORE_APP);
    ofRemoveListener(ofEvents().exit, this, &Document::exit);
    ofRemoveListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::lowest());

    ofRemoveListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::lowest());

    ofRemoveListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());
    ofRemoveListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::lowest());

    ofRemoveListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofRemoveListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofRemoveListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
    ofRemoveListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::lowest());
}


void Document::setup(ofEventArgs& e)
{
    if (_autoFillScreen)
    {
        setSize(ofGetWidth(), ofGetHeight());
    }

    Element::_setup(e);
}


void Document::update(ofEventArgs& e)
{
    Element::_update(e);
}


void Document::draw(ofEventArgs& e)
{
    Element::_draw(e);
}


void Document::exit(ofEventArgs& e)
{
    Element::_exit(e);
}


void Document::windowResized(ofResizeEventArgs& e)
{
    if (_autoFillScreen)
    {
        setSize(e.width, e.height);
    }
}


bool Document::fileDragEvent(ofDragInfo& e)
{
    return false;
}


bool Document::onKeyEvent(ofKeyEventArgs& e)
{
    return false;
}


void Document::setAutoFillScreen(bool autoFillScreen)
{
    _autoFillScreen = autoFillScreen;

    if (_autoFillScreen)
    {
        setSize(ofGetWidth(), ofGetHeight());
    }
}


bool Document::getAutoFillScreen() const
{
    return _autoFillScreen;
}


bool Document::onPointerEvent(PointerEventArgs& e)
{
    // Determine if the event was handled.
    bool wasEventHandled = false;

    // Add this pointer to the list of active pointers.
    _activePointers[e.id()] = e;

    // The last element that the current pointer was hitting.
    Element* lastActiveTarget = findElementInMap(e.id(), _activeTargets);

    // The Element that the pointer is currently hitting.
    // Find the current active target.
    // TODO: Use lastActiveTarget to seed target search?
    Element* activeTarget = recursiveHitTest(screenToParent(e.point()));

    // The event target is the target that will receive the event.
    //
    // We start off assuming that the event target is the active target.
    // If we determine that the pointer has already been captured, we will
    // correct this.
    Element* eventTarget = findElementInMap(e.id(), _capturedPointerIdToElementMap);

    // If there is no event target but there is an active target, attempt to
    // capture the pointer.
    if (eventTarget == nullptr &&
        activeTarget != nullptr &&
        e.eventType() == PointerEventArgs::POINTER_DOWN &&
        activeTarget->getImplicitPointerCapture())
    {
        eventTarget = activeTarget;
        setPointerCaptureForElement(eventTarget, e.id());
    }

    // If true, we call synthesizePointerOutAndLeave after the pointerup / pointercancel event.
    bool sythesizeUpAndLeaveForEventTarget = false;

    // If activeTarget is not equal to the last active target, then
    /// pointerout, pointerleave, pointerover and pointer enter events will
    // be synthesized as needed. This goes for any event type.
    if (activeTarget != lastActiveTarget)
    {
        // Synthesize pointerout and pointerleave events IF there was a
        // valid target last time.  Also make sure that the last target
        // isn't the current active target.
        if (lastActiveTarget != nullptr)
        {
            // From lastActiveTarget -> activeTarget.
            synthesizePointerOutAndLeave(e, lastActiveTarget, activeTarget);
        }

        // Sytnesize pointerover and pointerenter events if the current
        // active target is valid.
        if (activeTarget != nullptr)
        {
            // From lastActiveTarget -> activeTarget.
            synthesizePointerOverAndEnter(e, activeTarget, lastActiveTarget);
        }
    }


    // Here we handle a special case for non-hover sythesized pointer events.
    if (activeTarget != nullptr &&
        !e.canHover() &&
        (e.eventType() == PointerEventArgs::POINTER_UP ||
         e.eventType() == PointerEventArgs::POINTER_CANCEL))
    {
        if (activeTarget == eventTarget)
        {
            // Schedule synthesis, if the active target is the event target.
            sythesizeUpAndLeaveForEventTarget = true;
        }
        else
        {
            // Since lastActiveTarget may be nullptr, we use:
            // activeTarget -> nullptr.
            synthesizePointerOutAndLeave(e, activeTarget, nullptr);
        }
    }

    // Create a DOM pointer event.
    PointerUIEventArgs event(e, this, eventTarget);


    // Now, dispatch the original event if there is a target.
    // If eventTarget != nullptr, that means the current pointer id is captured.
    if (eventTarget != nullptr)
    {
        event.setPhase(EventArgs::Phase::AT_TARGET);

        // Update captured pointer data.
        auto capturedPointer = eventTarget->findCapturedPointerById(e.id());

        if (capturedPointer != eventTarget->capturedPointers().end())
        {
            capturedPointer->update(eventTarget, event);
        }
        else
        {
            throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Document::onPointerEvent");
        }

        // Handle event.
        eventTarget->handleEvent(event);

        // Sythesized events if needed.
        if (sythesizeUpAndLeaveForEventTarget)
        {
            synthesizePointerOutAndLeave(e, eventTarget, nullptr);
        }

        // Release pointer capture if needed.
        if (e.eventType() == PointerEventArgs::POINTER_UP ||
            e.eventType() == PointerEventArgs::POINTER_CANCEL)
        {
            releasePointerCaptureForElement(eventTarget, e.id());
        }

        wasEventHandled = true;

    }
    else if (activeTarget == nullptr || !activeTarget->dispatchEvent(event))
    {
        // TODO: call default action if there is one.
    }

    // Manage active targets and pointers lists.
    if (!e.canHover() && (e.eventType() == PointerEventArgs::POINTER_UP ||
                          e.eventType() == PointerEventArgs::POINTER_CANCEL))
    {
        _activeTargets.erase(e.id());
        _activePointers.erase(e.id());
    }
    else
    {
        // Active target may be a nullptr if outside of the Document.
        _activeTargets[e.id()] = activeTarget;
    }

    return wasEventHandled;
}


void Document::setPointerCaptureForElement(Element* element, std::size_t id)
{
    // Make sure the element isn't nullptr and the caller is this document.
    if (element != nullptr && this == element->document())
    {
        auto activePointersIter = _activePointers.find(id);

        // If called from within Document, this shouldn't happen, but check anyway.
        if (activePointersIter == _activePointers.end())
        {
            throw DOMException(DOMException::INVALID_POINTER_ID + ": " + "Document::setPointerCapture");
        }
        else if (activePointersIter->second.buttons() > 0 && // Capture only if "button" is pressed.
                 _capturedPointerIdToElementMap.find(id) == _capturedPointerIdToElementMap.end()) // And pointer is not yet captured.
        {
            // Add the pointer to our map.
            _capturedPointerIdToElementMap[id] = element;

            // Add the pointer id to the element's captured pointer list.
            element->_capturedPointers.emplace_back(CapturedPointer(id));

            // Emit a pointer captured event.
            PointerCaptureUIEventArgs e(id, true, this, element);
            element->dispatchEvent(e);
        }
        else
        {
            // Ignore if already captured OR no buttons were pressed.
        }
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Document::setPointerCapture");
    }
}


void Document::releasePointerCaptureForElement(Element* element, std::size_t id)
{
    // TODO: release if the target element is removed from the document.

    if (element != nullptr)
    {
        auto activePointersIter = _activePointers.find(id);

        if (activePointersIter != _activePointers.end())
        {
            auto iter = _capturedPointerIdToElementMap.find(id);

            if (iter != _capturedPointerIdToElementMap.end())
            {
                _capturedPointerIdToElementMap.erase(iter);

                element->_capturedPointers.erase(element->findCapturedPointerById(id));

                PointerCaptureUIEventArgs e(id, false, this, element);
                element->dispatchEvent(e);
            }
        }
        else
        {
            throw DOMException(DOMException::INVALID_POINTER_ID + ": " + "Document::releasePointerCapture");
        }
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Document::releasePointerCapture");
    }
}


Element* Document::findElementInMap(std::size_t id, PointerElementMap& pem)
{
    // Search the captured pointers for a matching pointer.
    auto iter = pem.find(id);

    if (iter != pem.end())
    {
        return iter->second;
    }
    else
    {
        return nullptr;
    }
}


void Document::synthesizePointerOutAndLeave(const PointerEventArgs& e,
                                            Element* target,
                                            Element* relatedTarget)
{
    PointerEventArgs pointerOut(PointerEventArgs::POINTER_OUT, e);

    // Call pointerout ONLY on old target
    PointerUIEventArgs pointerOutEvent(pointerOut,
                                       this,
                                       target,
                                       relatedTarget);

    pointerOutEvent.setPhase(EventArgs::Phase::AT_TARGET);

    target->handleEvent(pointerOutEvent);

    PointerEventArgs pointerLeave(PointerEventArgs::POINTER_LEAVE, e);

    // Call pointerleave on old target AND ancestors.
    PointerUIEventArgs pointerLeaveEvent(pointerLeave,
                                         this,
                                         target,
                                         relatedTarget);

    target->dispatchEvent(pointerLeaveEvent);
}


void Document::synthesizePointerOverAndEnter(const PointerEventArgs& e,
                                             Element* target,
                                             Element* relatedTarget)
{
    PointerEventArgs pointerOver(PointerEventArgs::POINTER_OVER, e);

    // Call pointerout ONLY on old target
    PointerUIEventArgs pointerOverEvent(pointerOver,
                                        this,
                                        target,
                                        relatedTarget);

    pointerOverEvent.setPhase(EventArgs::Phase::AT_TARGET);

    target->handleEvent(pointerOverEvent);

    PointerEventArgs pointerEnter(PointerEventArgs::POINTER_ENTER, e);

    // Call pointerover ONLY on the target.
    // Call pointerenter on target and ancestors.
    // Call pointerleave on old target and ancestors
    PointerUIEventArgs pointerEnterEvent(pointerEnter,
                                         this,
                                         target,
                                         relatedTarget);

    target->dispatchEvent(pointerEnterEvent);
}


} } // namespace ofx::DOM
