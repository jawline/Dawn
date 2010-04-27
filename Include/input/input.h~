#ifndef _INPUT_DEF_H_
#define _INPUT_DEF_H_
#include <common.h>
#include "devices.h"

/*
 The purpose of this file is to provide a abstraction between hardware drivers relating to input and the kernel while avoiding a reduction in functionality of input devices
 The manager is designed to know absolutely nothing about the device it is processing data from, and only routes it to the things that are listening
*/

//Callback when a input listener is triggered
typedef void (*input_listener_callback)(uint32 device, uint32 main, void* additional);

//Input listeners in a linked list
struct input_listener_structure {
	uint32 m_deviceid; //If -1 responds to all input devices
	input_listener_callback m_callback;
	struct input_listener_structure* m_next;
};

typedef struct input_listener_structure input_listener_t;

//Export - Initialize the input manager (Null everything)
void initialize_input();

//Export - Send a input message to the manager from a device driver (Or emulating a input message)
//Arguments: Device - Options defined in devices.h Main - The main data from the input event additional - Additional data that needs to be sent about the input event
void send_input_message(uint32 device, uint32 main, void* additional);

//Export - Register a input listener
void register_input_listener(uint32 device /* If -1 respond to all */, input_listener_callback cb);

#endif //_INPUT_DEF_H_
