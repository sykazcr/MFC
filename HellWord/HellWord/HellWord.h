
// HellWord.h : HellWord アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // メイン シンボル


// CHellWordApp:
// このクラスの実装については、HellWord.cpp を参照してください
//

class CHellWordApp : public CWinAppEx
{
public:
	CHellWordApp() noexcept;


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHellWordApp theApp;
