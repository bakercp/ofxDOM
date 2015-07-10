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


Element::Element(const std::string& id, float x, float y, float width, float height):
    _id(id),
    _x(x),
    _y(y),
    _width(width),
    _height(height)
{
}


Element::~Element()
{
}


Element* Element::attachChild(std::unique_ptr<Element> element)
{
    if (nullptr != element)
    {
        // Get a raw pointer to the node for later.
        Element* pNode = element.get();

        // Assign the parent to the node via the raw pointer.
        pNode->_parent = this;

        // Take ownership of the node.
        _children.push_back(std::move(element));


        // Alert the node that its parent was set.
        pNode->onParentSet(this);

        /// Alert the node's siblings that they have a new sibling.
        for (auto& child : _children)
        {
            // Don't alert itself.
            if (child.get() != pNode)
            {
                child->onSiblingAdded(pNode);
            }
        }

        return pNode;
    }
    else
    {
        return nullptr;
    }
}


std::unique_ptr<Element> Element::releaseChild(Element* element)
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

        // Alert the node that its parent was set.
        detachedChild->onParentSet(nullptr);

        /// Alert the node's siblings that it no longer has a sibling.
        for (auto& child : _children)
        {
            if (detachedChild.get() != child.get())
            {
                child->onSiblingRemoved(detachedChild.get());
            }
        }

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
    if (!isRoot())
    {
        _parent->moveChildToFront(this);
        onMovedToFront();
    }
}


void Element::moveForward()
{
    if (!isRoot())
    {
        _parent->moveChildForward(this);
        onMovedForward();
    }
}


void Element::moveToBack()
{
    if (!isRoot())
    {
        _parent->moveChildToBack(this);
        onMovedToBack();
    }
}


void Element::moveBackward()
{
    if (!isRoot())
    {
        _parent->moveChildBackward(this);
        onMovedBackward();
    }
}


void Element::moveChildToFront(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end())
    {
        auto detachedChild = std::move(*iter);
        _children.erase(iter);
        _children.insert(_children.begin(), std::move(detachedChild));
        onChildMovedToFront(element);
    }
}


void Element::moveChildForward(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end() &&
        iter != _children.begin())
    {
        std::iter_swap(iter, iter - 1);
        onChildMovedForward(element);
    }
}


void Element::moveChildToBack(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end() &&
        iter != _children.end() - 1)
    {
        auto detachedChild = std::move(*iter);
        _children.erase(iter);
        _children.push_back(std::move(detachedChild));
        onChildMovedToBack(element);
    }
}


void Element::moveChildBackward(Element* element)
{
    auto iter = findChild(element);

    if (iter != _children.end() &&
        iter != _children.end() - 1)
    {
        std::iter_swap(iter, iter + 1);
        onChildMovedBackward(element);
    }
}


bool Element::isChild(Element* element) const
{
    return (nullptr != element)
        && (this == element->_parent);
}


bool Element::isSibling(Element* element) const
{
    return (nullptr != element)
        && (nullptr != element->_parent)
        && (_parent == element->_parent);
}


