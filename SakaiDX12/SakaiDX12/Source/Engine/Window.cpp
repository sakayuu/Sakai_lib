#include "Window.h"
#include "../imgui/imgui_impl_win32.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const char Window::windowClassName[] = "サカイライブラリ";

LRESULT Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 1;
	}
	switch (msg)
	{
	case WM_DESTROY: //ウィンドウが破棄される時に呼ばれる(閉じるアイコンがクリックされた)
		PostQuitMessage(0); //OSに対し「このアプリ」の終了を通知
		return 0;
	case WM_KEYDOWN: //キー入力時に呼ばれる
		if (wparam == VK_ESCAPE) //ESCキーなら
			PostMessage(hwnd, WM_CLOSE, 0, 0); //終了メッセージを送る
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam); //規定処理実行
}

void Window::CreateGameWindow()
{
	//ウィンドウクラスの設定
	window_class.cbSize = sizeof(WNDCLASSEX);           //構造体のサイズ
	window_class.lpfnWndProc = (WNDPROC)WindowProc;     //コールバック関数の指定(プロシージャのアドレス)
	window_class.hInstance = GetModuleHandle(nullptr);  //ハンドルの取得
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW); //カーソル画像(NULLならデフォルト)
	window_class.lpszClassName = windowClassName;       //クラス名				 							

	//ウィンドウクラスの指定をOSに伝える
	RegisterClassEx(&window_class);

	RECT wrc = { 0,0,window_width,window_height };      //ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //関数を使ってウィンドウサイズを補正

	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(
		window_class.lpszClassName, //クラス名指定
		windowClassName,            //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        //タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,              //表示x座標はOSにお任せ
		CW_USEDEFAULT,              //表示y座標はOSにお任せ
		wrc.right - wrc.left,       //ウィンドウ幅
		wrc.bottom - wrc.top,       //ウィンドウ高
		nullptr,                    //親ウィンドウハンドル
		nullptr,                    //メニューハンドル
		window_class.hInstance,     //呼び出しアプリケーションハンドル
		nullptr);                   //追加パラメータ－

	//ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void Window::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

bool Window::ProcessMessage()
{
	MSG msg{}; // メッセージ
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) //メッセージがある？
	{
		TranslateMessage(&msg); //キー入力メッセージの処理
		DispatchMessage(&msg);  //ウィンドウプロシージャにメッセージを送る
	}
	if (msg.message == WM_QUIT) //終了メッセージが来たらループを抜ける
		return true;
	return false;
}