# 260520_vsSans
애니메이션 기능 구현으로 인해..
기능 활용을 하기 부적절한 소스라고 판단하여 기획 변경

# 260520_CoffiaDailyShop

## 주요 구현 내용

### 스프라이트 시트 자동 분할
스프라이트 애니메이션 혹은 같은 규격과 기능을 하는 스프라이트들을 보다 편히 관리하기 위해
스프라이트 시트 내에서 스프라이트를 잘라 사용할 수 있도록 구현하였다. (SetCurFrame() 사용)

### 오브젝트 속 이미지 구조 구현
GDI 2D 렌더링 환경에서 아미지 파츠별 Transform(position, rotation, scale), Pivot, 계층 구조를 직접 구현하였다.
부모 파츠의 이동/회전/크기 값이 모두 자식 파츠에 반영되도록 로컬/월드 트랜스폼 구조를 구현하였다.

### 오브젝트의 애니메이션 구현!!! (키 프레임 + 애니메이션 클립 단위)
에프터 이펙트, 프리미어 프로, 유니티, 3ds MAx, 블렌더 등등에서 지원하는 키프레임 애니메이션을 구현하였다.
오브젝트는 각각의 aniamtor를 가지고 있으며, animator에 저장된 AnimationClip의 애니메이션을 적용할 수 있다.
AnimationClip은 하나 이상의 'KeyFrame' 로 이루어져있다. 

키 프레임은 오브젝트 내 이미지들의 위치, 회전, 크기, 스프라이트, 활성화(렌더 여부) 값을 변경할 수 있다.
키 프레임 시작시간, 지속시간, 보간 타입 등을 자유롭게 지정할 수 있다.

아래 코드의 경우 (110/60)초부터 (40/60)초 동안 ArmL 이미지를 -123도 만큼 회전한다는 의미이다. (EaseInOut 큐빅 보간 적용)
clip.MakeRotationKeyFrame(110, "ArmL", -123, 40, EaseType::EaseInOut)

### 에프터 이펙트 키프레임 변환 코드 첨부
코드만 보고 애니메이팅을 하는 것도 물론 가능하지만 매우 어렵기 때문에.. 
에프터 이펙트에서 애니메이션 작업을 한 뒤, 키 프레임을 코드 형태로 변환하는 파이썬 프로그램을 첨부하였다.

### 에프터 이펙트(上) <-> GDI 내 키프레임 애니메이션(下)
<img width="853" height="480" alt="Image" src="https://github.com/user-attachments/assets/e1b23fe6-3ede-4312-a3a7-e4acb9f1536a" />
<img width="853" height="611" alt="Image" src="https://github.com/user-attachments/assets/b8bad2b4-55ac-4269-a65f-63e3b20774a5" />

C++ assignment project - VS Sans battle implementation
