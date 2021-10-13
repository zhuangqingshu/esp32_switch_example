// Copyright 2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __MDF_LIGHT_H__
#define __MDF_LIGHT_H__

#include "mdf_common.h"

#ifdef  __cplusplus
extern "C" {
#endif


/**
 * @brief  Light initialize
 *
 * @param  config [description]
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t light_driver_led_init();

/**
 * @brief  Light deinitialize
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t light_driver_led_deinit();

/**
 * @brief Set the color of the light
 *
 * @param  red   Red led brightness, the range is: 0 ~ 255
 * @param  green Green led brightness, the range is: 0 ~ 255
 * @param  blue  Blue led brightness, the range is: 0 ~ 255
 *
 * @return
 *     - MDF_OK
 *     - MDF_FAIL
 */
mdf_err_t light_driver_led_set_rgb(uint8_t red, uint8_t green, uint8_t blue);

/**
 * [light_driver_led_blink_start description]
 *
 * @param  red   Red led brightness, the range is: 0 ~ 255
 * @param  green Green led brightness, the range is: 0 ~ 255
 * @param  blue  Blue led brightness, the range is: 0 ~ 255
 * @param  period_ms breath period (uint: ms)
 *
 * @return
 *     - MDF_OK
 *     - MDF_FAIL
 */
mdf_err_t light_driver_led_blink_start(uint8_t red, uint8_t green, uint8_t blue, uint32_t period_ms);

/**
 * @brief Stop the blink of led
 *
 * @return
 *     - MDF_OK
 *     - MDF_FAIL
 */
mdf_err_t light_driver_led_blink_stop();


/**@}*/

#ifdef __cplusplus
}
#endif

#endif/**< __MDF_LIGHT_H__ */
