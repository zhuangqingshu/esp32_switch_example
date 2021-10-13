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

#include "mdf_common.h"
#include "switch_driver.h"
#include "mdf_info_store.h"
#include "driver/gpio.h"

#define LED_ON             (0)  //low level to on
#define LED_OFF            (1)

#define RELAY_GPIO_1_ON  (12)
#define RELAY_GPIO_1_OFF (16)
#define RELAY_GPIO_2_ON  (13)
#define RELAY_GPIO_2_OFF (17)
#define RELAY_GPIO_3_ON  (21)
#define RELAY_GPIO_3_OFF (18)
#define RELAY_GPIO_4_ON  (22)
#define RELAY_GPIO_4_OFF (19)

/**
 * @brief The state of the five-color switch
 */
typedef struct {
    uint8_t mode;
    uint8_t on[CHANNEL_NUM_MAX];
	uint32_t blink_period_ms;
} switch_status_t;


#define LIGHT_STATUS_STORE_KEY   "switch_status"

static const char *TAG               = "switch_driver";
static switch_status_t g_switch_status = {0};
#if (defined CONFIG_SWITCH_CHANNEL_4)

#define SWITCH_GPIO_0 12
#define SWITCH_GPIO_1 13
#define SWITCH_GPIO_2 21
#define SWITCH_GPIO_3 18

#define SWITCH_LED_GPIO_0 32
#define SWITCH_LED_GPIO_1 25
#define SWITCH_LED_GPIO_2 26
#define SWITCH_LED_GPIO_3 14

	static gpio_num_t gpio_num[CHANNEL_NUM_MAX] = {
		SWITCH_GPIO_0,
		SWITCH_GPIO_1,
		SWITCH_GPIO_2,
		SWITCH_GPIO_3
	};
	
	static gpio_num_t	g_button_led[CHANNEL_NUM_MAX] = {
		SWITCH_LED_GPIO_0,
		SWITCH_LED_GPIO_1,
		SWITCH_LED_GPIO_2,
		SWITCH_LED_GPIO_3
	};

	#define SWITCH_PIN_BIT_MASK (BIT64(SWITCH_GPIO_0) | BIT64(SWITCH_GPIO_1) | BIT64(SWITCH_GPIO_2) | BIT64(SWITCH_GPIO_3))
	#define SWITCH_LED_PIN_BIT_MASK (BIT64(SWITCH_LED_GPIO_0) | BIT64(SWITCH_LED_GPIO_1) | BIT64(SWITCH_LED_GPIO_2) | BIT64(SWITCH_LED_GPIO_3))
	
#elif (defined CONFIG_SWITCH_CHANNEL_3)

#define SWITCH_GPIO_0 12
#define SWITCH_GPIO_1 13
#define SWITCH_GPIO_2 21
	
#define SWITCH_LED_GPIO_0 32
#define SWITCH_LED_GPIO_1 33
#define SWITCH_LED_GPIO_2 25

	static gpio_num_t gpio_num[CHANNEL_NUM_MAX] = {
		SWITCH_GPIO_0,
		SWITCH_GPIO_1,
		SWITCH_GPIO_2,
	};

	static gpio_num_t	g_button_led[CHANNEL_NUM_MAX] = {
		SWITCH_LED_GPIO_0,
		SWITCH_LED_GPIO_1,
		SWITCH_LED_GPIO_2,
	};
	#define SWITCH_PIN_BIT_MASK (BIT64(SWITCH_GPIO_0) | BIT64(SWITCH_GPIO_1) | BIT64(SWITCH_GPIO_2) )
	#define SWITCH_LED_PIN_BIT_MASK (BIT64(SWITCH_LED_GPIO_0) | BIT64(SWITCH_LED_GPIO_1) | BIT64(SWITCH_LED_GPIO_2) )

#elif (defined CONFIG_SWITCH_CHANNEL_2)

#define SWITCH_GPIO_0 12
#define SWITCH_GPIO_1 13

#define SWITCH_LED_GPIO_0 32
#define SWITCH_LED_GPIO_1 25

	static gpio_num_t gpio_num[CHANNEL_NUM_MAX] = {
		SWITCH_GPIO_0,
		SWITCH_GPIO_1,
	};

	static gpio_num_t	g_button_led[CHANNEL_NUM_MAX] = {
		SWITCH_LED_GPIO_0,
		SWITCH_LED_GPIO_1,
	};
	
	#define SWITCH_PIN_BIT_MASK (BIT64(SWITCH_GPIO_0) | BIT64(SWITCH_GPIO_1) )
	#define SWITCH_LED_PIN_BIT_MASK (BIT64(SWITCH_LED_GPIO_0) | BIT64(SWITCH_LED_GPIO_1) )
	
