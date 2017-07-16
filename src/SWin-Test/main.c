#include <swin/SWin.h>
#include "glad/glad.h"
#include <stdio.h>

void buttonCallback(void* userData) {
    printf("%s\n", userData);
}

int main(int argc, const char * argv[]) {
    swInit();
    
    SWindow* window = swCreateWindow(800, 600, "Hello");
    //SView* rootView = swGetRootView(window);
    
    //SOpenGLView* glView = swCreateOpenGLView(swMakeRect(200, 0, 600, 600));
    //swSubView(rootView, glView);
    
    //SButton* button = swCreateButton(swMakeRect(0, 0, 200, 50), "Button", &buttonCallback, "Callback Test");
    //swSubView(rootView, button);
    
    //SLabel* label = swCreateLabel(swMakeRect(50, 200, 50, 50), "Hello, world!");
    //swSubView(rootView, label);
    
    double startTime = swGetTime();
    
    //swMakeContextCurrent(glView);
    
    if(!gladLoadGL()) {
        printf("Error loading OpenGL!\n");
        return -1;
    }
    
    uint32_t frames = 0;
    uint32_t fps = 0;

	glViewport(0, 0, 600, 600);

    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    
    while (!swCloseRequested(window)) {
        swPollEvents();
        
        double currentTime = swGetTime();
        
        frames++;
        if(currentTime - startTime >= 1) {
            startTime = currentTime;
            fps = frames;
            frames = 0;
            printf("%d\n", fps);
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();
        
        swSwapBufers(window);
        
		swDraw(window);
    }
    
    swCloseWindow(window);
    
    return 0;
}
