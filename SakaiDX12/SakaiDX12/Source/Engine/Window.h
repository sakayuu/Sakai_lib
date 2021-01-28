#pragma once
#include <Windows.h>

class Window
{
public: //静的メンバ変数
#pragma region ウィンドウ定数
	static const int window_width = 1280; //ウィンドウ横幅
	static const int window_height = 720; //ウィンドウ縦幅
	static const char windowClassName[];  //ウィンドウクラス名
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
	//ウィンドウ作成
	void CreateGameWindow();

	//ウィンドウの破棄
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

	/// <summary>
	/// 実体の取得
	/// </summary>
	/// <returns></returns>
	HINSTANCE GetInstance() { return window_class.hInstance; }

private: //メンバ変数
	HWND hwnd = nullptr;
	WNDCLASSEX window_class{};
};
