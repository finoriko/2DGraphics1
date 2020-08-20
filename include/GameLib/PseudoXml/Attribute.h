#ifndef INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H
#define INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H

namespace GameLib {
	namespace PseudoXml {

		class Attribute {
		public:
			///NULL ���� �ƴ϶� ����� ��������� �ְ� ���� ��. -1�� ������ �˾Ƽ� �� �Ŵϱ� ���ʸ� �� ���� �ִ�
			void set(const char* name, int nameSize, const char* value, int valueSize);
			void set(const char* name, const char* value);
			void set(const char* name, int value); //int��
			void set(const char* name, float value); //float��
			void set(const char* name, double value); //double��
			void set(const char* name, const int* values, int number); //int�迭
			void set(const char* name, const float* values, int number); //float�迭
			void set(const char* name, const double* values, int number); //double�迭
			const char* value() const;
			const char* name() const;
			///�����ϸ� 0�� ���������� ���� 0�� ����ִ� �Ͱ��� ������ ���� �ʴ´�.
			int getIntValue() const;
			///�����ϸ� 0.f�� ��ȯ������ ���� 0.f�� ��� �ִ� �Ͱ��� ������ �� �ȴ�.
			float getFloatValue() const;
			///�����ϸ� 0.0�� ���������� ���� 0.0�� �� �Ͱ��� ������ ���� �ʴ´�.
			double getDoubleValue() const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getIntValues(int* out, int number) const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getFloatValues(float* out, int number) const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getDoubleValues(double* out, int number) const;

			//���� ����ڴ� �ǽ����� �ʾƵ� �ȴ�
			Attribute();
			~Attribute();
			Attribute(const Attribute&);
			operator void* () const;
			Attribute& operator=(const Attribute&);
			void release();
			bool operator==(const Attribute&) const;
			bool operator!=(const Attribute&) const;

			class Impl;
		private:
			friend class Element;
			friend class Tag;

			explicit Attribute(Impl*);
			Impl* mImpl;
		};

		//const��
		class ConstAttribute {
		public:
			const char* value() const;
			const char* name() const;
			///�����ϸ� 0�� ���������� ���� 0�� ����ִ� �Ͱ��� ������ ���� �ʴ´�.
			int getIntValue() const;
			///�����ϸ� 0.f�� ��ȯ������ ���� 0.f�� ��� �ִ� �Ͱ��� ������ �� �ȴ�.
			float getFloatValue() const;
			///�����ϸ� 0.0�� ��ȯ������ ���� 0.0�� ��� �ִ� �Ͱ��� ������ �� �ȴ�.
			double getDoubleValue() const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getIntValues(int* out, int number) const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getFloatValues(float* out, int number) const;
			///��ȯ ���� ������ ���� �� �ִ� ��� ��
			int getDoubleValues(double* out, int number) const;

			//���� ����ڴ� �ǽ����� �ʾƵ� �ȴ�
			ConstAttribute();
			ConstAttribute(const Attribute&);
			~ConstAttribute();
			operator void* () const;
			ConstAttribute& operator=(const Attribute&);
			void release();
			bool operator==(const Attribute&) const;
			bool operator!=(const Attribute&) const;
		private:
			Attribute mBase;
		};


	} //namespace PseudoXml
} //namespace GameLib

#endif
