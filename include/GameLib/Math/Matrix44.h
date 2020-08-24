#ifndef INCLUDED_GAMELIB_MATH_MATRIX44_H
#define INCLUDED_GAMELIB_MATH_MATRIX44_H

namespace GameLib {
	namespace Math {

		class Vector3;
		class Vector4;
		class Matrix34;

		/*!
		44는 변환의 최종단에 밖에 사용하지 않는 것을 상정하고 있다.그러니까 기능은 빠듯할 수 밖에 없다.
		34로 할 수 있는 일은 34로 하는 것.사람들은 행렬의 곱셈이 무거운 조작임을 곧잘 잊어버린다.
		*/
		class Matrix44 {
		public:
			Matrix44();
			void setIdentity();
			//투시 변환 행렬의 생성
			void setPerspectiveTransform(
				float fieldOfViewYInRadian,
				float windowWidth,
				float windowHeight,
				float nearClip,
				float farClip,
				bool zBufferIsLinear = false);
			//정사영 행렬의 생성
			void setOrthogonalTransform(
				float left,
				float right,
				float bottom,
				float top,
				float front,
				float back);
			//view 행렬을 만들어 그 자리에서 곱셈하다
			void multiplyViewTransform(
				const Vector3& eyePosition,
				const Vector3& eyeTarget,
				const Vector3& upVector);

			void mul(Vector4* out, const Vector3& in) const;
			void operator*=(const Matrix34&);
			void setMul(const Matrix44&, const Matrix34&);
			///완전히 일치하는지 체크
			bool operator==(const Matrix44&) const;
			///조금이라도 다른지 체크
			bool operator!=(const Matrix44&) const;

			float& operator()(int row, int col);
			float operator()(int row, int col) const;

			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

	} //namespace Math
} //namespace GameLib

#endif
