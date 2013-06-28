//---------------------------------------------------------------------------

#ifndef isysProtocolH
#define isysProtocolH

#include "CommsProtocol.h"
#include <deque>

class IsysProtocol : public CommsProtocol
{
private:
	char txFrame, rxFrame;
	Buffer rxBuffer;
	bool endFrame;
	std::deque< String > txQueue;
	String response;
	TDateTime sleepTime;

	void startListening(unsigned char byte);
	void checkReadyToSend(unsigned char byte);
	void checkMessageSent(unsigned char byte);
	bool getNextFrame();
	int checkFrame(const unsigned char * data, unsigned length);
	int readFrame(const unsigned char * data, unsigned length);
	void createFrame(const String & message);
	unsigned fromHex(unsigned char byte);

public:
	enum enumProtocolState { NEUTRAL, LISTENING, RECEIVING, WAITING, SENDING } protocolState;
    static char * strProtocolState(int state) {
        char* strings[]   = {"NEUTRAL", "LISTENING", "RECEIVING", "WAITING", "SENDING"};
        return strings[state];
    }
	IsysProtocol() : protocolState(NEUTRAL)	{}
    enumProtocolState getProtocolState() { return protocolState; }

	// implementations of superclass virtual functions
	void init() {protocolState = NEUTRAL;}
	int accept(const unsigned char * data, unsigned length);
	bool messageReady() const;
	CommsMessage * getMessage();
	bool queue(const CommsMessage & data);
	String respond();
	void reset();
};


//---------------------------------------------------------------------------
#endif
