#include "Mesh.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Mesh::device = nullptr;

// テクスチャあり用パイプライン
Mesh::PipelineSet Mesh::pipelineSetTex;
// テクスチャなし用パイプライン
Mesh::PipelineSet Mesh::pipelineSetNoTex;

void Mesh::StaticInitialize(ID3D12Device* device) {
	// 再初期化チェック
	assert(!Mesh::device);

	Mesh::device = device;

	// グラフィックパイプラインの生成
	CreateGraphicsPipelineTex();
	CreateGraphicsPipelineNoTex();

	// マテリアルの静的初期化
	Material::StaticInitialize(device);
}

void Mesh::CreateGraphicsPipelineTex()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Source/Shader/OBJVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Source/Shader/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[5];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSetTex.rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = pipelineSetTex.rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSetTex.pipelinestate));
	assert(SUCCEEDED(result));
}

void Mesh::CreateGraphicsPipelineNoTex()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Source/Shader/OBJNoTexVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Source/Shader/OBJNoTexPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[5];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSetNoTex.rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = pipelineSetNoTex.rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSetNoTex.pipelinestate));
	assert(SUCCEEDED(result));

}

void Mesh::SetName(const std::string& name) {
	this->name = name;
}

void Mesh::AddVertex(const VertexPosNormalUv& vertex) {
	verticesTex.emplace_back(vertex);
}

void Mesh::AddVertex(const VertexPosNormal& vertex) {
	verticesNoTex.emplace_back(vertex);
}

void Mesh::AddIndex(unsigned short index) {
	indices.emplace_back(index);
}

void Mesh::AddSmoothData(unsigned short indexPosition, unsigned short indexVertex) {
	smoothData[indexPosition].emplace_back(indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals() {
	auto itr = smoothData.begin();

	if (material && material->textureFilename.size() > 0) {
		for (; itr != smoothData.end(); ++itr) {
			//各面用の共通頂点コレクション
			std::vector<unsigned short>& v = itr->second;
			//全頂点の法線を平均する
			XMVECTOR normal = {};
			for (unsigned short index : v) {
				normal += XMVectorSet(verticesTex[index].normal.x, verticesTex[index].normal.y, verticesTex[index].normal.z, 0);
			}
			normal = XMVector3Normalize(normal / (float)v.size());
			//共通法線を使用する全ての頂点データに書き込む
			for (unsigned short index : v) {
				verticesTex[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
			}
		}
	}
	else {
		for (; itr != smoothData.end(); ++itr) {
			//各面用の共通頂点コレクション
			std::vector<unsigned short>& v = itr->second;
			//全頂点の法線を平均する
			XMVECTOR normal = {};
			for (unsigned short index : v) {
				normal += XMVectorSet(verticesNoTex[index].normal.x, verticesNoTex[index].normal.y, verticesNoTex[index].normal.z, 0);
			}
			normal = XMVector3Normalize(normal / (float)v.size());
			//共通法線を使用する全ての頂点データに書き込む
			for (unsigned short index : v) {
				verticesNoTex[index].normal = { normal.m128_f32[0],normal.m128_f32[1],normal.m128_f32[2] };
			}
		}
	}
}

void Mesh::SetMaterial(Material* material) {
	this->material = material;
}

void Mesh::CreateBuffers() {
	HRESULT result;

	UINT sizeVB = 0;

	// テクスチャあり
	if (material && material->textureFilename.size() > 0) {
		sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * verticesTex.size());
		//無いと動かない
		auto _CD3DX12_HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto CD3DX12_RESOURCE_DESC_Buffer = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
		// 頂点バッファ生成
		result = device->CreateCommittedResource(
			&_CD3DX12_HEAP_PROPERTIES,
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC_Buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff));

		// 頂点バッファへのデータ転送
		VertexPosNormalUv* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result)) {
			std::copy(verticesTex.begin(), verticesTex.end(), vertMap);
			vertBuff->Unmap(0, nullptr);
		}

		// 頂点バッファビューの作成
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeVB;
		vbView.StrideInBytes = sizeof(verticesTex[0]);
	}
	// テクスチャなし
	else {
		sizeVB = static_cast<UINT>(sizeof(VertexPosNormal) * verticesNoTex.size());
		//無いと動かない
		auto _CD3DX12_HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto CD3DX12_RESOURCE_DESC_Buffer = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
		// 頂点バッファ生成
		result = device->CreateCommittedResource(
			&_CD3DX12_HEAP_PROPERTIES,
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC_Buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff));

		// 頂点バッファへのデータ転送
		VertexPosNormal* vertMap = nullptr;
		result = vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result)) {
			std::copy(verticesNoTex.begin(), verticesNoTex.end(), vertMap);
			vertBuff->Unmap(0, nullptr);
		}

		// 頂点バッファビューの作成
		vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
		vbView.SizeInBytes = sizeVB;
		vbView.StrideInBytes = sizeof(verticesNoTex[0]);
	}
	assert(SUCCEEDED(result));

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	//無いと動かない
	auto _CD3DX12_HEAP_PROPERTIES = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto CD3DX12_RESOURCE_DESC_Buffer = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&_CD3DX12_HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC_Buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	assert(SUCCEEDED(result));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList) {
	// 頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	if (material && material->textureFilename.size() > 0) {
		// パイプラインステートの設定
		cmdList->SetPipelineState(pipelineSetTex.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(pipelineSetTex.rootsignature.Get());
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(2, material->GetGpuHandle());
	}
	else {
		// パイプラインステートの設定
		cmdList->SetPipelineState(pipelineSetNoTex.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(pipelineSetNoTex.rootsignature.Get());
	}

	// マテリアルの定数バッファをセット
	ID3D12Resource* constBuff = material->GetConstantBuffer();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
