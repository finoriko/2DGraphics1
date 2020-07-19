#ifndef INCLUDED_GAMELIB_BASE_REFERENCETYPE_H
#define INCLUDED_GAMELIB_BASE_REFERENCETYPE_H

//참조 카운트 잡는 틀을 만들 때 편하게 하기 위한 기저 클래스
namespace GameLib {

	class ReferenceType 
	{
	public:
		ReferenceType() : mReferenceCount(1) {
		}
		void refer() {
			++mReferenceCount;
		}
		void release() {
			--mReferenceCount;
		}
		int referenceCount() const {
			return mReferenceCount;
		}
	protected:
		~ReferenceType() {
		}
	private:
		int mReferenceCount;
	};

} //namespace GameLib

#endif
