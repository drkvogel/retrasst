#ifndef TWOWAYTALK
#define TWOWAYTALK

//---------------------------------------------------------------------------

class TwoWayTalk
{
	HANDLE semaphore;
	long local_count;

public:

	explicit TwoWayTalk( const char * name );
	~TwoWayTalk();

	bool CheckIn();

	const static char * c_to_v, * v_to_ba, * ba_to_v, * v_to_c;
};

//---------------------------------------------------------------------------

#endif

