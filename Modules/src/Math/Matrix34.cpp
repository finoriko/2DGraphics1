#include "GameLib/GameLib.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix33.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Vector3.h"

namespace GameLib {
	namespace Math {

		Matrix34::Matrix34() {
		}

		void Matrix34::setIdentity() {
			m00 = m11 = m22 = 1.f;
			m01 = m02 = m03 = 0.f;
			m10 = m12 = m13 = 0.f;
			m20 = m21 = m23 = 0.f;
		}

		void Matrix34::setTranslation(float x, float y, float z) {
			m00 = m11 = m22 = 1.f;
			m03 = x;
			m13 = y;
			m23 = z;
			m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
		}

		void Matrix34::setTranslation(const Vector3& a) {
			m00 = m11 = m22 = 1.f;
			m03 = a.x;
			m13 = a.y;
			m23 = a.z;
			m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
		}

		void Matrix34::setRotationX(float r) {
			float c = cos(r);
			float s = sin(r);
			m11 = m22 = c;
			m12 = -s;
			m21 = s;
			m00 = 1.0;
			m01 = m02 = m10 = m20 = 0.0;
			m03 = m13 = m23 = 0.f;
		}

		void Matrix34::setRotationY(float r) {
			float c = cos(r);
			float s = sin(r);
			m00 = m22 = c;
			m20 = -s;
			m02 = s;
			m11 = 1.0;
			m01 = m10 = m12 = m21 = 0.0;
			m03 = m13 = m23 = 0.f;
		}

		void Matrix34::setRotationZ(float r) {
			float c = cos(r);
			float s = sin(r);
			m00 = m11 = c;
			m01 = -s;
			m10 = s;
			m22 = 1.0;
			m02 = m12 = m20 = m21 = 0.0;
			m03 = m13 = m23 = 0.f;
		}

		void Matrix34::setScaling(float xyz) {
			m00 = m11 = m22 = xyz;
			m01 = m02 = m03 = 0.f;
			m10 = m12 = m13 = 0.f;
			m20 = m21 = m23 = 0.f;
		}

		void Matrix34::setScaling(float x, float y, float z) {
			m00 = x;
			m11 = y;
			m22 = z;
			m01 = m02 = m03 = 0.f;
			m10 = m12 = m13 = 0.f;
			m20 = m21 = m23 = 0.f;
		}

		void Matrix34::setScaling(const Vector3& a) {
			m00 = a.x;
			m11 = a.y;
			m22 = a.z;
			m01 = m02 = m03 = 0.f;
			m10 = m12 = m13 = 0.f;
			m20 = m21 = m23 = 0.f;
		}

		/*
		3축 회전 뷰 행렬을 만드는 방법.

		회전부분은 3x3행렬이다.따라서 수가 9개다.
		식이 9개 있으면 이것을 요구받는 것이다.
		
		자, 그 식을 어디서부터 만들 것인가다.
		어떤 벡터s가 s'로 넘어간다고 알고 있다고 하자.
		구하고 싶은 회전행렬을 A라고 하면,
		
		s'=As
		
		이다. 이것을 분해하면,
		
		s'x = a00*sx + a01*sy + a02*sz
		s'y = a10*sx+a11*sy+a12*sz
		s'z = a20*sx+a21*sy+a22*sz

		하고 식이 3개 생긴다.s이외에 t와 r에 대해서도,
		t'=At
		r'=Ar
		라고 알고 있으면, 더욱 식이 6개 늘어나므로,
		아홉 개의 식이 서다.
		
		여기서 계산을 단순화하기 위해
		s=(1,0,0)
		t=(0,1,0)
		r=(0,0,1)
		라고 해보자.위의 식은 바보처럼 간단해지고,
		
		s'x = a00
		s'y = a10
		s'z = a20
		
		가 된다. t,r에 대해서도 같은 것이 되고, 결국 A는
			| s'x t'x r'x |
		A = | s'y t'y r'y |
			| s'z t'z r'z |

		라고 쓸 수 있게 되는 것이다.그리고 또 이제
		s' , t' , r' 를 알면 된다.
		
		지금 z 플러스 방향은 주시점에서 시점으로 향하는 벡터로,
		r' = (p-t) ) / | p-t |
		이된다.길이를1로했다는것을주의하자.
		
		그리고 하나 더, 위가 어느 쪽인가가 주어지는 것으로 알고 있다.
		이대로 Y축으로 삼을 수는 없지만,
		u와 r'의 외피를 잡으면 X축이 나온다.이게 s'다.
		그리고 s'와 r'의 외적에서 다시 Y축을 낸다.이것이 t'가 된다.
		아래의 코드에서, x,y,z 는 각각 s',t',r'이다.
		
		회전 행렬만 생겨 버리면 이동분만 계산하면 된다.
		V=RT로 R은 지금 만들었다.T는 p가 원점에 오는 듯한 이동행렬이다.
		아래 코드에서는 행렬의 곱셈을 통째로 하는 계산량을 피하기 위해
		RT의 곱셈이 필요한 부분만을 그 자리에서 행하고, m03,m13,m23을 계산하고 있다.
		*/
		void Matrix34::setViewTransform(
			const Vector3& p, //시점
			const Vector3& t, //주시점
			const Vector3& u) { //위를 나타내는 벡터
			Vector3 x, y, z;
			z.setSub(p, t); //주시점에서 카메라 위치로의 벡터를 Z축으로
			z.normalize();
			x.setCross(u, z); //위 벡터와 Z축의 외적을 X축으로. 위 벡터가 위쪽이면 왼쪽 방향이 X축 플러스.
			x.normalize();
			y.setCross(z, x); //Z,X의 외적이 Y

			//직교 기저를 3줄 방향으로 넣으면 완성.
			m00 = x.x; m01 = x.y; m02 = x.z;
			m10 = y.x; m11 = y.y; m12 = y.z;
			m20 = z.x; m21 = z.y; m22 = z.z;
			//이동분은 지금 만든 회전행렬에 카메라 좌표를 곱해 마이너스로 만들면 된다.
			m03 = -(m00 * p.x + m01 * p.y + m02 * p.z);
			m13 = -(m10 * p.x + m11 * p.y + m12 * p.z);
			m23 = -(m20 * p.x + m21 * p.y + m22 * p.z);
		}

