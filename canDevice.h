#ifndef __CANDEVICE_H__
#define __CANDEVICE_H__

typedef enum __CAN_DEVICE__ {
  CAN_NONE = 0,
  CAN_NEEDLE,
  CAN_SPEED,
  CAN_GEAR,
  CAN_TEMP,
  CAN_FUEL,
  CAN_ENG5TC,
  CAN_PHONECALL,
  CAN_BAR,
  CAN_VSC,
  CAN_WIFI,
  CAN_BLINK
} CAN_DEVICE;

typedef enum __TURN_LIGHT__ {
  TURN_LIGHT_OFF = 0,
  TURN_LIGHT_RIGHT,
  TURN_LIGHT_LEFT
}TURN_LIGHT;

typedef enum __HEAD_LIGHT__ {
  HEAD_LIGHT_OFF = 0,
  HEAD_LIGHT_ON
}HEAD_LIGHT;

#endif // end of __CANDEICE_h__

// end of file
