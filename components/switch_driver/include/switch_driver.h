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

#ifndef __MDF_SWITCH_H__
#define __MDF_SWITCH_H__

#include "mdf_common.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief The mode of the five-color switch
 */
enum switch_mode {
    MODE_NONE                = 0,
    MODE_OFF                 = 1,
    MODE_ON                  = 2,
};

/**
 * @brief The channel of the five-color switch
 */
#if (defined CONFIG_SWITCH_CHANNEL_4)
	
	enum switch_channel {
		CHANNEL_ID_SWITCH_0 = 0,
		CHANNEL_ID_SWITCH_1 = 1,
		CHANNEL_ID_SWITCH_2 = 2,
		CHANNEL_ID_SWITCH_3 = 3,
		CHANNEL_NUM_MAX
	};

		
#elif (defined CONFIG_SWITCH_CHANNEL_3)
	enum switch_channel {
		CHANNEL_ID_SWITCH_0 = 0,
		CHANNEL_ID_SWITCH_1 = 1,
		CHANNEL_ID_SWITCH_2 = 2,
		CHANNEL_NUM_MAX
	};

	
#elif (defined CONFIG_SWITCH_CHANNEL_2)
	enum switch_channel {
		CHANNEL_ID_SWITCH_0 = 0,
		CHANNEL_ID_SWITCH_1 = 1,
		CHANNEL_NUM_MAX
	};

	
#elif (defined CONFIG_SWITCH_CHANNEL_1)
	enum switch_channel {
		CHANNEL_ID_SWITCH_0 = 0,
		CHANNEL_NUM_MAX
	};

#endif



/**
 * @brief Light driven configuration
 */
#if 0
typedef struct {
	gpio_num_t gpio_switch_0;	  /**< Switch corresponds to GPIO */
    uint32_t blink_period_ms; /**< Period of flashing switchs */
} switch_driver_config_t;
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
mdf_err_t switch_driver_init(void);

/**
 * @brief  Light deinitialize
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t switch_driver_deinit();


/**
 * @brief Set the fade time of the switch
 *
 * @param  fade_period_ms  The time from the current color to the next color
 * @param  blink_period_ms Light flashing frequency
 *
 * @return
 *      - MDF_OK
 *      - MDF_FAIL
 */
mdf_err_t switch_driver_config(uint32_t blink_period_ms);

/**@{*/
/**
 * @brief  Set the mode of the switch
 *
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t switch_driver_set_mode(uint8_t mode);

/**@{*/
/**
 * @brief  Set the status of the switch
 *
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t switch_driver_set_switch(int channel, bool status);

/**@}*/

/**@{*/
/**
 * @brief  Set the status of the switch
 */
bool switch_driver_get_switch(int channel);
uint8_t switch_driver_get_mode();
/**@}*/

/**@{*/
/**
 * @brief  Used to indicate the operating mode, such as configuring the network mode, upgrading mode
 *
 * @note   The state of the switch is not saved in nvs
 *
 * @return
 *      - MDF_OK
 *      - MDF_ERR_INVALID_ARG
 */
mdf_err_t switch_driver_blink_start(uint8_t channel);
mdf_err_t switch_driver_blink_stop();
/**@}*/


/**@}*/

#ifdef __cplusplus
}
#endif

#endif/**< __MDF_SWITCH_H__ */
