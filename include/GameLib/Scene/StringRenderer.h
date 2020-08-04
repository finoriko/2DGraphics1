#ifndef INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H
#define INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H

namespace GameLib {

	namespace Graphics {
		class Texture;
	}
	namespace Math {
		class Vector2;
	}
	using namespace Math;

	namespace Scene {

		class Font;

		///���ڿ� ��ȭ Ŭ�����켱�� ���� ����.�ڵ����� �����մϴ�.
		class StringRenderer {
		public:
			///�ִ� �հ� ���ڼ�, �ִ� �䱸 ����� ����.������ assert
			static StringRenderer create(int charCapacity, int requestCapacity);
			///�ȼ� ��ǥ�� ����.z��ŭ float�̰�[0,1]�� �Ϳ� ����
			void add(
				int x,
				int y,
				const char* string,
				unsigned color = 0xffffffff,
				bool wrapAround = false,
				float z = 0.f);
			///�ȼ������� float���� ��ġ�� ����(x=[0,w], y = [0,h], z = [0,1] )
			void add(
				const Vector2& position,
				const char* string,
				unsigned color = 0xffffffff,
				bool wrapAroung = false,
				float z = 0.f);
			///�з��� �䱸�� �Ѳ����� �׸���
			void draw();
			///��Ʈ�� ��Ʈ.�ּ� �� ���� �̰� �ؾ� �۾��� �� �� �־�.
			void setFont(Font&);

			//���� �ǽ����� �ʾƵ� �ȴ�
			StringRenderer();
			~StringRenderer();
			StringRenderer(const StringRenderer&);
			operator void* () const;
			StringRenderer& operator=(const StringRenderer&);
			void release();
			bool operator==(const StringRenderer&) const;
			bool operator!=(const StringRenderer&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Scene
} //namespace GameLib

#endif
