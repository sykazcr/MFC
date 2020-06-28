// CTestFormB.cpp : 実装ファイル
//

#include "pch.h"
#include "HellWord.h"
#include "CTestFormB.h"


// CTestFormB

IMPLEMENT_DYNCREATE(CTestFormB, CFormView)

CTestFormB::CTestFormB()
	: CFormView(IDD_CTestFormB)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CTestFormB::~CTestFormB()
{
}

void CTestFormB::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestFormB, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestFormB::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON2_CREATEPROC, &CTestFormB::OnBnClickedButton2Createproc)
	ON_BN_CLICKED(IDC_BUTTON2_CREATETHREAD, &CTestFormB::OnBnClickedButton2Createthread)
END_MESSAGE_MAP()


// CTestFormB 診断

#ifdef _DEBUG
void CTestFormB::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTestFormB::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTestFormB メッセージ ハンドラー

#include <vector>
#include <iostream>
#include <fstream>

#pragma warning(suppress : 4996)

void CTestFormB::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	std::vector<int> v{ 1,2,3,4,5 };
	int a{ 100 };
	for (const auto& i : v) {
		std::cout << i << std::endl;
	}
//	AllocConsole();
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
	std::ofstream log;
	log.open("log.txt");
	log << "Writing this to a file.\n";
	log.close();

	AfxMessageBox(_T("Hello there 2"));
	//CPaintDC dc(this); // device context for painting
	//dc.TextOutW(500, 100, L"只在此山中");
	//Invalidate();
	//RefreshConstants();

}


void CTestFormB::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ここにメッセージ ハンドラー コードを追加します。
					   // 描画メッセージで CFormView::OnPaint() を呼び出さないでください。
//	dc.TextOutW(600, 100, L"雲深不知処");
//	dc.TextOutW(L"雲深不知処");
	dc.Rectangle(400, 400, 600, 100);
	dc.Ellipse(400, 400, 600, 100);
	dc.TextOutW(500, 200, L"雲深不知処");
//	pDC->TextOutW(600, 100, L"雲深不知処");

	dc.MoveTo(400, 400);
	dc.LineTo(600, 100);

}


void CTestFormB::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CTestFormB::OnBnClickedButton2Createproc()
{
	setlocale(LC_ALL, "Japanese");
	HRESULT hResult = E_FAIL;

	STARTUPINFO tStartupInfo = { 0 };
	PROCESS_INFORMATION tProcessInfomation = { 0 };

	// start notepade
	BOOL bResult = CreateProcess(
		L"c:\\Windows\\System32\\notepad.exe",
		L"",
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&tStartupInfo,
		&tProcessInfomation
	);
	if (0 == bResult) {
		return;// (HRESULT_FROM_WIN32(::GetLastError()));
	}

	// wait end of process
	DWORD dwResult = ::WaitForSingleObject(
		tProcessInfomation.hProcess,
		INFINITE
	);
	if (WAIT_FAILED == dwResult) {
		hResult = HRESULT_FROM_WIN32(::GetLastError());
		goto err;
	}

	// get exit code of process
	DWORD dwExitCode;
	bResult = ::GetExitCodeProcess(
		tProcessInfomation.hProcess,
		&dwExitCode
	);
	if (0 == bResult) {
		hResult = HRESULT_FROM_WIN32(::GetLastError());
	}

	wprintf(L"dwExitCode = %d", dwExitCode);


err:
	::CloseHandle(tProcessInfomation.hProcess);
	::CloseHandle(tProcessInfomation.hThread);

	return;
}

//#include <windows.h>
#include <strsafe.h>
//#include <Processthreadsapi.h>
#define MAX_THREADS 3
#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);

typedef struct MyData {
	int val1;
	int val2;
}MYDATA, *PMYDATA; 

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;
	DWORD dwMode;

	// Make sure there is  a console to receive output results

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		return 1;
	}
	else if (hStdout == NULL) {
		ErrorHandler(TEXT("GetStdHandle"));
		return 1;
	}

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because
	// it was checked for NULL before the  thread was created.
	pDataArray = (PMYDATA)lpParam;

	// Print the parameter values using thread-safe function.
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("pARAMETERS = %d, %d\n"),
		pDataArray->val1, pDataArray->val2);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	if (GetConsoleMode(hStdout, &dwMode) == 0) {
	//	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
	}
	OutputDebugString(msgBuf);
	return 0;
}

void ErrorHandler(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code.
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL
	);

	// Display the error message.
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	// Free error-handling buffer allocation.
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

void CTestFormB::OnBnClickedButton2Createthread()
{
	PMYDATA pDataArray[MAX_THREADS];
	DWORD dwThreadIdArray[MAX_THREADS];
	HANDLE hThreadArray[MAX_THREADS];

	// Create MAX_THREADS worker threads.
	for (int i = 0; i < MAX_THREADS; i++)
	{
		// Allocate memory for thread data.
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(MYDATA));
		if (pDataArray[i] == NULL)
		{
			ExitProcess(2);
		}

		// Generate unique data for each thread to work with.
		pDataArray[i]->val1 = i;
		pDataArray[i]->val2 = i + 100;

		// Create thethread to begin execution on its own.
		hThreadArray[i] = CreateThread(
			NULL,					// default security attributes
			0,						// use default stack size
			MyThreadFunction,		// thread function name
			pDataArray[i],			// argument to thread function
			0,						// use default creation flags
			&dwThreadIdArray[i]
		);

		// Check the return value for success.
		// This will automaticall clean  up threads and memory.
		if (hThreadArray[i] == NULL)
		{
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	} // End of main thread creation loop.

	// Wait until  all theads have terminated.
	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE); 
	// Close all thread handles and free memory allocations.
	for (int i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;  // Ensure address is not reused.
		}
	}
	return;
} 

	