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

    void setPointerCapture(Element* element, std::size_t id);
    void releasePointerCapture(Element* element, std::size_t id);

protected:
    /// \brief Captured pointer and their capture target.
    std::unordered_map<std::size_t, Element*> _capturedPointerIdToElementMap;

//	std::unordered_map<Element*, std::vector<CapturedPointer>> _elementPointerMap;

    /// \brief Currently active targets.
	///
	/// It is not required that these targets are captured, only that they
	/// passed a hit test. This is used for doing pointer in / pointer out, etc
	/// when the pointer isn't captured (e.g. mouse over, etc).
    std::unordered_map<std::size_t, Element*> _activeTargets;

    /// \brief Currently active pointers and their last associated event.
	///
	/// This map is a basic list of the pointers that we know about.
    std::unordered_map<std::size_t, PointerEventArgs> _activePointers;

};


} } // namespace ofx::DOM
