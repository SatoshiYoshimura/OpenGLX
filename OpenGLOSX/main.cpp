//
//  main.cpp
//  OpenGLOSX
//
//  Created by 吉村 智志 on 7/5/15.
//  Copyright (c) 2015 吉村 智志. All rights reserved.
//
#define GLFW_DLL

#include "glfw.h"
#include <cstdlib>

#if defined (_MSC_VER)
#pragma comment(lib,"GLFWDLL.lib")
#pragma comment(lib,"opengl32.lib")
#endif
int main(int argc,const char * argv[]){
    //初期化
    if(!glfwInit()){
        return EXIT_FAILURE;
    }

    if(!glfwOpenWindow(0, 0,
                       0, 0, 0,
                       0,
                       0, 0,
                       GLFW_WINDOW)){
   
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwSwapInterval(1);
    
    while (glfwGetWindowParam(GLFW_OPENED)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //点
        //        static const GLfloat vtx[] = { 0.0f, 0.0f };
        //線
        //頂点座標群
        static const GLfloat vtx[] = {
            -0.5f, -0.5f,
            0.5f, 0.5f
        };
        
        glVertexPointer(2, GL_FLOAT, 0, vtx);
        //太さ
//        glPointSize(4.0f);
        glLineWidth(4.0f);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
        glEnableClientState(GL_VERTEX_ARRAY);
        //ここの引数の2とは？
        glDrawArrays(GL_LINES, 0, 2);
        glDisableClientState(GL_VERTEX_ARRAY);
        glfwSwapBuffers();
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}