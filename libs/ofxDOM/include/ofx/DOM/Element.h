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
#include "ofx/DOM/CapturedPointer.h"
#include "ofx/DOM/Events.h"
#include "ofx/DOM/EventTarget.h"
#include "ofx/DOM/Exceptions.h"
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Document;
class AbstractLayout;


/// \brief A class representing a DOM Element.
///
///	There are several DOM coordiante systems with respect to an Element.
///
/// 1. Local Coordinates: The origin of the local coordiantes are at 0, 0 of the
/// Element Geometry.  The coordiantes of the local Geometry range from (0, 0)
/// - the upper left corner to (width, height) - the lower right corner of the
/// Element.
///
/// 2. Parent Coordinates: The (x, y) position of the Element geometry are in
/// "Parent" Coordinates, meaning they are oriented with respect to the
/// parent's Local Coordinates.
///
/// 3. Screen Coordinates: The screen coordinates are the coordinates (x, y)
/// in terms of the global screen coordiantes where (0, 0) is the upper left
/// corner of the screen / window, and (ofGetWidth(), ofGetHeight()) are the
/// coordiantes of the lower right corner.
class Element: public EventTarget<Element>
{
public:
    /// \brief Construct a new Element with the given parameters.
    ///
    /// The Element will take the default id, an empty string.
    ///
    /// \param x the X position of the Element in its parent coordinates.
    /// \param y the Y position of the Element in its parent coordinates.
    /// \param width the width of the Element.
    /// \param height the height of the Element.
    Element(float x, float y, float width, float height);

    /// \brief Construct a new Element with the given parameters.
    /// \param id The id of the Element.
    /// \param x the X position of the Element in its parent coordinates.
    /// \param y the Y position of the Element in its parent coordinates.
    /// \param width the width of the Element.
    /// \param height the height of the Element.
    Element(const std::string& id, float x, float y, float width, float height);

    /// \brief Destroy the Element.
    virtual ~Element();

    /// \brief Take ownership of the passed std::unique_ptr<Element>.
    ///
    /// This this is "sink" meaning that any child passed to this will be
    /// owned by this Node.
    ///
    /// \param element the rvalue reference to the child node.
	/// \returns A pointer to the added Element. The parent Element retains
	/// ownership of the pointer via a std::unique_ptr.
    template<typename ElementType>
    ElementType* addChild(std::unique_ptr<ElementType> element);

	/// \brief Create a child using a templated Element type.
	///
	/// To create a child Element you can use this method like:
	///
	/// ElementType* element = parentElement->addChild<ElementType>(arguments ...);
	///
	/// \tparam ElementType The subclass of Element that will be added.
	/// \tparam Args The variable constructor arguments for the ElementType.
	/// \param args The variable constructor arguments for the ElementType.
	/// \returns A pointer to the added Element. The parent Element retains
	/// ownership of the pointer via a std::unique_ptr.
	template <typename ElementType, typename... Args>
	ElementType* addChild(Args&&... args);

    /// \brief Release ownership of a child Element.
	/// \param element The Element to release.
	/// \returns a std::unique_ptr<Element> to the child.
    std::unique_ptr<Element> removeChild(Element* element);

    /// \brief Move this Element in front of all of its siblings.
    void moveToFront();

    /// \brief Move this Element in front of its next sibling.
    void moveForward();

    /// \brief Move this Element in back of all of its siblings.
    void moveToBack();

    /// \brief Move this Element in back of its next sibling.
    void moveBackward();

    /// \brief Move the given Element in front of all of its siblings.
    /// \param element The child element to move.
    /// \throws DOMException(DOMException::INVALID_STATE_ERROR) if no matching
    /// child element exists.
    void moveChildToFront(Element* element);

    /// \brief Move the given Element in front of its next sibling.
    /// \param element The child element to move.
    /// \throws DOMException(DOMException::INVALID_STATE_ERROR) if no matching
    /// child element exists.
    void moveChildForward(Element* element);

    /// \brief Move the given Element in back of all of its siblings.
    /// \param element The child element to move.
    /// \throws DOMException(DOMException::INVALID_STATE_ERROR) if no matching
    /// child element exists.
    void moveChildToBack(Element* element);

    /// \brief Move the given Element in back of its next sibling.
    /// \param element The child element to move.
    /// \throws DOMException(DOMException::INVALID_STATE_ERROR) if no matching
    /// child element exists.
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

