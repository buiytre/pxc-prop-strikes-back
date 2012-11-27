#ifndef _DATAGRAM_H_
#define _DATAGRAM_H_

#include <cstdlib>
#include <string.h>
#include <string>
#include <typeinfo>
#include <exception>

#include <Transferable.h>

//Datagram DOES NOT ALLOW classes with non-static members
template <class T>
class Datagram : public Transferable {
	size_t mSize;
	std::string mType;
protected:
	Datagram(const std::string& type) 	: mSize(sizeof(T))
						, mType(type) { }
public:
	class Creator : public TransferableCreator {
		std::string mType;
		public:
		Creator(const std::string& type) : mType(type) {}

		Transferable* create(void* tr) const throw(WrongTransferableException&) {
			//if (tr.size() != typeSize()) throw WrongTransferableException("Sizes are different.");
			T* ret = new T();
			memcpy(((void*)ret) + sizeof(Datagram<T>), &tr, tr.size());
			return ret;
		}

		size_t typeSize() const throw() {
			return sizeof(T)-sizeof(Datagram<T>);
		}

		int type() const throw(TransferableVersionException&) {
			return TransferableFactory::instance().getTypeId(mType);
		}
	};

	void* transferableObject() const throw() { return (void*) this+sizeof(Datagram<T>); }

	size_t size() const throw() { return mSize; }

	int type() const throw (TransferableVersionException&) {
		return TransferableFactory::instance().getSendingId(mType);
	}

	virtual void exec() const throw() {}
};

#endif

