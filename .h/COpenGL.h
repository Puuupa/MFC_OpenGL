#pragma once
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <Gl/gl.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CBaseDlg;

/***************************************
* 
*   封装的OpenGL类
*   COpenGL()			DC指针初始化一下
*	~COpenGL()			DC指针善后
*	PIXEL()				填写像素格式
*	Resize()			窗口改变，传入窗口的宽高
*	Shader()			Shader,顶点和片段，对编译和链接都有报错(后续可能会封装Shader类)
*	Destroy()           不同于析构，会把OpenGL相关的资源释放
*	DataBuff()			读取顶点数据，绑定VAO,VBO,解析顶点属性
*	Render()			渲染!
*
***************************************/

class COpenGL
{
public:
	COpenGL();
	~COpenGL();

	int PIXEL(CBaseDlg* pointer);
	int Resize(int cx,int cy);
	void Render();
	bool Destroy();
	bool Shader();
	bool DataBuff();

private:
	CClientDC* m_pDC;
	HGLRC m_hglrc;
	int shaderProgram;
	unsigned int VBO, VAO;

};

