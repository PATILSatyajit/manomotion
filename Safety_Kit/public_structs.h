/**
*  @brief This file contains the structures used publically to communicate the output information to higher levels of the software. 
*  Any developer on requires a similar file on their project so they can understand the information that is being returned.
* 
*  Copyright © 2022 ManoMotion. All rights reserved.
*/

#ifndef public_structs_h
#define public_structs_h
#include "opencv2/opencv.hpp"
#include "mano_enums.h"

/// <summary>
/// Struct used to return to the user the license properties
/// </summary>
struct LicenseStatus {
	LicenseAnswer license_answer; // communicates whether the license is valid or not and what the reason is behind
	int machines_left; // communicates how many machines the developer is allowed to run wih given license
	int days_left; // communicates how many days are left for the expiration of the given license
};

/// <summary>
/// Struct utilised to return a normalized rectangle that can be used on high level to locate the hand.
/// In order to obtain the actual rectangle, all its parameter must be multiplied by image size accordingly.
/// </summary>
struct BoundingBox {
	cv::Point3f top_left; // Left corner of the rectangle
	float width; // size of the rectangle horizontally
	float height; // size of the rectangle vertically
};

/// <summary>
/// Struct used to store the tracking information details about one hand
/// </summary>
struct TrackingInfo {
	BoundingBox bounding_box; // represents the ROI (Region of Interest) of the hand. The output is normalised values. These need to be multiplied by the image size.
	cv::Point3f palm_center; // represents the center of the palm. These values need to be denormalised multiplying the its value with the image size
	float hand_radius; // represents the radious of the circle drawn around the palm of the hand taking as center the previous point, palm_center
	float rotation; // 
	int hand_id; //
	int detection_area_position = 1; // 0 - outside, 1 - inside, -1 - border
	int hand_pose;
};


/// <summary>
/// Struct used to store the status information of one hand
/// </summary>
struct StatusInfo {
	GESTURE_ACTIVATED activated_status{ GESTURE_NOT_ACTIVATED };
	WARNING_STATUS warning_status{ NO_WARNING };
};



/// <summary>
/// Struct used to store all the information that we are able to retrieve about the hand. In this case we only have tracking information.
/// </summary>
struct HandInfo {
	TrackingInfo tracking_info; // Hand tracking information details
};



/// <summary>
/// Struct that stores the info of hand detection area
/// </summary>
struct DetectionAreaInfo {
	cv::Rect detection_zone;
	cv::Rect warning_zone;
	int detection_status = 1;
	int calibration_status = 0;
	StatusInfo status_info; // Status of the tracked hand
	int camera_moved = 0;
	int camera_blocked = 0;
	int no_status_markers = 0;

};

/// <summary>
/// Struct used to communicate the high level and low level on every frame. 
/// This struct is used to notify the SDK at runtime if there are any changes in regards the orientation of the camera, additional plugins, control smoothing. 
/// </summary>
struct Session {
	int orientation; // orientation of the camera; landscape left, right, portrait, etc
	int add_on; // defaults which mean no add-ons are being used
	float smoothing_controller; // by default we apply 0.15 smoothing to the output signal. Developer can change it any time up to 0.9
};

/// <summary>
/// Struct used to configure the SDK with a given license key, platform and image format. 
/// Based on the image format the SDK later will know how to deal with the input image
/// </summary>
struct ManoSettings {
	int platform; // in this project we work with Windows and JetsonNano platforms
	int image_format; // Image format can differ so developer must specify it
	char* serial_key; // given by ManoMotion, it is crucial for our SDK business but not for this specific client
};

#endif /* public_structs_h */
