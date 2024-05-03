/**
* @brief This file contains Enums used to group different type of components.
* Copyright 2015-2022 ManoMotion. All rights reserved.
*/

#pragma once
/// <summary>
/// Define the different type of answer the SDK returns to the user
/// </summary>
enum LicenseAnswer {
	LICENSE_OK = 30, // no problems encountered
	LICENSE_KEY_NOT_FOUND = 31, // key does not exist
	LICENSE_EXPIRED = 32, // key exists but is no longer valid
	LICENSE_INVALID_PLAN = 33, // 
	LICENSE_KEY_BLOCKED = 34,
	LICENSE_INVALID_ACCESS_TOKEN = 35,
	LICENSE_ACCESS_DENIED = 36,
	LICENSE_MAX_NUM_DEVICES = 37,
	LICENSE_UNKNOWN_SERVER_REPLY = 38,
	LICENSE_PRODUCT_NOT_FOUND = 39,
	LICENSE_INCORRECT_INPUT_PARAMETER = 40,
	LICENSE_INTERNET_REQUIRED = 41,
	LICENSE_INCORRECT_BUNDLE_ID = 42
};

/// <summary>
/// Define the different type of AddOns that can be used within the SDK. In Windows we typically use the default option.
/// </summary>
enum AddOn {
	ADD_ON_DEFAULT = 0,
	ADD_ON_ARKIT = 1,
	ADD_ON_CORE = 2,
	ADD_ON_VUFORIA = 3,
	ADD_ON_FOUNDATION = 4
};

/// <summary>
/// Define the available orientations for the camera. 
/// </summary>
enum Orientations {
	LANDSCAPE_RIGHT = 4,
	LANDSCAPE_LEFT = 3,
	PORTRAIT = 2,
	PORTRAIT_INVERTED = 1
};

/// <summary>
/// Define the different image formats in which the image is formed. 
/// </summary>
enum IMAGE_FORMAT {
	GRAYSCALE_FORMAT = 5,
	BGRA_FORMAT = 4,
	RGBA_FORMAT = 3,
	RGB_FORMAT = 2,
	BGR_FORMAT = 1
};

/// <summary>
/// Define gesture active status. 
/// </summary>
enum GESTURE_ACTIVATED {
	GESTURE_NOT_ACTIVATED = 0,
	GESTURE_BEING_ACTIVATED = 1,
	GESTURE_BE_ACTIVATED = 2,
};

/// <summary>
/// Define warning status. 
/// </summary>
enum WARNING_STATUS {
	NO_WARNING = 0,
	MULTI_USERS = 1,
	MULTI_HAND_INFERENCE = 2,
};