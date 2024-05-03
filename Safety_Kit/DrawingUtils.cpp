#include "DrawingUtils.hpp"

using namespace cv;
using namespace std;

void DrawingUtils::paintRGB(Mat &drawing, cv::Mat &grabed_frame){
	Mat grabed_frame_rgba;
	show_frame=grabed_frame.clone();

	if(portrait_mode) {
		transpose(show_frame,show_frame);
		flip(show_frame,show_frame,-1);
	}
	else
		flip(show_frame,show_frame,-1);

	cvtColor(show_frame , grabed_frame_rgba , COLOR_RGB2RGBA);
	drawing = grabed_frame_rgba;
}


void DrawingUtils::paintBox(Mat drawing , BoundingBox box, int status_flag){

	Rect drawRect;
	if(portrait_mode) {
		int w = box.width*show_frame.cols;
		int h = box.height*show_frame.rows;
		Point tl(box.top_left.x*show_frame.cols  , (box.top_left.y)*show_frame.rows);
		drawRect  = Rect(tl.x , tl.y , w , h);
	}
	else {
		int w = box.width*show_frame.rows;
		int h = box.height*show_frame.cols;
		Point tl(box.top_left.x*show_frame.rows  , box.top_left.y*show_frame.cols);
		drawRect = Rect(tl.y , tl.x , h, w);
	}
		
	Scalar color;

	// To show differnt hand states
	switch (status_flag)
	{
	case 1:
		color = Scalar(0, 0, 255, 255);
		break;
	case 2:
		color = Scalar(0, 165, 255, 255);
		break;
	default:
		color = Scalar(0, 255, 0, 255);
		break;
	}

	rectangle(drawing , drawRect , color, 2);
}

void DrawingUtils::paintPalm(Mat drawing , Point3f palm, float radius){
	Point drawPoint;
	if(portrait_mode) 
		drawPoint = Point(palm.x*show_frame.cols , palm.y*show_frame.rows);
	else
		drawPoint = Point(palm.y*show_frame.cols, palm.x*show_frame.rows );
	int size = 10;
	Scalar color_c(255 , 0 , 0 , 255);
	circle(drawing , drawPoint , 2 , color_c,-1 );
	Scalar color(0 , 255 , 0 , 255);
	circle(drawing , drawPoint , radius * show_frame.cols , color );
}


void DrawingUtils::paintHandId(Mat drawing, int hand_id, BoundingBox box){
	Scalar color(255,255,0);
	string text = to_string(hand_id);

	Point p;	

	if(portrait_mode) 
		p = Point(box.top_left.x * show_frame.cols, box.top_left.y * show_frame.rows);
	else
		p = Point(box.top_left.y * show_frame.cols, box.top_left.x * show_frame.rows);

	putText(drawing, text, p, 1,2,color);
}

void DrawingUtils::paintHandTracking(Mat drawing , TrackingInfo tracking_info){
	paintBox(drawing , tracking_info.bounding_box, tracking_info.detection_area_position);
    	paintPalm(drawing , tracking_info.palm_center, tracking_info.hand_radius);
	paintHandId(drawing, tracking_info.hand_id, tracking_info.bounding_box);
}


void DrawingUtils::paintHandInfo(Mat drawing, std::vector<HandInfo> &_hand_info){
	for (int i = 0; i < _hand_info.size(); i++) {
		paintHandTracking(drawing, _hand_info[i].tracking_info);
	}
}


