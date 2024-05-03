#include "Main_safety.h"
#include "gpio_com.h"
using namespace cv;
using namespace std;

#pragma region InstantiateVariables

Session createDefaultSession(){
	Session session;
	session.add_on = ADD_ON_DEFAULT;
	session.orientation = 2;
	
	session.smoothing_controller = 0.01f;
	return session;

}

Point3f createDefaultPoint(){
	Point3f point;
	point.x = -1;
	point.y = -1;
	point.z = -1;
	return point;
}

BoundingBox createDefaultBoundingBox(){
	BoundingBox bounding_box;
	bounding_box.height = 0;
	bounding_box.width = 0;
	bounding_box.top_left = createDefaultPoint();
	return bounding_box;
}

TrackingInfo createDefaultTrackingInfo(){
	TrackingInfo tracking_info;

	BoundingBox bounding_box = createDefaultBoundingBox();

	tracking_info.bounding_box = bounding_box;
	tracking_info.hand_radius = 0;
	tracking_info.palm_center = createDefaultPoint();
	return tracking_info;
}
HandInfo createDefaultHandInfo(){
	HandInfo hand_info;

	TrackingInfo tracking_info = createDefaultTrackingInfo();;

	hand_info.tracking_info = tracking_info;

	return hand_info;
}

ManoSettings instantiateSettings(char * serial_key){
	ManoSettings settings;
	settings.image_format = IMAGE_FORMAT::RGB_FORMAT;
	settings.platform = 0;
	settings.serial_key = serial_key;
	return settings;
}

#pragma endregion




void startCamera(){

#ifdef USING_CAMERA

	if(USING_THREAD ){
		if (helper_init_cam(default_videodev, defauls_width, default_height, V4L2_PIX_FMT_UYVY, IO_METHOD_USERPTR) < 0) {
			_system_error = true;
			gpio_set_value(GPIO_PORT_IDS_OUT[0], 1);
			throw "Camera cannot be openned";
		}
		_yuyv_frame = Mat(default_height, defauls_width, CV_8UC2);
	}
	else {
		_camera.set(VideoCaptureProperties::CAP_PROP_FRAME_WIDTH , defauls_width);
		_camera.set(VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT , default_height);
		_camera.open("v4l2src device=/dev/video0 ! video/x-raw, width=640, height=480 ! videoconvert ! appsink");
	}

#elif defined(USING_VIDEO)
	//file_input_name = "/home/mano/Videos/input.avi";
	/file_output_name = "../output.avi";
	_camera.open(file_input_name);

#elif defined( USING_PICTURES)
	//file_input_name = "test.png";
	_camera.open(file_input_name);
#endif

}

void initLicense(){
	ManoSettings settings = instantiateSettings("DXCCN-SZWBK-QYFVF-DTNJB");
	LicenseStatus license_status = init(settings);
	if(license_status.license_answer != LicenseAnswer::LICENSE_OK){
		//throw "license Answer Unexpected: " + license_status.license_answer;
		std::cout<< "license Answer Unexpected: " << license_status.license_answer  << std::endl;
	}
	if(license_status.days_left <= 0){
		throw "license expired";
	}
	if(license_status.machines_left <= 0){
		throw "maximum amount of devices reached";
	}

}



void setVariables(){
	setFrameArray(_frame_data);
	setResolution(defauls_width , default_height);
}


void start(){

	startCamera();

	initLicense();

	setVariables();

	_session = createDefaultSession();

	_hand_info.clear();

}

bool grabFrame(Mat &frame){

#ifdef USING_CAMERA
	if(!_camera.isOpened()){
		std::cout<< "camera did not open";
		return false;
		throw "camera did not open";
	}
	if(_camera.get(VideoCaptureProperties::CAP_PROP_FRAME_WIDTH) != defauls_width){
		return false;
		throw "camera width does not match";
	}
	if(_camera.get(VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT) != default_height){
		return false;
		throw "camera height does not match";
	}
	if(_camera.read(frame)){
		return true;
	} else{
		throw "could not grab grabed_frame";

		return false;
	}

#elif defined(USING_VIDEO)
	if(_camera.read(frame)){
	} else{
		return false;
		//throw "could not grab grabed_frame";
	}

	resize(frame , frame , Size(defauls_width , default_height));
	flip(frame,frame,-1);
	return true;

#elif defined( USING_PICTURES)
	string file_name = "/home/mano/test_frames/20210413_122733766_RAW_"+to_string(frame_count++)+".jpg";
	frame = imread(file_name);
	transpose(frame,frame);
	flip(frame,frame,-1);
	return true;

#endif
}

