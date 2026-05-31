#include "INC_Windows.h"
#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "CoffiaAnimationClips.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <unordered_set>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
    m_pGameTimer = new GameTimer(); // _타이머 객체를 만듦(힙)
    m_pGameTimer->Reset();          // _타이머 초기화

    const wchar_t* className = L"MyFirstWndGame";   // _클래스 이름? 찾아보니 WNDCLASSEX 구조체에 들어가는 클래스 이름 지정? 같은거임
    const wchar_t* windowName = L"MyFirstWndGame";  // _윈도우 창 이름

    // super는 부모를 뜻함, 부모에 Create를 불러와요~ 라는 뜻
    if (false == __super::Create(className, windowName, 1440, 900))
    {
        return false;
    }

    RECT rcClient = {}; // _사각형 상하좌우 값을 입력받는 데이터형
    GetClientRect(m_hWnd, &rcClient);   // _윈도우 창의 클라이언트 영역을 두번째 인자에 반환함(대충 윈도우 사이즈 저장됨)
    m_width = rcClient.right - rcClient.left;   // _부모(NzWndBase)한테 프로텍티드로 박혀있는 변수 둘. 윈도우 사이즈 저장
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd); // _화면에 표시하는 그런 무언가.. DC를 프론트버퍼에 띄우면 화면이 나옴
    m_hBackDC = CreateCompatibleDC(m_hFrontDC); // _실제 DC가 아니라 가상의 DC? 메모리에 띄워볼 수 있는 DC. 더블버퍼링으로 쓰는 용도.
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height); // _DC에 넣을 비트맵. 이미지 정보라고 봐도 무방함.

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap); // _백DC에 백비트맵을 집어넣고 기존에 있던 비트맵을 반환함

    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];    // _아직 좀 더 봐야할듯

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i) // _모든 자리에 nullptr 집어넣기
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

#pragma region resource

    // 스프라이트 이미지 불러오기
    m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/redbird.png");
    m_pEnemyBitmapInfo = renderHelp::CreateBitmapInfo(L"./Resource/graybird.png");

    m_pSans_Head = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Head.png", 17, 1, 17, 0, -50);
    m_pSans_Torso = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Torso.png", 8, 1, 8, 0, 0);
    m_pSans_Legs_01 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Legs_01.png", 1, 1 ,1 , 0, 35);
    m_pSans_Legs_02 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Legs_02.png");
    m_pSans_Attack_Horizontal = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Attack_Horizontal.png", 6, 1, 6);
    m_pSans_Attack_Vertical = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_Attack_Vertical.png", 7, 1, 7);
    m_pSans_SpeechBubble = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Sans_SpeechBubble.png");
    m_pGB_Default = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/GB_Default.png", 6, 1, 6);
    m_pBone_Horizontal_01 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Horizontal_01.png");
    m_pBone_Horizontal_02 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Horizontal_02.png");
    m_pBone_Vertical_01 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Vertical_01.png");
    m_pBone_Vertical_02 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Vertical_02.png");
    m_pBone_Vertical_03 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Vertical_03.png");
    m_pBone_Vertical_04 = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Vertical_04.png");
    m_pBone_Dense_Horizontal = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Dense_Horizontal.png");
    m_pBone_Dense_Vertical = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/Sans/Attacks/Bone_Dense_Vertical.png");

    m_pPlayer_Heart = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Player_Heart.png");
    m_pPlayer_HeartBroken = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Player_HeartBroken.png");
    m_pPlayer_HeartFragment = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Player_HeartFragment.png");
    m_pUI_ChoiceButton = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/UI_ChoiceButton.png", 1, 8, 8);
    m_pUI_AttackMiss = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/UI_AttackMiss.png");
    m_pUI_AttackDamage = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/UI_AttackDamage.png");
    m_pAttack_Background = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Attack_Background.png");
    m_pAttack_Bar = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Attack_Bar.png", 2, 1, 2);
    m_pAttack_Slash = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/Attack_Slash.png", 6, 1, 6);
    m_pUI_Box = renderHelp::CreateBitmapInfo(L"./Resource/Sprites/Battle/UI/UI_Box.png");


	// 배경 및 오브젝트 이미지 불러오기
    m_pBg_Shop = renderHelp::CreateBitmapInfo(L"./Resource/CoffiaDailyShop/Backgorund_Shop.png");
    m_pBg_Black = renderHelp::CreateBitmapInfo(L"./Resource/CoffiaDailyShop/Backgorund_Black.png");
    m_pTable = renderHelp::CreateBitmapInfo(L"./Resource/CoffiaDailyShop/Table.png");

	// 커피아 이미지 불러오기
    m_pArmL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/ArmL.png", 2, 1, 2);
    m_pBody = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Body.png");
    m_pBody_ArmL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Body_ArmL.png");
    m_pBody_ArmR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Body_ArmR.png");
    m_pEffects = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Effects.png", 2, 3, 6);
    m_pEyeL_Brow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeL_Brow.png");
    m_pEyeL_Lid = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeL_Lid.png", 1, 2, 2);
    m_pEyeL_Pupil = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeL_Pupil.png", 1, 2, 2);
    m_pEyeL_White = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeL_White.png");
    m_pEyeR_Brow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeR_Brow.png");
    m_pEyeR_Lid = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeR_Lid.png", 1, 2, 2);
    m_pEyeR_Pupil = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeR_Pupil.png", 1, 2, 2);
    m_pEyeR_White = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/EyeR_White.png");
    m_pFace = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Face.png");
    m_pHair_AhogeL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeL.png", 3, 1, 3);
    m_pHair_AhogeR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeR.png", 3, 1, 3);
    m_pHairBL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);
    m_pHairBR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);
    m_pHairB_HeadL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_HeadR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_Head_SideTuftL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftL.png", 1, 3, 3);
    m_pHairB_Head_SideTuftR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftR.png", 1, 3, 3);
    m_pHairF = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairF.png");
    m_pHairF_Bang = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairF_Bang.png");
    m_pHairF_SideL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairF_SideL.png");
    m_pHairF_SideR = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairF_SideR.png");
    m_pHairSideL_1 = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairSideL_1.png");
    m_pHairSideL_2 = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairSideL_2.png");
    m_pHairSideR_1 = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairSideR_1.png");
    m_pHairSideR_2 = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairSideR_2.png");
    m_pHandL = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HandL.png", 2, 2, 4);
    m_pMouth = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Mouth.png", 9, 1, 9);
    m_pNose = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Nose.png");

    m_pHair_AhogeL_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeL.png", 3, 1, 3);
    m_pHair_AhogeR_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeR.png", 3, 1, 3);
    m_pHairB_HeadL_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_HeadR_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_Head_SideTuftL_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftL.png", 1, 3, 3);
    m_pHairB_Head_SideTuftR_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftR.png", 1, 3, 3);
    m_pHairBL_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);
    m_pHairBR_Shadow = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);

    m_pHair_AhogeL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeL.png", 3, 1, 3);
    m_pHair_AhogeR_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Hair_AhogeR.png", 3, 1, 3);
    m_pHairB_HeadL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_HeadR_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head.png", 3, 2, 6);
    m_pHairB_Head_SideTuftL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftL.png", 1, 3, 3);
    m_pHairB_Head_SideTuftR_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB_Head_SideTuftR.png", 1, 3, 3);
    m_pHairBL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);
    m_pHairBR_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HairB.png", 3, 2, 6);
    m_pArmL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/ArmL.png", 2, 1, 2);
    m_pHandL_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/HandL.png", 2, 2, 4);
    m_pEffect_Line = renderHelp::CreateBitmapInfo(L"./Resource/Coffia/Effects.png", 2, 3, 6);

    m_pNull_Hair = renderHelp::CreateBitmapInfo(L"./Resource/NullObject.png");


    //std::cout << "Debuggggg" << std::endl;
    //std::cout << "sansHead curFrame:" << m_pSans_Head->GetCurFrame() << " / GetWidth: " << m_pSans_Head->GetWidth() << std::endl;
    //std::cout << "sansHead GetFrameCountX:" << m_pSans_Head->GetFrameCountX() << " / GetWidth: " << m_pSans_Head->GetFrameWidth() << std::endl;

