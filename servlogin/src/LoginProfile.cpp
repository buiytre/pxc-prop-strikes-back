#include <LoginRequest.h>
#include <LogoutRequest.h>
#include <LoginProfile.h>
#include <ControlConnection.h>
#include <iostream>


using namespace std;

LoginProfile::LoginProfile() : TransferableProfile() {
	mCreators.push_back(std::pair<int, TransferableCreator*>(100, new LoginRequestRcvd::Creator("LoginRequestRcvd"))); 
	mCreators.push_back(std::pair<int, TransferableCreator*>(102, new LogoutRequestRcvd::Creator("LogoutRequestRcvd")));
	mCreators.push_back(std::pair<int, TransferableCreator*>(51, new ControlConnectionRcvd::Creator("ControlConnectionRcvd")));
	
	mCreatorIds.push_back(std::pair<std::string, int>("LoginRequestSend", 101)); 
	mCreatorIds.push_back(std::pair<std::string, int>("LogoutRequestSend", 103));
	mCreatorIds.push_back(std::pair<std::string, int>("NewClientSend", 50)); 
	mCreatorIds.push_back(std::pair<std::string, int>("ClientDisconnectSend", 52)); 
}

LoginProfile::~LoginProfile() {
	cout << "Destruct LoginProfile";
	fflush(stdout);
	for(int i = 0; i < mCreators.size(); ++i) delete mCreators[i].second;
	mCreators.clear();
	cout << "End Destruct controlProfile";
	fflush(stdout);
}