/** To draw danger and warning zones
*/
void DrawingUtils::paintDetectionAreaInfo(Mat drawing, DetectionAreaInfo& detection_area) {
	
	Rect detection_rect = detection_area.detection_zone;
	Rect warning_rect = detection_area.warning_zone;

	if(!portrait_mode) {
		detection_rect = Rect(detection_rect.y, detection_rect.x, detection_rect.height, detection_rect.width);
		warning_rect = Rect(warning_rect.y, warning_rect.x, warning_rect.height, warning_rect.width);
	}


	int calib_status = detection_area.calibration_status;
	int detect_status = detection_area.detection_status;
	int no_status_markers = detection_area.no_status_markers;

	Point text_location = Point(10,25);
	Point text_gap = Point(0,25);

	switch (calib_status)
	{
	case 1:
		putText(drawing, "Place Zone/Status Markers", text_location, 1, 2, Scalar(0, 0, 0), 4); // to have a black border
		putText(drawing, "Place Zone/Status Markers", text_location, 1, 2, Scalar(0, 255, 255), 2);
		text_location += text_gap;
	
		// draw danger zone
		if (detection_rect.area())
			rectangle(drawing, detection_rect, Scalar(0, 0, 255), 3);

		//// draw warning zone, if any
		if (warning_rect.area())
			rectangle(drawing, warning_rect, Scalar(0, 165, 255), 3);

		//// show number of status markers if there is at least 1
		if(no_status_markers)
		{
			putText(drawing, "Status Markers Detected = " + to_string(no_status_markers), 
				text_location, 1, 2, Scalar(0, 0, 0), 4); // to have a black border
			putText(drawing, "Status Markers Detected = " + to_string(no_status_markers), 
				text_location, 1, 2, Scalar(150, 255, 0), 2);
			text_location += text_gap;

		}

		break;
	case 2: // updating danger zone
		//// draw warning zone, if any
		if (warning_rect.area())
			rectangle(drawing, warning_rect, Scalar(0, 165, 255), 3);

		rectangle(drawing, detection_rect, Scalar(0, 255, 0), 3);
		putText(drawing, "Updating Danger Zone", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Updating Danger Zone", text_location, 1, 2, Scalar(0, 255, 0), 2);
		text_location += text_gap;

		break;
	case 3: // updating warning zone
		//// draw danger zone, if any
		if (detection_rect.area())
			rectangle(drawing, detection_rect, Scalar(0, 0, 255), 3);

		rectangle(drawing, warning_rect, Scalar(0, 255, 0), 3);
		putText(drawing, "Updating Warning Zone", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Updating Warning Zone", text_location, 1, 2, Scalar(0, 255, 0), 2);
		text_location += text_gap;

		break;

	case 4: // updating both zones

		// danger
		rectangle(drawing, detection_rect, Scalar(0, 255, 0), 3);
		putText(drawing, "Updating Detection Zone", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Updating Detection Zone", text_location, 1, 2, Scalar(0, 255, 0), 2);
		text_location += text_gap;


		// warning
		rectangle(drawing, warning_rect, Scalar(255, 255, 0), 3);
		putText(drawing, "Updating Warning Zone", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Updating Warning Zone", text_location, 1, 2, Scalar(255, 255, 0), 2);
		text_location += text_gap;

		break;

	case -2:
		putText(drawing, "Too far !!!", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Too far !!!", text_location, 1, 2, Scalar(0, 0, 255), 2);
		text_location += text_gap;

		break;
	case -3:
		putText(drawing, "Too close !!!", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Too close !!!", text_location, 1, 2, Scalar(0, 0, 255), 2);
		text_location += text_gap;

		break;
	case 0: // calibrated
		rectangle(drawing, warning_rect, Scalar(0, 165, 255), 3);
		rectangle(drawing, detection_rect, Scalar(0, 0, 255), 3);
		if (detect_status == 1) // hands in danger zone
		{	
			// show a transparent red area inside the danger zone
			Mat mask = drawing.clone();
			rectangle(mask, detection_rect, Scalar(0, 0, 255), -1);
			addWeighted(drawing, .9, mask, .1, 0, drawing);
			// draw a thick box
			rectangle(drawing, detection_rect, Scalar(0, 0, 255), 6);
		}
		else if (detect_status == 2) // hands in warning zone not in dange zone
		{
			// show a transparent orange area inside the warning zone
			Mat mask = drawing.clone();
			rectangle(mask, warning_rect, Scalar(0, 165, 255), -1);
			addWeighted(drawing, .9, mask, .1, 0, drawing);
			// draw a thick box
			rectangle(drawing, warning_rect, Scalar(0, 165, 255), 6);
		}
		break;

	case -1: // default - just show the detection rect, without the mask
		rectangle(drawing, detection_rect, Scalar(0, 0, 255), 3);
		break;
	default:
		break;
	}

	// paint camera status info
	if(detection_area.camera_moved)
	{
		putText(drawing, "Camera Moved!", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Camera Moved!", text_location, 1, 2, Scalar(0, 0, 255), 2);
		text_location += text_gap;
	}

	if(detection_area.camera_blocked)
	{
		putText(drawing, "Camera Blocked!", text_location, 1, 2, Scalar(0, 0, 0), 4);
		putText(drawing, "Camera Blocked!", text_location, 1, 2, Scalar(0, 0, 255), 2);
		text_location += text_gap;
	}
	
}
