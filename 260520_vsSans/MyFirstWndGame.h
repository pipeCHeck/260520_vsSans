#pragma once
#include "NzWndBase.h"
#include "Utillity.h"
#include <functional>

// [CHECK] #7  전방 선언을 사용하여 헤더파일의 의존성을 줄임.
class GameTimer;
class GameObjectBase;
class GameObject;

namespace renderHelp
{
    class BitmapInfo;
}

class MyFirstWndGame : public NzWndBase
{
public:
    MyFirstWndGame() = default;
    ~MyFirstWndGame() override = default;

    bool Initialize();
    void Run();
    void Finalize();

    learning::Vector2f Rotate(learning::Vector2f dir, float angleOffset);

#pragma region resource
    using BitmapInfo = renderHelp::BitmapInfo;

    BitmapInfo* m_pPlayerBitmapInfo = nullptr;
    BitmapInfo* m_pEnemyBitmapInfo = nullptr;

    BitmapInfo* m_pSans_Head = nullptr;
    BitmapInfo* m_pSans_Torso = nullptr;
    BitmapInfo* m_pSans_Legs_01 = nullptr;
    BitmapInfo* m_pSans_Legs_02 = nullptr;
    BitmapInfo* m_pSans_Attack_Horizontal = nullptr;
    BitmapInfo* m_pSans_Attack_Vertical = nullptr;
    BitmapInfo* m_pSans_SpeechBubble = nullptr;
    BitmapInfo* m_pGB_Default = nullptr;
    BitmapInfo* m_pBone_Horizontal_01 = nullptr;
    BitmapInfo* m_pBone_Horizontal_02 = nullptr;
    BitmapInfo* m_pBone_Vertical_01 = nullptr;
    BitmapInfo* m_pBone_Vertical_02 = nullptr;
    BitmapInfo* m_pBone_Vertical_03 = nullptr;
    BitmapInfo* m_pBone_Vertical_04 = nullptr;
    BitmapInfo* m_pBone_Dense_Horizontal = nullptr;
    BitmapInfo* m_pBone_Dense_Vertical = nullptr;

    BitmapInfo* m_pPlayer_Heart = nullptr;
    BitmapInfo* m_pPlayer_HeartBroken = nullptr;
    BitmapInfo* m_pPlayer_HeartFragment = nullptr;
    BitmapInfo* m_pUI_ChoiceButton = nullptr;
    BitmapInfo* m_pUI_AttackMiss = nullptr;
    BitmapInfo* m_pUI_AttackDamage = nullptr;
    BitmapInfo* m_pAttack_Background = nullptr;
    BitmapInfo* m_pAttack_Bar = nullptr;
    BitmapInfo* m_pAttack_Slash = nullptr;
    BitmapInfo* m_pUI_Box = nullptr;

	BitmapInfo* m_pBg_Shop = nullptr;
	BitmapInfo* m_pBg_Black = nullptr;
	BitmapInfo* m_pTable = nullptr;

    BitmapInfo* m_pArmL = nullptr;
    BitmapInfo* m_pBody = nullptr;
    BitmapInfo* m_pBody_ArmL = nullptr;
    BitmapInfo* m_pBody_ArmR = nullptr;
    BitmapInfo* m_pEffects = nullptr;
    BitmapInfo* m_pEyeL_Brow = nullptr;
    BitmapInfo* m_pEyeL_Lid = nullptr;
    BitmapInfo* m_pEyeL_Pupil = nullptr;
    BitmapInfo* m_pEyeL_White = nullptr;
    BitmapInfo* m_pEyeR_Brow = nullptr;
    BitmapInfo* m_pEyeR_Lid = nullptr;
    BitmapInfo* m_pEyeR_Pupil = nullptr;
    BitmapInfo* m_pEyeR_White = nullptr;
    BitmapInfo* m_pFace = nullptr;

