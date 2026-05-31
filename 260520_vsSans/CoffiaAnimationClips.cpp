#include "CoffiaAnimationClips.h"

AnimationClip CoffiaAnimationClips::TestClip()
{
	AnimationClip clip;
	clip.SetName("TestClip");
	clip.SetIsLooping(true);

	clip.AddKeyFrame(clip.MakePositionKeyFrame(0, "Head", Vector2f(0, 0), Vector2f(100, 100), 1, EaseType::Linear));
	clip.AddKeyFrame(clip.MakeRotationKeyFrame(0, "Head", 0, 720, 2, EaseType::Linear));
	clip.AddKeyFrame(clip.MakeScaleKeyFrame(0, "Head", Vector2f(1, 1), Vector2f(2, 2), 1, EaseType::Linear));
	clip.AddKeyFrame(clip.MakePositionKeyFrame(1, "Head", Vector2f(100, 100), Vector2f(-100, 50), 1, EaseType::EaseInOut));
	clip.AddKeyFrame(clip.MakePositionKeyFrame(2, "Head", Vector2f(-100, 50), Vector2f(100, 50), 1, EaseType::EaseOut));
	clip.AddKeyFrame(clip.MakeRotationKeyFrame(2, "Head", 0, -180, 1, EaseType::EaseOut));
	clip.AddKeyFrame(clip.MakePositionKeyFrame(1, "Head", Vector2f(100, 100), Vector2f(-100, 50), 1, EaseType::EaseIn));

	return clip;
}