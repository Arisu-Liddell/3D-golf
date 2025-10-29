#pragma once

enum SCENE {
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
};
void SetScene(SCENE scene);
void Transition(SCENE Next);