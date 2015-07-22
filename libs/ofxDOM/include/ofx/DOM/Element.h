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
#include "ofx/PointerEvents.h"
#include "ofx/DOM/Events.h"
#include "ofx/DOM/EventTarget.h"
#include "ofx/DOM/Exceptions.h"
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Document;
class AbstractLayout;


class Element: public EventTarget<Element>
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
    template<typename ElementType>
    ElementType* addChild(std::unique_ptr<ElementType> element);
    
    /// \brief Release ownership
    std::unique_ptr<Element> removeChild(Element* element);

    void moveToFront();
    void moveForward();
    void moveToBack();
    void moveBackward();

    void moveChildToFront(Element* element);
    void moveChildForward(Element* element);
    void moveChildToBack(Element* element);
    void moveChildBackward(Element* element);

    /// \brief Determine if the given Element is a child of this Element.
    /// \param element A pointer the the Element to test.
    /// \returns true iff the given element is a child of this Element.
    bool isChild(Element* element) const;

    /// \brief Determine if the given Element is a sibling of this Element.
    /// \param element A pointer the the Element to test.
    /// \returns true iff the given element is a sibling of this Element.
    bool isSibling(Element* element) const;

    /// \brief Determine if the given Element is the parent of this Element.
    /// \param element A pointer the the Element to test.
    /// \returns true iff the given element is the parent of this Element.
    bool isParent(Element* element) const;

    /// \brief Determine if this Element has any children.
    /// \returns true iff this Element has any children.
    bool hasChildren() const;

    /// \brief Find this Element's first child by its id.
    /// \param id The id of the child Element to find.
    /// \returns a pointer to the first child or nullptr if no such child exists.
    Element* findFirstChildById(const std::string& id);

    /// \brief Find all of this Element's matching the given id.
    /// \param id The id of the child Elements to find.
    /// \returns a vector of pointers to child elements or an empty vector if none exist.
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

    virtual bool childHitTest(const Position& localPosition) const;

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

    /// \brief Set the position of the Element in its parent coordinates.
    /// \param x The new x position.
    /// \param y The new y position.
    void setPosition(float x, float y);

    /// \brief Set the position of the Element in its parent coordinates.
    /// \param position The new position.
    void setPosition(const Position& position);

    /// \brief Get the position of the Element in its parent coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
    /// \returns The position in parent coordinates.
    Position getPosition() const;

    /// \brief Set the size of the Element.
    /// \param width The new width of the Element.
    /// \param height The new height of the Element.
    void setSize(float width, float height);

    /// \brief Get the Size of the Element.
    /// \returns the Size of the Element.
    Size getSize() const;

    /// \brief Get the geometry of the Element in its parent coordinates.
    /// \returns the Geometry of the Element in its parent coordinates.
    Geometry getGeometry() const;

    /// \brief Set the geometry of the Element in its parent coordinates.
    /// \param geometry The new geometry of the Element in its parent coordinates.
    void setGeometry(const Geometry& geometry);

    /// \brief Get the bounding box representing all child elements.
    /// \returns the bounding box representing all child elements, or
    /// a rectangle of zero width and height at the origin if no children.
    Geometry getChildGeometry() const;

    /// \brief Get the bounding box representing the union of the child geometry and the element geometry.
    
    Geometry getTotalGeometry() const;

    /// \brief Get the id of this Element.
    ///
    /// The id is optional and an empty std::string by default.
    ///
    /// \returns the id of the Element.
    std::string getId() const;

    /// \brief Set the id of the Element.
    /// \param id The new id of the Element.
    void setId(const std::string& id);

    /// \brief Determine of the Element has an attribute with the given name.
    /// \param name The name of the Attribute to query.
    /// \returns true iff the Element has an attribute with the given name.
    bool hasAttribute(const std::string& name) const;

    /// \brief Get a named attribute via its key.
    ///
    /// Users should check to see if the attribute exists using hasAttribute or
    /// catch the DOMException.
    ///
    /// \throws DOMException if no such key.
    /// \param key The name of the attribute.
    /// \returns The value corresponding to the key, or throws an exception if
    /// non found.
    const std::string getAttribute(const std::string& key) const;

    /// \brief Set a value for a named attribute.
    ///
    /// If the given attribute exists, it will be overwritten with the given
    /// value.
    ///
    /// \param name The name of the attribute.
    /// \param value The new value of the attribute called name.
    void setAttribute(const std::string& name, const std::string& value);

    /// \brief Clear a named attribute.
    /// \param The name of the attribute to clear.
    void clearAttribute(const std::string& name);

    /// \brief Request that the parent Document capture the given pointer id.
    ///
    /// Captured pointers send all of thei revents to the capturing Element.
    ///
    /// \param id The pointer id to capture.
    void setPointerCapture(std::size_t id);

    /// \brief Release a captured pointer with the given id.
    ///
    /// Usually this call is not required as the parent Document's pointer
    /// dispatching system will automatically release a pointer on the next
    /// POINTER_UP event.  In some cases, the user may want to explicity release
    /// the event to fire the associated lostPointerCapture events.
    ///
    /// \param id The pointer id to release.
    void releasePointerCapture(std::size_t id);

    /// \returns true iff the Element is enabled.
    bool isEnabled() const;

    /// \brief Enable or disable this Element.
    /// \param enabled The enabled state to set. True to enable, false to disable.
    void setEnabled(bool enabled);

    /// \returns true iff the Element is hidden.
    bool isHidden() const;

    /// \brief Hide or show this Element.
    /// \param hidden The visible state to set. True to hide, false to show.
    void setHidden(bool hidden);

    /// \returns true iff the Element is locked.
    bool isLocked() const;

    /// \brief Lock or unlock a this Element.
    /// \param locked The locked state to set. True to lock, false to unlock.
    void setLocked(bool locked);