		void Matrix34::rotateX(float r) {
			float c = cos(r);
			float s = sin(r);
			float t;
			t = c * m01 + s * m02;
			m02 = -s * m01 + c * m02;
			m01 = t;
			t = c * m11 + s * m12;
			m12 = -s * m11 + c * m12;
			m11 = t;
			t = c * m21 + s * m22;
			m22 = -s * m21 + c * m22;
			m21 = t;
		}

		void Matrix34::rotateY(float r) {
			float c = cos(r);
			float s = sin(r);
			float t;
			t = c * m00 - s * m02;
			m02 = s * m00 + c * m02;
			m00 = t;
			t = c * m10 - s * m12;
			m12 = s * m10 + c * m12;
			m10 = t;
			t = c * m20 - s * m22;
			m22 = s * m20 + c * m22;
			m20 = t;
		}

		void Matrix34::rotateZ(float r) {
			float c = cos(r);
			float s = sin(r);
			float t;
			t = c * m00 + s * m01;
			m01 = -s * m00 + c * m01;
			m00 = t;
			t = c * m10 + s * m11;
			m11 = -s * m10 + c * m11;
			m10 = t;
			t = c * m20 + s * m21;
			m21 = -s * m20 + c * m21;
			m20 = t;
		}

		void Matrix34::scale(float xyz) {
			m00 *= xyz;
			m01 *= xyz;
			m02 *= xyz;
			m10 *= xyz;
			m11 *= xyz;
			m12 *= xyz;
			m20 *= xyz;
			m21 *= xyz;
			m22 *= xyz;
		}

		void Matrix34::scale(float x, float y, float z) {
			m00 *= x;
			m01 *= y;
			m02 *= z;
			m10 *= x;
			m11 *= y;
			m12 *= z;
			m20 *= x;
			m21 *= y;
			m22 *= z;
		}

		void Matrix34::scale(const Vector3& a) {
			m00 *= a.x;
			m01 *= a.y;
			m02 *= a.z;
			m10 *= a.x;
			m11 *= a.y;
			m12 *= a.z;
			m20 *= a.x;
			m21 *= a.y;
			m22 *= a.z;
		}

		void Matrix34::translate(float x, float y, float z) {
			m03 += m00 * x + m01 * y + m02 * z;
			m13 += m10 * x + m11 * y + m12 * z;
			m23 += m20 * x + m21 * y + m22 * z;
		}

		void Matrix34::translate(const Vector3& a) {
			m03 += m00 * a.x + m01 * a.y + m02 * a.z;
			m13 += m10 * a.x + m11 * a.y + m12 * a.z;
			m23 += m20 * a.x + m21 * a.y + m22 * a.z;
		}


