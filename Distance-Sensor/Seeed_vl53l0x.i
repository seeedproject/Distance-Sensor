%module Seeed_vl53l0x
%{
#include "./src/Seeed_vl53l0x.h"
#include "./src/i2crw.h"
#include "./src/vl53l0x_api.h"
#include "./src/vl53l0x_api_calibration.h"
#include "./src/vl53l0x_api_core.h"
#include "./src/vl53l0x_api_ranging.h"
#include "./src/vl53l0x_api_strings.h"
#include "./src/vl53l0x_def.h"
#include "./src/vl53l0x_device.h"
#include "./src/vl53l0x_i2c_platform.h"
#include "./src/vl53l0x_interrupt_threshold_settings.h"
#include "./src/vl53l0x_platform.h"
#include "./src/vl53l0x_platform_log.h"
#include "./src/vl53l0x_tuning.h"
#include "./src/vl53l0x_types.h"
%}
%include "./src/Seeed_vl53l0x.h"
%include "./src/vl53l0x_api.h"
%include "./src/vl53l0x_platform.h"
%include "./src/required_version.h"
%include "./src/vl53l0x_def.h"