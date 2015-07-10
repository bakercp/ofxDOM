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


#include <unordered_set>
#include "ofEvents.h"
#include "ofRectangle.h"
#include "ofx/PointerEvents.h"
#include "ofx/DOM/Events.h"
#include "ofx/DOM/EventListener.h"
#include "ofx/DOM/Exceptions.h"
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {



class Exception: public std::exception
{

};



class Document;
class AbstractLayout;


class Element: public EventListener
{
public:
    Element(float x, float y, float width, float height);

    Element(const std::string& id, float x, float y, float width, float height);

    virtual ~Element();

    /// \brief Take ownership of the passed std::unique_ptr<Element>.
    ///
    /// This this is "sink" meaning that any child passed to this will be
    /// owned by this Node.
    ///
    /// \param nodePtr the rvalue reference to the child node.
    ///
    /// \returns a pointer to the child that was attached.
    Element* attachChild(std::unique_ptr<Element> element);

    /// \brief Release ownership
    std::unique_ptr<Element> releaseChild(Element* element);

    void moveToFront();
    void moveForward();
    void moveToBack();
    void moveBackward();

    void moveChildToFront(Element* element);
    void moveChildForward(Element* element);
    void moveChildToBack(Element* element);
    void moveChildBackward(Element* element);

    bool isChild(Element* element) const;
    bool isSibling(Element* element) const;
    bool isParent(Element* element) const;

    Element* findFirstChildById(const std::string& id);
    std::vector<Element*> findChildrenById(const std::string& id);

    /// \brief Get a pointer to the parent.
    /// \returns a pointer to the parent or a nullptr.
    Element* parent();

    /// \brief Get a pointer to the parent Document.
    /// \returns a pointer to the parent Document or a nullptr.
    Document* document();

    /// \returns true iff Element has no parent (i.e. parent is nullptr).
    bool isRoot() const;

    virtual bool hitTest(const Position& localPosition) const;

    /// \brief Get the Position of this Element in screen coordinates.
    /// \returns the Position of this Element in screen coordinates.
    Position screenPosition() const;

    /// \brief Convert the local coordinates to screen coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
    /// \returns the position converted from local to screen coordinates.
    Position localToScreen(const Position& localPosition) const;

    /// \brief Convert the screen coordinates to local coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
    /// \returns the position converted from screen to local coordinates.
    Position screenToLocal(const Position& screenPosition) const;

    void setPosition(float x, float y);
    void setPosition(const Position& position);

    /// \brief Get the position of the Element in its parent coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
    /// \returns The position in parent coordinates.
    Position getPosition() const;

    void setSize(float width, float height);
    Size getSize() const;

    Geometry getGeometry() const;
    void setGeometry(const Geometry& geometry);

    /// \returns true iff the Element is enabled.
    bool isEnabled() const;
    void setEnabled(bool enabled);

    /// \returns true iff the Element is hidden.
    bool isHidden() const;
    void setHidden(bool hidden);

    /// \returns true iff the Element is locked.
    bool isLocked() const;
    void setLocked(bool locked);

    std::string getId() const;
    void setId(const std::string& id);

    bool hasAttribute(const std::string& name) const;
    const std::string getAttribute(const std::string& name) const;
    void setAttribute(const std::string& name, const std::string& value);
    void clearAttribute(const std::string& name);

    void setPointerCapture(std::size_t id);
    void releasePointerCapture(std::size_t id);

protected:
    void setup();
    void update();
    void draw();
    void exit();

    bool recursiveHitTest(const std::string& event,
                          const Position& screenPosition,
                          const Position& localPosition,
                          std::vector<Element*>& path);

    /// \brief Find a child by a raw Element pointer.
    /// \param The pointer to the child.
    /// \returns An iterator pointing to the matching Element or the end.
    std::vector<std::unique_ptr<Element>>::iterator findChild(Element* element);

    /// \brief An optional pointer to a parent Node.
    Element* _parent = nullptr;

    /// \brief A vector to Elements.
    std::vector<std::unique_ptr<Element>> _children;

    std::string _id;

    float _x = 0;
    float _y = 0;
    float _width = 0;
    float _height = 0;

    bool _enabled = true;
    bool _locked = false;
    bool _hidden = false;

    /// \brief A collection of named attributes.
    std::unordered_map<std::string, std::string> _attributes;

private:
    /// \brief Not construction-copyable.
    Element(const Element& other) = delete; // non-construction-copyable

    /// \brief Non copyable.
    Element& operator = (const Element&) = delete;

};


} } // namespace ofx::DOM
