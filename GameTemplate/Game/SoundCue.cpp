#include "stdafx.h"
#include "SoundCue.h"


/// <summary>
/// 初期化用関数
/// wavファイルのパスを指定して初期化する
/// </summary>
/// <param name="filePath">wavファイルパス</param>
/// <param name="is3DSound">3D再生するか？（たぶんできない）</param>
void CSoundCue::Init(const wchar_t* filePath, const bool is3DSound)
{
	//引数で渡された const wchat_t* を、
	//データメンバの wchar の配列に入れる
	swprintf(m_filePath, filePath);

	//一度、作って破棄することで、
	//Flyweightパターンを使用しているWaveFileBankに
	//登録させる。
	CSoundSource* ss = NewGO<CSoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	DeleteGO(ss);
}


/// <summary>
/// サウンドを再生
/// </summary>
/// <param name="isLoop">ループ再生するか？</param>
void CSoundCue::Play(const bool isLoop)
{
	//ループ再生かワンショット再生かで処理を振り分ける

	if (isLoop)
	{
		//ループ再生
		PlayLoop();
	}
	else
	{
		//ワンショット再生
		PlayOneShot();
	}
}


//ループ再生の処理
void CSoundCue::PlayLoop()
{
	//ループ再生用のサウンドソースが
	//すでに作れれていたら
	if (m_loopSoundSource)
	{
		//一時停止したものを再開する
		m_loopSoundSource->Play(true);
		//フラグを一時停止中から解除する
		m_isPaused = false;
	}
	else
	{
		//ループ再生用のサウンドソースが
		//まだ未使用の場合は
		//新しく作って再生する。
		m_loopSoundSource = NewGO<CSoundSource>(0);
		m_loopSoundSource->Init(m_filePath, m_is3DSound);
		m_loopSoundSource->Play(true);
	}
}

//ワンショット再生の処理
void CSoundCue::PlayOneShot()
{
	//毎回、サウンドソースを作って再生させる
	//Flyweightパターンを使用しているから
	//重くならないぜ！
	CSoundSource* ss = NewGO<CSoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	ss->SetVolume(m_volume);	//OSの場合はここでボリュームを設定する
	ss->Play(false);
}


/// <summary>
/// ボリュームを設定
/// 1.0fが通常。
/// 負の値を入れると勝手に絶対値になるから注意
/// </summary>
/// <param name="volume">ボリューム</param>
void CSoundCue::SetVolume(const float volume)
{
	//ボリュームを保持
	m_volume = volume;

	//ループ再生用のサウンドソースが使われていたら
	if (!m_loopSoundSource)
		return;

	//ループ再生用のサウンドソースのボリュームを設定する
	m_loopSoundSource->SetVolume(m_volume);
}

/// <summary>
/// 再生を停止する。
/// これで停止すると、次に再生した時、
/// 最初からの再生になる。
/// 今の仕様だと、ループ再生は停止できるが、
/// ワンショット再生の停止はできない。
/// </summary>
void CSoundCue::Stop()
{
	//ループ再生用のサウンドソースが使われていたら
	if (!m_loopSoundSource)
		return;

	//サウンドを停止する
	m_loopSoundSource->Stop();
}

/// <summary>
/// 再生を一時停止する。
/// これで一時停止すると、次に再生した時、
/// 途中からの再生になる。
/// 今の仕様だと、ループ再生は一時停止できるが、
/// ワンショット再生の一時停止はできない。
/// </summary>
void CSoundCue::Pause()
{
	//ループ再生用のサウンドソースが使われていたら
	if (!m_loopSoundSource)
		return;

	//サウンドを一時停止する
	m_loopSoundSource->Pause();
	//フラグを一時停止中にする
	m_isPaused = true;
}

/// <summary>
/// 今再生中かどうか？
/// 今の仕様だと、ループ再生は検知できるが、
/// ワンショット再生の検知はできない。
/// </summary>
/// <returns>再生中か？</returns>
const bool CSoundCue::IsPlaying()const
{
	//ループ再生用のサウンドソースが使われていたら
	if (!m_loopSoundSource)
		return false;

	//再生中かどうか調べる
	return m_loopSoundSource->IsPlaying();
}