#elif (defined CONFIG_SWITCH_CHANNEL_1)

#define SWITCH_GPIO_0 13
	
#define SWITCH_LED_GPIO_0 33

	static gpio_num_t gpio_num[CHANNEL_NUM_MAX] = {
		SWITCH_GPIO_0,
	};

	static gpio_num_t	g_button_led[CHANNEL_NUM_MAX] = {
		SWITCH_LED_GPIO_0,
	};
	#define SWITCH_PIN_BIT_MASK (BIT64(SWITCH_GPIO_0) )
	#define SWITCH_LED_PIN_BIT_MASK (BIT64(SWITCH_LED_GPIO_0) )	

#endif


mdf_err_t switch_driver_init(void)
{
	int ret = MDF_OK;
	int i;
/*
	// unuse gpio init to inpul no pull
	gpio_config_t unuse_io_conf;
	//disable interrupt
	unuse_io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	unuse_io_conf.mode = GPIO_MODE_INPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	unuse_io_conf.pin_bit_mask = BIT64(RELAY_GPIO_1_ON) | BIT64(RELAY_GPIO_1_OFF) | BIT64(RELAY_GPIO_2_ON) | BIT64(RELAY_GPIO_2_OFF) | BIT64(RELAY_GPIO_3_ON) | BIT64(RELAY_GPIO_3_OFF) | BIT64(RELAY_GPIO_4_ON) | BIT64(RELAY_GPIO_4_OFF);
	//disable pull-down mode
	unuse_io_conf.pull_down_en = 0;
	//disable pull-up mode
	unuse_io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&unuse_io_conf);	
	*/
    if (mdf_info_load(LIGHT_STATUS_STORE_KEY, &g_switch_status, sizeof(switch_status_t)) != MDF_OK) {
        memset(&g_switch_status, 0, sizeof(switch_status_t));
        g_switch_status.mode              = MODE_NONE;
		for(i=0; i<CHANNEL_NUM_MAX; i++) {
			g_switch_status.on[i]                = 0;
		}
        
		ret = mdf_info_save(LIGHT_STATUS_STORE_KEY, &g_switch_status, sizeof(switch_status_t));
		MDF_ERROR_CHECK(ret < 0, ESP_FAIL, "mdf_info_save, ret: %d", ret);
    }

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = SWITCH_PIN_BIT_MASK;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);


	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = SWITCH_LED_PIN_BIT_MASK;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

	// reload status
	for(i=0; i<CHANNEL_NUM_MAX; i++) {
		if (g_switch_status.on[i]) {
			gpio_set_level(gpio_num[i], 1);
			gpio_set_level(g_button_led[i], LED_ON);
		} else {
			gpio_set_level(gpio_num[i], 0);
			gpio_set_level(g_button_led[i], LED_OFF);
		}
	}

    return ret;
}

mdf_err_t switch_driver_deinit()
{
    mdf_err_t ret = MDF_OK;

    return ret;
}

mdf_err_t switch_driver_config(uint32_t blink_period_ms)
{
    g_switch_status.blink_period_ms = blink_period_ms;

    return MDF_OK;
}

mdf_err_t switch_driver_set_mode(uint8_t mode)
{
	esp_err_t ret	  = MDF_OK;
	int i;
	
	switch(mode) {
		case MODE_OFF:
			for(i=0; i<CHANNEL_NUM_MAX; i++) {
				switch_driver_set_switch(i, 0);
			}
			break;
		case MODE_ON:
			for(i=0; i<CHANNEL_NUM_MAX; i++) {
				switch_driver_set_switch(i, 1);
			}
			break;
		default :
			ret = MDF_ERR_INVALID_ARG;
	}
    return ret;
}

uint8_t switch_driver_get_mode()
{
    return g_switch_status.mode;
}
mdf_err_t switch_driver_set_switch(int channel, bool on)
{
    esp_err_t ret     = MDF_OK;

	if(channel >= CHANNEL_NUM_MAX)
		return MDF_ERR_INVALID_ARG;
	
    g_switch_status.on[channel] = on;

    if (g_switch_status.on[channel]) {
		gpio_set_level(gpio_num[channel], 1);
		gpio_set_level(g_button_led[channel], LED_ON);
    } else {
		gpio_set_level(gpio_num[channel], 0);
		gpio_set_level(g_button_led[channel], LED_OFF);
    }

    ret = mdf_info_save(LIGHT_STATUS_STORE_KEY, &g_switch_status, sizeof(switch_status_t));
    MDF_ERROR_CHECK(ret < 0, ESP_FAIL, "mdf_info_save, ret: %d", ret);

    return MDF_OK;
}

bool switch_driver_get_switch(int channel)
{
	if(channel >= CHANNEL_NUM_MAX)
		return false;

    return g_switch_status.on[channel];
}


