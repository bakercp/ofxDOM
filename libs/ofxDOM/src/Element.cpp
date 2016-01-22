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


#include "ofx/DOM/Element.h"
#include "ofx/DOM/Document.h"
#include "ofGraphics.h"
#include <algorithm>


namespace ofx {
namespace DOM {


Element::Element(float x, float y, float width, float height):
    Element("", x, y, width, height)
{
}


Element::Element(const std::string& id,
                 float x,
                 float y,
                 float width,
                 float height):
    _id(id),
    _geometry(x, y, width, height)
{
}


Element::~Element()
{
}


std::unique_ptr<Element> Element::removeChild(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        // Move the child out of the children array.
        std::unique_ptr<Element> detachedChild = std::move(*iter);

        // Disown the detached child
        _children.erase(iter);

        // Set the parent to nullptr.
        detachedChild->_parent = nullptr;

        // Invalidate all cached child geometry.
        invalidateChildGeometry();

        // Alert the node that its parent was set.
        ElementEventArgs removedFromEvent(this);
        ofNotifyEvent(detachedChild->removedFrom, removedFromEvent, this);

        ElementEventArgs childRemovedEvent(detachedChild.get());
        ofNotifyEvent(childRemoved, childRemovedEvent, this);

        /// Alert the node's siblings that it no longer has a sibling.
        for (auto& child : _children)
        {
            if (detachedChild.get() != child.get())
            {
                ElementEventArgs e(detachedChild.get());
                ofNotifyEvent(child->siblingRemoved, e, this);
            }
        }

        // Detatch child listeners.
        ofRemoveListener(detachedChild.get()->move, this, &Element::_onChildMoved);
        ofRemoveListener(detachedChild.get()->resize, this, &Element::_onChildResized);

        // Return the detached child.
        // If the return value is ignored, it will be deleted.

        return detachedChild;
    }
    else
    {
        // Return nullptr because we couldn't find anything.
        return nullptr;
    }
}


void Element::moveToFront()
{
    if (hasParent())
    {
        _parent->moveChildToFront(this);
    }
}


void Element::moveForward()
{
    if (hasParent())
    {
        _parent->moveChildForward(this);
    }
}


void Element::moveToBack()
{
    if (hasParent())
    {
        _parent->moveChildToBack(this);
    }
}


void Element::moveBackward()
{
    if (hasParent())
    {
        _parent->moveChildBackward(this);
    }
}


void Element::moveChildToIndex(Element* element, std::size_t index)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        std::size_t oldIndex = iter - _children.begin();
        std::size_t newIndex = std::min(index, _children.size() - 1);

        auto detachedChild = std::move(*iter);

        _children.erase(iter);

        _children.insert(_children.begin() + newIndex, std::move(detachedChild));

        ElementOrderEventArgs e(element, oldIndex, newIndex);
        ofNotifyEvent(reordered, e, element);
        ofNotifyEvent(childReordered, e, this);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToFront: Element does not exist.");
    }
}


void Element::moveChildToFront(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        std::size_t oldIndex = iter - _children.begin();
        std::size_t newIndex = 0;

        auto detachedChild = std::move(*iter);
        _children.erase(iter);
        _children.insert(_children.begin(), std::move(detachedChild));

        ElementOrderEventArgs e(element, oldIndex, newIndex);
        ofNotifyEvent(reordered, e, element);
        ofNotifyEvent(childReordered, e, this);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToFront: Element does not exist.");
    }
}


void Element::moveChildForward(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        // Make sure it's not already in the front.
        if (iter != _children.begin())
        {
            std::size_t oldIndex = iter - _children.begin();
            std::size_t newIndex = oldIndex - 1;

            std::iter_swap(iter, iter - 1);

            ElementOrderEventArgs e(element, oldIndex, newIndex);
            ofNotifyEvent(reordered, e, element);
            ofNotifyEvent(childReordered, e, this);
        }
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildForward: Element does not exist.");
    }
}


void Element::moveChildToBack(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        // Make sure it's not already in the back.
        if (iter != _children.end() - 1)
        {
            std::size_t oldIndex = iter - _children.begin();
            std::size_t newIndex = _children.size() - 1;

            auto detachedChild = std::move(*iter);
            _children.erase(iter);
            _children.push_back(std::move(detachedChild));

            ElementOrderEventArgs e(element, oldIndex, newIndex);
            ofNotifyEvent(reordered, e, element);
            ofNotifyEvent(childReordered, e, this);
        }
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::moveChildToBack: Element does not exist.");
    }
}


void Element::moveChildBackward(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        if (iter != _children.end() - 1)
        {
            std::size_t oldIndex = iter - _children.begin();
            std::size_t newIndex = _children.size() + 1;

            std::iter_swap(iter, iter + 1);

            ElementOrderEventArgs e(element, oldIndex, newIndex);
            ofNotifyEvent(reordered, e, element);
            ofNotifyEvent(childReordered, e, this);
        }
    }
}


