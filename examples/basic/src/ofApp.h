//
// Copyright (c) 2009 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofMain.h"
#include "ofxDOM.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void draw() override;

    ofxDOM::Document document;

};
