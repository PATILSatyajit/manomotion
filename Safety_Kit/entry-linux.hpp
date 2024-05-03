#ifndef ENTRY_POINT_WIN
#define ENTRY_POINT_WIN
#include "public_structs.h"
#define ENTRY_POINT __attribute__ ((visibility ("default")))
extern "C" {
	/**
	To init the license
	*/
	ENTRY_POINT LicenseStatus init(ManoSettings settings);

	/**
	To check library
	*/
	ENTRY_POINT bool checkLibrary();

	
	/**
	To set resoluion
	*/
	ENTRY_POINT void setResolution(int width , int height);

	/**
	To set frame data
	*/
	ENTRY_POINT void setFrameArray(void* data);

	/**
	To process frame
	*/
	ENTRY_POINT void processFrame(std::vector<HandInfo>& hand_info, Session* manomotion_session, DetectionAreaInfo* detection_area);
}
#endif
