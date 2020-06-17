#include "pch.h"
#include "COpenGL.h"
#include "BaseDlg.h"


float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

COpenGL::COpenGL()
{
	m_pDC		= nullptr;
	m_hglrc		= NULL;
}

COpenGL::~COpenGL()
{
	m_pDC = nullptr;
}



/**************************************************************************
*
*	PIXEL     --      填写像素格式，创建Gl上下文
*   input: 传入一个MFC的类的指针（单文档的，多文档的，对话框的），用于创建DC
*	output: nope
*   ps: 打开了深度缓冲，而且初始化了glew
*
******************************************************************************/
int COpenGL::PIXEL(CBaseDlg* pointer)
{
	//创建ClientDC
	m_pDC = new CClientDC(pointer);
	ASSERT(m_pDC != NULL);
	//填写像素结构体
	static PIXELFORMATDESCRIPTOR pfd =

	{

		sizeof(PIXELFORMATDESCRIPTOR),    //  pfd结构的大小
		1,                              // 版本号

		PFD_DRAW_TO_WINDOW |            // // 支持在窗口中绘图

		PFD_SUPPORT_OPENGL |            //支持OpenGL

		PFD_DOUBLEBUFFER,                // 双缓存模式

		PFD_TYPE_RGBA,                  // RGBA 颜色模式
		24,                             // 24 位颜色深度
		0, 0, 0, 0, 0, 0,               // 忽略颜色位
		0,                              // 没有非透明度缓存
		0,                              // 忽略移位位
		0,                              // 无累加缓存
		0, 0, 0, 0,                     // 忽略累加位
		32,                             // 32 位深度缓存   
		0,                              // 无模板缓存
		0,                              // 无辅助缓存
		PFD_MAIN_PLANE,                 // 主层
		0,                              // 保留
		0, 0, 0                         // 忽略层,可见性和损毁掩模

	};
	//选择像素格式
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)

	{
		return FALSE;
	}
	//设置像素格式
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	//GDI绘图所使用的HDC创建对应的HGLRC  ,
	m_hglrc = ::wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hglrc == 0)
	{
		return -1;
	}
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc) == FALSE)
	{
		return -1;
	}

	//清屏
	::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//1
	::glClearDepth(1.0f);
	//打开深度缓冲区
	::glEnable(GL_DEPTH_TEST);

	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		AfxMessageBox(L"Init GLEW Failed\n");
		return -1;
	}

	return 0;
}


/**************************************************************************
*
*	Resize     --      重新设置一下视口
*   input: 传入宽高
*	output: nope
*   ps: 
*
******************************************************************************/
int COpenGL::Resize(int cx, int cy)
{
	::glViewport(0, 0, cx, cy);
	return 0;
}


/**************************************************************************
*
*	Resize     --      渲染相关
*   input: nope
*	output: nope
*   ps:
*
******************************************************************************/
void COpenGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, 3);
	SwapBuffers(m_pDC->GetSafeHdc());
}



/**************************************************************************
*
*	Destroy     --      善后操作，DC之类的在析构，GL相关的在这里
*   input: nope
*	output: nope
*   ps: 
*
******************************************************************************/
bool COpenGL::Destroy()
{
	::wglMakeCurrent(NULL, NULL);
	if (m_hglrc)
		::wglDeleteContext(m_hglrc);
	if (m_pDC)
		delete m_pDC;
	return true;
}



/********************************************************************************
*
*	Shader     --      着色器，目前只有顶点和片段，使用的是硬编码，后续可能会封装一个类
*   input: nope
*	output: nope
*   ps: 编译和链接的报错信息通过cout输出，后续有需要再改
*
**********************************************************************************/
bool COpenGL::Shader()
{
	//顶点Shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// 检查编译错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 片段Shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// 检查编译错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 链接!融合!
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// 检查链接错误
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//链接后，就不需要了，顺便Delete掉
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}



/********************************************************************************
*
*	DataBuff     --      对顶点数据的读取，绑定VAO，VBO
*   input: nope
*	output: nope
*   ps: 感觉这样写不够灵活，后续再改进
*
**********************************************************************************/
bool COpenGL::DataBuff()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//绑定VAO
	glBindVertexArray(VAO);
	//绑定VBO，并把数据存到里面
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//数据怎么解析， location 为0   ，顶点属性，3（XYZ）,数据类型 float, 不需要标准化,步长为3，偏移量为0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//解绑VBO，VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}
