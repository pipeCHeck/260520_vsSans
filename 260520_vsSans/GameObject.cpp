#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <assert.h>
#include <iostream>
using BitmapInfo = renderHelp::BitmapInfo;

GameObject::~GameObject()
{
    if (m_pColliderCircle)
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }
}

void GameObject::ClearBitmapInfo() 
{
    m_pBitmapInfo.clear();
}

void GameObject::AddBitmapInfo(BitmapInfo* bitmapInfo) 
{
    if (bitmapInfo == nullptr)
        return;

    m_pBitmapInfo.push_back(bitmapInfo);
}

// _오브젝트 하나의 업데이트
void GameObject::Update(float deltaTime)
{
    // _로직 업데이트 안에서 먼저 방향벡터 설정했으니, 
    Move(deltaTime);

    UpdateFrame(deltaTime);

    m_animator.Play(deltaTime);

    // Collider 업데이트
    // _콜라이더의 중심점 위치를 이동시킴 (구조체 변수인데, 도형 형태에 따라 둘중 하나만 있을 예정)
    if (m_pColliderCircle)
    {
        m_pColliderCircle->center = m_pos;
    }
    if (m_pColliderBox)
    {
        m_pColliderBox->center = m_pos;
    }


}

// _렌더 메서드
void GameObject::Render(HDC hdc, COLORREF color)
{
    DrawBitmap(hdc);
    DrawCollider(hdc, color);
}


// _원 콜라이더 반지름 지정
void GameObject::SetColliderCircle(float radius)
{
    if (m_pColliderCircle)  // _만약에 이미 원이 존재하면 나 삭제. (이미 있는 상태에서 새로운걸로 초기화되지 않도록 처리하는듯)
                            // _싱글톤이랑 생긴건 비슷한데 객체는 이미 만들어져있고, 그 객체 내에서 있는 콜라이더 객체 하나에 한정함
                            // _작은 구조의 싱글톤이라 봐도 무방할 듯..?
    {
        delete m_pColliderCircle;
        m_pColliderCircle = nullptr;
    }

    m_pColliderCircle = new ColliderCircle; // _원 객체 생성

    assert(m_pColliderCircle != nullptr && "Failed to create ColliderCircle!"); // _원 객체가 널이면 오류 체크

    m_pColliderCircle->radius = radius; // _반지름 값 넣기
    m_pColliderCircle->center = m_pos;  // _중앙 위치값 넣기
}


void GameObject::SetColliderBox(float width, float height)
{
    if (m_pColliderBox)
    {
        delete m_pColliderBox;
        m_pColliderBox = nullptr;
    }

    m_pColliderBox = new ColliderBox;

    assert(m_pColliderBox != nullptr && "Failed to create ColliderBox!");

    m_pColliderBox->center = m_pos;
    m_pColliderBox->halfSize.x = width / 2.0f;
    m_pColliderBox->halfSize.y = height / 2.0f;
}

// 과제: 해당 코드의 문제는 무엇일까요? 어떻게 개선하면 좋을까요?
// 개선 방향에 대해 서로 토론하고 비교해 보세요.
void GameObject::DrawCollider(HDC hdc, COLORREF color)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, color);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // _hdc에 새로만든 hPen을 집어넣고 기존에 hdc에 있던 팬을 hOldPen에 저장
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    // _원 그리기
    if (m_pColliderCircle)
    {
        // _너무 m_pColliderCircle->center 같은거를 너무 자주 접근하는게 문제인가..?
        //Ellipse(hdc, m_pColliderCircle->center.x - m_pColliderCircle->radius,
        //    m_pColliderCircle->center.y - m_pColliderCircle->radius,
        //    m_pColliderCircle->center.x + m_pColliderCircle->radius,
        //    m_pColliderCircle->center.y + m_pColliderCircle->radius);

        learning::Vector2f center = m_pColliderCircle->center;
        double radius = m_pColliderCircle->radius;

        Ellipse(hdc, center.x - radius,
            center.y - radius,
            center.x + radius,
            center.y + radius);
    }

    // _박스 그리기
    if (m_pColliderBox)
    {
        //Rectangle(hdc, m_pColliderBox->center.x - m_pColliderBox->halfSize.x,
        //    m_pColliderBox->center.y - m_pColliderBox->halfSize.y,
        //    m_pColliderBox->center.x + m_pColliderBox->halfSize.x,
        //    m_pColliderBox->center.y + m_pColliderBox->halfSize.y);

        learning::Vector2f center = m_pColliderBox->center;
        learning::Vector2f halfSize = m_pColliderBox->halfSize;

        Ellipse(hdc, center.x - halfSize.x,
            center.y - halfSize.y,
            center.x + halfSize.x,
            center.y + halfSize.y);
    }

    // 이전 객체 복원 및 펜 삭제
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

