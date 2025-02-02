#include <Zone.h>
#include <ctime>
#include <unistd.h>
#include <assert.h>

ZoneCallback::ZoneCallback(Zone* zone) : mZone(zone), mInstructions() {}

ZoneCallback::~ZoneCallback() {
	delete mZone;

	while (!mInstructions.empty()) {
		delete mInstructions.front().first;
		mInstructions.pop();
	}
}

Zone* ZoneCallback::zone() const { return mZone; }

bool ZoneCallback::step (double stepTime) throw() {
	bool valid = true;
	valid = gameStep(stepTime);
	if (valid) execInstructions();

	return valid;
}

void ZoneCallback::execInstructions() throw() {
	std::pair<Instruction*, Connection*> ins;
	while(!mInstructions.empty()) {
		//Exec every pending instruction and delete ir afterwards
		ins = mInstructions.front();
		ins.first->exec(ins.second);
		delete ins.first;

		mInstructions.pop();
	}
}

void ZoneCallback::addInstruction (Instruction* ins, Connection* c) throw() {
	if (ins) mInstructions.push(std::pair<Instruction*, Connection*>(ins, c));
}

// ZoneCallbackCreator* ZoneHandler::sGameCreator = NULL;

// void ZoneHandler::setGameCreator(ZoneCallbackCreator* zcb) {
// 	sGameCreator = zcb;
// }

ZoneHandler::ZoneHandler(int zoneId) throw()	: mGame(sGameCreator->create(zoneId))
						, mDetachableZone(-1)
						, mRunThread(NULL)
						, mGameMutex()
						, mParamMutex()
						, mMaxSteps(DEFAULT_MAX_STEPS_PER_SEC)
						, mMinSteps(DEFAULT_MIN_STEPS_PER_SEC)
						, mMinStepTime(1.0 / DEFAULT_MAX_STEPS_PER_SEC)
						, mMaxStepTime(1.0 / DEFAULT_MIN_STEPS_PER_SEC)
						, mTDBoundary(TD_DEFAULT_BOUNDARY)
						, mTDCriticalBoundary(TD_DEFAULT_CRITICAL_BOUNDARY)
						, mTDDelay(1.0)
						, mTDDecreaser(TD_DEFAULT_DECREASER)
						, mAvgStepTime(0.0)
						, mLoad(0.0)
						, mRun(false) {
	
}

ZoneHandler::~ZoneHandler() throw() {
	run(false);
	stop();
	
	if (mGame) {
		delete mGame;
	}
}

void		ZoneHandler::sendStressCall() throw() {
	//TODO: IMPLEMENT
	//Critical == TD > maxTD
}

bool		ZoneHandler::run() throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mRun;
}

void		ZoneHandler::run(bool r) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mRun=r;
}

void		ZoneHandler::saveStats(double avgStepTime) throw() {
	//TODO: IMPLEMENT
}

Zone* ZoneHandler::zone() throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	if (mGame) return mGame->zone();
	return NULL;
}

ZoneHandler& ZoneHandler::minSteps (double steps) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mMinSteps = steps;
	return *this;
}

ZoneHandler& ZoneHandler::maxSteps (double steps) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mMaxSteps = steps;
	return *this;
}

ZoneHandler& ZoneHandler::timeDilationBoundary (double loadPercentage) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);	
	mTDBoundary = loadPercentage;
	return *this;
}

ZoneHandler& ZoneHandler::timeDilationCriticalBoundary (double loadPercentage) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mTDCriticalBoundary = loadPercentage;
	return *this;
}

ZoneHandler& ZoneHandler::timeDilationDecreaser (double decreaser) throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mTDDecreaser = decreaser;
	return *this;
}

double ZoneHandler::minSteps() throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mMinSteps;
}

double ZoneHandler::maxSteps() throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mMaxSteps;
}

double ZoneHandler::timeDilation () throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mTDDelay;
}

double ZoneHandler::timeDilationBoundary () throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mTDBoundary;
}

double ZoneHandler::timeDilationDecreaser () throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mTDDecreaser;
}

void ZoneHandler::startThread() throw() {
	time_t startTime, endTime;
	double stepTime = 0.0;

	int stepsPerSec = 0;
	double timeTillSec = 0.0;
	double avgStepTime = 0.0;

	//Init the zone callback (a.k.a. game)
	mGameMutex.lock();
	if (!mGame || !mGame->init()) return;
	mGameMutex.unlock();
	run(true);
	
	//Start main loop
	time(&startTime);
	while(run()) {
		mGameMutex.lock();
		
		//Run the frame step
		if (run()) run(mGame->step(stepTime * mTDDelay));
		
		time(&endTime);
		stepTime = difftime(endTime, startTime);


		//In case we are running faster than allowed, nop a little.
		if (stepTime < mMinStepTime) {
			usleep((unsigned int)(mMinStepTime - stepTime)*1000);
			stepTime = difftime(endTime, startTime);
		}

		++stepsPerSec;
		timeTillSec += stepTime;
		
		//Calcculate load every second
		if (timeTillSec >= 1.0) {
			//Calculate load
			avgStepTime = timeTillSec / stepsPerSec;
			//Load is compared with step time max and min boundaries.
			
			mParamMutex.lock();
			mLoad = (avgStepTime - mMinStepTime)/(mMaxStepTime - mMinStepTime);

			mLoad = mLoad > 1.0 ? 1.0 : mLoad; //Load can't be negative.
			mLoad = mLoad < 0.0 ? 0.0 : mLoad; //Load can't be more than 100%.
			
			//Check load boundaries.
			if (mLoad > mTDBoundary) {
			//Start Time Dilation.
				//Increase time dilation by decreasing the time multiplier.
				mTDDelay *= mTDDecreaser;
				
				//Send stress call to balance server.
				sendStressCall();
			}
			
			//If we're on time dilation, but the load is below boundary, stop time dilation.
			else if (mTDDelay < 1.0 && mLoad < mTDBoundary) mTDDelay = 1.0;

			//Save statistics.
			saveStats(avgStepTime);
			mParamMutex.unlock();
			
			//Reset counters
			timeTillSec = 0.0;
			stepsPerSec = 0;
		}
		time(&startTime);
		
		mGameMutex.unlock();
	}
}

void ZoneHandler::maskAsDetachable(int newServer) {
	std::lock_guard<std::mutex> lk(mParamMutex);
	mDetachableZone = newServer;
}

int ZoneHandler::detachable() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mDetachableZone;
}

void ZoneHandler::start() throw() {
	if (mRunThread) return;
	mRunThread = new std::thread(&ZoneHandler::startThread, this);
}

void ZoneHandler::stop() throw() {
	run(false);
	if (mRunThread) mRunThread->join();
	delete mRunThread;
	mRunThread = NULL;
}

void ZoneHandler::doBackup() throw() {
	void* dbInfo = NULL;
	std::lock_guard<std::mutex> lk(mGameMutex);
	
	if (mGame) {
		//dbInfo = mGame->zone()->dbData();
		//TODO: SAVE IN DB
		//delete dbInfo;
	}
}

void ZoneHandler::addInstruction(Instruction* ins, Connection* c) throw() {
	//TODO
}

double ZoneHandler::getLoad() throw() {
	std::lock_guard<std::mutex> lk(mParamMutex);
	return mLoad;
}