protected:
    void setup();
    void update();
    void draw();
    void exit();

    Element* recursiveHitTest(const Position& localPosition);

    /// \brief Find a child by a raw Element pointer.
    /// \param The pointer to the child.
    /// \returns An iterator pointing to the matching Element or the end.
    std::vector<std::unique_ptr<Element>>::iterator findChild(Element* element);

    /// \brief An optional pointer to a parent Node.
    Element* _parent = nullptr;

    /// \brief A vector to Elements.
    std::vector<std::unique_ptr<Element>> _children;

    /// \brief The id for this element.
    std::string _id;

    /// \brief A list of the pointer ids currently captured by this Element.
    std::unordered_set<std::size_t> _capturedPointers;

    void invalidateChildGeometry() const;

private:
    /// \brief Not construction-copyable.
    Element(const Element& other) = delete; // non-construction-copyable

    /// \brief Non copyable.
    Element& operator = (const Element&) = delete;

    /// \brief The basic geometry of this element.
    Geometry _geometry;

    /// \brief The union of all child geometries.
    mutable Geometry _childGeometry;

    /// \brief True if the child geometry is invalid.
    ///
    /// This variable usually set by callbacks from the child elements.
    mutable bool _childGeometryInvalid = true;

    /// \brief The enabled state of this Element.
    bool _enabled = true;

    /// \brief The visibility of this Element.
    bool _hidden = false;

    /// \brief The locked state of this Element.
    bool _locked = false;

    /// \brief A collection of named attributes.
    std::unordered_map<std::string, std::string> _attributes;

    /// \brief A callback for child Elements to notify their parent of movement.
    void _onChildMoved(MoveEvent& evt);

    /// \brief A callback for child Elements to notify their parent size changes.
    void _onChildResized(ResizeEvent& evt);

};


template<typename ElementType>
ElementType* Element::addChild(std::unique_ptr<ElementType> element)
{
    if (nullptr != element)
    {
        // Get a raw pointer to the node for later.
        ElementType* pNode = element.get();

        // Assign the parent to the node via the raw pointer.
        pNode->_parent = this;

        // Take ownership of the node.
        _children.push_back(std::move(element));

        // Alert the node that its parent was set.
        ElementEvent addedEvent(this);
        ofNotifyEvent(pNode->addedTo, addedEvent, this);

        ElementEvent childAddedEvent(pNode);
        ofNotifyEvent(childAdded, childAddedEvent, this);

        // Attach child listeners.
        ofAddListener(pNode->move, this, &Element::_onChildMoved);
        ofAddListener(pNode->resize, this, &Element::_onChildResized);

        /// Alert the node's siblings that they have a new sibling.
        for (auto& child : _children)
        {
            // Don't alert itself.
            if (child.get() != pNode)
            {
                ElementEvent evt(pNode);
                ofNotifyEvent(child->siblingAdded, evt, this);
            }
        }

        return pNode;
    }
    else
    {
        return nullptr;
    }
}




} } // namespace ofx::DOM