	/// \brief Determine if this Element has a parent Element.
	/// \returns true if this Element has a parent Element.
	bool hasParent() const;

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

	/// \brief Get a pointer to the parent.
	/// \returns a pointer to the parent or a nullptr.
	const Element* parent() const;

    /// \brief Get a pointer to the parent Document.
    /// \returns a pointer to the parent Document, self if a Document or a nullptr.
    Document* document();

    /// \brief Get a pointer to the parent Document.
    /// \returns a pointer to the parent Document, self if a Document or a nullptr.
    const Document* document() const;

	/// \brief Perform a hit test on the Element.
	///
	/// For a normal Element, the hit test will test the rectangular geometry
	/// of the Element. Subclasses can override this method for custom hit test
	/// geometry.
	///
	/// Parent coordinates are used because the geometry / position of the
	/// Element are in parent coordinates.
	///
	/// \param parentPosition The Position to test in parent coordinates.
	/// \returns true iff the local position is within the hit test region.
    virtual bool hitTest(const Position& parentPosition) const;

	/// \brief Perform a hit test on a child Element.
	/// \param localPosition The Position to test in local coordinates.
	/// \returns true iff the local position is within the hit test region.
    virtual bool childHitTest(const Position& localPosition) const;

    /// \brief Convert the local coordinates to screen coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
	/// \param localPosition The local coordinates to convert.
    /// \returns the position converted from local to screen coordinates.
    Position localToScreen(const Position& localPosition) const;

    /// \brief Convert the screen coordinates to local coordinates.
    ///
    /// Local coordinates are defined with reference to the position of the box.
    /// The Position of this element will be in its parent's local coordinates.
    ///
	/// \param screenPosition The screen coordinates to convert.
    /// \returns the position converted from screen to local coordinates.
    Position screenToLocal(const Position& screenPosition) const;

	/// \brief Convert the parent coordinates to screen coordinates.
	/// \param parentPosition The parent coordinates to convert.
	/// \returns the position converted from parent to screen coordinates.
	Position parentToScreen(const Position& parentPosition) const;

	/// \brief Convert the screen coordinates to parent coordinates.
	/// \param screenPosition The screen coordinates to convert.
	/// \returns the position converted from screen to parent coordinates.
	Position screenToParent(const Position& screenPosition) const;

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

    /// \brief Get the X position of the Element in its parent coordinates.
    /// \returns the X position of the Element in its parent coordinates.
    float getX() const;

    /// \brief Get the Y position of the Element in its parent coordinates.
    /// \returns the Y position of the Element in its parent coordinates.
    float getY() const;

	/// \brief Get the Position of the Element in screen coordinates.
	/// \returns the Position of the Element in screen coordinates.
	/// \todo Cache screen position w/ geometry.
	Position getScreenPosition() const;

	/// \brief Get the X position of the Element in screen coordinates.
	/// \returns the X position of the Element in screen coordinates.
	float getScreenX() const;

	/// \brief Get the Y position of the Element in screen coordinates.
	/// \returns the Y position of the Element in screen coordinates.
	float getScreenY() const;

    /// \brief Set the size of the Element.
    /// \param width The new width of the Element.
    /// \param height The new height of the Element.
    void setSize(float width, float height);

    /// \brief Get the Size of the Element.
    /// \returns the Size of the Element.
    Size getSize() const;

    /// \brief Get the width of the Element.
    /// \returns The width of the Element.
    float getWidth() const;

    /// \brief Get the height of the Element.
    /// \returns The height of the Element.
    float getHeight() const;

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
    /// \returns the bounding box representing the union of the child geometry and the element geometry.
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
    /// \throws Poco::BadCastException if the types do not match.
    /// \param key The name of the attribute.
    /// \param inherit True if the Element should query its ancestors for the attribute.
    /// \returns The value corresponding to the key, or throws an exception.
    template<typename AnyType>
    AnyType getAttribute(const std::string& key, bool inherit = false) const;

    /// \brief Set a value for a named attribute.
    ///
    /// If the given attribute exists, it will be overwritten with the given
    /// value.
    ///
    /// \param name The name of the attribute.
    /// \param value The new value of the attribute called name.
    void setAttribute(const std::string& name, const Any& value);

    /// \brief Clear a named attribute.
    /// \param The name of the attribute to clear.
    void clearAttribute(const std::string& name);

    /// \brief Request that the parent Document capture the given pointer id.
    ///
    /// Captured pointers send all of their revents to the capturing Element.
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
	/// \brief Setup method called by parent Element.
    void _setup(ofEventArgs& e);

