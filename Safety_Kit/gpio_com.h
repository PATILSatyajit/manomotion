/**
 * @file gpio_com.h
 * @author Manu
 * @brief To send information through gpio ports
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 */

#include "gpio.h"

// for nx module and nx dev kit
const int GPIO_PORT_IDS_OUT[7] = { 436, 428, 480, 268, 484, 445, 493 }; //{ 436, 428, 480, 268, 493, 445, 484 };
/* GPIO Pin numbers are            { 7,  11,  13,  15,  19,  12, 16 }*/

int no_gpio_ports_out = 0;
const int GPIO_PORT_IDS_IN = 447;
//int no_gpio_ports_in = 0;


/**
To send signals  to GPIO ports of NX board based on detected gestures
*/
void send_signal_to_gpio_ports() { 
	
	if(_system_error || !_is_grabed_frame)
		gpio_set_value(GPIO_PORT_IDS_OUT[0], 1);
	else
		gpio_set_value(GPIO_PORT_IDS_OUT[0], 0);

	// Detection status
	int detect_status = _detection_area.detection_status;

	switch (detect_status)
	{
	case 1: // hands in warning zone
		
		gpio_set_value(GPIO_PORT_IDS_OUT[2], 0);
		gpio_set_value(GPIO_PORT_IDS_OUT[6], 0);
		gpio_set_value(GPIO_PORT_IDS_OUT[1], 1);
		break;
	case 2: // hands in danger zone
		
		gpio_set_value(GPIO_PORT_IDS_OUT[1], 0);
		gpio_set_value(GPIO_PORT_IDS_OUT[6], 0);
		gpio_set_value(GPIO_PORT_IDS_OUT[2], 1);
		break;
	default:
		gpio_set_value(GPIO_PORT_IDS_OUT[1], 0);
		gpio_set_value(GPIO_PORT_IDS_OUT[2], 0);
		
		// set active green
		gpio_set_value(GPIO_PORT_IDS_OUT[6], 1);
		//std::cout << "here" << std::endl;

		break;
	}

	// Hand detection status
	int no_hands = _hand_info.size();

	if(no_hands > 0)
		gpio_set_value(GPIO_PORT_IDS_OUT[3], 1);
	else
		gpio_set_value(GPIO_PORT_IDS_OUT[3], 0);		
	

	// Clean the camera
	int camera_needs_cleaning = _detection_area.camera_blocked;
	if (camera_needs_cleaning)
		gpio_set_value(GPIO_PORT_IDS_OUT[4], 1);
	else
		gpio_set_value(GPIO_PORT_IDS_OUT[4], 0);

	// camera not aligned
	int camera_not_aligned = _detection_area.camera_moved;
	if (camera_not_aligned)
		gpio_set_value(GPIO_PORT_IDS_OUT[5], 1);
	else
		gpio_set_value(GPIO_PORT_IDS_OUT[5], 0);

}

/**
To initialize the required gpios
*/
void initialize_gpio()
{
	// gpios outputs
	no_gpio_ports_out = sizeof(GPIO_PORT_IDS_OUT)/sizeof(GPIO_PORT_IDS_OUT[0]);
	for(int i=0; i < no_gpio_ports_out; i++)
	{
		gpio_export(GPIO_PORT_IDS_OUT[i]);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	
	for(int i=0; i < no_gpio_ports_out; i++)
	{
		gpio_set_dir(GPIO_PORT_IDS_OUT[i], 1 );
	}
	
	// gpios inputs
	//int no_gpio_ports_in = sizeof(GPIO_PORT_IDS_IN) / sizeof(GPIO_PORT_IDS_IN[0]);
	//for (int i = 0; i < no_gpio_ports_in; i++)
	//{
	//	gpio_export(GPIO_PORT_IDS_IN[i]);
	//}

	//std::this_thread::sleep_for(std::chrono::milliseconds(50));

	//for (int i = 0; i < no_gpio_ports_in; i++)
	//{
	//	gpio_set_dir(GPIO_PORT_IDS_IN[i], 0);
	//	gpio_set_edge(GPIO_PORT_IDS_IN[i], "none");
	//}

	// for system shut down
	gpio_export(GPIO_PORT_IDS_IN);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	gpio_set_dir(GPIO_PORT_IDS_IN, 0);
	gpio_set_edge(GPIO_PORT_IDS_IN, "none");

}

/**
Toremove the initlizations
*/
void deint_gpio()
{
	// output ports
	for(int i=0; i < no_gpio_ports_out; i++)
	{
		gpio_set_value(GPIO_PORT_IDS_OUT[i], 0); // turn off
		gpio_unexport(GPIO_PORT_IDS_OUT[i]);
	}

	//// input ports
	/*
	int no_gpio_ports_in = sizeof(GPIO_PORT_IDS_IN) / sizeof(GPIO_PORT_IDS_IN[0]);
	for (int i = 0; i < no_gpio_ports_in; i++)
	{
		gpio_unexport(GPIO_PORT_IDS_IN[i]);
	}
	*/

	gpio_unexport(GPIO_PORT_IDS_IN);
	
}


/**
Get input from gpio and shutdown if high
*/

void check_if_shutdown() {
	unsigned int port_input = 0;
	gpio_get_value(GPIO_PORT_IDS_IN, &port_input);

	int no_iterations = 1000;

	while(port_input)
	{
	gpio_get_value(GPIO_PORT_IDS_IN, &port_input);
	no_iterations--;

	if(no_iterations == 0)
		break;
	}

	if(port_input)
	{
	std::cout << "Shutting down" << std::endl;

	// indicate using leds
	
	for (int i = 0; i < 3; i++){
	_detection_area.detection_status = 2; // need to activate the leds
	send_signal_to_gpio_ports();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	_detection_area.detection_status = 1; // need to activate the leds
	send_signal_to_gpio_ports();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	system("shutdown -P now");
	exit(-1);
	}
}

