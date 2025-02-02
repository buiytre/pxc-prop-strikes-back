#ifndef _TEST_CLASSES_H_
#define _TEST_CLASSES_H_

#include <Datagram.h>

//Orden que se envía
class TestTransferableSent : public Datagram<TestTransferableSent> {
	int m_1;
	int m_2;
	int m_3;
	int m_4;

public:
	TestTransferableSent();
	virtual ~TestTransferableSent();
	
	TestTransferableSent& m1(int);
};

//Orden que se recibe
class TestTransferableRcvd : public Datagram<TestTransferableRcvd> {
	int m_1;
	int m_2;
	int m_3;
	int m_4;

public:
	TestTransferableRcvd();
	virtual ~TestTransferableRcvd();
	
	void exec(Connection* c) const throw();
};

class TestProfile : public TransferableProfile {
	std::vector<std::pair<int, TransferableCreator*> > mCreators;
	std::vector<std::pair<std::string, int> > mCreatorIds;
public:
	TestProfile();
	virtual ~TestProfile();
	const TransferableProfile::Creators& getCreators(const std::string& protocol) const throw(TransferableVersionException&);
	const TransferableProfile::CreatorIds& getCreatorIds(const std::string& protocol) const throw(TransferableVersionException&);
};

#endif