#pragma endregion

    // 키 입력 함수 정의
#pragma region keyFunc

    // Z 키 입력 시
    OnKeyDown['Z'] = [](int, int) {
        std::cout << "Z down" << std::endl;
        };
    OnKey['Z'] = []() {
        std::cout << "Z";
        };
    OnKeyUp['Z'] = [](int, int) {
        std::cout << std::endl << "Z up" << std::endl;
        };

    // X 키 입력 시
    OnKeyDown['X'] = [](int, int) {
        std::cout << "X down" << std::endl;
        };
    OnKey['X'] = []() {
        std::cout << "X";
        };
    OnKeyUp['X'] = [](int, int) {
        std::cout << std::endl << "X up" << std::endl;
        };


    // VK_UP 키 입력 시
    OnKeyDown[VK_UP] = [](int, int) {
        std::cout << "VK_UP down" << std::endl;
        };
    OnKey[VK_UP] = []() {
        std::cout << "VK_UP ";
        };
    OnKeyUp[VK_UP] = [](int, int) {
        std::cout << std::endl << "VK_UP up" << std::endl;
        };

    // VK_DOWN 키 입력 시
    OnKeyDown[VK_DOWN] = [](int, int) {
        std::cout << "VK_DOWN down" << std::endl;
        };
    OnKey[VK_DOWN] = []() {
        std::cout << "VK_DOWN ";
        };
    OnKeyUp[VK_DOWN] = [](int, int) {
        std::cout << std::endl << "VK_DOWN up" << std::endl;
        };

    // VK_LEFT 키 입력 시
    OnKeyDown[VK_LEFT] = [](int, int) {
        std::cout << "VK_LEFT down" << std::endl;
        };
    OnKey[VK_LEFT] = []() {
        std::cout << "VK_LEFT ";
        };
    OnKeyUp[VK_LEFT] = [](int, int) {
        std::cout << std::endl << "VK_LEFT up" << std::endl;
        };

    // VK_RIGHT 키 입력 시
    OnKeyDown[VK_RIGHT] = [](int, int) {
        std::cout << "VK_RIGHT down" << std::endl;
        };
    OnKey[VK_RIGHT] = []() {
        std::cout << "VK_RIGHT ";
        };
    OnKeyUp[VK_RIGHT] = [](int, int) {
        std::cout << std::endl << "VK_RIGHT up" << std::endl;
        };