// _오브젝트 이동하기
void GameObject::Move(float deltaTime)
{
    GameObjectBase::Move(deltaTime);    // _이번 프레임의 델타타임에 비례한 만큼 이동시키기
}

// _이름 지정
void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}

// _충돌하는지 확인하고 벡터 반환

learning::Vector2f GameObject::TryGetCollisionDirection(Vector2f pos, float angleOffset) {
    std::cout << __FUNCTION__ << std::endl;

    Vector2f targetPos = pos;

    Vector2f dir = m_pos - pos;  // 두 사이 방향 계산
    float distance = dir.Length(); // 거리 계산
    Vector2f dirNormalized = Rotate(dir, angleOffset).Normalized(); // 노멀라이즈

    if (distance < m_pColliderCircle->radius * 2) {
        // 충돌
        targetPos = m_pos + (dirNormalized * -1) * m_pColliderCircle->radius * 2.1f;
    }

    std::cout << "dirNormalized: " << dirNormalized.x << ", " << dirNormalized.y << std::endl;

    return targetPos;
}

// _충돌했는지 여부만 따지기
bool GameObject::IsCollidingWith(Vector2f pos) {
    Vector2f dir = m_pos - pos;  // 두 사이 방향 계산
    float distance = dir.Length(); // 거리 계산
    //Vector2f dirNormalized = dir.Normalized(); // 노멀라이즈

    if (distance < m_pColliderCircle->radius * 2 - 2.0f) {
        return true;
    }
    else {
        return false;
    }
}

double GameObject::GetRadius()
{
    if (m_pColliderCircle == nullptr)
        return 0.0;

    return m_pColliderCircle->radius;
}

learning::Vector2f GameObject::Rotate(learning::Vector2f dir, float angleOffset) {

    // 1도 돌리는데 사용할 라디안 값 (더 돌릴거면 1.0값을 바꾸면 됨)
    float radian = angleOffset * (3.141592f / 180.0f);

    float cosA = cos(radian);
    float sinA = sin(radian);

    return learning::Vector2f(
        dir.x * cosA - dir.y * sinA,
        dir.x * sinA + dir.y * cosA
    );
}

void GameObject::DrawBitmap(HDC hdc)
{
    if (m_pBitmapInfo.empty()) return;

    HDC hBitmapDC = CreateCompatibleDC(hdc);

    for (int i = 0; i < m_pBitmapInfo.size(); i++) {

        if (m_pBitmapInfo[i] == nullptr) continue;
        if (m_pBitmapInfo[i]->GetBitmapHandle() == nullptr) continue;
        if (!(m_pBitmapInfo[i]->GetActive())) continue;

        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo[i]->GetBitmapHandle());

        // BLENDFUNCTION 설정 (알파 채널 처리)
        BLENDFUNCTION blend = { 0 };
        blend.BlendOp = AC_SRC_OVER;
        blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
        blend.AlphaFormat = AC_SRC_ALPHA;

		// 이미지의 월드 Transform
        Transform worldTransform = m_pBitmapInfo[i]->GetWorldTransform() + Transform{ m_pos, 0, Vector2f{1,1} };

		// 이미지의 프레임 크기와 피벗 계산
        Vector2f frameSize = m_pBitmapInfo[i]->GetFrameSize();
        Vector2f pivot = m_pBitmapInfo[i]->GetPivot();

        // 이미지의 위치 계산
        Vector2f imagePos = worldTransform.position - (frameSize / 2.0f + pivot) * worldTransform.scale;

		// 이미지의 실제 중심점 위치 계산 (회전의 중심점)
		Vector2f pivotPos = worldTransform.position;

        // 이미지의 크기 계산
		Vector2f drawSize = frameSize * worldTransform.scale;

        // hdc 상태 저장 후 회전 계산
        SaveDC(hdc);
        SetGraphicsMode(hdc, GM_ADVANCED);
        SetRotationTransform(hdc, worldTransform.rotation, pivotPos);

        // 시트 속 스프라이트 위치 계산
        Vector2f srcPos{
            m_pBitmapInfo[i]->GetCurFrame() % m_pBitmapInfo[i]->GetFrameColumns() * (float)m_pBitmapInfo[i]->GetFrameWidth(),
            m_pBitmapInfo[i]->GetCurFrame() / m_pBitmapInfo[i]->GetFrameColumns() * (float)m_pBitmapInfo[i]->GetFrameHeight()
        };

		// 시트 속 스프라이트 크기 계산 (보통은 프레임 크기와 같지만, 혹시나 해서)
		Vector2f srcSize = frameSize;

		// hdc, 출력 위치, 크기, hBitmapDC, 원본 이미지의 시작 위치, 원본 이미지의 크기, 블렌드 함수
        AlphaBlend(hdc, imagePos.x, imagePos.y, drawSize.x, drawSize.y, hBitmapDC, srcPos.x, srcPos.y, srcSize.x, srcSize.y, blend);
        
		// hdc 상태 복원(회전에 의해 설정한 것 돌리기)
        RestoreDC(hdc, -1);

		// 피벗 마커 그리기 (디버그용)
        DrawPivotMarker(hdc, pivotPos);

        // 비트맵 핸들 복원
        SelectObject(hBitmapDC, hOldBitmap);
    }

    DeleteDC(hBitmapDC);
}