	/// \brief Update method called by parent Element.
	void _update(ofEventArgs& e);

	/// \brief Draw method called by parent Element.
	void _draw(ofEventArgs& e);

	/// \brief Exit method called by parent Element.
	void _exit(ofEventArgs& e);

    /// \brief A recursive hit test to find a target element.
    /// \param parentPosition The parent coordinates to test.
    /// \returns A pointer to the target Element or a nullptr if none found.
    Element* recursiveHitTest(const Position& parentPosition);

    /// \brief Find a child by a raw Element pointer.
    /// \param The pointer to the child.
    /// \returns An iterator pointing to the matching Element or the end.
    std::vector<std::unique_ptr<Element>>::iterator findChild(Element* element);

    /// \brief An optional pointer to a parent Node.
    Element* _parent;

    /// \brief A vector to Elements.
    std::vector<std::unique_ptr<Element>> _children;

    /// \brief The id for this element.
    std::string _id;

	/// \brief Determine if a given pointer id is captured.
	/// \param id The pointer id to test.
	/// \returns true if the given pointer id is captured.
	bool isPointerCaptured(std::size_t id) const;

	/// \brief Find the given CapturePointer info by id.
	/// \param id The pointer id to find.
	/// \returns An const iterator to the CapturedPointer info if found.
	std::vector<CapturedPointer>::iterator findCapturedPointerById(std::size_t id);

	/// \brief Find the given CapturePointer info by id.
	/// \param id The pointer id to find.
	/// \returns An const iterator to the CapturedPointer info if found.
	std::vector<CapturedPointer>::const_iterator findCapturedPointerById(std::size_t id) const;

	/// \brief Get a reference to the captured pointer vector.
	/// \returns a reference to all pointers captured by this Element.
	std::vector<CapturedPointer>& capturedPointers();

	/// \brief Get a reference to the captured pointer vector.
	/// \returns a reference to all pointers captured by this Element.
	const std::vector<CapturedPointer>& capturedPointers() const;

    /// \brief Called internally to invalidate the child geometry tree.
    void invalidateChildGeometry() const;

	/// \brief Set if the pointer is implicitly captured on pointer down.
	///
	/// This does not enable the pointer listener, only if the pointer should
	/// be automatically captured when a listener is enabled.
	///
	/// \param implicitPointerCapture True if the pointer should be captured.
	void setImplicitPointerCapture(bool implicitPointerCapture);

	/// \brief Determine if pointer is automatically captured on pointer down.
	/// \returns true iff implicit pointer capture is enabled.
	bool getImplicitPointerCapture() const;

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
    mutable bool _childGeometryInvalid;

    /// \brief The enabled state of this Element.
    bool _enabled;

    /// \brief The visibility of this Element.
    bool _hidden;

    /// \brief The locked state of this Element.
    bool _locked;

    /// \brief A collection of named attributes.
    std::unordered_map<std::string, Any> _attributes;

	/// \brief Automatically capture the pointer on pointer down.
	bool _implicitPointerCapture;

	/// \brief A list of the pointer ids currently captured by this Element.
	/// Captured pointers are pushed back, so the pointer at the front was the
	/// first pointer captured, and thus the primary pointer.
	std::vector<CapturedPointer> _capturedPointers;

    /// \brief A callback for child Elements to notify their parent of movement.
    void _onChildMoved(MoveEvent& evt);

    /// \brief A callback for child Elements to notify their parent size changes.
    void _onChildResized(ResizeEvent& evt);

	/// \brief Make Document a friend class.
	friend class Document;
};


template <typename ElementType, typename... Args>
ElementType* Element::addChild(Args&&... args)
{
	return addChild(std::make_unique<ElementType>(std::forward<Args>(args)...));
}


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

		// Invalidate all cached child geometry.
		invalidateChildGeometry();

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


template<typename AnyType>
AnyType Element::getAttribute(const std::string& key, bool inherit) const
{
    auto iter = _attributes.find(key);

    if (iter != _attributes.end() && iter->second.is<AnyType>())
    {
        return iter->second.as<AnyType>();
    }
    else if (inherit && hasParent())
    {
        return parent()->getAttribute<AnyType>(key);
    }
    else
    {
        throw DOMException(DOMException::INVALID_ATTRIBUTE_KEY);
    }
}


} } // namespace ofx::DOM