#pragma endregion


    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
    CreatePlayer(); // _0번 인덱스에 플레이어 원 객체 넣기

    return true;

}

void MyFirstWndGame::Run()
{
    MSG msg = { 0 };    // _MSG 구조체 내 모든 값들을 0으로 초기화
    while (msg.message != WM_QUIT)  // _message는 언사인드 int 변수인데 그 안의 값이 WM_QUIT라면 루프 종료
    {
        // 몬가 입력이 있다면 msg에 들어감
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // _메시지 확인할 곳, 어떤 메시지(nullptr은 모두), 모든 범위, 확인하고 확인한 메시지는 지울것.
        {
            //
            if (msg.wParam < 256) {
                bool hasKeyEvent = (OnKey[msg.wParam] || OnKeyDown[msg.wParam] || OnKeyUp[msg.wParam]);

                if (msg.message == WM_KEYDOWN && hasKeyEvent) {
                    if (!keys[msg.wParam] && OnKeyDown[msg.wParam]) {
                        OnKeyDown[msg.wParam](LOWORD(msg.lParam), HIWORD(msg.lParam));
                    }
                    keys[msg.wParam] = true;
                }
                else if (msg.message == WM_KEYUP && hasKeyEvent) {
                    if (OnKeyUp[msg.wParam])
                        OnKeyUp[msg.wParam](LOWORD(msg.lParam), HIWORD(msg.lParam));
                    keys[msg.wParam] = false;
                }
            }
            
            if (msg.message == WM_LBUTTONDOWN)  // _좌버튼클릭
            {
                MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam)); // _lParam은 그냥 롱롱 변수지만 상하위 비트에 나눠서 x,y값을 저장해놓는거라, 그걸 분리해서 xy값을 각각 인자로 전달하는 거.
            }
            else if (msg.message == WM_RBUTTONDOWN) // _우클릭
            {
                MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else if (msg.message == WM_MOUSEMOVE)   // _마우스무브
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }

            else    // _암것도 아니라면(내가 원하는 종류의 입력이 아니니까, 윈도우 니가 알아서 받아먹어라~ 하고 던져줌)
            {
                TranslateMessage(&msg); // _윈도우 프로시저가 알아먹는 메시지로 바꿔서(새로 생성해서..) 
                DispatchMessage(&msg);  // _다시 집어넣음
            }
        }
        else
        {
            // _위치 이동하고 dc에 복사하고 그런건 알겠는데 왜 else에 있는지 모르겠음
            Update();
            Render();
        }
    }
}

void MyFirstWndGame::Finalize()
{
    delete m_pGameTimer;    // 메모리에 있는 실제 객체 삭제
    m_pGameTimer = nullptr; // 객체 지워서 빈곳을 가르키니 그것도 삭제

    if (m_GameObjectPtrTable)   // 도형도 삭제
    {
        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
        {
            if (m_GameObjectPtrTable[i])
            {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
            }
        }
        delete m_GameObjectPtrTable;
    }

    __super::Destroy(); // 부모 클래스 디스트로이 호출
}

void MyFirstWndGame::FixedUpdate()
{
    /*
    if (m_EnemySpawnPos.x != 0 && m_EnemySpawnPos.y != 0)
    {
        CreateEnemy();
    }
    */
}

void MyFirstWndGame::LogicUpdate()
{
    for (int i = 0; i < sizeof(OnKey) / sizeof(OnKey[0]); i++) {
        if (keys[i]) {
            OnKey[i]();
        }
    }

    UpdatePlayerInfo();

    // 각 오브젝트를 다 체크해서 업데이트하기
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])    // 오브젝트 테이블에 있는 오브젝트들 싹다 검사
        {
			UpdateEnemyInfo(i, m_fDeltaTime);   // 적 정보 업데이트
            m_GameObjectPtrTable[i]->Update(m_fDeltaTime);  // 이번 프레임의 델타타임 넘거주기
        }
    }
}

void MyFirstWndGame::CreateBackground() 
{
    GameObject* pNewObject = new GameObject(ObjectType::BACKGROUND);
    pNewObject->SetName("Background");
    pNewObject->SetPosition(0.0f, 0.0f);
    pNewObject->SetSpeed(0.0f);
    // 이미지 설정
    //
}


// 커피아 생성
void MyFirstWndGame::CreateCoffia() 
{
    GameObject* pNewObject = new GameObject(ObjectType::BUTTON);

	pNewObject->SetName("Coffia");          // 이름 설정
    pNewObject->SetPosition(0.0f, 0.0f);    // 임의 설정
	pNewObject->SetSpeed(0.0f); 

    pNewObject->SetColliderCircle(50.0f);

    // 이미지 설정
    //
}