void grabFrame_thread() {
	while (_is_running) {

		uint64_t t_cam_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
				now().time_since_epoch()).count();

		bool is_get_frame = true;

		if (helper_get_cam_frame(&_ptr_cam_frame, &_bytes_used) < 0) {
			is_get_frame = false;
		}

		_yuyv_frame.data = _ptr_cam_frame;
		if(_yuyv_frame.empty()) {
			is_get_frame = false;
		}

		if(is_get_frame) {
			cvtColor(_yuyv_frame, _grabed_frame, COLOR_YUV2BGR_UYVY);	
			_is_grabed_frame = true;

			if (helper_release_cam_frame() < 0) {
				_is_grabed_frame = false;
			}
		}
		else
			_is_grabed_frame = true;

		uint64_t t_cam_end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
					now().time_since_epoch()).count();

		_no_of_cam_call++;
		_time_cam_total = _time_cam_total + t_cam_end - t_cam_start;


	}

}

void showFrame(Mat &drawing){

	_drawing_utils.paintHandInfo(drawing,  _hand_info);

	_drawing_utils.paintDetectionAreaInfo(drawing,  _detection_area);

	namedWindow(win_name, WINDOW_NORMAL);
	setWindowProperty(win_name, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);

	imshow(win_name , drawing);


}

void saveOutput(Mat _drawing){
	if(!_is_starting_saving_video)
		return;
	Mat drawing;
	flip(_drawing,drawing,-1);
	if(drawing.channels() == 4)
		cvtColor(drawing , drawing , COLOR_RGBA2RGB);

	int frame_rate = 30;

	//int fcc = VideoWriter::fourcc('X' , 'V' , 'I' , 'D');
	int fcc = VideoWriter::fourcc('M' , 'J' , 'P' , 'G');
	
	if(!_video.isOpened()){
		auto current_time = std::chrono::high_resolution_clock::now();
		uint64_t t_video_save = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
		//string file_output_name = file_output_path + "video_" + std::to_string(t_video_save) +".avi" ; // without folder
		
		// inside a folder with date as name
		time_t current_time_t = chrono::system_clock::to_time_t(current_time);
		tm* ltm = localtime(&current_time_t);
		string folder_name_date = to_string(ltm->tm_mday)+ "_" + to_string(1 + ltm->tm_mon) + "_" + to_string(1900 + ltm->tm_year);
		string folder_path = file_output_path + "/" + folder_name_date ;
		//make folder
		mkdir(folder_path.c_str(), 0777); 
		string file_output_name = folder_path + "/video_" + std::to_string(t_video_save) +".avi" ;	
 
		//if(_video.open(file_output_name , CAP_GSTREAMER, fcc, frame_rate , Size(drawing.cols , drawing.rows),true)){
		if(_video.open(file_output_name , CAP_FFMPEG, fcc, frame_rate , Size(drawing.cols , drawing.rows),true)){
			cout << "Video is created" << endl;
		}
		_t_video_save_begin = t_video_save;
	}	

	if(_video.isOpened()) {
		uint64_t t_current = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::
				now().time_since_epoch()).count();
		if(t_current- _t_video_save_begin > SAVING_DURATION ) {
			_video.release();
			//_is_starting_saving_video = false;
		}
		else
			_video.write(drawing);
	}
}


int handleError(int status, const char* func_name,
	const char* err_msg, const char* file_name,
	int line, void* userdata) {
	_system_error = true;
	return 0;   //Return value is not used
}



