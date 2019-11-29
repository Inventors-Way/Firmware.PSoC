#ifndef SERVICES_OS_SYSTEM_H
#define SERVICES_OS_SYSTEM_H

#include <stdint.h>    
#include <os/Message.h>

/** \addtogroup System
 *  @{
 */

#define MANUFACTURER_ID       1
#define DEVICE_TYPE           255
#define MAJOR_REVISION        0
#define MINOR_REVISION        1
#define PATCH_REVISION        0
#define ENGINEERING_REVISION  1

#define DEVICE_NAME           "Firmware.PSoC"
#define MANUFACTORER_NAME     "Inventors' Way ApS"

enum HandlerID
{
    HID_SERIAL_HANDLER        = 0,
    HID_EOL
};

struct Task
{
    enum HandlerID id;
    void* vself;
    void (*initialize)(void* vself);
    void (*run)(void* vself);
    void (*process)(void* vself, const enum MessageID  id, const uint32_t data);

    struct Task* next;
};

/**
 * \brief Initialization of the system
 */
void System_Initialize(void);

/**
 * \brief 
 * \details
 */
void System_Run(void);
    
/**
 * \brief Printf for debugging purposes.
 * \details
 *
 * \param[in] format string to print and format instructions for subsequent parameters to the function
 */
void System_Printf(const char* format, ...);

/** @}*/

#endif    