    BitmapInfo* m_pHair_AhogeL = nullptr;
    BitmapInfo* m_pHair_AhogeR = nullptr;
    BitmapInfo* m_pHairBL = nullptr;
    BitmapInfo* m_pHairBR = nullptr;
    BitmapInfo* m_pHairB_HeadL = nullptr;
    BitmapInfo* m_pHairB_HeadR = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftL = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftR = nullptr;
    BitmapInfo* m_pHairF = nullptr;
    BitmapInfo* m_pHairF_Bang = nullptr;
    BitmapInfo* m_pHairF_SideL = nullptr;
    BitmapInfo* m_pHairF_SideR = nullptr;
    BitmapInfo* m_pHairSideL_1 = nullptr;
    BitmapInfo* m_pHairSideL_2 = nullptr;
    BitmapInfo* m_pHairSideR_1 = nullptr;
    BitmapInfo* m_pHairSideR_2 = nullptr;
    BitmapInfo* m_pHandL = nullptr;
    BitmapInfo* m_pMouth = nullptr;
    BitmapInfo* m_pNose = nullptr;

    BitmapInfo* m_pHair_AhogeL_Shadow = nullptr;
    BitmapInfo* m_pHair_AhogeR_Shadow = nullptr;
    BitmapInfo* m_pHairB_HeadL_Shadow = nullptr;
    BitmapInfo* m_pHairB_HeadR_Shadow = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftL_Shadow = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftR_Shadow = nullptr;
    BitmapInfo* m_pHairBL_Shadow = nullptr;
    BitmapInfo* m_pHairBR_Shadow = nullptr;

    BitmapInfo* m_pHair_AhogeL_Line = nullptr;
    BitmapInfo* m_pHair_AhogeR_Line = nullptr;
    BitmapInfo* m_pHairB_HeadL_Line = nullptr;
    BitmapInfo* m_pHairB_HeadR_Line = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftL_Line = nullptr;
    BitmapInfo* m_pHairB_Head_SideTuftR_Line = nullptr;
    BitmapInfo* m_pHairBL_Line = nullptr;
    BitmapInfo* m_pHairBR_Line = nullptr;
    BitmapInfo* m_pArmL_Line = nullptr;
    BitmapInfo* m_pHandL_Line = nullptr;
    BitmapInfo* m_pEffect_Line = nullptr;

	// m_pNull 이 앞에 붙은 객체는 이미지 없는 컨트롤러 객체
    BitmapInfo* m_pNull_Hair = nullptr;

#pragma endregion

private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);
    void OnRButtonDown(int x, int y);

    void OnZKeyDown();
    void OnXKeyDown();

    void OnUpKeyDown();
    void OnDownKeyDown();
    void OnLeftKeyDown();
    void OnRightKeyDown();

    bool IsValidSpawnPosition(learning::Vector2f pos, int selfCheck = -1);

    void FixedUpdate();
    void LogicUpdate();

    void CreatePlayer();
    void CreateEnemy();

	void CreateBackground();
	void CreateTable();
    void CreateCoffia();

    void UpdatePlayerInfo();
	void UpdateEnemyInfo(int index, float deltaTime);

    int FindCollisionAt(learning::Vector2f pos, int selfIndex, int ignoreIndex);
    int FindClosestObjectAt(learning::Vector2f pos, int selfIndex = -1, int ignoreIndex = -1);
    bool IsOverlapping(learning::Vector2f pos, float radius, GameObject* targetObject);

    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }
    GameObject* GetGameObject(int index) const { return (GameObject*)m_GameObjectPtrTable[index]; }

private:
    HDC m_hFrontDC = nullptr;
    HDC m_hBackDC = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hDefaultBitmap = nullptr;

    // [CHECK] #8. 게임 타이머를 사용하여 프레임을 관리하는 예시.
    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    // [CHECK] #8. 게임 오브젝트를 관리하는 컨테이너.
    GameObjectBase** m_GameObjectPtrTable = nullptr;

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const
        {
            return (x != other.x || y != other.y);
        }
    };

    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

    MOUSE_POS m_PlayerTargetPos = { 0, 0 };
    MOUSE_POS m_EnemySpawnPos = { 0, 0 };

    bool keys[256] = {};    // 특정 키가 눌려있는지 확인
    
    std::function<void()> OnKey[256];       // 키가 눌릴때 나올 메서드
    std::function<void(int, int)> OnKeyDown[256];   // 키가 딱 눌린 순간에만 나올 메서드
    std::function<void(int, int)> OnKeyUp[256];   // 키가 딱 눌린 순간에만 나올 메서드
};