// MicroVi.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "MicroVi.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

//创建文本编辑器类
Typer typer;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MICROVI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MICROVI));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MICROVI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //初始化文本编辑器类
   typer.init(hWnd);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            for (int i = 0; i < typer.getCyBuffer(); i++)
            {
                for (int j = 0; j < typer.getCxBuffer(); j++)
                {
                    TCHAR tch = typer.readBuffer(j, i);
                    TextOut(hdc, typer.getCxChar() * j, typer.getCyChar() * i, &tch, 1);
                }
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SETFOCUS:
        typer.setFocus();
        break;
    case WM_KILLFOCUS:
        typer.killFocus();
        break;
    case WM_SIZE:
        typer.setClientSize(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:
        {
            int x, y;
            switch (wParam)
            {
            case VK_HOME:
                typer.setCurrentPos(0, -1);
                break;
            case VK_END:
                typer.setCurrentPos(typer.getCxBuffer() - 1, -1);
                break;
            case VK_LEFT:
                x = max(0, typer.getXCurrent()-1);
                typer.setCurrentPos(x, -1);
                break;
            case VK_RIGHT:
                x = min(typer.getCxBuffer() - 1, typer.getXCurrent() + 1);
                typer.setCurrentPos(x, -1);
                break;
            case VK_UP:
                y = max(0, typer.getYCurrent() - 1);
                typer.setCurrentPos(-1, y);
                break;
            case VK_DOWN:
                y = min(typer.getCyBuffer() - 1, typer.getYCurrent() + 1);
                typer.setCurrentPos(-1, y);
                break;
            case VK_RETURN:
                typer.setCurrentPos(0, typer.getYCurrent() + 1);
                break;
            case VK_TAB:
                typer.setCurrentPos(typer.getXCurrent() + 8, -1);
                break;
            case VK_DELETE:
            case VK_BACK:
                typer.deleteChar();
                break;
            }
            typer.setFocus();
            break;
        }
    case WM_CHAR:
        {
            switch (wParam)
            {
            case '\t':
                break; 
            case 0x0D: //Enter
                break;
            case 0x2e: //Delete
                break;
            case 0x08: //BackSpace
                break;
            default:
                typer.writeBuffer(wParam);
                typer.setCurrentPos(typer.getXCurrent() + 1, -1);
                InvalidateRect(hWnd, NULL, true);
                PostMessage(hWnd, WM_PAINT, 0, 0);
                break;
            }
            break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
