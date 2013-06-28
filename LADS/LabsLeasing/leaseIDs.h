#ifndef LEASEIDS_H
#define LEASEIDS_H

#define		LEASEE_DEFAULT			0  		// Doesn't block anything (at the moment)

// the original applications from the version 2 release
#define		LEASEE_BUDDY2ALPHA		10 		// Blocks itself, wrappers, Reception, GC, Wizard and AA
#define		LEASEE_SAMPLERECEPTION	20		// Blocks itself, wrappers, B2A, GC, Wizard and AA
#define		LEASEE_EXTERNALWRAPPER	30		// Blocks itself, Buddy2Alpha and Aliquot Allocation
#define		LEASEE_WIZARD			40 		// Blocks everything else and vice versa
#define		LEASEE_GARBAGECOLLECT 	50		// Blocks itself, sample reception and Buddy2Alpha
#define		LEASEE_PLOTQC			60  	// Blocks itself and Buddy2Alpha

// added 1/03/08 (with corresponding records in ldbc)
#define 	LEASEE_ALLOCUTIL		70		// Blocks itself, B2A, GC, Wizard and wrappers
#define 	LEASEE_BOXRECEPTION		75		// Cannot block anything - runs on a laptop
#define 	LEASEE_BOXSYNC			80		// Blocks itself and other storage systems

// added 9/04/08 (with corresponding records in ldbc)
#define 	LEASEE_NIGHTSHIFT		90		// Replaces B2A and GC; blocks almost anything
#define 	LEASEE_STOREMAN		   100		// Storage management; blocks itself and Storage Sync

// 9 March 2012: added Generic comms (for permissions screen)
#define		LEASEE_GENCOMMS		   110
#endif
