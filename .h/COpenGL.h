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
*   ��װ��OpenGL��
*   COpenGL()			DCָ���ʼ��һ��
*	~COpenGL()			DCָ���ƺ�
*	PIXEL()				��д���ظ�ʽ
*	Resize()			���ڸı䣬���봰�ڵĿ��
*	Shader()			Shader,�����Ƭ�Σ��Ա�������Ӷ��б���(�������ܻ��װShader��)
*	Destroy()           ��ͬ�����������OpenGL��ص���Դ�ͷ�
*	DataBuff()			��ȡ�������ݣ���VAO,VBO,������������
*	Render()			��Ⱦ!
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

