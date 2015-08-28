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


#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"


namespace ofx {
namespace DOM {


Document::Document(): Element("document", 0, 0, ofGetWidth(), ofGetHeight())
{
    ofAddListener(ofEvents().setup, this, &Document::setup);
    ofAddListener(ofEvents().update, this, &Document::update);
    ofAddListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().exit, this, &Document::exit);
    ofAddListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::min());

    ofAddListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::min());

    ofAddListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::min());
    ofAddListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::min());

    ofAddListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofAddListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
}


Document::~Document()
{
    ofRemoveListener(ofEvents().setup, this, &Document::setup);
    ofRemoveListener(ofEvents().update, this, &Document::update);
    ofRemoveListener(ofEvents().draw, this, &Document::draw, OF_EVENT_ORDER_BEFORE_APP);
    ofRemoveListener(ofEvents().exit, this, &Document::exit);
    ofRemoveListener(ofEvents().windowResized, this, &Document::windowResized, std::numeric_limits<int>::min());

    ofRemoveListener(ofEvents().fileDragEvent, this, &Document::fileDragEvent, std::numeric_limits<int>::min());

    ofRemoveListener(ofEvents().keyPressed, this, &Document::onKeyEvent, std::numeric_limits<int>::min());
    ofRemoveListener(ofEvents().keyReleased, this, &Document::onKeyEvent, std::numeric_limits<int>::min());

    ofRemoveListener(PointerEvents::instance().onPointerDown, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerUp, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerMove, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
    ofRemoveListener(PointerEvents::instance().onPointerCancel, this, &Document::onPointerEvent, std::numeric_limits<int>::min());
}


void Document::setup(ofEventArgs& e)
{
}


void Document::update(ofEventArgs& e)
{
}


void Document::draw(ofEventArgs& e)
{
    Element::_draw();
}


void Document::exit(ofEventArgs& e)
{
//    doExit();
}


void Document::windowResized(ofResizeEventArgs& e)
{
    setSize(e.width, e.height);
}


bool Document::fileDragEvent(ofDragInfo& e)
{
    return false;
}


bool Document::onKeyEvent(ofKeyEventArgs& e)
{
    return false;
}


bool Document::onPointerEvent(PointerEventArgs& e)
{
	// If there's an event, then this is an active pointer.
	_activePointers[e.id()] = e;

	// The begins unhandled.
    bool wasEventHandled = false;

	// Determine the current and last active target elements.
	Element* activeTarget = nullptr;
	Element* lastActiveTarget = nullptr;

	// Search for an existing active target associated with this pointer id.
	auto i = _activeTargets.find(e.id());

	if (i != _activeTargets.end())
	{
		lastActiveTarget = i->second;
	}

	// Cache the local position.
	Position localPosition = screenToLocal(e.point());

	// Find the current active target.
	// TODO: Use lastActiveTarget to seed target search.
	activeTarget = recursiveHitTest(localPosition);

	// The event target is the target that will receive the event.
	// This may be different from the active target and last active target if
	// the pointer is captured.
	Element* eventTarget = nullptr;

	bool isCaptured = false;

	// Determine if this is a captured pointer.
	auto capturedPointerIter = _capturedPointerIdToElementMap.find(e.id());

	if (capturedPointerIter != _capturedPointerIdToElementMap.end())
	{
		eventTarget = capturedPointerIter->second;
		isCaptured = true;
	}
	else
	{
		eventTarget = activeTarget;
	}

	// If this is a move event, we need to synthesize some other events.
	if (e.eventType() == PointerEventArgs::POINTER_MOVE)
	{
		// Has the target changed since the last event?
		if (lastActiveTarget != activeTarget)
		{
			if (lastActiveTarget != nullptr && (!isCaptured || lastActiveTarget == eventTarget))
			{
				PointerEventArgs pointerOut(PointerEventArgs::POINTER_OUT,
											e.point(),
											e.deviceId(),
											e.index(),
											e.deviceType(),
											e.isPrimary(),
											e.button(),
											e.buttons(),
											e.modifiers(),
											e.pressCount(),
											e.timestamp());

				// Call pointerout ONLY on old target
				PointerEvent pointerOutEvent(pointerOut, this, lastActiveTarget);
				pointerOutEvent.setPhase(Event::Phase::AT_TARGET);
				lastActiveTarget->handleEvent(pointerOutEvent);

				PointerEventArgs pointerLeave(PointerEventArgs::POINTER_LEAVE,
											  e.point(),
											  e.deviceId(),
											  e.index(),
											  e.deviceType(),
											  e.isPrimary(),
											  e.button(),
											  e.buttons(),
											  e.modifiers(),
											  e.pressCount(),
											  e.timestamp());

				// Call pointerleave on old target AND ancestors.
				PointerEvent pointerLeaveEvent(pointerLeave, this, lastActiveTarget);
				lastActiveTarget->dispatchEvent(pointerLeaveEvent);
			}

			if (activeTarget != nullptr && (!isCaptured || activeTarget == eventTarget))
			{
				PointerEventArgs pointerOver(PointerEventArgs::POINTER_OVER,
											 e.point(),
											 e.deviceId(),
											 e.index(),
											 e.deviceType(),
											 e.isPrimary(),
											 e.button(),
											 e.buttons(),
											 e.modifiers(),
											 e.pressCount(),
											 e.timestamp());

				// Call pointerout ONLY on old target
				PointerEvent pointerOverEvent(pointerOver, this, activeTarget);
				pointerOverEvent.setPhase(Event::Phase::AT_TARGET);
				activeTarget->handleEvent(pointerOverEvent);

				PointerEventArgs pointerEnter(PointerEventArgs::POINTER_ENTER,
											  e.point(),
											  e.deviceId(),
											  e.index(),
											  e.deviceType(),
											  e.isPrimary(),
											  e.button(),
											  e.buttons(),
											  e.modifiers(),
											  e.pressCount(),
											  e.timestamp());

				// Call pointerover ONLY on the target.
				// Call pointerenter on target and ancestors.
				// Call pointerleave on old target and ancestors
				PointerEvent pointerEnterEvent(pointerEnter, this, activeTarget);
				activeTarget->dispatchEvent(pointerEnterEvent);
			}
		}
	}

	// Now, dispatch the original event if there is a target.
	if (eventTarget != nullptr)
	{
		PointerEvent event(e, this, eventTarget);

		if (isCaptured)
		{
			event.setPhase(Event::Phase::AT_TARGET);

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

			// Release pointer capture if needed.
			if (e.eventType() == PointerEventArgs::POINTER_UP ||
				e.eventType() == PointerEventArgs::POINTER_CANCEL)
			{
				releasePointerCapture(eventTarget, e.id());
			}
		}
		else
		{
			if (!eventTarget->dispatchEvent(event))
			{
				// TODO: call default action if there is one.
			}

			// Capture the pointer if implicitly required to.
			// This prevents the default event listeners from being
			// configured on the Element.
			if (eventTarget->getImplicitPointerCapture() &&
				e.eventType() == PointerEventArgs::POINTER_DOWN)
			{
				setPointerCapture(eventTarget, e.id());
			}
		}

		wasEventHandled = true;
	}

	// Bookkeeping
	if (activeTarget != nullptr)
	{
		_activeTargets[e.id()] = activeTarget;
	}
	else
	{
		_activeTargets.erase(e.id());
	}

	// Mouse events are always active.
	// Touch and pen events are no longer active if up or cancelled.
	if (e.deviceType() != PointerEventArgs::TYPE_MOUSE &&
		(e.eventType() == PointerEventArgs::POINTER_UP ||
		 e.eventType() == PointerEventArgs::POINTER_CANCEL))
	{
		_activePointers.erase(e.id());
	}

    return wasEventHandled;
}


void Document::setPointerCapture(Element* element, std::size_t id)
{
	// Is the element null

	if (element != nullptr && this == element->document())
    {
		auto activePointersIter = _activePointers.find(id);

		if (activePointersIter == _activePointers.end())
		{
			throw DOMException(DOMException::INVALID_POINTER_ID + ": " + "Document::setPointerCapture");
		}
		else if (activePointersIter->second.buttons() > 0 &&
				 _capturedPointerIdToElementMap.find(id) == _capturedPointerIdToElementMap.end())
		{
			_capturedPointerIdToElementMap[id] = element;

			element->_capturedPointers.emplace_back(CapturedPointer(id));

			PointerCaptureEvent evt(id, true, this, element);
			element->dispatchEvent(evt);
		}
    }
    else
    {
		throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Document::setPointerCapture");
    }
}


void Document::releasePointerCapture(Element* element, std::size_t id)
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

				PointerCaptureEvent evt(id, false, this, element);
				element->dispatchEvent(evt);
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


} } // namespace ofx::DOM
