#pragma once
#include "INC_Windows.h"
#include "Transform.h"
#include <string>

namespace renderHelp
{
    struct WICInitializer;
    class BitmapInfo
    {
        using Vector2f = learning::Vector2f;
        using Transform = transform::Transform;
        using String = std::string;
    public:
        HBITMAP GetBitmapHandle() const { return m_hBitmap; }
        
        // 전체 이미지의 크기 반환
        int GetSheetWidth() const { return m_sheetWidth; }
        int GetSheetHeight() const { return m_sheetHeight; }

        // 스프라이트 한 칸의 크기 반환
		Vector2f GetFrameSize() const { return Vector2f(GetFrameWidth(), GetFrameHeight()); }
        int GetFrameWidth() const { return m_sheetWidth / frameColumns; }
        int GetFrameHeight() const { return m_sheetHeight / frameRows; }

        // 스프라이트 갯수 반환
        int GetFrameColumns() const { return frameColumns; }
        int GetFrameRows() const { return frameRows; }
        int GetFrameCount() const { return frameCount; }

        // 현재 사용할 스프라이트 인덱스 반환
        int GetCurFrame() { return curFrame; }
        void SetCurFrame(int index);

		bool GetActive() const { return active; }
        void SetActive(bool active) { this->active = active; }

		void SetName(String name) { this->name = name; }
        void SetParentImage(BitmapInfo* parent) { this->parentImage = parent; }

		Transform& GetTransform() { return transform; }
		Transform& GetAniTransform() { return aniTransform; }
        Vector2f& GetPivot() { return pivot; }

        Transform GetWorldTransform() const;
        Vector2f RotateVector(Vector2f value, float degree) const;

		String GetName() const { return name; }
		BitmapInfo* GetParentImage() const { return parentImage; }

    private:
        friend struct WICInitializer;

        BitmapInfo() = default;

        BitmapInfo(HBITMAP hBitmap)
        {
            m_hBitmap = hBitmap;
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            m_sheetWidth = bitmap.bmWidth;
            m_sheetHeight = bitmap.bmHeight;
        }

        BitmapInfo(HBITMAP hBitmap, int frameCountX, int frameCountY, int frameCount = 1, int offsetX = 0, int offsetY = 0)
        {
            m_hBitmap = hBitmap;
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            m_sheetWidth = bitmap.bmWidth;
            m_sheetHeight = bitmap.bmHeight;

            this->frameColumns = frameCountX;
            this->frameRows = frameCountY;
            this->frameCount = frameCount;
            this->pivot = Vector2f(offsetX, offsetY);
        }

        ~BitmapInfo()
        {
            if (m_hBitmap)
            {
                DeleteObject(m_hBitmap);
                m_hBitmap = nullptr;
            }
        }

    private:

        HBITMAP m_hBitmap = nullptr;        // 실제 이미지 비트맵
        BitmapInfo* parentImage = nullptr;  // 부모 비트맵 

		Transform transform;        // _위치, 회전, 크기 정보
		Transform aniTransform;     // _애니메이션 위치, 회전, 크기 정보
		Transform parTransform;     // _부모 위치, 회전, 크기 정보
        Vector2f pivot = { 0.0f, 0.0f };   // 이미지의 피벗 위치(0,0 이 중앙)

		String name;        // 검색 위해 사용되는 키값
        int curFrame = 0;   // 현재 이미지 프레임 값
		bool active = true; // 이미지 활성화 여부 (false면 렌더 스킵)

        // 스프라이트 시트의 크기
        int m_sheetWidth = 0;
        int m_sheetHeight = 0;

        // 시트 속 스프라이트의 갯수
        int frameColumns = 1;
        int frameRows = 1;
        int frameCount = 1;

        BitmapInfo(const BitmapInfo&) = delete;
        BitmapInfo& operator=(const BitmapInfo&) = delete;
    };

    BitmapInfo* CreateBitmapInfo(LPCWSTR filename);
    BitmapInfo* CreateBitmapInfo(LPCWSTR filename, int frameCountX, int frameCountY, int frameCount, int offsetX = 0, int offsetY = 0);
}
