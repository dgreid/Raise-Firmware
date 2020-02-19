// -*- mode: c++ -*-
/* Raise-Firmware -- Factory firmware for the Dygma Raise
 * Copyright (C) 2019, 2020  DygmaLab, SE.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BUILD_INFORMATION
#define BUILD_INFORMATION "locally built"
#endif

#include "Kaleidoscope.h"

#include "Kaleidoscope-MouseKeys.h"
#include "Kaleidoscope-LEDControl.h"
#include "Kaleidoscope-PersistentLEDMode.h"
#include "Kaleidoscope-FocusSerial.h"
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"
#include "Kaleidoscope-IdleLEDs.h"
#include "Kaleidoscope-Colormap.h"
#include "Kaleidoscope-LED-Palette-Theme.h"
#include "Kaleidoscope-LEDEffect-Rainbow.h"
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-TapDance.h"
#include "Kaleidoscope-DynamicTapDance.h"
#include "Kaleidoscope-DynamicMacros.h"
#include "Kaleidoscope-MagicCombo.h"
#include "Kaleidoscope-USB-Quirks.h"
#include "Kaleidoscope-LayerFocus.h"
#include "RaiseIdleLEDs.h"
#include "kaleidoscope/device/dygma/raise/Focus.h"
#include "kaleidoscope/device/dygma/raise/SideFlash.h"

#include "Kaleidoscope-OneShot.h"
#include "Kaleidoscope-Qukeys.h"
#include "Kaleidoscope-Escape-OneShot.h"

#include "LED-CapsLockLight.h"
#include "EEPROMPadding.h"

#include "EEPROMUpgrade.h"

#include "attiny_firmware.h"

enum { QWERTY, SYMBOL, NUMPAD, FUNCTION, _LAYER_MAX }; // layers

/* This comment temporarily turns off astyle's indent enforcement so we can make
 * the keymaps actually resemble the physical key layout better
 */
// *INDENT-OFF*