void MyFirstWndGame::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");  // _첫 인자가 거짓이면 에러

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);    // _객체를 하나 만드는데, enum 값 하나 지정함

    pNewObject->SetName("Player");  // _이름, 위치, 속도를 저장
    pNewObject->SetPosition(0.0f, 0.0f); // 일단, 임의로 설정 // _m_pos에 값 저장
    pNewObject->SetSpeed(0.8f); // 일단, 임의로 설정   // _m_speed에 값 저장

    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->SetWidth(100);
    pNewObject->SetHeight(100);

	// 이미지 설정
    BitmapInfo* image = nullptr;
    
    // 이펙트
    pNewObject->AddBitmapInfo(m_pEffects);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Effect");
    image->GetTransform().SetPosition(Vector2f(145, -403));
    image->SetCurFrame(2);;

    pNewObject->AddBitmapInfo(m_pArmL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("ArmL");
    image->GetTransform().SetPosition(Vector2f(-205, 210));
	image->GetPivot() = Vector2f(-45, 95);

    pNewObject->AddBitmapInfo(m_pHandL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HandL");
    image->GetTransform().SetPosition(Vector2f(100, -207));
    image->GetPivot() = Vector2f(-12, 70);

    pNewObject->AddBitmapInfo(m_pEyeR_Brow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeR_Brow");
    image->GetTransform().SetPosition(Vector2f(-3, -70));

    pNewObject->AddBitmapInfo(m_pEyeR_Lid);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeR_Lid");
    image->GetTransform().SetPosition(Vector2f(2, -24));

    pNewObject->AddBitmapInfo(m_pEyeR_Pupil);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeR_Pupil");
    image->GetTransform().SetPosition(Vector2f(-12, -2));

    pNewObject->AddBitmapInfo(m_pEyeR_White);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeR_White");
    image->GetTransform().SetPosition(Vector2f(80, -20));

    pNewObject->AddBitmapInfo(m_pEyeL_Brow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeL_Brow");
    image->GetTransform().SetPosition(Vector2f(3, -70));

    pNewObject->AddBitmapInfo(m_pEyeL_Lid);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeL_Lid");
    image->GetTransform().SetPosition(Vector2f(-2, -24));

    pNewObject->AddBitmapInfo(m_pEyeL_Pupil);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeL_Pupil");
    image->GetTransform().SetPosition(Vector2f(12, -2));

    pNewObject->AddBitmapInfo(m_pEyeL_White);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("EyeL_White");
    image->GetTransform().SetPosition(Vector2f(-80, -20));

    pNewObject->AddBitmapInfo(m_pMouth);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Mouth");
    image->GetTransform().SetPosition(Vector2f(0, 33));

    pNewObject->AddBitmapInfo(m_pNose);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Nose");
    image->GetTransform().SetPosition(Vector2f(0, -70));



    pNewObject->AddBitmapInfo(m_pHairF_SideL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairF_SideL");
    image->GetTransform().SetPosition(Vector2f(2, -13));
    image->GetPivot() = Vector2f(93, -135);

    pNewObject->AddBitmapInfo(m_pHairF_SideR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairF_SideR");
    image->GetTransform().SetPosition(Vector2f(2, -13));
    image->GetPivot() = Vector2f(-93, -135);

    pNewObject->AddBitmapInfo(m_pHairF_Bang);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairF_Bang");
    image->GetTransform().SetPosition(Vector2f(4, -7));
    image->GetPivot() = Vector2f(7, -65);

    pNewObject->AddBitmapInfo(m_pHairF);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairF");
    image->GetTransform().SetPosition(Vector2f(7, -15));
    image->GetPivot() = Vector2f(0, -115);

    pNewObject->AddBitmapInfo(m_pHairSideL_1);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairSideL_1");
    image->GetTransform().SetPosition(Vector2f(-105, 31));
    image->GetPivot() = Vector2f(30, -200);

    pNewObject->AddBitmapInfo(m_pHairSideL_2);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairSideL_2");
    image->GetTransform().SetPosition(Vector2f(-102, 43));
    image->GetPivot() = Vector2f(50, -300);

    pNewObject->AddBitmapInfo(m_pHairSideR_1);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairSideR_1");
    image->GetTransform().SetPosition(Vector2f(100, 37));
    image->GetPivot() = Vector2f(-30, -200);

    pNewObject->AddBitmapInfo(m_pHairSideR_2);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairSideR_2");
    image->GetTransform().SetPosition(Vector2f(93, 66));
    image->GetPivot() = Vector2f(-50, -300);

    pNewObject->AddBitmapInfo(m_pFace);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Face");
    image->GetTransform().SetPosition(Vector2f(2, -362));
    image->GetPivot() = Vector2f(0, 140);

    pNewObject->AddBitmapInfo(m_pBody);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Body");
    image->GetTransform().SetPosition(Vector2f(0, 220));
    image->GetPivot() = Vector2f(0, 180);

    pNewObject->AddBitmapInfo(m_pBody_ArmL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Body_ArmL");
    image->GetTransform().SetPosition(Vector2f(-60, -298));
    image->GetPivot() = Vector2f(117, -158);

    pNewObject->AddBitmapInfo(m_pBody_ArmR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Body_ArmR");
    image->GetTransform().SetPosition(Vector2f(60, -298));
    image->GetPivot() = Vector2f(-107, -158);

    pNewObject->AddBitmapInfo(m_pHair_AhogeL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeL");
    image->GetTransform().SetPosition(Vector2f(0, -26));
    image->GetPivot() = Vector2f(12, 40);

    pNewObject->AddBitmapInfo(m_pHair_AhogeR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeR");
    image->GetTransform().SetPosition(Vector2f(0, -26));
    image->GetPivot() = Vector2f(-42, 40);

    pNewObject->AddBitmapInfo(m_pHairB_HeadL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadL");
    image->GetTransform().SetPosition(Vector2f(0, -45));
    image->GetPivot() = Vector2f(0, -170);

    pNewObject->AddBitmapInfo(m_pHairB_HeadR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadR");
    image->GetTransform().SetPosition(Vector2f(0, -45));
    image->GetPivot() = Vector2f(0, -170);
	image->SetCurFrame(3);

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftL");
    image->GetTransform().SetPosition(Vector2f(-120, 22));
    image->GetPivot() = Vector2f(65, -35);

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftR");
    image->GetTransform().SetPosition(Vector2f(120, 18));
    image->GetPivot() = Vector2f(-62, -50);

    pNewObject->AddBitmapInfo(m_pHairBL);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBL");
    image->GetTransform().SetPosition(Vector2f(0, 140));
    image->GetPivot() = Vector2f(0, -300);

    pNewObject->AddBitmapInfo(m_pHairBR);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBR");
    image->GetTransform().SetPosition(Vector2f(0, 140));
    image->GetPivot() = Vector2f(0, -300);
    image->SetCurFrame(3);



    // 그림자
    pNewObject->AddBitmapInfo(m_pHair_AhogeL_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeL_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(12, 40);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("Hair_AhogeL"));

    pNewObject->AddBitmapInfo(m_pHair_AhogeR_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeR_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-42, 40);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("Hair_AhogeR"));

    pNewObject->AddBitmapInfo(m_pHairB_HeadL_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadL_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -170);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_HeadL"));

    pNewObject->AddBitmapInfo(m_pHairB_HeadR_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadR_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -170);
    image->SetCurFrame(4);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_HeadR"));

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftL_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftL_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(65, -35);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_Head_SideTuftL"));

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftR_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftR_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-62, -50);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_Head_SideTuftR"));

    pNewObject->AddBitmapInfo(m_pHairBL_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBL_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -300);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairBL"));

    pNewObject->AddBitmapInfo(m_pHairBR_Shadow);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBR_Shadow");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -300);
    image->SetCurFrame(4);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairBR"));



    // 라인
    pNewObject->AddBitmapInfo(m_pHair_AhogeL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(12, 40);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("Hair_AhogeL"));

    pNewObject->AddBitmapInfo(m_pHair_AhogeR_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Hair_AhogeR_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-42, 40);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("Hair_AhogeR"));

    pNewObject->AddBitmapInfo(m_pHairB_HeadL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -170);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_HeadL"));

    pNewObject->AddBitmapInfo(m_pHairB_HeadR_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_HeadR_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -170);
    image->SetCurFrame(5);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_HeadR"));

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(65, -35);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_Head_SideTuftL"));

    pNewObject->AddBitmapInfo(m_pHairB_Head_SideTuftR_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairB_Head_SideTuftR_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-62, -50);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairB_Head_SideTuftR"));

    pNewObject->AddBitmapInfo(m_pHairBL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -300);
    image->SetCurFrame(2);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairBL"));

    pNewObject->AddBitmapInfo(m_pHairBR_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HairBR_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(0, -300);
    image->SetCurFrame(5);
    image->SetParentImage(pNewObject->GetBitmapInfo("HairBR"));

    pNewObject->AddBitmapInfo(m_pArmL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("ArmL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-45, 95);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("ArmL"));

    pNewObject->AddBitmapInfo(m_pHandL_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("HandL_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->GetPivot() = Vector2f(-12, 70);
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("HandL"));

    pNewObject->AddBitmapInfo(m_pEffect_Line);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Effect_Line");
    image->GetTransform().SetPosition(Vector2f(0, 0));
    image->SetCurFrame(1);
    image->SetParentImage(pNewObject->GetBitmapInfo("Effect"));



    // 널(컨트롤러) 객체
    pNewObject->AddBitmapInfo(m_pNull_Hair);
    image = pNewObject->GetLastBitmapInfo();
    image->SetName("Null_Hair");
    image->GetTransform().SetPosition(Vector2f(0, -300));
	image->SetActive(false); // 널 객체는 보이지 않게 설정



	// 부모-자식 관계 설정
    pNewObject->GetBitmapInfo("Face")->SetParentImage(pNewObject->GetBitmapInfo("Body"));
    pNewObject->GetBitmapInfo("Body_ArmL")->SetParentImage(pNewObject->GetBitmapInfo("Body"));
    pNewObject->GetBitmapInfo("Body_ArmR")->SetParentImage(pNewObject->GetBitmapInfo("Body"));

    pNewObject->GetBitmapInfo("Nose")->SetParentImage(pNewObject->GetBitmapInfo("Face"));
    pNewObject->GetBitmapInfo("Null_Hair")->SetParentImage(pNewObject->GetBitmapInfo("Face"));

    pNewObject->GetBitmapInfo("EyeL_White")->SetParentImage(pNewObject->GetBitmapInfo("Nose"));
    pNewObject->GetBitmapInfo("EyeR_White")->SetParentImage(pNewObject->GetBitmapInfo("Nose"));
    pNewObject->GetBitmapInfo("Mouth")->SetParentImage(pNewObject->GetBitmapInfo("Nose"));

    pNewObject->GetBitmapInfo("EyeL_Brow")->SetParentImage(pNewObject->GetBitmapInfo("EyeL_White"));
    pNewObject->GetBitmapInfo("EyeL_Lid")->SetParentImage(pNewObject->GetBitmapInfo("EyeL_White"));
    pNewObject->GetBitmapInfo("EyeL_Pupil")->SetParentImage(pNewObject->GetBitmapInfo("EyeL_White"));

    pNewObject->GetBitmapInfo("EyeR_Brow")->SetParentImage(pNewObject->GetBitmapInfo("EyeR_White"));
    pNewObject->GetBitmapInfo("EyeR_Lid")->SetParentImage(pNewObject->GetBitmapInfo("EyeR_White"));
    pNewObject->GetBitmapInfo("EyeR_Pupil")->SetParentImage(pNewObject->GetBitmapInfo("EyeR_White"));

    pNewObject->GetBitmapInfo("HairF_SideL")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairF_SideR")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairF_Bang")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairF")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairSideL_1")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairSideL_2")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairSideR_1")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairSideR_2")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("Hair_AhogeL")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("Hair_AhogeR")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairB_HeadL")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairB_HeadR")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairB_Head_SideTuftL")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairB_Head_SideTuftR")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairBL")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));
    pNewObject->GetBitmapInfo("HairBR")->SetParentImage(pNewObject->GetBitmapInfo("Null_Hair"));

    pNewObject->GetBitmapInfo("HandL")->SetParentImage(pNewObject->GetBitmapInfo("ArmL"));



    pNewObject->ReverseBitmapInfo();

	// 애니메이터 설정
	pNewObject->GetAnimator().SetOwner(pNewObject);
	pNewObject->GetAnimator().AddAnimationClip(CoffiaAnimationClips::TestClip()); 
    //pNewObject->GetAnimator().SetCurrentClip("TestClip");

    m_GameObjectPtrTable[0] = pNewObject; // 첫번째 자리에 만든 객체 넣기
}

void MyFirstWndGame::CreateEnemy()
{
    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    float x = m_EnemySpawnPos.x;
    float y = m_EnemySpawnPos.y;

    m_EnemySpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(0.4f); // 일단, 임의로 설정   

    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    pNewObject->AddBitmapInfo(m_pEnemyBitmapInfo);

    pNewObject->SetWidth(100);
    pNewObject->SetHeight(100);

    int i = 0;
    while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
    {
        if (nullptr == m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i] = pNewObject;
            break;
        }
    }

    if (i == MAX_GAME_OBJECT_COUNT)
    {
        // 게임 오브젝트 테이블이 가득 찼습니다.
        delete pNewObject;
        pNewObject = nullptr;
    }
}

// 플레이어
void MyFirstWndGame::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer(); // _아까 만들었던 플레이어 오브젝트의 주소를 받아옴(0번 인덱트 오브젝트 직접 접근)

    assert(pPlayer != nullptr);

    Vector2f mousePos(m_PlayerTargetPos.x, m_PlayerTargetPos.y);    // _마우스 위치 xy값 벡터2로 저장
    Vector2f playerPos = pPlayer->GetPosition();    // _지금 플레이어 위 xy값을 벡터2로 저장

    Vector2f playerDir = mousePos - playerPos;  // _좌표 빼고
    float distance = playerDir.Length(); // 거리 계산

    if (distance > 50.f) // 임의로 설정한 거리 // _클릭 좌표가 50보다 멀어지면 이동하게 하는 코드
    {
        playerDir.Normalize(); // 정규화   // 방향 계산만 했으니까 거리 1로 딱 맞추기
        pPlayer->SetDirection(playerDir); // 플레이어 방향 설정 // _플레이어가 이동할 방향을 저장해둠
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // 플레이어 정지
    }
}

// 적
void MyFirstWndGame::UpdateEnemyInfo(int index, float deltaTime)
{
	if (index == 0) return; // 플레이어는 업데이트할 필요 없음


	GameObject* pPlayer = GetPlayer();
    GameObject* pCurEnemy = GetGameObject(index);

    int closestIndex = FindClosestObjectAt(pCurEnemy->GetPosition(), index);
    GameObject* pClosestEnemy = closestIndex == -1 ? nullptr : GetGameObject(closestIndex);

    assert(pPlayer != nullptr);
    assert(pCurEnemy != nullptr);
    assert(pClosestEnemy != nullptr);

    // 계속 써서 저장
    Vector2f curEnemyPos = pCurEnemy->GetPosition();

    // 현재 충돌 중인지 확인
    if (pClosestEnemy->IsCollidingWith(pCurEnemy->GetPosition())) {
		Vector2f escapeDir = pClosestEnemy->GetPosition() - pCurEnemy->GetPosition(); 
		escapeDir.Normalize(); // 탈출 방향 정규화
		escapeDir = escapeDir * -2.0f * pCurEnemy->GetRadius() + pClosestEnemy->GetPosition(); // 탈출 방향을 반대로 하고, 적의 반지름만큼 멀어지도록 조정
        pCurEnemy->SetPosition(escapeDir);
        return;
    }

	// 플레이어를 향하는 벡터 계산
	Vector2f playerDir = pPlayer->GetPosition() - curEnemyPos;
    Vector2f targetDir = playerDir;

    // 다음 프레임에 이동할 위치 계산
	Vector2f nextPos = curEnemyPos + playerDir.Normalized() * pCurEnemy->GetSpeed() * deltaTime;

    // 플레이어한테 도달했으면 멈추기
    if (pPlayer->IsCollidingWith(nextPos)) {
        pCurEnemy->SetDirection(Vector2f(0, 0)); // 정지
        return;
    }

    // 다음 프레임에 이동할 위치 계산하고 장애물이 있는지 확인
    std::unordered_set<int> collidedObjects;
    pClosestEnemy = GetGameObject(FindClosestObjectAt(nextPos, index));
    while (pClosestEnemy && pClosestEnemy->IsCollidingWith(nextPos)) {
		
		// 충돌한 객체의 인덱스 찾기
		int closestIndex = FindClosestObjectAt(nextPos, index);

		// 이미 충돌한 객체이거나 플레이어라면 정지
        if (collidedObjects.find(closestIndex) != collidedObjects.end() || closestIndex == 0) {
            pCurEnemy->SetDirection(Vector2f(0, 0)); // 정지
			return;
        }
        collidedObjects.insert(closestIndex);

		Vector2f closestDir = pClosestEnemy->GetPosition() - curEnemyPos;
        targetDir = { closestDir.y, -closestDir.x }; // 충돌한 객체와의 법선 벡터 계산(임시)
        if (targetDir.x * playerDir.x + targetDir.y * playerDir.y < 0) {
            targetDir = targetDir * -1.0f; // 법선 벡터가 플레이어에게서 멀어진다면 반대 방향 법선 벡터 채용
        }

        nextPos = curEnemyPos + targetDir.Normalized() * pCurEnemy->GetSpeed() * deltaTime;
        pClosestEnemy = GetGameObject(FindClosestObjectAt(nextPos, index));
    }

    targetDir.Normalize();
    pCurEnemy->SetDirection(targetDir); // 이동 방향 지정
}

void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();   // _타이머에 한 틱 추가(시간 재기)

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS(); // _이번 프레임의 델타타임 저장
    m_fFrameCount += m_fDeltaTime;  // _매 프레임 델타타임 저장(실제 지나간 시간 측정)

    LogicUpdate();  // 플레이어한테 방향 벡터 설정하기

    while (m_fFrameCount >= 200.0f) //  _0.2초마다 실행
    {
        FixedUpdate();
        m_fFrameCount -= 200.0f;
    }
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);    // _백버퍼를 하얗게 초기화

    //메모리 DC에 그리기
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i) // _모든 오브젝트를 하나하나 출력(정확히는 백DC에 그리기)
    {
        if (m_GameObjectPtrTable[i])
        {
            if (IsValidSpawnPosition(m_GameObjectPtrTable[i]->GetPosition(), i))
                m_GameObjectPtrTable[i]->Render(m_hBackDC, RGB(255, 0, 0));
            else
                m_GameObjectPtrTable[i]->Render(m_hBackDC, RGB(0, 0, 255));
        }
    }

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY); // 백dc 내용을 프론트dc에 복사
}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
    /*   std::cout << __FUNCTION__ << std::endl;
       std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
    /*  std::cout << __FUNCTION__ << std::endl;
 std::cout << "x: " << x << ", y: " << y << std::endl;*/

    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;

}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
    std::cout << __FUNCTION__ << std::endl;
    /*  std::cout << __FUNCTION__ << std::endl;
   std::cout << "x: " << x << ", y: " << y << std::endl;*/


    Vector2f targetPos = { (float)x,(float)y }; // 최종적으로 들어갈 enemy 좌표
    
    const Vector2f startPos = targetPos;  // _시작 위치
    const Vector2f normal = { 1,0 };

    int angle = 0;  // 검사할 각도 0~359
    float spawnDistance = 0;
    std::vector<GameObjectBase*> nearbyObjects;
    double radius = m_GameObjectPtrTable[0]->GetRadius();

    float minDistance = -1;
    float curDistance = 0;

    bool firstCheck;      // 적당한 위치 정착 성공 여부(최초 1회만)

    // 최초 검사
    angle = 1;
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])    // 오브젝트 테이블에 있는 오브젝트들 싹다 검사
        {
            targetPos = m_GameObjectPtrTable[i]->TryGetCollisionDirection(targetPos, angle);
        }
        angle *= -1;
    }
    firstCheck = IsValidSpawnPosition(targetPos);

    float targetPosDistance = (startPos - targetPos).Length();
    if (targetPosDistance > radius * 2)
        firstCheck = false;

    // 첫번째 검사에서 제대로 된 위치가 도출됐다면
    if (firstCheck) {
        m_EnemySpawnPos.x = targetPos.x;
        m_EnemySpawnPos.y = targetPos.y;
        return;
    }

    // 첫번째 검사 이후 좌표 계산
    angle = 0;
    while (true) {

        // 범위 조건에 맞는 오브젝트 탐색, 그 중 가장 가까운 거리값 저장
        nearbyObjects.clear();
        minDistance = -1;
        curDistance = 0;

        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i) {
            if (!m_GameObjectPtrTable[i]) continue;   // null 값이 나오면 반복문 탈출
            if (m_GameObjectPtrTable[i]->IsAtDistanceFrom(startPos, spawnDistance, curDistance)) { // 오브젝트가 좌표, 거리가 조건에 해당하는지 확인 
                nearbyObjects.push_back(m_GameObjectPtrTable[i]);
                if (minDistance == -1 || minDistance > curDistance) {
                    minDistance = curDistance;
                }
            }
        }
        std::cout << nearbyObjects.size() << std::endl;

        // 클릭 좌표로부터 같은 거리를 가지는 모든 각도의 좌표 확인
        for (angle = 0; angle < 360; angle++) {
            // 예비 생성 좌표 생성
            Vector2f targetPos = startPos + Rotate(normal, (float)angle).Normalized() * spawnDistance;

            int count = 0;
            for (int i = 0; i < nearbyObjects.size(); ++i) {
                if (nearbyObjects[i]->IsCollidingWith(targetPos)) {
                    count++;
                    break;
                }
            }
            if (count == 0) {
                m_EnemySpawnPos.x = targetPos.x;
                m_EnemySpawnPos.y = targetPos.y;
                return;
            }
        }

        // 가장 가까운 오브젝트와의 거리 + 반지름 + 여유값 (가장 가까운 오브젝트가 다음번에는 )
        spawnDistance = minDistance + (radius * 2) + 1.0f;
    }
}

