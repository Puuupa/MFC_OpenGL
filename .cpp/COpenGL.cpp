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
*	PIXEL     --      ��д���ظ�ʽ������Gl������
*   input: ����һ��MFC�����ָ�루���ĵ��ģ����ĵ��ģ��Ի���ģ������ڴ���DC
*	output: nope
*   ps: ������Ȼ��壬���ҳ�ʼ����glew
*
******************************************************************************/
int COpenGL::PIXEL(CBaseDlg* pointer)
{
	//����ClientDC
	m_pDC = new CClientDC(pointer);
	ASSERT(m_pDC != NULL);
	//��д���ؽṹ��
	static PIXELFORMATDESCRIPTOR pfd =

	{

		sizeof(PIXELFORMATDESCRIPTOR),    //  pfd�ṹ�Ĵ�С
		1,                              // �汾��

		PFD_DRAW_TO_WINDOW |            // // ֧���ڴ����л�ͼ

		PFD_SUPPORT_OPENGL |            //֧��OpenGL

		PFD_DOUBLEBUFFER,                // ˫����ģʽ

		PFD_TYPE_RGBA,                  // RGBA ��ɫģʽ
		24,                             // 24 λ��ɫ���
		0, 0, 0, 0, 0, 0,               // ������ɫλ
		0,                              // û�з�͸���Ȼ���
		0,                              // ������λλ
		0,                              // ���ۼӻ���
		0, 0, 0, 0,                     // �����ۼ�λ
		32,                             // 32 λ��Ȼ���   
		0,                              // ��ģ�建��
		0,                              // �޸�������
		PFD_MAIN_PLANE,                 // ����
		0,                              // ����
		0, 0, 0                         // ���Բ�,�ɼ��Ժ������ģ

	};
	//ѡ�����ظ�ʽ
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)

	{
		return FALSE;
	}
	//�������ظ�ʽ
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	//GDI��ͼ��ʹ�õ�HDC������Ӧ��HGLRC  ,
	m_hglrc = ::wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hglrc == 0)
	{
		return -1;
	}
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hglrc) == FALSE)
	{
		return -1;
	}

	//����
	::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//1
	::glClearDepth(1.0f);
	//����Ȼ�����
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
*	Resize     --      ��������һ���ӿ�
*   input: ������
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
*	Resize     --      ��Ⱦ���
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
*	Destroy     --      �ƺ������DC֮�����������GL��ص�������
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
*	Shader     --      ��ɫ����Ŀǰֻ�ж����Ƭ�Σ�ʹ�õ���Ӳ���룬�������ܻ��װһ����
*   input: nope
*	output: nope
*   ps: ��������ӵı�����Ϣͨ��cout�������������Ҫ�ٸ�
*
**********************************************************************************/
bool COpenGL::Shader()
{
	//����Shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// ���������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Ƭ��Shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// ���������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ����!�ں�!
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// ������Ӵ���
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//���Ӻ󣬾Ͳ���Ҫ�ˣ�˳��Delete��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return true;
}



/********************************************************************************
*
*	DataBuff     --      �Զ������ݵĶ�ȡ����VAO��VBO
*   input: nope
*	output: nope
*   ps: �о�����д�����������ٸĽ�
*
**********************************************************************************/
bool COpenGL::DataBuff()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��VAO
	glBindVertexArray(VAO);
	//��VBO���������ݴ浽����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//������ô������ location Ϊ0   ���������ԣ�3��XYZ��,�������� float, ����Ҫ��׼��,����Ϊ3��ƫ����Ϊ0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}
