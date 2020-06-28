
// HellWordView.cpp : CHellWordView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "HellWord.h"
#endif

#include "HellWordDoc.h"
#include "HellWordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHellWordView

IMPLEMENT_DYNCREATE(CHellWordView, CView)

BEGIN_MESSAGE_MAP(CHellWordView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CHellWordView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CHellWordView コンストラクション/デストラクション

CHellWordView::CHellWordView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CHellWordView::~CHellWordView()
{
}

BOOL CHellWordView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CHellWordView 描画

void CHellWordView::OnDraw(CDC* pDC)
{
	CHellWordDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
//	pDC->TextOutW(600, 100, L"雲深不知処");
}


// CHellWordView の印刷


void CHellWordView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CHellWordView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CHellWordView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CHellWordView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}

void CHellWordView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHellWordView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHellWordView の診断

#ifdef _DEBUG
void CHellWordView::AssertValid() const
{
	CView::AssertValid();
}

void CHellWordView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHellWordDoc* CHellWordView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHellWordDoc)));
	return (CHellWordDoc*)m_pDocument;
}
#endif //_DEBUG


// CHellWordView メッセージ ハンドラー


void CHellWordView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	AfxMessageBox(_T("Hello there"));

	CView::OnLButtonDown(nFlags, point);
}
