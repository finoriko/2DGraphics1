#ifndef INCLUDED_GAMELIB_THREADING_EVENT_H
#define INCLUDED_GAMELIB_THREADING_EVENT_H

namespace GameLib {
	namespace Threading {

		class Event {
		public:
			static Event create(bool initialiState = false);
			///true가 되기를 기다리다
			void wait() const;
			///true로 함
			void set();
			///false로 함
			void reset();
			///true? 아니면 false?
			bool isSet() const;

			Event();
			~Event();
			Event(const Event&);
			operator void* () const;
			Event& operator=(const Event&);
			void release();
			bool operator==(const Event&) const;
			bool operator!=(const Event&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Threading
} //namespace GameLib

#endif
