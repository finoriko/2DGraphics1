#ifndef INCLUDED_GAMELIB_HASHMAP_H
#define INCLUDED_GAMELIB_HASHMAP_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib
{
	///�Ѱ�뷮�����ؽ����̺�
	/*!
	�ؽ��Լ��� ��ü ��Ʈ�� ���� ���̺� ������� �׿��� ���ϴ� �ܼ��� ����̴�.
	��ü ��Ʈ�̱� ������ Ŭ������ ����� ũ�� ����� Ŀ�� ���ɼ��� ����.
	���� ���� Ű�� ���� ��Ҵ� �߰����� �ʴ´�.
	*/
	template< class Key, class Value, class H = Hash< Key > > class HashMap 
	{
	public:
		// �⺻��.set capacity�� �ҷ��� �۵��Ѵ�
		HashMap();
		/// �ִ� �뷮 ����.���̺� ũ��� �⺻������ capacity �̻��� �ּ� �Ҽ�.
		explicit HashMap(int capacity, int tableSize = 0);
		~HashMap();
		/// ��Ȯ��.���빰�� �ִ� ���¿��� �θ��� assert. clear()�ض�.
		void setCapacity(int capacity, int tableSize = 0);
		/// �ִ� �뷮 ȹ��
		int capacity() const;
		/// ���� ��� �� ȹ��
		int size() const;
		/// ���ع�
		void clear();
		/// �߰�. �߰��� ��Ҹ� ��ȯ�Ѵ�.���� ���� �̹� ������ �����ϰ� ��ȿ ���ͷ����͸� ��ȯ�Ѵ�.
		int add(const Key&, const Value&);
		/// �߰�. �߰��� ��Ҹ� ��ȯ�Ѵ�.���� ���� �̹� ������ �����ϰ� ��ȿ ���ͷ����͸� ��ȯ�Ѵ�. ���� getValue�ϰ� ���߿� ��ִ´�.
		int add(const Key&);
		/// �˻�. �ǵ��ƿ��� ���� ���ͷ�����
		int find(const Key&) const;
		///���׷����͸� �����Ͽ� ����. ���� �� true�� ��ȯ�Ѵ�.
		bool remove(int position);
		///Ű ���(ȯ���� �����̹Ƿ� const�� �ش�)
		const Key* key(int position) const;
		/// �� ���(const)
		const Value* value(int position) const;
		/// �� ���(��const)
		Value* value(int position);
		/// �˻��Ͽ� �� ���(const)
		const Value* operator[](const Key&) const;
		/// �˻��Ͽ� �� ���(��const)
		Value* operator[](const Key&);
		//�Ʒ� �ܼ��� ������� ������ ���� �������̽�. �� �� ����
		int next(int position) const;
		int first() const;
		///position�� �������� ����.
		bool isEnd(int position) const;
		///�ܼ��� �迭�� ����.�̸� size()�� ũ��� new�� ������ �Ѱ��ش�.
		void copyTo(Value*) const;
		///Array�� ����.�� ���� �ǳ׶�.
		void copyTo(Array < Value >*) const;
	private:
		void operator=(const HashMap&); //���Ա���
		HashMap(const HashMap&); //��������� ����

		int* mNexts;
		Key* mKeys;
		Value* mValues;
		Stack< int > mEmptyStack;
		int mCapacity;
		int mSize;
		int mTableSize;
	};
}//namespace GameLib
#include "GameLib/Base/Impl/HashMapImpl.h"// ���� �� ��
#endif