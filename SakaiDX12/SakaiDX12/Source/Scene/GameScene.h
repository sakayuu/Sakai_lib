/**
* @file GameScene.h
* @brief ゲーム本編シーンの関数の宣言
*/
#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

/**
* @brief ゲーム本編シーンの更新関数@n
* ゲーム本編シーンの仕様が実行される
* @return 遷移先シーンID(基本はSceneId::GameSceneが返る)
*/
SceneId UpdateGameScene();

/**
* @brief ゲーム本編シーンの描画関数@n
* ゲーム本編シーンのオブジェクトを描画するための関数
*/
void DrawGameScene();

#endif
