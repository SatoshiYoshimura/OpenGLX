//
//  main.cpp
//  OpenGLOSX
//
//  Created by 吉村 智志 on 7/5/15.
//  Copyright (c) 2015 吉村 智志. All rights reserved.
//
#define GLFW_DLL
#define GLEW_STATIC

#include "glew.h"
#include "glfw.h"
#include <cstdlib>
#include <fstream>
#include <vector>

#if defined (_MSC_VER)
#pragma comment(lib,"GLFWDLL.lib")
#pragma comment(lib,"opengl32.lib")
#ifdef _DEBUG
#pragma comment(lib,"glew32sd,lib")
#else
#pragma comment(lib,"glew32s.lib")
#endif
#endif

bool setupTexture(const GLuint id, const char* file, const int width, const int height){
    
    std::ifstream fstr(file,std::ios::binary);
    
    if(!fstr) {
        return false;
    }
    
    //ファイルの最初から最後まで読み込んだときのサイズ？
    const size_t file_size = static_cast<size_t> (fstr.seekg(0,fstr.end).tellg());
    //開始位置に戻す
    fstr.seekg(0, fstr.beg);
    
    //画像がなんでchar型のvecterに保存できる
    /*
     ★CHAR = char
     1バイト（8ビット）　-128～127
     
     ★BYTE = unsigned char
     1バイト（8ビット）　0～255
     */
    //byteではだめか？後ほど試してみる
    std::vector<char> texture_buffer(file_size);
    
    fstr.read(&texture_buffer[0],file_size);
    
    glBindTexture(GL_TEXTURE_2D,id);
    
    //１ピクセルに赤,翠,青,αの情報
    //幅256,高さ256　転送
    //2Dて関数名に書いてあるのになんでわざわざ引数で指定しなきゃならんのか？
    glTexImage2D(
                 GL_TEXTURE_2D,
                 0,
                 //テクスチャをOpenGL内部で保持する形式?
                 GL_RGBA,
                 width ,height,
                 0,
                 //引数pixelsに指定したメモリ上の画像形式? GLでのテクスチャの保持と画像保持は別もの・
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &texture_buffer[0]
                 );
    
    //画像拡大時の振る舞い
    glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR
                    );
    
    //画像が縮小された場合の振る舞い
    glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    
    //丸め込み方法 //色々試すと面白い
    
    glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_MIRRORED_REPEAT
    );
    glTexParameteri(
                    GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_MIRRORED_REPEAT
                    );
    
    
    
    return true;
}

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
#if defined (_MSC_VER)
    if(glewInit() != GLEW_OK){
        glfwTerminate();
        return EXIT_FAILURE;
    }
#endif
    
    //OpenGLにtexture識別子を１つ作ってもらう
    GLuint texture_id;
//    GLuint texture_idArr[10];

    glGenTextures(1,&texture_id);
    
    //画像ファイルを読み込んでテクスチャ識別しに設定する
    if(!setupTexture(texture_id,"sample.raw",256,256)){
    //失敗した後始末
        glDeleteTextures(1, &texture_id);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    while( glfwGetWindowParam(GLFW_OPENED))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        static const GLfloat vtx[] = {
            -0.8f, -0.8f,
            0.8f,  -0.8f,
            0.8f,  0.8f,
            -0.8f, 0.8f
        };
        
        glVertexPointer(2, GL_FLOAT, 0, vtx);
        
        //頂点ごとのテクスチャ座標　UV
        static const GLfloat texture_uv[] = {
            -2.5f, 0.0f,
            2.5f, 0.0f,
            2.5f, 5.0f,
            -2.5f, 5.0f
        };
        glTexCoordPointer(2,GL_FLOAT, 0, texture_uv);
        
        //OpenGLに
        glEnable(GL_TEXTURE_2D);
//        glColor4f(0.2f,0.3f,1.0f,1.0f);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
        
        glfwSwapBuffers();
    }
    
    //TODO:田中さんにテクスチャ識別子以上の数指定したら内部でどうなるか聞く
    //試しても落ちなかった。内部のメモリは？
    glDeleteTextures(1,&texture_id);
    glfwTerminate();
    
    return EXIT_SUCCESS;
    
//    glfwSwapInterval(1);
//    
//    while (glfwGetWindowParam(GLFW_OPENED)) {
//        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        //点
//        //        static const GLfloat vtx[] = { 0.0f, 0.0f };
//        //線
//        //頂点座標群
//        static const GLfloat vtx[] = {
//            -0.5f, -0.5f,
//            0.5f, 0.5f
//        };
//        
//        glVertexPointer(2, GL_FLOAT, 0, vtx);
//        //太さ
////        glPointSize(4.0f);
//        glLineWidth(4.0f);
//        glColor4f(1.0f,1.0f,1.0f,1.0f);
//        glEnableClientState(GL_VERTEX_ARRAY);
//        //ここの引数の2とは？
//        glDrawArrays(GL_LINES, 0, 2);
//        glDisableClientState(GL_VERTEX_ARRAY);
//        glfwSwapBuffers();
//    }
//    
//    glfwTerminate();
//    
//    return EXIT_SUCCESS;
}