#pragma once
#include "Utillity.h"

namespace transform {
	struct Transform
	{
		using Vector2f = learning::Vector2f;

		Vector2f position;
		float rotation;
		Vector2f scale;

		// °ª ¼öÁ¤
		void SetPosition(Vector2f value) {
			position = value;
		}
		void ModifyPosition(Vector2f value) {
			position += value;
		}
		void SetRotation(float value) {
			rotation = value;
		}
		void ModifyRotation(float value) {
			rotation += value;
		}
		void SetScale(Vector2f value) {
			scale = value;
		}
		void ModifyScale(Vector2f value) {
			scale += value;
		}
	};
}