KEYMAPS(
[QWERTY] = KEYMAP_STACKED
(
    Key_Backtick    ,Key_1         ,Key_2       ,Key_3                ,Key_4     ,Key_5 ,Key_6
   ,Key_Tab         ,Key_Q         ,Key_W       ,Key_E                ,Key_R     ,Key_T
   ,Key_Escape      ,Key_A         ,Key_S       ,Key_D                ,Key_F     ,Key_G
   ,Key_LeftShift   ,Key_Backslash ,Key_Z       ,Key_X                ,Key_C     ,Key_V ,Key_B
   ,Key_LeftControl ,Key_LeftGui   ,Key_LeftAlt ,Key_LeftAlt          ,Key_LeftControl
                                                ,ShiftToLayer(NUMPAD) ,ShiftToLayer(SYMBOL)

   ,Key_7               ,Key_8           ,Key_9        ,Key_0        ,Key_Minus         ,Key_Equals       ,Key_Backspace
   ,Key_Y               ,Key_U           ,Key_I        ,Key_O        ,Key_P             ,Key_LeftBracket  ,Key_RightBracket ,Key_Enter
   ,Key_H               ,Key_J           ,Key_K        ,Key_L        ,Key_Semicolon     ,Key_Quote        ,Key_Backslash
   ,Key_N               ,Key_M           ,Key_Comma    ,Key_Period   ,Key_Slash         ,Key_RightShift
   ,Key_Space           ,Key_RightShift  ,Key_RightAlt ,Key_RightGui ,Key_LEDEffectNext ,Key_RightControl
   ,Key_Backspace       ,Key_Delete
),

[SYMBOL] = KEYMAP_STACKED
(
    Key_Escape      ,Key_F1            ,Key_F2             ,Key_F3                   ,Key_F4 ,Key_F5 ,Key_F6
   ,Key_Tab         ,Key_LeftBracket   ,Key_RightBracket   ,LSHIFT(Key_LeftBracket)  ,LSHIFT(Key_RightBracket)    ,XXX
   ,Key_Escape      ,LSHIFT(Key_Comma) ,LSHIFT(Key_Period) ,LSHIFT(Key_9)            ,LSHIFT(Key_0)               ,XXX
   ,Key_LeftShift   ,Key_Backslash     ,XXX                ,XXX                      ,LSHIFT(Key_Backtick)        ,LSHIFT(Key_4)    ,XXX
   ,Key_LeftControl ,Key_LeftGui       ,Key_LeftAlt        ,Key_Space                ,Key_Space
                                                           ,Key_Backspace            ,Key_Enter

   ,Key_F7                 ,Key_F8              ,Key_F9                  ,Key_F10                 ,Key_F11            ,Key_F12         ,Key_Backspace
   ,LSHIFT(Key_1)          ,LSHIFT(Key_Equals)  ,LSHIFT(Key_3)           ,LSHIFT(Key_Backslash)   ,LSHIFT(Key_5)      ,XXX             ,XXX            ,Key_Enter
   ,Key_Backslash          ,LSHIFT(Key_Minus)   ,LSHIFT(Key_8)           ,Key_Minus               ,Key_Equals         ,Key_Backtick    ,Key_Backslash
   ,LSHIFT(Key_6)          ,Key_Slash           ,LSHIFT(Key_Semicolon)   ,LSHIFT(Key_7)           ,LSHIFT(Key_Slash)  ,LSHIFT(Key_2)
   ,ShiftToLayer(FUNCTION) ,Key_Space           ,Key_LeftArrow           ,Key_DownArrow           ,Key_RightArrow     ,Key_RightControl
   ,ShiftToLayer(NUMPAD)   ,Key_Delete
),

[NUMPAD] = KEYMAP_STACKED
(
    Key_Escape      ,Key_F1        ,Key_F2        ,Key_F3         ,Key_F4 ,Key_F5 ,Key_F6
   ,Key_Tab         ,XXX           ,Key_UpArrow   ,Key_UpArrow    ,XXX    ,XXX
   ,Key_Escape      ,Key_LeftArrow ,Key_LeftArrow ,Key_DownArrow  ,Key_RightArrow ,XXX
   ,Key_LeftShift   ,Key_Backslash ,XXX           ,XXX            ,XXX    ,XXX    ,XXX
   ,Key_LeftControl ,Key_LeftGui   ,Key_LeftAlt   ,Key_Space      ,Key_Space
                                                  ,Key_Backspace  ,Key_Enter

   ,Key_F7              ,Key_F8    ,Key_F9        ,Key_F10       ,Key_F11            ,Key_F12 ,Key_Backspace
   ,Key_KeypadSubtract  ,Key_7     ,Key_8         ,Key_9         ,Key_KeypadDivide   ,XXX     ,XXX, Key_Enter
   ,Key_Comma           ,Key_4     ,Key_5         ,Key_6         ,Key_KeypadMultiply ,XXX     ,Key_Backslash
   ,Key_Period          ,Key_1     ,Key_2         ,Key_3         ,Key_0              ,Key_RightShift
   ,Key_0               ,Key_Space ,Key_0         ,Key_DownArrow ,Key_RightArrow     ,Key_RightControl
   ,Key_Space           ,Key_Delete
),

[FUNCTION] = KEYMAP_STACKED
(
    Key_Escape      ,Key_F1        ,Key_F2        ,Key_F3         ,Key_F4 ,Key_F5 ,Key_F6
   ,Key_Tab         ,XXX           ,Key_UpArrow   ,Key_UpArrow    ,XXX    ,XXX
   ,Key_Escape      ,Key_LeftArrow ,Key_LeftArrow ,Key_DownArrow  ,Key_RightArrow ,XXX
   ,Key_LeftShift   ,Key_Backslash ,XXX           ,XXX            ,XXX    ,XXX    ,XXX
   ,Key_LeftControl ,Key_LeftGui   ,Key_LeftAlt   ,Key_Space      ,Key_Space
                                                  ,Key_Backspace  ,Key_Enter

   ,Key_F7              ,Key_F8    ,Key_F9        ,Key_F10       ,Key_F11            ,Key_F12 ,Key_Backspace
   ,Key_KeypadSubtract  ,Key_F7    ,Key_F8        ,Key_F9        ,Key_KeypadDivide   ,XXX     ,XXX, Key_Enter
   ,Key_Comma           ,Key_F4    ,Key_F5        ,Key_F6        ,Key_KeypadMultiply ,XXX     ,Key_Backslash
   ,Key_Period          ,Key_F1    ,Key_F2        ,Key_F3        ,Key_0              ,Key_RightShift
   ,Key_0               ,Key_Space ,Key_0         ,Key_DownArrow ,Key_RightArrow     ,Key_RightControl
   ,Key_Space           ,Key_Delete
 )
);


