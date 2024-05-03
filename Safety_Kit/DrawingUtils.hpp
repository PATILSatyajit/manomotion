#pragma once
#include "opencv2/opencv.hpp"
#include "public_structs.h"

//Color table
const float colors[25][3] = {
	{ 1.0, 237.0 / 255.0, 96.0 / 255.0 },
	{ 227.0 / 255.0, 207.0 / 255.0, 53.0 / 255.0 },
	{ 157.0 / 255.0, 132.0 / 255.0, 0 },
	{ 142.0 / 255.0, 135.0 / 255.0, 0 },
	{ 137.0 / 255.0, 239.0 / 255.0, 132.0 / 255.0 },
	{ 102.0 / 255.0, 219.0 / 255.0, 100.0 / 255.0 },
	{ 65.0 / 255.0, 182.0 / 255.0, 60.0 / 255.0 },
	{ 50.0 / 255.0, 162.0 / 255.0, 50.0 / 255.0 },
	{ 49.0 / 255.0, 147.0 / 255.0, 42.0 / 255.0 },
	{ 143.0 / 255.0, 242.0 / 255.0, 1.0 },
	{ 111.0 / 255.0, 209.0 / 255.0, 1.0 },
	{ 91.0 / 255.0, 174.0 / 255.0, 1.0 },
	{ 32.0 / 255.0, 143.0 / 255.0, 248.0 / 255.0 },
	{ 104.0 / 255.0, 166.0 / 255.0, 1.0 },
	{ 64.0 / 255.0, 135.0 / 255.0, 1.0 },
	{ 39.0 / 255.0, 102.0 / 255.0, 1.0 },
	{ 0, 72.0 / 255.0, 1.0 },
	{ 243.0 / 255.0, 169.0 / 255.0, 1.0 },
	{ 214.0 / 255.0, 140.0 / 255.0, 1.0 },
	{ 176.0 / 255.0, 107.0 / 255.0, 1.0 },
	{ 142.0 / 255.0, 78.0 / 255.0, 1.0 } };

const bool portrait_mode {false};

class DrawingUtils {
	
public:
    cv::Mat show_frame;

public:
	/**
	* Paint the drawing frame
	* @param drawing Drawing frame
	* @param grabed_frame Inut frame
	*/
    void paintRGB(cv::Mat &drawing, cv::Mat &grabed_frame);

	/**
	* Paint the drawing frame with hand boxes
	* @param drawing Drawing frame
	* @param box box
	* @param status_flag - position of hand related to detection area
	*/
    void paintBox(cv::Mat drawing , BoundingBox box, int status_flag);


    /**
	* Paint the drawing frame palm information
	* @param drawing Drawing frame
	* @param palm Palm center
	* @param radius Palm radius
	*/
	void paintPalm(cv::Mat drawing , cv::Point3f palm, float radius);


    /**
	* Paint the drawing frame with trackng information
	* @param drawing Drawing frame
	* @param tracking_info Tracking information
	*/
	void paintHandTracking(cv::Mat drawing , TrackingInfo tracking_info);


	/**
	* Paint the drawing frame with hand information
	* @param drawing Drawing frame
	* @param _hand_info Hand information
	*/
    void paintHandInfo(cv::Mat drawing, std::vector<HandInfo> &_hand_info);


	/**
	* Paint the drawing frame with detection area
	* @param drawing Drawing frame
	* @param detection_area Information of detection area
	*/
    void paintDetectionAreaInfo(cv::Mat drawing,  DetectionAreaInfo &detection_area);

	/**
	* Paint the drawing frame with hand ID
	* @param drawing Drawing frame
	* @param hand_id Hand ID
	* @param p Location to draw hand ID
	*/
	void paintHandId(cv::Mat drawing, int hand_id, BoundingBox box);
};