void MyFirstWndGame::OnZKeyDown() {

}

void MyFirstWndGame::OnXKeyDown() {

}

// _이 위치가 유효한 스폰 위치인지 확인하는 함수(겹치는거 있으면 false, 없는 true)
bool MyFirstWndGame::IsValidSpawnPosition(learning::Vector2f pos, int selfCheck)
{
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (selfCheck != -1 && i == selfCheck) continue;
        if (m_GameObjectPtrTable[i])    // 오브젝트 테이블에 있는 오브젝트들 싹다 검사
        {
            if (m_GameObjectPtrTable[i]->IsCollidingWith(pos)) {
                return false;
            }
        }
    }
    return true;
}

learning::Vector2f MyFirstWndGame::Rotate(learning::Vector2f dir, float angleOffset) {

    // 1도 돌리는데 사용할 라디안 값 (더 돌릴거면 1.0값을 바꾸면 됨)
    float radian = angleOffset * (3.141592f / 180.0f);

    float cosA = cos(radian);
    float sinA = sin(radian);

    return learning::Vector2f(
        dir.x * cosA - dir.y * sinA,
        dir.x * sinA + dir.y * cosA
    );
}

// _해당 좌표에서 충돌하는 오브젝트의 인덱스 반환하는 함수(자기 자신과 ignoreIndex는 검사에서 제외)
int MyFirstWndGame::FindCollisionAt(learning::Vector2f pos, int selfIndex, int ignoreIndex)
{


    return 0;
}