void GameObject::DrawPivotMarker(HDC hdc, Vector2f pivotPos)
{
    int radius = 4;
    int line = 8;

    int x = (int)pivotPos.x;
    int y = (int)pivotPos.y;

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    MoveToEx(hdc, x - line, y, nullptr);
    LineTo(hdc, x + line, y);

    MoveToEx(hdc, x, y - line, nullptr);
    LineTo(hdc, x, y + line);

    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);

    DeleteObject(hPen);
}

void GameObject::SetRotationTransform(HDC hdc, float degree, Vector2f pivot)
{
    float rad = degree * 3.14159265f / 180.0f;

    float cosA = cosf(rad);
    float sinA = sinf(rad);

	// 회전 변환 행렬 설정
    XFORM xForm;

    xForm.eM11 = cosA;
    xForm.eM12 = sinA;
    xForm.eM21 = -sinA;
    xForm.eM22 = cosA;

    xForm.eDx = pivot.x - cosA * pivot.x + sinA * pivot.y;
    xForm.eDy = pivot.y - sinA * pivot.x - cosA * pivot.y;

    SetWorldTransform(hdc, &xForm);
}

void GameObject::UpdateFrame(float deltaTime)
{
    m_frameTime += deltaTime;
    if (m_frameTime >= m_frameDuration)
    {
        m_frameTime = 0.0f;
        //m_frameIndex = (m_frameIndex + 1) % (m_frameCount);

        for (int i = 0; i < m_pBitmapInfo.size(); i++) {
            m_pBitmapInfo[i]->SetCurFrame(m_pBitmapInfo[i]->GetCurFrame());
        }
    }
}

// 해당 좌표와의 거리가 생성거리 기준선에 충돌하는지 확인(참조로 실제 거리도 반환)
bool GameObject::IsAtDistanceFrom(Vector2f pos, float targetDistance, float& distanceToTarget) {

    Vector2f dir = m_pos - pos;  // 두 사이 방향 계산
    float distance = dir.Length(); // 거리 계산

    float minDist = targetDistance - m_pColliderCircle->radius * 2; // _기준선에 충돌하는 최소 거리 
    float maxDist = targetDistance + m_pColliderCircle->radius * 2; // _기준선에 충돌하는 최대 거리 

    if (minDist < 0) minDist = 0;

    distanceToTarget = distance;    // _실제 거리 반환
    return (minDist <= distance && distance <= maxDist);    // _기준선에 충돌하는 오브젝트라면 true
}

BitmapInfo* GameObject::GetBitmapInfo(string name) {
    for (int i = 0; i < m_pBitmapInfo.size(); i++) {
        if (m_pBitmapInfo[i]->GetName() == name) {
            return m_pBitmapInfo[i];
        }
    }
    return nullptr;
}

void GameObject::ReverseBitmapInfo() 
{
	reverse(m_pBitmapInfo.begin(), m_pBitmapInfo.end());
}