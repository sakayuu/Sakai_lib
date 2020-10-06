//#include <d3d9.h>
//#include <d3dx9.h>
//#include "Graphics.h"
//#include "Window.h"
//#include "../Utility/Size.h"
//
//// 静的ライブラリ
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
//
//struct CustomVertex
//{
//	float X;			// X座標
//	float Y;			// Y座標
//	float Z;			// Z座標	
//	float Rhw;			// 除算数
//
//	float TextureX;		// テクスチャ座標X
//	float TexrureY;		// テクスチャ座標Y
//};
//
//// グローバル変数
//static LPDIRECT3D9 g_D3DInterface;						// DirectGraphicsインターフェース
//static LPDIRECT3DDEVICE9 g_D3DDevice;					// DirectGraphicsデバイス
//static LPD3DXFONT g_FontList[FontSize::FontSizeMax];	// フォントデバイスリスト
//
//// インターフェースの作成
//bool CreateGraphicsInterface();
//
//// グラフィックデバイスの作成
//bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param);
//
//// ViewPortの設定
//bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param);
//
//// フォントデバイスの生成
//bool CreateFontDevice();
//
//bool InitGraphics()
//{
//	D3DPRESENT_PARAMETERS present_param;
//	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));
//
//	if (CreateGraphicsInterface() == false)
//	{
//		return false;
//	}
//
//	if (CreateGraphicsDevice(&present_param) == false)
//	{
//		return false;
//	}
//
//	if (SetUpViewPort(&present_param) == false)
//	{
//		return false;
//	}
//
//	if (CreateFontDevice() == false)
//	{
//		return false;
//	}
//
//	g_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
//	g_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	g_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	return true;
//}
//
//void ReleaseGraphics()
//{
//	for (int i = 0; i < FontSize::FontSizeMax; i++)
//	{
//		g_FontList[i]->Release();
//	}
//	g_D3DDevice->Release();
//	g_D3DInterface->Release();
//}
//
//bool DrawStart()
//{
//	g_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);
//
//	if (D3D_OK == g_D3DDevice->BeginScene())
//	{
//		return true;
//	}
//
//	return false;
//}
//
//void DrawEnd()
//{
//	g_D3DDevice->EndScene();
//	g_D3DDevice->Present(NULL, NULL, NULL, NULL);
//}
//
//void DrawTextureUV(float x, float y, Texture* texture_data, float tex_x, float tex_y, float sprite_width, float sprite_height)
//{
//	/*
//		tex_x => テクスチャ座標X
//		tex_y => テクスチャ座標Y
//		sprite_width => 矩形サイズ(横)
//		sprite_height => 矩形サイズ(縦)
//	
//	*/
//
//	// テクスチャ座標 => UV変換
//	float u_left = tex_x / texture_data->Width;
//	float u_right = (tex_x + sprite_width) / texture_data->Width;
//	float v_top = tex_y / texture_data->Height;
//	float v_bottom = (tex_y + sprite_height) / texture_data->Height;
//
//	CustomVertex v[4] =
//	{
//		{ x, y, 0.0f, 1.0f, u_left, v_top },
//		{ x + sprite_width, y, 0.0f, 1.0f, u_right, v_top },
//		{ x + sprite_width, y + sprite_height, 0.0f, 1.0f, u_right, v_bottom },
//		{ x, y + sprite_height, 0.0f, 1.0f, u_left, v_bottom },
//	};
//
//	// 頂点構造の指定
//	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//
//	g_D3DDevice->SetTexture(0, texture_data->TexutreData);
//
//	g_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
//}
//
//void DrawTexture(float x, float y, Texture* texture_data)
//{
//	CustomVertex v[4] =
//	{
//		{ x, y, 0.0f, 1.0f, 0.0f, 0.0f },
//		{ x + texture_data->Width, y, 0.0f, 1.0f, 1.0f, 0.0f },
//		{ x + texture_data->Width, y + texture_data->Height, 0.0f, 1.0f, 1.0f, 1.0f },
//		{ x, y + texture_data->Height, 0.0f, 1.0f, 0.0f, 1.0f },
//	};
//
//	// 頂点構造の指定
//	g_D3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
//
//	g_D3DDevice->SetTexture(0, texture_data->TexutreData);
//
//	g_D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(CustomVertex));
//
//}
//
//void DrawFont(float pos_x, float pos_y, const char* text, FontSize font_type, FontColor color)
//{
//	RECT rect =
//	{
//		(long)pos_x,
//		(long)pos_y,
//		(long)pos_x + 400,
//		(long)pos_y + 200,
//	};
//
//	int r, g, b;
//	r = g = b = 255;
//
//	if (color == FontColor::Black)
//	{
//		r = g = b = 0;
//	}
//	else if (color == FontColor::Red)
//	{
//		r = 255;
//		g = b = 0;
//	}
//
//	g_FontList[font_type]->DrawTextA(
//		NULL,
//		text,
//		-1,
//		&rect,
//		DT_LEFT,
//		D3DCOLOR_XRGB(r, g, b)
//	);
//}
//
//bool CreateTexture(const char* file_name, Texture* texture_data)
//{
//	Size size;
//	D3DXIMAGE_INFO info;
//
//	// 2の累乗じゃないケースを想定して元のサイズを取得してD3DXCreateTextureFromFileExで使う
//	D3DXGetImageInfoFromFileA(file_name, &info);
//
//	if (FAILED(D3DXCreateTextureFromFileExA(g_D3DDevice,
//		file_name,
//		0,
//		0,
//		1,
//		0,
//		D3DFMT_UNKNOWN,
//		D3DPOOL_MANAGED,
//		D3DX_DEFAULT,
//		D3DX_DEFAULT,
//		0x0000ff00,
//		nullptr,
//		nullptr,
//		&texture_data->TexutreData)))
//	{
//		return false;
//	}
//	else
//	{
//		// テクスチャサイズの取得
//		D3DSURFACE_DESC desc;
//
//		if (FAILED(texture_data->TexutreData->GetLevelDesc(0, &desc)))
//		{
//			texture_data->TexutreData->Release();
//			return false;
//		}
//		texture_data->Width = desc.Width;
//		texture_data->Height = desc.Height;
//	}
//
//	return true;
//}
//
//bool CreateGraphicsInterface()
//{
//	// インターフェース作成
//	g_D3DInterface = Direct3DCreate9(D3D_SDK_VERSION);
//	if (g_D3DInterface == NULL)
//	{
//		// 作成失敗
//		return false;
//	}
//
//	// 作成成功
//	return true;
//}
//
//bool CreateGraphicsDevice(D3DPRESENT_PARAMETERS* present_param)
//{
//	// バックバッファの数 => 1
//	present_param->BackBufferCount = 1;
//	// バックバッファのフォーマット => D3DFMT_UNKNOWN(フォーマットを知りません)
//	present_param->BackBufferFormat = D3DFMT_UNKNOWN;
//	// ウィンドウモード設定 => 定数で切り替え
//	present_param->Windowed = true;
//
//	// スワップエフェクト => バックバッファとフロントバッファへの切り替え方法
//	present_param->SwapEffect = D3DSWAPEFFECT_DISCARD;
//
//	// DirectDeviceの作成
//	if (FAILED(g_D3DInterface->CreateDevice(D3DADAPTER_DEFAULT,
//		D3DDEVTYPE_HAL,
//		FindWindowA(WINDOW_CLASS_NAME, nullptr),
//		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
//		present_param,
//		&g_D3DDevice)))
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool SetUpViewPort(D3DPRESENT_PARAMETERS* present_param)
//{
//	// ビューポートパラメータ
//	D3DVIEWPORT9 view_port;
//
//	// ビューポートの左上座標
//	view_port.X = 0;
//	view_port.Y = 0;
//	// ビューポートの幅
//	view_port.Width = present_param->BackBufferWidth;
//	// ビューポートの高さ
//	view_port.Height = present_param->BackBufferHeight;
//	// ビューポート深度設定
//	view_port.MinZ = 0.0f;
//	view_port.MaxZ = 1.0f;
//
//	// ビューポート設定
//	if (FAILED(g_D3DDevice->SetViewport(&view_port)))
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool CreateFontDevice()
//{
//	int size_list[] =
//	{
//		SMALL_FONT_SIZE,
//		REGULAR_FONT_SIZE,
//		LARGE_FONT_SIZE
//	};
//
//	for (int i = 0; i < FontSize::FontSizeMax; i++)
//	{
//		if (FAILED(D3DXCreateFont(g_D3DDevice,
//			size_list[i],
//			size_list[i] / 2,
//			FW_REGULAR,
//			NULL,
//			FALSE,
//			DEFAULT_CHARSET,
//			OUT_DEFAULT_PRECIS,
//			PROOF_QUALITY,
//			FIXED_PITCH | FF_SCRIPT,
//			TEXT("ＭＳ　Ｐゴシック"),
//			&g_FontList[i])))
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
