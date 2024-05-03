#pragma once
#include "opencv2/opencv.hpp"
#include "entry-linux.hpp"
#include "v4l2_helper.h"
#include <thread> 
#include <mutex>  
#include <chrono>
#include <sys/stat.h>
#include "DrawingUtils.hpp"

#define USING_CAMERA
//#define USING_VIDEO

//#define USING_PICTURES

#ifdef USING_CAMERA
	bool USING_THREAD = true;
#endif

//The string of camera
static const char* default_videodev = "/dev/video0";

//Set this flag to "true" to print time logs
const bool CHECK_TIME = false;
const int PRINT_TIME_FRAME = 100; //Frequency to print time logs
//------------------------------------------------

//To process video/images, need to set the path 
const std::string file_input_name = "";
std::string file_output_path =  "../";

enum VIDEO_SAVING {
	NOT_SAVING = 0,
	SAVING_WITHOUT_PROCESSING = 1,
	SAVING_WITH_PROCESSING = 2,
};

VIDEO_SAVING _video_save_request{NOT_SAVING};
bool _is_starting_saving_video  = true; // start saving from the beginning
uint64_t _t_video_save_begin;
uint64_t SAVING_DURATION = 60*30; //60s
//-------------------------------------------------


//Display information
const std::string win_name = "Hand Safety - Version 1.0.0";

//Input resolution of camera
const int defauls_width = 640;
const int default_height = 480;

//Global vbs

cv::VideoCapture _camera; //Camera instance
cv::VideoWriter _video; //Video writer

//Camera buffers
//------------------------------------
unsigned char* _ptr_cam_frame;
int _bytes_used;
cv::Mat _yuyv_frame;
//------------------------------------

//Time logging:
//------------------------------------
//Camera time
int _no_of_cam_call = 0;
double _time_cam_total = 0;

//Detector time
int _no_of_detector_call = 0;
double _time_detector_total = 0;

//Total time
int _no_of_call = 0;
double _time_total = 0;

int _no_of_detector_call_sum = 0;
double _time_detector_total_sum = 0;

//Total time
int _no_of_call_sum = 0;
double _time_total_sum = 0;
//------------------------------------

//Boolean flags to process the flow
bool _is_grabed_frame = false; //frame is available
bool _is_running = true; //keep running or not
bool _system_error = false;

//Variables to send/get information from Manomotion SDK
std::vector<HandInfo> _hand_info;
Session _session;
cv::Mat _grabed_frame;
DetectionAreaInfo _detection_area;

uchar *_frame_data;
uchar *_frame_mr_data;
cv::Mat _mr_frame;


//Drawing tools
DrawingUtils _drawing_utils;

//Definition of functions in Main

//Initialization:
//------------------------------------
/**
To create a default session
*/
Session createDefaultSession();

/**
To create a default point
*/
cv::Point3f createDefaultPoint();

/**
To create a default bounding box
*/
BoundingBox createDefaultBoundingBox();

/**
To create a default tracking information
*/
TrackingInfo createDefaultTrackingInfo();

/**
To create a default hand information
*/
HandInfo createDefaultHandInfo();

/**
To init the setting
*/
ManoSettings instantiateSettings(char * serial_key);
//------------------------------------



//Main:
//------------------------------------
/**
To start camera
*/
void startCamera();

/**
To init the license
*/
void initLicense();

/**
To initialise global variables
*/
void setVariables();

/**
To start processing
*/
void start();

/**
To get Mat frame
*/
bool grabFrame(cv::Mat &frame);

/**
To get frame using threads
*/
void grabFrame_thread();


/**
To draw ouput on a Mat 
*/
void showFrame(cv::Mat &drawing);


/**
To save ouput into a video
*/
void saveOutput(cv::Mat drawing);

/**
To handle OpenCV error
*/
int handleError(int status, const char* func_name,
	const char* err_msg, const char* file_name,
	int line, void* userdata);

/**
To update frame
*/
void update();
//------------------------------------
