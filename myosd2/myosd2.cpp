#include <Windows.h>
#include <thread>
#include <gdiplus.h>
#include <gdiplusheaders.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#pragma region OSD Internal Implementation

struct mTparam
{
    const wchar_t* txT3;
    int line2;
	int color;
};


BOOL LinesInUse[17]= { 0 };

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	
    switch (message)
    {
    
    case WM_CREATE:
	{
		
	}
    
    case WM_PAINT:
    {
        SetTimer(hwnd, 1 , 1000, NULL);
        
        //return 0;
    }
    break;
    case WM_TIMER:
    {
		
        DestroyWindow(hwnd);

        KillTimer(hwnd, 1);

        //return 0;
    }
    break;
    case WM_DESTROY:
    {
		
        //globalline--;
		
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
};


//bool createOSD(const wchar_t* txT)
DWORD WINAPI createOSD(LPVOID lpParam)
{
    //MessageBox(NULL, L"createOSD called", L"Title", MB_ICONEXCLAMATION | MB_OK);
    mTparam* nipple2 = reinterpret_cast<mTparam*>(lpParam);
    const wchar_t* txT = nipple2->txT3;
    int line = nipple2->line2;
	int color = nipple2->color;
	if (color > 5 || color <1 ) color = 4;
    //MessageBox(NULL, txT, L"Title", MB_ICONEXCLAMATION | MB_OK);
    #define MAX_WIDTH 1920
    #define MAX_HEIGHT 1080

    HWND hWnd;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    //Initialize GDI+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    HINSTANCE  hinst = GetModuleHandle(NULL);
    // create a window class:
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hinst;
	wc.lpszClassName =  L"win32";
	

    // register class with operating system:
    RegisterClass(&wc);

    // create and show window:
    hWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_NOACTIVATE | WS_EX_TRANSPARENT,
        L"win32",
        L"nippleOSD",
        WS_POPUP,
        0, 0, 1920, 1080,
        nullptr,
        nullptr,
        //hInstance,
        NULL,
        nullptr
    );

    if (hWnd == NULL) {
        return 0;
    }
    
    FontFamily  fontFamily(L"Arial");
    //Font        font(&fontFamily, 28, FontStyleRegular, UnitPixel);
    Font        font(&fontFamily, 48, FontStyleBold, UnitPixel);
    PointF      pointF(1.0f, 1.0f);
	SolidBrush  solidBrush(Color(255, 0, 215, 0)); //green
	switch (color)
	{
	case 1:
		solidBrush.SetColor(Color(255, 0, 215, 0)); //green
		break;
	case 2:
		solidBrush.SetColor(Color(255, 255, 0, 0)); //red
		break;
	case 3:
		solidBrush.SetColor(Color(255, 0, 0, 255)); //Blue
		break;
	case 4:
		solidBrush.SetColor(Color(255, 255, 128, 0)); //orange
		break;
	case 5:
		solidBrush.SetColor(Color(255, 0, 255, 255)); //Cyan
		break;

	}

    Bitmap softwareBitmap(MAX_WIDTH, MAX_HEIGHT, PixelFormat32bppARGB);
    Graphics g(&softwareBitmap);

    g.Clear(Gdiplus::Color(0, 0, 0, 0));  // 30: alpha value 

    g.DrawString(txT, -1, &font, pointF, &solidBrush);

    HBITMAP bmp;
    softwareBitmap.GetHBITMAP(Color(0, 0, 0, 0), &bmp);

    HDC memdc = CreateCompatibleDC(GetDC(hWnd));
    HGDIOBJ original = SelectObject(memdc, bmp);


    BLENDFUNCTION blend = { 0 };
    blend.BlendOp = AC_SRC_OVER;
    blend.SourceConstantAlpha = 255;
    blend.AlphaFormat = AC_SRC_ALPHA;
    POINT ptLocation = { 0, line*40 };// ( horizontal , vertical )
    SIZE szWnd = { MAX_WIDTH, MAX_HEIGHT };
    POINT ptSrc = { 0, 0 };
    BOOL l = UpdateLayeredWindow(hWnd, GetDC(hWnd), &ptLocation, &szWnd, memdc, &ptSrc, 0, &blend, ULW_ALPHA);
    int err = GetLastError();
    SelectObject(GetDC(hWnd), original);

    DeleteObject(bmp);
    DeleteObject(memdc);

    ShowWindow(hWnd, SW_SHOW);



    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) 
	{
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	LinesInUse[line] = 0;

}




void createOSDmT(const wchar_t* txT2,int colorz)
{

    mTparam nipple;
    nipple.txT3 = txT2;
	nipple.line2 = 1;
	for (int i = 0; i < 17; i++)
	{
		if (LinesInUse[i] == 0) 
		{
			nipple.line2 = i;
			LinesInUse[i] = 1;
			break;
		}
	}
	nipple.color = colorz;
	 

    HANDLE h = CreateThread(NULL, 0, &createOSD, &nipple, 0, NULL);
	Sleep(1);
	
}
#pragma endregion


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, PSTR szCmdLine, int iCmdShow)
{
	
	createOSDmT(L"Cleaning and Tidying work1s",4);
	createOSDmT(L"Cleaning and Tidying work1s", 1);
	createOSDmT(L"Cleaning and Tidying work1s", 5);
	createOSDmT(L"Cleaning and Tidying work1s", 2);
	createOSDmT(L"Cleaning and Tidying work1s", 25);
	createOSDmT(L"Cleaning and Tidying work1s", -14);
    Sleep(5000);
    return 0;
}