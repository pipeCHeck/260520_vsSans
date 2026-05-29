#pragma once

#include "Utillity.h"
#include "Transform.h"
#include <algorithm>
#include <vector>
using namespace std;

// [CHECK]. namespace 포함해서 전방 선언해야 함
namespace learning
{
    struct ColliderCircle;
    struct ColliderBox;
}

enum class ObjectType
{
    PLAYER,
    ENEMY,
    BULLET,
    ITEM,
    BACKGROUND,
};

namespace renderHelp
{
    class BitmapInfo;
}


constexpr int OBJECT_NAME_LEN_MAX = 15;

class GameObjectBase
{
    using Vector2f = learning::Vector2f;
public:
    GameObjectBase() = delete;
    GameObjectBase(const GameObjectBase&) = delete;

    GameObjectBase(ObjectType type) : m_type(type) {}

    virtual ~GameObjectBase() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(HDC hdc, COLORREF color) = 0;

    void SetPosition(float x, float y) { m_pos = { x, y }; }
    void SetPosition(Vector2f pos) { m_pos = { pos.x, pos.y }; }
    void SetDirection(Vector2f dir) { m_dir = dir; }
    void SetSpeed(float speed) { m_speed = speed; }
    void SetName(const char* name);

    ObjectType Type() const { return m_type; }

    const char* GetName() const { return m_name; }

    Vector2f GetPosition() const { return m_pos; }
    Vector2f GetDirection() const { return m_dir; }

    float GetSpeed() const { return m_speed; }

    virtual Vector2f TryGetCollisionDirection(Vector2f pos, float angleOffset) = 0;
    virtual bool IsCollidingWith(Vector2f pos) = 0;
    virtual bool IsAtDistanceFrom(Vector2f pos, float targetDistance, float& distanceToTarget) = 0;
    virtual double GetRadius() = 0;
protected:

    void Move(float deltaTime)
    {
        m_pos.x += m_dir.x * m_speed * deltaTime;
        m_pos.y += m_dir.y * m_speed * deltaTime;
    }

protected:
    ObjectType m_type;

    Vector2f m_pos = { 0.0f, 0.0f };
    Vector2f m_dir = { 0.0f, 0.0f }; // 방향 (단위 벡터)

    float m_speed = 0.0f; // 속력

    char m_name[OBJECT_NAME_LEN_MAX] = "";
};

class GameObject : public GameObjectBase
{
    using Vector2f = learning::Vector2f;
    using ColliderCircle = learning::ColliderCircle;
    using ColliderBox = learning::ColliderBox;
    using BitmapInfo = renderHelp::BitmapInfo;
    using Transform = transform::Transform;

public:
    GameObject(const GameObject&) = delete;
    GameObject(ObjectType type) : GameObjectBase(type) {}
    ~GameObject() override;

    void ClearBitmapInfo();
    void AddBitmapInfo(BitmapInfo* bitmapInfo);

    void Update(float deltaTime) override;
    void Render(HDC hdc, COLORREF color) override;

    void SetColliderCircle(float radius);
    void SetColliderBox(float halfWidth, float halfHeight);

    Vector2f TryGetCollisionDirection(Vector2f pos, float angleOffset) override;   // _실제 특정 객체와 위치 연산 후 충돌하지 않는 벡터 반환
    bool IsCollidingWith(Vector2f pos) override;    // _실제 충돌했는지 여부만 따지기
    bool IsAtDistanceFrom(Vector2f pos, float targetDistance, float& distanceToTarget) override;
    double GetRadius() override { return m_pColliderCircle->radius; }

    void SetWidth(int w) { m_width = w; }
    void SetHeight(int h) { m_height = h; }

protected: 
    Transform transform;

    void DrawCollider(HDC hdc, COLORREF color);

    void Move(float deltaTime);
    Vector2f Rotate(Vector2f dir, float angleOffset);  // _방향 벡터를 받아 회전시키기

    void DrawBitmap(HDC hdc);
    void UpdateFrame(float deltaTime);

    // Bitmap 정보
    //BitmapInfo* m_pBitmapInfo = nullptr;
    vector<BitmapInfo*> m_pBitmapInfo = {};

    int m_width = 0;
    int m_height = 0;

    float m_frameTime = 0.0f;
    float m_frameDuration = 100.0f; // 임의 설정

    // Collider
    ColliderCircle* m_pColliderCircle = nullptr;
    ColliderBox* m_pColliderBox = nullptr;
};