/* Re-enable astyle's indent enforcement */
// *INDENT-ON*

kaleidoscope::device::dygma::raise::SideFlash<ATTinyFirmware> SideFlash;

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr,
                    uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  DynamicTapDance.dance(tap_dance_index, key_addr, tap_count, tap_dance_action);
}

enum {
  COMBO_TOGGLE_NKRO_MODE
};

static uint32_t protocol_toggle_start = 0;

static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
  protocol_toggle_start = Kaleidoscope.millisAtCycleStart();
}

static void protocolBreathe() {
  if (Kaleidoscope.hasTimeExpired(protocol_toggle_start, uint16_t(10000))) {
    protocol_toggle_start = 0;
  }
  if (protocol_toggle_start == 0)
    return;

  uint8_t hue = 120;
  if (Kaleidoscope.hid().keyboard().getProtocol() == HID_BOOT_PROTOCOL) {
    hue = 0;
  }

  cRGB color = breath_compute(hue);
  ::LEDControl.setCrgbAt(KeyAddr(4, 0), color);
  ::LEDControl.setCrgbAt(KeyAddr(3, 0), color);
  ::LEDControl.setCrgbAt(KeyAddr(4, 2), color);
  ::LEDControl.setCrgbAt(KeyAddr(0, 6), color);
  ::LEDControl.syncLeds();
}

USE_MAGIC_COMBOS(
    {
      .action = toggleKeyboardProtocol,
      // Left Ctrl + Left Shift + Left Alt + 6
      .keys = { R4C0, R3C0, R4C2, R0C6 }
    }
);

kaleidoscope::plugin::EEPROMPadding JointPadding(8);

KALEIDOSCOPE_INIT_PLUGINS(
  USBQuirks,
  MagicCombo,
  RaiseIdleLEDs,
  EEPROMSettings,
  EEPROMKeymap,
  FocusSettingsCommand,
  FocusEEPROMCommand,
  LEDCapsLockLight,
  LEDControl,
  PersistentLEDMode,
  FocusLEDCommand,
  LEDPaletteTheme,
  JointPadding,
  ColormapEffect,
  LEDRainbowWaveEffect, LEDRainbowEffect, StalkerEffect,
  PersistentIdleLEDs,
  RaiseFocus,
  TapDance,
  DynamicTapDance,
  DynamicMacros,
  SideFlash,
  Focus,
  MouseKeys,
  OneShot,
  EscapeOneShot,
  Qukeys,
  LayerFocus,
  EEPROMUpgrade
);

void setup() {
  Kaleidoscope.serialPort().begin(9600);
  Kaleidoscope.setup();

  // Reserve space in the keyboard's EEPROM for the keymaps
  EEPROMKeymap.setup(10);

  // Reserve space for the number of Colormap layers we will use
  ColormapEffect.max_layers(10);
  LEDRainbowEffect.brightness(255);
  LEDRainbowWaveEffect.brightness(255);
  StalkerEffect.variant = STALKER(BlazingTrail);

  DynamicTapDance.setup(0, 1024);
  DynamicMacros.reserve_storage(2048);

  EEPROMUpgrade.reserveStorage();
  EEPROMUpgrade.upgrade();
}

void loop() {
  Kaleidoscope.loop();
  protocolBreathe();
}