void update(){
if(!USING_THREAD) {
		uint64_t t_cam_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
				now().time_since_epoch()).count();

		_is_grabed_frame =  grabFrame(_grabed_frame);

		uint64_t t_cam_end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
					now().time_since_epoch()).count();

		_no_of_cam_call++;
		_time_cam_total = _time_cam_total + t_cam_end - t_cam_start;
}	

	if(_is_grabed_frame){

		if(_video_save_request == SAVING_WITHOUT_PROCESSING)
		 	saveOutput(_grabed_frame);

		_frame_data = _grabed_frame.data;
		setFrameArray(_frame_data);
		cvtColor(_grabed_frame , _mr_frame , COLOR_RGB2RGBA);
		_frame_mr_data = _mr_frame.data;
		Mat drawing;
		_drawing_utils.paintRGB(drawing, _grabed_frame);
		uint64_t t_detect_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
				now().time_since_epoch()).count();	
		_system_error = false;
		cv::redirectError(handleError);
		try {
			processFrame(_hand_info, &_session, &_detection_area);
		}
		catch (cv::Exception& e)	{
				_system_error = true;
		}
	
		int no_hands = _hand_info.size();
		if(no_hands > 0)
			_is_starting_saving_video = true;


		uint64_t t_detect_end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
					now().time_since_epoch()).count();
		_no_of_detector_call++;
		_time_detector_total = _time_detector_total + t_detect_end - t_detect_start;

		send_signal_to_gpio_ports();
		//check_if_shutdown();
		
		showFrame(drawing);
		if(_video_save_request == SAVING_WITH_PROCESSING)
		 	saveOutput(drawing);

	} 
}



int main(int argc, char *argv[]){


	if(argc>2) {
		cout << "Maximum one argument" << std::endl; 
	}
	else if (argc==2) {
		if(strcmp(argv[1],"-r")==0) {
			_video_save_request = SAVING_WITHOUT_PROCESSING;
			std::cout <<"Saving video without processing frame" <<std::endl;
		}
		else if(strcmp(argv[1],"-rp")==0) {
			_video_save_request = SAVING_WITH_PROCESSING;
			std::cout <<"Saving video and processing frame" <<std::endl;
		}
		else
			std::cout <<"Wrong argument!" <<std::endl;
	}
	


	initialize_gpio();

	start();
	
	// indicate start using leds
	int _delay = 300;
	for (int i = 0; i < 3; i++){

	_detection_area.detection_status = 0; // need to activate the leds
	send_signal_to_gpio_ports();
	std::this_thread::sleep_for(std::chrono::milliseconds(_delay));

	_detection_area.detection_status = 2; // need to activate the leds
	send_signal_to_gpio_ports();
	std::this_thread::sleep_for(std::chrono::milliseconds(_delay));

	_detection_area.detection_status = 1; // need to activate the leds
	send_signal_to_gpio_ports();
	std::this_thread::sleep_for(std::chrono::milliseconds(_delay));

	}
	

	//namedWindow(win_name, WINDOW_AUTOSIZE); 
	namedWindow(win_name, WINDOW_NORMAL);
	setWindowProperty(win_name, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	std::thread grab_thread;
	
if(USING_THREAD)
		grab_thread = std::thread(grabFrame_thread);

	std::cout <<"Open camera ..." <<std::endl;

	for(; waitKey(1) != 'q';){
		uint64_t t_start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
				now().time_since_epoch()).count();	

		update();
		uint64_t t_end = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::
					now().time_since_epoch()).count();
		if(_is_grabed_frame){
			_no_of_call++;
			_time_total  = _time_total  + t_end - t_start;
			if (CHECK_TIME &&_no_of_call % PRINT_TIME_FRAME == 0) {
				std::cout << "Avg on "<< PRINT_TIME_FRAME << " frames: Camera=" << _time_cam_total / (_no_of_cam_call * 1000) << "ms";
				std::cout << ",MM Detector=" << _time_detector_total / (_no_of_detector_call * 1000) << "ms";
				std::cout << ",Main=" << _time_total / (_no_of_call * 1000) << "ms" << std::endl;
				_time_detector_total = 0;
				_time_total = 0;
				_no_of_call = 0;
				_no_of_detector_call = 0;
			}
		}
	}

	_is_running = false;

	deint_gpio();

if(USING_THREAD)
		grab_thread.join();

	_video.release();
	if (CHECK_TIME)  {
		std::cout << " =================================== " << std::endl;
		std::cout << "Processing time per frame: " << std::endl;
		std::cout << "Avg (MM Detector)  = " << _time_detector_total / (_no_of_detector_call * 1000) << "ms" << std::endl;
		std::cout << "Total frame (MM Detector)  = " << _no_of_detector_call  <<  std::endl;
		std::cout << "Avg (Main) = " << _time_total / (_no_of_call * 1000) << "ms" << std::endl;
		std::cout << " =================================== " << std::endl;
	}
	cv::destroyAllWindows();

if(USING_THREAD)
	helper_deinit_cam();
else
	_camera.release();


	_Exit(0);


}
