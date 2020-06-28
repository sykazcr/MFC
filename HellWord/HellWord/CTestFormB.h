#pragma once



// CTestFormB フォーム ビュー

class CTestFormB : public CFormView
{
	DECLARE_DYNCREATE(CTestFormB)

protected:
	CTestFormB();           // 動的生成で使用される protected コンストラクター
	virtual ~CTestFormB();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTestFormB };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedButton2Createproc();
	afx_msg void OnBnClickedButton2Createthread();
};


