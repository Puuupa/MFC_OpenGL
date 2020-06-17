﻿
// BaseDlg.h: 头文件
//
#pragma once
#include "COpenGL.h"

// CBaseDlg 对话框
class CBaseDlg : public CDialogEx
{
// 构造
public:
	CBaseDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	COpenGL m_opengl;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
