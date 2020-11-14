#pragma once
#include <Windows.h>

class Window
{
public: //静的メンバ変数
#pragma region ウィンドウ定数
	static const unsigned int window_width = 1280; //ウィンドウ横幅
	static const unsigned int window_height = 720; //ウィンドウ縦幅
	static const char windowClassName[];        //ウィンドウクラス名
#pragma endregion

public: //静的メンバ関数
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ情報1</param>
	/// <param name="lparam">メッセージ情報2</param>
	/// <returns>成否</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: //メンバ関数
	/// <summary>
	/// ウィンドウ作成
	/// </summary>
	void CreateGameWindow();
	
	/// <summary>
	/// ウィンドウの破棄
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetInstance() { return window_class.hInstance; }
private:
	HWND hwnd = nullptr;
	WNDCLASSEX window_class{};
};