		void Matrix34::mul(Vector3* out, const Vector3& in) const {
			float tx = in.x;
			float ty = in.y;
			out->x = m00 * tx + m01 * ty + m02 * in.z + m03;
			out->y = m10 * tx + m11 * ty + m12 * in.z + m13;
			out->z = m20 * tx + m21 * ty + m22 * in.z + m23;
		}

		void Matrix34::mul33(Vector3* out, const Vector3& in) const {
			float tx = in.x;
			float ty = in.y;
			out->x = m00 * tx + m01 * ty + m02 * in.z;
			out->y = m10 * tx + m11 * ty + m12 * in.z;
			out->z = m20 * tx + m21 * ty + m22 * in.z;
		}

		void Matrix34::operator*=(const Matrix34& a) {
			float tx = m00;
			float ty = m01;
			float tz = m02;
			m00 = tx * a.m00 + ty * a.m10 + tz * a.m20;
			m01 = tx * a.m01 + ty * a.m11 + tz * a.m21;
			m02 = tx * a.m02 + ty * a.m12 + tz * a.m22;
			m03 += tx * a.m03 + ty * a.m13 + tz * a.m23;
			tx = m10;
			ty = m11;
			tz = m12;
			m10 = tx * a.m00 + ty * a.m10 + tz * a.m20;
			m11 = tx * a.m01 + ty * a.m11 + tz * a.m21;
			m12 = tx * a.m02 + ty * a.m12 + tz * a.m22;
			m13 += tx * a.m03 + ty * a.m13 + tz * a.m23;
			tx = m20;
			ty = m21;
			tz = m22;
			m20 = tx * a.m00 + ty * a.m10 + tz * a.m20;
			m21 = tx * a.m01 + ty * a.m11 + tz * a.m21;
			m22 = tx * a.m02 + ty * a.m12 + tz * a.m22;
			m23 += tx * a.m03 + ty * a.m13 + tz * a.m23;
		}

		void Matrix34::operator*=(const Matrix33& a) {
			float tx = m00;
			float ty = m01;
			m00 = tx * a.m00 + ty * a.m10 + m02 * a.m20;
			m01 = tx * a.m01 + ty * a.m11 + m02 * a.m21;
			m02 = tx * a.m02 + ty * a.m12 + m02 * a.m22;
			tx = m10;
			ty = m11;
			m10 = tx * a.m00 + ty * a.m10 + m12 * a.m20;
			m11 = tx * a.m01 + ty * a.m11 + m12 * a.m21;
			m12 = tx * a.m02 + ty * a.m12 + m12 * a.m22;
			tx = m20;
			ty = m21;
			m20 = tx * a.m00 + ty * a.m10 + m22 * a.m20;
			m21 = tx * a.m01 + ty * a.m11 + m22 * a.m21;
			m22 = tx * a.m02 + ty * a.m12 + m22 * a.m22;
		}

		void Matrix34::setMul(const Matrix34& a, const Matrix34& b) {
			//a==*this, b==*this쌍방을 생각하지 않으면 안 된다
			float t00, t01, t02, t03;
			t00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
			t01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
			t02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
			t03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03;

			float t10, t11, t12, t13;
			t10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
			t11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
			t12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
			t13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13;

			float t20, t21, t22;
			t20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
			t21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
			t22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
			m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23;

			m00 = t00; m01 = t01; m02 = t02; m03 = t03;
			m10 = t10; m11 = t11; m12 = t12; m13 = t13;
			m20 = t20; m21 = t21; m22 = t22;
		}

		void Matrix34::setMul(const Matrix34& a, const Matrix33& b) {
			float tx = a.m00;
			float ty = a.m01;
			m00 = tx * b.m00 + ty * b.m10 + a.m02 * b.m20;
			m01 = tx * b.m01 + ty * b.m11 + a.m02 * b.m21;
			m02 = tx * b.m02 + ty * b.m12 + a.m02 * b.m22;
			m03 = a.m03;
			tx = a.m10;
			ty = a.m11;
			m10 = tx * b.m00 + ty * b.m10 + a.m12 * b.m20;
			m11 = tx * b.m01 + ty * b.m11 + a.m12 * b.m21;
			m12 = tx * b.m02 + ty * b.m12 + a.m12 * b.m22;
			m13 = a.m13;
			tx = a.m20;
			ty = a.m21;
			m20 = tx * b.m00 + ty * b.m10 + a.m22 * b.m20;
			m21 = tx * b.m01 + ty * b.m11 + a.m22 * b.m21;
			m22 = tx * b.m02 + ty * b.m12 + a.m22 * b.m22;
			m23 = a.m23;
		}

		float& Matrix34::operator()(int r, int c) {
			ASSERT(r >= 0 && r < 3);
			ASSERT(c >= 0 && c < 4);
			return (&m00)[4 * r + c];
		}