bool Element::isChild(Element* element) const
{
    return element
        && element->_parent == this;
}


bool Element::isSibling(Element* element) const
{
    return element
        && element->_parent
        && element->_parent == _parent;
}


std::size_t Element::numSiblings() const
{
    // Don't count self.
    return _parent ? (_parent->numChildren() - 1) : 0;
}


bool Element::isParent(Element* element) const
{
    return element
        && element == this->_parent;
}


bool Element::hasChildren() const
{
    return !_children.empty();
}


std::size_t Element::numChildren() const
{
    return _children.size();
}


bool Element::hasParent() const
{
    return _parent;
}


Element* Element::findFirstChildById(const std::string& id)
{
    auto iter = std::find_if(_children.begin(),
                             _children.end(),
                             [&](const std::unique_ptr<Element>& child) {
                                 return child->getId() == id;
                             });

    return (iter != _children.end()) ? iter->get() : nullptr;
}


std::vector<Element*> Element::findChildrenById(const std::string& id)
{
    std::vector<Element*> matches;

    for (auto& child : _children)
    {
        if (child->getId() == id)
        {
            matches.push_back(child.get());
        }
    }

    return matches;
}


std::vector<std::unique_ptr<Element>>::iterator Element::findChild(Element* element)
{
    return std::find_if(_children.begin(),
                        _children.end(),
                        [&](const std::unique_ptr<Element>& child) {
                            return element == child.get();
                        });
}


Element* Element::parent()
{
    return _parent;
}


const Element* Element::parent() const
{
    return _parent;
}


Document* Element::document()
{
    if (hasParent())
    {
        // If a parent exists, return it recursively.
        return _parent->document();
    }
    else
    {
        // Return self if a Document, otherwise, will return nullptr.
        return dynamic_cast<Document*>(this);
    }
}


const Document* Element::document() const
{
    if (_parent)
    {
        // If a parent exists, return it recursively.
        return _parent->document();
    }
    else
    {
        // Return self if a Document, otherwise, will return nullptr.
        return dynamic_cast<const Document*>(this);
    }
}


bool Element::hitTest(const Position& parentPosition) const
{
    return getGeometry().inside(parentPosition);
}


bool Element::childHitTest(const Position& localPosition) const
{
    return getChildGeometry().inside(localPosition);
}


Position Element::localToScreen(const Position& localPosition) const
{
    return localPosition + getScreenPosition();;
}


Position Element::screenToLocal(const Position& screenPosition) const
{
    return screenPosition - getScreenPosition();
}


Position Element::parentToScreen(const Position& parentPosition) const
{
    if (hasParent())
    {
        return parentPosition + _parent->getScreenPosition();
    }
    else
    {
        return parentPosition;
    }
}


Position Element::screenToParent(const Position& screenPosition) const
{
    if (hasParent())
    {
        return screenPosition - _parent->getScreenPosition();
    }
    else
    {
        return screenPosition;
    }
}


void Element::setPosition(float x, float y)
{
    _geometry.setPosition(x, y);
    MoveEventArgs e(getPosition());
    ofNotifyEvent(move, e, this);
}


void Element::setPosition(const Position& position)
{
    setPosition(position.x, position.y);
}


Position Element::getPosition() const
{
    return _geometry.getPosition();
}


float Element::getX() const
{
    return _geometry.getX();
}


float Element::getY() const
{
    return _geometry.getY();
}


Position Element::getScreenPosition() const
{
    if (hasParent())
    {
        return getPosition() + _parent->getScreenPosition();
    }
    else
    {
        return getPosition();
    }
}


float Element::getScreenX() const
{
    return getScreenPosition().x;
}


float Element::getScreenY() const
{
    return getScreenPosition().y;
}


void Element::setSize(float width, float height)
{
    _geometry.setWidth(width);
    _geometry.setHeight(height);
    _geometry.standardize();
    ResizeEventArgs e(_geometry);
    ofNotifyEvent(resize, e, this);
}


Size Element::getSize() const
{
    return Size(_geometry.width, _geometry.height);
}


float Element::getWidth() const
{
    return _geometry.getWidth();
}


float Element::getHeight() const
{
    return _geometry.getHeight();
}


Geometry Element::getGeometry() const
{
    return _geometry;
}


void Element::setGeometry(const Geometry& geometry)
{
    setPosition(geometry.x, geometry.y);
    setSize(geometry.width, geometry.height);
}


Geometry Element::getChildGeometry() const
{
    if (_childGeometryInvalid)
    {
        _childGeometry = Geometry(); // Clear.

        auto iter = _children.begin();

        while (iter != _children.end())
        {
            const Element& child = *(iter->get());

            if (iter == _children.begin())
            {
                _childGeometry = child.getTotalGeometry();
            }
            else
            {
                _childGeometry.growToInclude(child.getTotalGeometry());
            }

            ++iter;
        }

        _childGeometryInvalid = false;
    }

    return _childGeometry;
}