bool Element::isParent(Element* element) const
{
    return (nullptr != element)
        && (this == element->_parent);
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


bool Element::isRoot() const
{
    return nullptr == _parent;
}


Document* Element::document()
{
    if (nullptr == _parent)
    {
        return dynamic_cast<Document*>(this);
    }
    else
    {
        // If a parent exists, return it recursively.
        return _parent->document();
    }
}


bool Element::hitTest(const Position& localPosition) const
{
    return getGeometry().inside(localPosition);
}


Position Element::screenPosition() const
{
    return localToScreen(getPosition());
}


Position Element::localToScreen(const Position& position) const
{
    if (isRoot())
    {
        return position;
    }
    else
    {
        return _parent->screenPosition() + position;
    }
}


Position Element::screenToLocal(const Position& position) const
{
    return position - screenPosition();
}
    
    
void Element::setPosition(float x, float y)
{
    if (_x != x || _y != y)
    {
        if (!isRoot())
        {
            Geometry g = parent()->getGeometry();
            _x = ofClamp(x, 0, g.width - _width);
            _y = ofClamp(y, 0, g.height - _height);
        }
        else
        {
            _x = x;
            _y = y;
        }

        onMoved(_x, _y);
    }
}


void Element::setPosition(const Position& position)
{
    setPosition(position.x, position.y);
}


Position Element::getPosition() const
{
    return Position(_x, _y);
}


void Element::setSize(float width, float height)
{
    if (_width != width || _height != height)
    {
        _width = width;
        _height = height;
        onResized(_width, _height);
    }
}


Size Element::getSize() const
{
    return Size(_width, _height);
}


Geometry Element::getGeometry() const
{
    return Geometry(_x, _y, _width, _height);
}


void Element::setGeometry(const Geometry& geometry)
{
    setPosition(geometry.x, geometry.y);
    setSize(geometry.width, geometry.height);
}


bool Element::isEnabled() const
{
    return _enabled;
}

void Element::setEnabled(bool enabled)
{
    if (_enabled != enabled)
    {
        _enabled = enabled;
        if (_enabled)
            onEnabled();
        else
            onDisabled();
    }
}


bool Element::isHidden() const
{
    return _hidden;
}


void Element::setHidden(bool hidden)
{
    if (_hidden != hidden)
    {
        _hidden = hidden;
        if (_hidden)
            onHidden();
        else
            onUnhidden();
    }
}


bool Element::isLocked() const
{
    return _locked;
}


void Element::setLocked(bool locked)
{
    if(_locked != locked)
    {
        _locked = locked;

        if (_locked)
            onLocked();
        else
            onUnlocked();
    }
}


std::string Element::getId() const
{
    return _id;
}


void Element::setId(const std::string& id)
{
    _id = id;
}


bool Element::hasAttribute(const std::string& name) const
{
    return _attributes.find(name) != _attributes.end();
}


const std::string Element::getAttribute(const std::string& name) const
{
    auto iter = _attributes.find(name);

    if (iter != _attributes.end())
    {
        return iter->second;
    }
    else
    {
        ofLogWarning("Element::hasAttribute") << "Attribute " << name << " not found.";
        return "";
    }
}


void Element::setAttribute(const std::string& name, const std::string& value)
{
    _attributes[name] = value;
    onAttributeSet(name, value);
}


void Element::clearAttribute(const std::string& name)
{
    _attributes.erase(name);
    onAttributeCleared(name);
}



void Element::setup()
{
    for (auto& child : _children) child->setup();
    onSetup();
}


void Element::update()
{
    if (isEnabled() && !isHidden())
    {
        for (auto& child : _children) child->update();
        onUpdate();
    }
}


void Element::draw()
{
    if (isEnabled() && !isHidden())
    {
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(_x, _y);
        for (auto& child : _children) child->draw();
        onDraw();
        ofPopMatrix();
        ofPopStyle();
    }
}


void Element::exit()
{
    for (auto& child : _children) child->exit();
    onExit();
}


bool Element::recursiveHitTest(const std::string& event,
                               const Position& screenPosition,
                               const Position& localPosition,
                               std::vector<Element*>& path)
{
    if (isEnabled() && !isHidden() && hitTest(localPosition))
    {
        if (isEventListener(event, false) || isEventListener(event, true))
        {
            path.push_back(this);
        }

        for (auto& child : _children)
        {
            // If a child hit is found, return immediately.
            if (child->recursiveHitTest(event,
                                        screenPosition,
                                        (localPosition - this->getPosition()),
                                        path))
            {
                return true;
            }
        }
        
        return false;
    }
    else
    {
        return false;
    }
}


void Element::setPointerCapture(std::size_t id)
{
    Document* d = document();

    if (nullptr != d)
    {
        d->setPointerCapture(this, id);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR);
    }
}


void Element::releasePointerCapture(std::size_t id)
{
    Document* d = document();

    if (nullptr != d)
    {
        d->releasePointerCapture(this, id);
    }
    else
    {
        throw DOMException(DOMException::INVALID_STATE_ERROR);
    }
}


} } // namespace ofx::DOM