		float Matrix34::operator()(int r, int c) const {
			ASSERT(r >= 0 && r < 3);
			ASSERT(c >= 0 && c < 4);
			return (&m00)[4 * r + c];
		}

		void Matrix34::transpose33() {
			float t;
			t = m01; m01 = m10; m10 = t;
			t = m02; m02 = m20; m20 = t;
			t = m12; m12 = m21; m21 = t;
			m03 = m13 = m23 = 0.f;
		}

		void Matrix34::setTransposed33(const Matrix34& a) {
			m00 = a.m00;
			m01 = a.m10;
			m02 = a.m20;
			m10 = a.m01;
			m11 = a.m11;
			m12 = a.m12;
			m20 = a.m21;
			m21 = a.m21;
			m22 = a.m22;
			m03 = m13 = m23 = 0.f;
		}

		//크라멜법에서의 역행렬
		/*
		33가지 방식에 23에 쓴 생각을 적용하면 쉽게 풀린다.
		Matrix33.cpp과 Matrix23.cpp을 보자.
		*/
		void Matrix34::setInverse(const Matrix34& a) {
			// 먼저 3x3부분의 역행렬 만들기
			//- 3x3부터 카피앤 패이스트(코삐페) 일어를 의역
			// 이항곱을 한꺼번에 만들다(첨자가 작은 순서로 나열해 보았으므로 오류가 적을 것으로 생각됨)
			float m0012 = a.m00 * a.m12;
			float m0021 = a.m00 * a.m21;
			float m0022 = a.m00 * a.m22;

			float m0110 = a.m01 * a.m10;
			float m0112 = a.m01 * a.m12;
			float m0120 = a.m01 * a.m20;
			float m0122 = a.m01 * a.m22;

			float m0210 = a.m02 * a.m10;
			float m0211 = a.m02 * a.m11;
			float m0220 = a.m02 * a.m20;
			float m0221 = a.m02 * a.m21;

			float m1021 = a.m10 * a.m21;
			float m1022 = a.m10 * a.m22;

			float m1120 = a.m11 * a.m20;
			float m1122 = a.m11 * a.m22;

			float m1220 = a.m12 * a.m20;
			float m1221 = a.m12 * a.m21; //18個

			//2항곱의 차를 세 가지.행렬식용이지만 나중에 다시 쓰게 한다.
			float m1122_m1221 = m1122 - m1221;
			float m1220_m1022 = m1220 - m1022; //マイナス
			float m1021_m1120 = m1021 - m1120;

			//행렬식
			//00*( 11*22 - 12*21 ) - 01*( 10*22 - 12*20 ) + 02*( 10*21 - 11*20 )
			float delta = a.m00 * (m1122_m1221)+a.m01 * (m1220_m1022)+a.m02 * (m1021_m1120);
			float rcpDelta = 1.f / delta; //0할 가능성은 있지만 여기에서는 터지지 않는다.

			//요소를 메운다. 전치에 주의! 왼쪽 변은 첨자가 뒤집혀있다!
			m00 = (m1122_m1221)*rcpDelta;
			m10 = (m1220_m1022)*rcpDelta; //마이너스
			m20 = (m1021_m1120)*rcpDelta;
			m01 = (m0221 - m0122) * rcpDelta; //마이너스
			m11 = (m0022 - m0220) * rcpDelta;
			m21 = (m0120 - m0021) * rcpDelta; //마이너스
			m02 = (m0112 - m0211) * rcpDelta;
			m12 = (m0210 - m0012) * rcpDelta; //마이너스
			m22 = (m0011 - m0110) * rcpDelta;
			//---여기까지 카피패이스트
			float t03 = a.m03; //백업
			float t13 = a.m13;
			m03 = -(m00 * t03 + m01 * t13 + m02 * a.m23);
			m13 = -(m10 * t03 + m11 * t13 + m12 * a.m23);
			m23 = -(m20 * t03 + m21 * t13 + m22 * a.m23);
		}

		void Matrix34::invert() {
			setInverse(*this); //setInverse()하지만 스스로도 괜찮은 구조이므로 그대로 건넨다.
		}

		bool Matrix34::operator==(const Matrix34& a) const {
			for (int i = 0; i < 3; ++i) {
				for (int j = 0; j < 4; ++j) {
					if ((*this)(i, j) != a(i, j)) {
						return false;
					}
				}
			}
			return true;
		}

		bool Matrix34::operator!=(const Matrix34& a) const {
			return !(operator==(a));
		}




	} //namespace Math
} //namespace GameLib