Geometry Element::getTotalGeometry() const
{
    Geometry totalGeometry(_geometry);

    if (hasChildren())
    {
        totalGeometry.growToInclude(getChildGeometry() + getPosition());
    }

    return totalGeometry;
}


std::string Element::getId() const
{
    return _id;
}


void Element::setId(const std::string& id)
{
    _id = id;
}


bool Element::hasAttribute(const std::string& key) const
{
    return _attributes.find(key) != _attributes.end();
}


void Element::setAttribute(const std::string& key, const Any& value)
{
    _attributes[key] = value;

    AttributeEventArgs e(key, value);
    ofNotifyEvent(attributeSet, e, this);
}


void Element::clearAttribute(const std::string& key)
{
    _attributes.erase(key);
    AttributeEventArgs e(key);
    ofNotifyEvent(attributeCleared, e, this);
}



void Element::_setup(ofEventArgs& e)
{
    for (auto& child : _children)
    {
        child->_setup(e);
    }

    onSetup();
}


void Element::_update(ofEventArgs& e)
{
    if (_enabled && !_hidden)
    {
        for (auto& child : _children)
        {
            child->_update(e);
        }

        onUpdate();
    }
}


void Element::_draw(ofEventArgs& e)
{
    if (_enabled && !_hidden)
    {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(_geometry.getPosition());

        // Draw parent behind children.
        onDraw();

        // Now draw in reverse order.
        auto iter = _children.rbegin();

        while (iter != _children.rend())
        {
            (*iter)->_draw(e);
            ++iter;
        }

        ofPopMatrix();
        ofPopStyle();
    }
}


void Element::_exit(ofEventArgs& e)
{
    for (auto& child : _children)
    {
        child->_exit(e);
    }

    onExit();
}


Element* Element::recursiveHitTest(const Position& parentPosition)
{
    if (_enabled && !_hidden)
    {
        Position childLocal = parentPosition - this->getPosition();

        if (hasChildren() && childHitTest(childLocal))
        {
            for (auto& child : _children)
            {
                Element* target = child->recursiveHitTest(childLocal);

                if (target)
                {
                    return target;
                }
            }
        }

        // If there is no child target, is this a viable target?
        if (hitTest(parentPosition))
        {
            return this;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}


void Element::setPointerCapture(std::size_t id)
{
    Document* _document = document();

    if (_document)
    {
        _document->setPointerCaptureForElement(this, id);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": " + "Element::setPointerCapture");
    }
}


void Element::releasePointerCapture(std::size_t id)
{
    Document* _document = document();

    if (_document)
    {
        _document->releasePointerCaptureForElement(this, id);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR + ": Element::releasePointerCapture");
    }
}


bool Element::isEnabled() const
{
    return _enabled;
}


void Element::setEnabled(bool enabled_)
{
    _enabled = enabled_;
    EnablerEventArgs e(_enabled);
    ofNotifyEvent(enabled, e, this);
}


bool Element::isHidden() const
{
    return _hidden;
}


void Element::setHidden(bool hidden_)
{
    _hidden = hidden_;
    EnablerEventArgs e(_hidden);
    ofNotifyEvent(hidden, e, this);
}


bool Element::isLocked() const
{
    return _locked;
}


void Element::setLocked(bool locked_)
{
    _locked = locked_;
    EnablerEventArgs e(_locked);
    ofNotifyEvent(locked, e, this);
}


void Element::invalidateChildGeometry() const
{
    _childGeometryInvalid = true;

    if (hasParent())
    {
        _parent->invalidateChildGeometry();
    }
}


bool Element::isPointerCaptured(std::size_t id) const
{
    return findCapturedPointerById(id) != _capturedPointers.end();
}


std::vector<CapturedPointer>::iterator Element::findCapturedPointerById(std::size_t id)
{
    return std::find_if(_capturedPointers.begin(),
                        _capturedPointers.end(),
                        [id](const CapturedPointer& pointer)
                        {
                            return id == pointer.id();
                        });
}



std::vector<CapturedPointer>::const_iterator Element::findCapturedPointerById(std::size_t id) const
{
    return findCapturedPointerById(id);
}


std::vector<CapturedPointer>& Element::capturedPointers()
{
    return _capturedPointers;
}


const std::vector<CapturedPointer>& Element::capturedPointers() const
{
    return _capturedPointers;
}


void Element::setImplicitPointerCapture(bool implicitPointerCapture)
{
    _implicitPointerCapture = implicitPointerCapture;
}


bool Element::getImplicitPointerCapture() const
{
    return _implicitPointerCapture;
}


void Element::_onChildMoved(MoveEventArgs&)
{
    invalidateChildGeometry();
}


void Element::_onChildResized(ResizeEventArgs&)
{
    invalidateChildGeometry();
}


} } // namespace ofx::DOM
