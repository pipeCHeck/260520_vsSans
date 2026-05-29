Transform

struct Vector2f {
	float x,
	float y
}
public:
	# 그냥 값반환
	GetPosition
	GetRotation
	GetScale
	
	# 이동 회전 크기 수정
	SetPosition
	ModifyPosition
	SetRotation	
	ModifyRotation
	SetScale	
	ModifyScale

private:
	Vector2f pos;
	float rot;
	Vector2f sca;




오브젝트
Transform transform	// 위치회전크기 값
Transform aniTransform	// 애니메이션에 의해 수정될 위치회전크기값(실제로는 트랜스폼+애니트랜스폼 값으로 렌더)
					// 애니메이션에 의해 값을 직접적으로 바꾸면 애니메이션 적용마다 위치가 꼬임
vector<비트맵>	// 오브젝트가 가지고 있는 비트맵
Animation 		// 지금 적용 중인 애니메이션


# 비트맵 관여
원하는 비트맵을 검색(string 키값) => 해당 비트맵 반환

# 이미지 간의 z포지션 변경
특정 비트맵을 맨 앞으로(string 키값)
특정 비트맵을 맨 뒤로(string 키값)
특정 비트맵을 다른 비트맵 앞으로(string 키값, string 목표키값)
특정 비트맵을 다른 비트맵 뒤로(string 키값, string 목표키값)
특정 비트맵을 특정 인덱스로(string 키값, string 인덱스)






비트맵
Transform transform		// 위치회전크기 값
Transform aniTransform	// 애니메이션에 의해 수정될 위치회전크기값
Transform parTransform	// 부모 비트맵에 의해 수정될 위치회전크기값
string nickname	// 비트맵 검색을 위해 사용되는 키값 
Vector2f pivot	// 회전을 위한 피벗
int curIndex	// 스프라이트 시트의 경우, 현재 출력할 
bool active		// 이미지를 띄울지말지 여부(false이면 그냥 렌더 스킵)
부모 비트맵	// 부모의 위치회전크기 값에 영향을 받음

void SetNickName(string 키값) { 키값 설정 }
void 부모 비트맵(string 키값 = null) { 널이면 부모없음, 키값들어가면 부모로 지정 }
void SetPivot(flaot x, float y)	{피벗값 설정}
		(Vector2f pivot)
void SetActive(bool 여부) {active 값 설정}

Transform 실제 트랜스폼값 반환() { transform + aniTransform + parTransform }





class Animation 

// 각각 위치변경, 회전값변경, 크기변경, 
enum KeyFrameType {
	pos, rot, sca, sprite, active,
}

enum EaseType {
	Linear, EaseIn, EaseOut, EaseInOut,
}

struct KeyFrame {
	float 시작 시간
	string 비트맵 키값
	KeyFrameType 어떤 애니메이션을 줄지 타입
	float value1, value2 // 위치크기면 xy, 회전이면 1만 사용
	float duration	// sprite, active 면 의미없음
	EaseType 보간여부
}

vector<KeyFrame> Get


private:
	vector<KeyFrame> aniKey;
	bool isLoop;
