//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#pragma once


#include <vector>
#include "ofx/DOM/Types.h"


namespace ofx {
namespace DOM {


class Element;


/// \brief A base class for laying out child Elements.
///
/// Generally this class and its subclasses should not be instantiated directly
/// but instead should be created using Element::createLayout<LayoutType>(...).
/// This pattern preserves the ownership of the Layout.
class Layout
{
public:
    /// \brief Create a Layout for the children of a given Element.
    /// \param owner The parent Element whose child elements will be laid out.
    Layout(Element* owner);

    /// \brief Destroy the layout.
    virtual ~Layout();

    /// \returns a pointer to the parent Element or nullptr if none.
    Element* owner();

    /// \returns true iff this layout is currently being done.
    bool isDoingLayout() const;

//    /// \brief Get all of the children for this element.
//    /// \returns all children for this element.
//    std::vector<Element*> children();

    /// \brief Do the layout.
    virtual void doLayout() = 0;

protected:
    /// \brief The owning  Element.
    Element* _owner = nullptr;

    /// \brief True if in doLayout(). Used to prevent recursive calls.
    bool _isDoingLayout = false;

    friend class Element;

};


/// \brief The FlowLayout uses LayoutProperties properties to do the Layout.
class FlowLayout: public Layout
{
public:
    FlowLayout(Element* parent);

    virtual ~FlowLayout();

    virtual void doLayout() override;

};


class BoxLayout: public Layout
{
public:
    /// If the Orientation::DEFAULT is chosen, the default will be set to
    /// Orientation::HORIZONTAL.
    BoxLayout(Element* parent, Orientation orientation = Orientation::VERTICAL);

    virtual ~BoxLayout();

    virtual void doLayout() override;

    /// \returns the box layout orientation.
    Orientation orientation() const;

protected:
    /// \brief The box layout orientation.
    Orientation _orientation = Orientation::HORIZONTAL;

};


} } // ofx::DOM
