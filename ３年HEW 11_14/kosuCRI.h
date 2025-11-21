#pragma once
//====================================================================================
//  3D音響CRI
// 
//　誰かに渡って、わかんないことあったら、質問する人出てくると思うから、名前残しとく
// 
//　　　　　　　　　　　　　                                         　by　小菅　友暉
//====================================================================================

#include <cri_le_atom_ex.h>
#include <cri_le_atom_asr.h>
#include <cri_le_atom_wasapi.h>
#include <cri_le_error.h>
#include <windows.h>
#include <string>

//acbの数
//サンプルの音声持ってきてるだけだからこう書いてるけど内容物は違う
//BGM    ２D用
//SE　　 ３D用
//VOICE　ピッチを変えると音が重なって違う音に聞こえるやつ
enum Sounds {
    SOUNDS_NULL = -1,

    BGM,
    SE,
    VOICE,


    SOUNDS_MAX
};


//sound呼び出す前の初期設定用
struct SePlayParams {
    const char* cueName = nullptr;             // 再生するCue名
    float            volume = 1.0f;            // 音量
    float            pitch = 0.0f;             // ピッチ
    int              loopCount = 1;            // CRIATOMPLAYER_FORCE_LOOP / NO_LOOP_LIMITATION / 任意回数
    CriSint64        startTimeMs = 0;          // 再生開始位置(ms)
    Sounds           categoryId = SOUNDS_NULL; // 参照するACBカテゴリ
    int              priority = 0;             // 任意（未使用なら0でOK）
    int              fadeInMs = 0;             // 任意（Start前に設定する場合用）
    int              fadeOutMs = 0;            // 任意
    float            pan = 0.0f;               // 2Dパン(-1.0～+1.0)

    // 3D再生を開始するかどうか（初期位置は pos を使用）
    bool             is3D = false;
    CriAtomExVector  pos = { 0.0f, 0.0f, 0.0f };
};

class AudioManager {
public:
    //ゲームループするとこに置け（今回はめんどいからmain）
    static void Init();
    static void Update();
    static void Uninit();
    //キューの名前忘れた用
    static void DebugCuesName(int number);
    
    //ゲームオブジェクト関連-------------------------------
   
    //再生（Start）
    static CriAtomExPlaybackId PlaySeStart(const SePlayParams& params, CriAtomEx3dSourceHn source = nullptr);
    // 再生の停止（PlaybackId 単位）
    static void StopPlayback(CriAtomExPlaybackId id, bool immediate = false);
    // プレイヤー単位の停止（サンプル互換の簡易停止）
    static void StopPlayer(bool immediate = false);


   //更新-------------------------------
#pragma region 3D使うなら確定
   //位置更新（SetPosition → Update を内包）
   static void Update_3DSource_Position(CriAtomEx3dSourceHn source, float x, float y, float z);

   //更新用：耳の位置（普通はプレイヤー、監視カメラとか置くなら変わるよね）
   static void Update_3DListener_Position(const float& x, const float& y, const float& z);
#pragma endregion

#pragma region 3Dのちょっとした工夫用
   //ドップラー効果の計算用（レースゲームとか車系のにしか使わないかも）
        static void Config3DSource_Velocity(CriAtomEx3dSourceHn source, float vx, float vy, float vz);

    //更新用：耳の位置（プレイヤーが首動くなら使えるんじゃないかな）
    static void Config3DListener_Orientation(CriAtomExVector forward, CriAtomExVector up);
#pragma endregion

private:
    static CriAtomEx3dListenerHn s_Listener;
    static CriAtomExAcbHn sounds[SOUNDS_MAX];
    static CriAtomExPlayerHn s_Player;
    static CriAtomExVoicePoolHn s_VoicePool;

    static CriAtomExDbasId s_DbasId;

    static CriAtomExVector s_PlayerPos;
};

/*readme
コード変えないならこの説明だけで十分なはず

まず.acfは一つしか入らない。てか、汚い。

次に.acbは3つくらいでいい（増やしてもいい）
acbはキューなのでカテゴリー分けるのに使える。例：BGM.acb,SE.acb,VOICE.acb
Atom_Craftで生成される紙にオレンジが乗ったやつがキューシート

"CriAtomExAcbHn"は.acbを入れるもの
"CriAtomExAcbHn"は入れるacbの数だけ入れろ



*/
