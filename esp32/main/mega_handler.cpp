
#include <Arduino.h>
#include <Wire.h>
#include <Bluepad32.h>

#include "mega_handler.h"


#define MAP_BUTTON(INPUT, OUTPUT, I_MASK, O_MASK) (OUTPUT |= ( INPUT & I_MASK ) ? O_MASK : 0)

#define GBA_OUT_A      (0x010)
#define GBA_OUT_B      (0x020)
#define GBA_OUT_UP     (0x001)
#define GBA_OUT_DOWN   (0x002)
#define GBA_OUT_LEFT   (0x004)
#define GBA_OUT_RIGHT  (0x008)
#define GBA_OUT_L      (0x040)
#define GBA_OUT_R      (0x080)
#define GBA_OUT_START  (0x100)
#define GBA_OUT_SELECT (0x200)

GamepadPtr Mega_Handler_Class::controller;


void Mega_Handler_Class::onConnectedGamepad(GamepadPtr gp) 
{
  controller = gp;
  Serial.println("CALLBACK: Controller is connected!");
}

void Mega_Handler_Class::onDisconnectedGamepad(GamepadPtr gp) 
{
  Serial.println("CALLBACK: Controller is disconnected!");
  controller = nullptr;
}

void Mega_Handler_Class::update_controller()
{
    uint16_t newOutputs = 0U;
    if (controller && controller->isConnected()) 
    {
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_UP, GBA_OUT_UP);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_DOWN, GBA_OUT_DOWN);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_LEFT, GBA_OUT_LEFT);
        MAP_BUTTON(controller->dpad(), newOutputs, DPAD_RIGHT, GBA_OUT_RIGHT);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_A, GBA_OUT_A);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_A, GBA_OUT_B);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_L, GBA_OUT_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_L, GBA_OUT_L);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_TRIGGER_R, GBA_OUT_R);
        MAP_BUTTON(controller->buttons(), newOutputs, BUTTON_SHOULDER_R, GBA_OUT_R);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_HOME, GBA_OUT_START);
        MAP_BUTTON(controller->miscButtons(), newOutputs, MISC_BUTTON_BACK, GBA_OUT_SELECT);
    }

    Serial.println(newOutputs);
}


void Mega_Handler_Class::init()
{
  String fv = BP32.firmwareVersion();
  Serial.print("Bluepad32 Firmware: ");
  Serial.println(fv);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
}

void Mega_Handler_Class::update()
{
    update_controller();
}

Mega_Handler_Class Mega_Handler;