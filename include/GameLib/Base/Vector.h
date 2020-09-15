#ifndef INCLUDED_GAMELIB_VECTOR_H
#define INCLUDED_GAMELIB_VECTOR_H

namespace GameLib {

	///������� std::vector
/*!
std ::vector�� ������� �ʰ� ������ ����ϴ� ������ �����Դϱ�?
��� ���� ����.�󵵰� ���� �Լ��� ������ �Լ��� �����ϰ� �����Ƿ�
����ϱ� ���� �н��� ������ ���̴�.
�������� STL���� size_t�� ���ƿ��� �Ϳ� �����ִ� ������Դ� ��õ�� ���ϴ�.int�� ���ݾ�!

�⺻������ �� Ŭ������ ���ϰ� ���� ���� ������, ������ ���� �迭�� �Լ���
�θ��� �翬�� �ʾ���.Array�� �ٸ��� ���縦 �� ���� ������, �ƹ� �������� ����� �ĸ����� ������ ��ȭ��
�ʴ��� ���̴�.
*/
	template< class T > class Vector {
	public:
		///capacity�� �����ϸ� �� ����ŭ �þ�� �̻����� �ʴ´�.
		explicit Vector(int size = 0, int capacity = 0);
		/// ��°�� �����Ѵ�.���� ���� �����϶�
		Vector(const Vector&); //���� �ܽ�Ʈ���͸� �������� ����
		~Vector();
		// ����. ��°�� �����Ѵ�.���� ���� �����϶�
		void operator=(const Vector&); //��������򭪷�ʪ�
		/// �߰�
		void push(const T&);
		/// ��Ҹ� ����� �߰�.�����͸� ������ �״� ���⿡ ���� ��.
		T* push();
		/// ���� ��� ����
		void pop();
		/// ��ü ����
		void clear();
		/// ������ ȹ��
		int size() const;
		/// ������ ����. �۰� �ص� �̻簡 �Ͼ�� �ʴ´�.
		void setSize(int);
		// ÷��(��const)
		T& operator[](int);
		//÷��(const)
		const T& operator[](int) const;
		///���빰�� �ٸ� ���ͷ� �ű��.���� �������.��뿡�� ��� �ִ� ���� �������.
		void moveTo(Vector*);
	private:
		T* mElements;
		int mSize;
		int mCapacity;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/VectorImpl.h" //������ ����

#endif

