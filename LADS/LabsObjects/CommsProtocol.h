//---------------------------------------------------------------------------

#ifndef CommsProtocolH
#define CommsProtocolH

#include "SerialComms.h"
#include "LIMSAnalysis.h"

//---------------------------------------------------------------------------
//	base class for messages sent and received by the analyser
//---------------------------------------------------------------------------

class CommsMessage
{
public:
	virtual ~CommsMessage() {}
	virtual String toString() const = 0;
};

//---------------------------------------------------------------------------
//	base class for records: one or more records make a message
//---------------------------------------------------------------------------

class CommsRecord
{
protected:
	virtual String getField( int num ) const = 0;

public:
	virtual ~CommsRecord() {}
	static String split( const String & input, int num, const char * delimiters );
	virtual String toString() const = 0;
};

//---------------------------------------------------------------------------
//	base class for work list handler: accepts barcodes + category IDs;
//	produces messages to program analyser with tests from the work list
//---------------------------------------------------------------------------

class CommsWorklist
{
	int categoryID;
	std::deque< LIMSAnalysis > pending;

protected:

	CommsWorklist() : categoryID( 0 ) {}
	bool sampleWaiting() const { return !pending.empty(); }
	LIMSAnalysis getNextSample();

public:

	virtual ~CommsWorklist() {}

	// queue a programme/clear request for the given barcode
	void setCategory( int id ) { categoryID = id; }
	LIMSAnalysis & add( const String & barcode );

	// check work list for next barcode; list tests required
	virtual CommsMessage * nextRequest( LQuery qCluster, LQuery qCentral ) = 0;
};

//---------------------------------------------------------------------------
//	base class for results handler: interprets messages and stores results
//	and/or passes request to companion CommsWorklist to be handled later
//---------------------------------------------------------------------------

class CommsResults
{
public:
	virtual ~CommsResults() {}
	virtual bool add( const CommsMessage & received, LQuery data, LQuery worklist ) = 0;
};

//---------------------------------------------------------------------------
//	base class for low-level handler: claims line, adds/checks CRC etc.
//---------------------------------------------------------------------------

class CommsProtocol
{
public:
	virtual ~CommsProtocol() {}

	//	prepare to receive/transmit when program starts
	virtual void init() = 0;

	//	copy data sent from analyser; return length used
	virtual int accept( const unsigned char * data, unsigned length ) = 0;

	//	return true once complete message has been received
	virtual bool messageReady() const = 0;

	//	return next message from the analyser; clear from buffer
	virtual CommsMessage * getMessage() = 0;

	//	queue message to send the analyser; return false if not ready
	virtual bool queue( const CommsMessage & data ) = 0;

	//	return next block of data to send the analyser
	virtual String respond() = 0;

	//	discard buffered data and return to neutral state
	virtual void reset() = 0;
};

//---------------------------------------------------------------------------

#endif
