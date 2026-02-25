
//sound.cpp

#include "sound.h"

int		SoundID = -1;		//サウンドの識別子
bool	Repeat = false;		//リピートフラグ

//ゲームループの最後で一回呼ぶ
void	UpdateSound()
{
}
//サウンド再生開始
int		StartSound(char* filename, bool repeat)
{
	//再生中のサウンドを停止
	if (SoundID != -1)
	{
		StopSound();
	}
	//指定のサウンドを再生
	SoundID = opensound(filename);
	playsound(SoundID, repeat);
	Repeat = repeat;

	return SoundID;

}
//サウンド再生終了
void	StopSound()
{
	if (SoundID != -1)
	{
		stopsound(SoundID);	//再生停止
		closesound(SoundID);//データの解放
		SoundID = -1;
	}

}

