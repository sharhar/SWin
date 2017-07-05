//
//  SWin.h
//  OBJCTest
//
//  Created by Shahar Sandhaus on 7/5/17.
//  Copyright © 2017 Shahar Sandhaus. All rights reserved.
//

#ifndef SWin_h
#define SWin_h

typedef void SWindow;

void initSWin();

SWindow* createSWindow(int width, int height, const char* title);

void pollEvents(SWindow* window);
void draw(SWindow* window);

void terminateSWin();

#endif /* SWin_h */
