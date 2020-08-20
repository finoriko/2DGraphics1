#ifndef INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H
#define INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H

namespace GameLib {
	namespace PseudoXml {

		class Attribute {
		public:
			///NULL 끝이 아니라서 사이즈를 명시적으로 주고 싶을 때. -1을 넣으면 알아서 잴 거니까 한쪽만 줄 수도 있는
			void set(const char* name, int nameSize, const char* value, int valueSize);
			void set(const char* name, const char* value);
			void set(const char* name, int value); //int값
			void set(const char* name, float value); //float값
			void set(const char* name, double value); //double값
			void set(const char* name, const int* values, int number); //int배열
			void set(const char* name, const float* values, int number); //float배열
			void set(const char* name, const double* values, int number); //double배열
			const char* value() const;
			const char* name() const;
			///실패하면 0을 돌려주지만 원래 0이 들어있던 것과는 구별이 되지 않는다.
			int getIntValue() const;
			///실패하면 0.f를 반환하지만 원래 0.f가 들어 있던 것과는 구별이 안 된다.
			float getFloatValue() const;
			///실패하면 0.0을 돌려주지만 원래 0.0이 들어간 것과는 구별이 되지 않는다.
			double getDoubleValue() const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getIntValues(int* out, int number) const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getFloatValues(float* out, int number) const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getDoubleValues(double* out, int number) const;

			//이하 사용자는 의식하지 않아도 된다
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

		//const판
		class ConstAttribute {
		public:
			const char* value() const;
			const char* name() const;
			///실패하면 0을 돌려주지만 원래 0이 들어있던 것과는 구별이 되지 않는다.
			int getIntValue() const;
			///실패하면 0.f를 반환하지만 원래 0.f가 들어 있던 것과는 구별이 안 된다.
			float getFloatValue() const;
			///실패하면 0.0를 반환하지만 원래 0.0가 들어 있던 것과는 구별이 안 된다.
			double getDoubleValue() const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getIntValues(int* out, int number) const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getFloatValues(float* out, int number) const;
			///반환 값은 실제로 읽을 수 있는 요소 수
			int getDoubleValues(double* out, int number) const;

			//이하 사용자는 의식하지 않아도 된다
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
