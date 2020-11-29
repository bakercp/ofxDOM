//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier: MIT
//


#include "ofx/DOM/Layout.h"
#include "ofx/DOM/Element.h"
#include <iostream>


namespace ofx {
namespace DOM {


Layout::Layout(Element* owner): _owner(owner)
{
}


Layout::~Layout()
{
}


Element* Layout::owner()
{
    return _owner;
}


bool Layout::isDoingLayout() const
{
    return _isDoingLayout;
}


//std::vector<Element*> Layout::children()
//{
//    if (_parent)
//    {
//        return _parent->children();
//    }
//
//    std::vector<Element*> _children;
//    return _children;
//}


FlowLayout::FlowLayout(Element* owner): Layout(owner)
{
}


FlowLayout::~FlowLayout()
{
}


void FlowLayout::doLayout()
{
    if (_owner && !_isDoingLayout)
    {
//        std::cout << std::string(_owner->depth(0), '\t') << "DOING LAYOUT FOR: " << _owner->getId() << std::endl;
        
        // Prevent recursive calls to doLayout.
        _isDoingLayout = true;

        float totalWidth = 0;
        float totalHeight = 0;
        float currentX = 0;
        float currentY = 0;

        for (Element* element : _owner->children())
        {
            if (element)
            {
                std::string positionAttr = element->getAttribute<std::string>("position", "static");
                
                
                if (positionAttr == "absolute")
                {
                    element->setPosition(element->getAttribute<float>("left", 0.0f),
                                         element->getAttribute<float>("top", 0.0f));

                    // Don't change the position.
                }
                else if (positionAttr == "static")
                {
                    float elementWidth = element->getAttribute<float>("width", 0.0f);
                    float elementHeight = element->getAttribute<float>("height", 0.0f);

//                    std::cout << "elementWidth " << elementWidth << std::endl;
//                    std::cout << "elementHeight " << elementHeight << std::endl;

                    elementWidth = std::max(element->getAttribute<float>("min-width", elementWidth), elementWidth);
                    elementWidth = std::min(element->getAttribute<float>("max-width", elementWidth), elementWidth);
                    elementHeight = std::max(element->getAttribute<float>("min-height", elementHeight), elementHeight);
                    elementHeight = std::min(element->getAttribute<float>("max-height", elementHeight), elementHeight);
                    
//                    std::cout << "elementWidth " << elementWidth << std::endl;
//                    std::cout << "elementHeight " << elementHeight << std::endl;

                    
                    element->setPosition(currentX, currentY);
                    totalWidth = std::max(totalWidth, elementWidth);
                    currentY += elementHeight;
                    totalHeight = currentY;

//                    std::cout << std::string(_owner->depth(0), '\t') << " this one " << element->getId() << " " << currentX << " / " << currentY << std::endl;

                }

            }
        }

        _owner->setSize(totalWidth, totalHeight);

//        std::cout << "X" << std::string(_owner->depth(0), '\t') << _owner->getId() << " " << totalWidth << "/" << totalHeight << std::endl;

        _isDoingLayout = false;
    }
    else
    {
//        std::cout << "NO OWNER FOR LAYOUT" << std::endl;
    }
}






















BoxLayout::BoxLayout(Element* parent, Orientation orientation):
    Layout(parent),
    _orientation((orientation == Orientation::DEFAULT) ? Orientation::HORIZONTAL : orientation)
{
}


BoxLayout::~BoxLayout()
{
}


void BoxLayout::doLayout()
{
    if (_owner && !_isDoingLayout)
    {
        // Prevent recursive calls to doLayout.
        _isDoingLayout = true;

        float totalWidth = 0;
        float totalHeight = 0;
        float currentX = 0;
        float currentY = 0;

        for (Element* element : _owner->children())
        {
            if (element)
            {
                element->setPosition(currentX, currentY);

                if (_orientation == Orientation::HORIZONTAL)
                {
                    totalHeight = std::max(totalHeight, element->getHeight());
                    currentX += element->getWidth();
                    totalWidth = currentX;
                }
                else
                {
                    totalWidth = std::max(totalWidth, element->getWidth());
                    currentY += element->getHeight();
                    totalHeight = currentY;
                }
            }
        }

        _owner->setSize(totalWidth, totalHeight);

        _isDoingLayout = false;
    }
}


Orientation BoxLayout::orientation() const
{
    return _orientation;
}


} } // namespace ofx::DOM