// _해당 좌표에서 가장 가까운 오브젝트의 인덱스 반환하는 함수(자기 자신과 ignoreIndex는 검사에서 제외)
int MyFirstWndGame::FindClosestObjectAt(learning::Vector2f pos, int selfIndex, int ignoreIndex)
{
	float distance = -1;
    int index = -1;

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
		if (i == selfIndex) continue;           // 자기 자신은 검사할 필요 없음
		if (i == ignoreIndex) continue;         // 제외 인덱스는 검사에서 제외
		if (!m_GameObjectPtrTable[i]) continue; // null 값이 나오면 반복문 탈출

        float curDistance = (m_GameObjectPtrTable[i]->GetPosition() - pos).Length();
        if (distance == -1 || distance > curDistance) {
            distance = curDistance;
            index = i;
        }
	}
	return index;   // 가장 가까운 오브젝트의 인덱스 반환
	                // -1 반환은 충돌하는 오브젝트가 없다는 뜻 (사실상 오류)
}

// _반지름 값을 가진 원 오브젝트가 pos 좌표로 이동했을때, 타겟 오브젝트와 충돌하는지 여부 
bool MyFirstWndGame::IsOverlapping(learning::Vector2f pos, float radius, GameObject* targetObject) {
    return true;
}

/*

대략적인 구조

검사 각도 = 0; //0~359;
생성거리 = 0;
오브젝트 목록 초기화
float 가장가까운거리

while true
{
    모든 오브젝트 검사
    {
        해당 좌표와의 거리가 (클릭 좌표+생성거리+-반지름 값인 객체를 따로 목록에 저장)
        위 조건에 맞는 오브젝트 중에서 거리가 가장 가까운 오브젝트와의 거리를 저장해둠
    }
    for (검사각도 =0; 검사각도 < 360;검사각도++)
    {
        새로운 예비생성 좌표 = 클릭좌표 + ((1,0)벡터*검사각도).노말라이즈 * 생성거리;
        int count =0;
        해당 오브젝트 목록 검사
        {
            좌표랑 충돌하는 오브젝트 있음?
            {
                count++;
                        break;
            }
        }
        if (count ==0)
            해당 좌표 반환 및 종료!!!
    }
    가장 가까운 오브젝트와의 거리+ 반지름 + 보정값1 을 다음 생성 범위로 지정
}


*/