#ifndef INCLUDED_GAMELIB_BASE_REFERENCETYPE_H
#define INCLUDED_GAMELIB_BASE_REFERENCETYPE_H

//���� ī��Ʈ ��� Ʋ�� ���� �� ���ϰ� �ϱ� ���� ���� Ŭ����
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
