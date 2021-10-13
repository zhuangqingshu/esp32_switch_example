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
#include "button_driver.h"
#include "iot_button.h"
#include "driver/gpio.h"

//#define __TEST 1

#define MDF_EVENT_BUTTON_KEY_LONG_PRESS_SEC (5)
#define MDF_EVENT_BUTTON_KEY_LONG_LONG_PRESS_SEC (8)
#define MDF_EVENT_BUTTON_KEY_INVALID_SEC    (11)

#define BUTTON_ACTIVE_LEVEL (0)  //active level low

static const char *TAG                    = "button_driver";

#if (defined CONFIG_SWITCH_CHANNEL_4)
	
#define BUTTON_KEY_NUM                     (6)
#define BUTTON_GPIO_KEY0                   36
#define BUTTON_GPIO_KEY1                   34
#define BUTTON_GPIO_KEY2                   35
#define BUTTON_GPIO_KEY3                   5
#define BUTTON_GPIO_KEY_SET                23
#define BUTTON_GPIO_KEY_SET2               0

static uint32_t gpio_num[BUTTON_KEY_NUM] = {
	BUTTON_GPIO_KEY0,
	BUTTON_GPIO_KEY1,
	BUTTON_GPIO_KEY2,
	BUTTON_GPIO_KEY3,
	BUTTON_GPIO_KEY_SET,
	BUTTON_GPIO_KEY_SET2
};

#elif (defined CONFIG_SWITCH_CHANNEL_3)

#define BUTTON_KEY_NUM                     (5)
#define BUTTON_GPIO_KEY0                   36
#define BUTTON_GPIO_KEY1                   39
#define BUTTON_GPIO_KEY2                   34
#define BUTTON_GPIO_KEY_SET                23
#define BUTTON_GPIO_KEY_SET2               0

static uint32_t gpio_num[BUTTON_KEY_NUM] = {
	BUTTON_GPIO_KEY0,
	BUTTON_GPIO_KEY1,
	BUTTON_GPIO_KEY2,
	BUTTON_GPIO_KEY_SET,
	BUTTON_GPIO_KEY_SET2
};

#elif (defined CONFIG_SWITCH_CHANNEL_2)

#define BUTTON_KEY_NUM                     (4)
#define BUTTON_GPIO_KEY0                   36
#define BUTTON_GPIO_KEY1                   34
#define BUTTON_GPIO_KEY_SET                23
#define BUTTON_GPIO_KEY_SET2               0

static uint32_t gpio_num[BUTTON_KEY_NUM] = {
	BUTTON_GPIO_KEY0,
	BUTTON_GPIO_KEY1,
	BUTTON_GPIO_KEY_SET,
	BUTTON_GPIO_KEY_SET2
};

#elif (defined CONFIG_SWITCH_CHANNEL_1)

#define BUTTON_KEY_NUM                     (3)
#define BUTTON_GPIO_KEY0                   39
#define BUTTON_GPIO_KEY_SET                23
#define BUTTON_GPIO_KEY_SET2               0

static uint32_t gpio_num[BUTTON_KEY_NUM] = {
	BUTTON_GPIO_KEY0,
	BUTTON_GPIO_KEY_SET,
	BUTTON_GPIO_KEY_SET2
};

#endif

static button_handle_t btn_handle[BUTTON_KEY_NUM];

static void button_press_cb(void* arg)
{
	ESP_LOGI(TAG, "press cb(%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
	mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_PUSH, (void *)arg);
}

static void button_release_cb(void* arg)
{
	ESP_LOGI(TAG, "release cb(%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
	mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_RELEASE, (void *)arg);
}

static void button_tap_cb(void* arg)
{
    ESP_LOGI(TAG, "tap cb(%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_SHORT_PRESS, (void *)arg);
}

static void button_long_press_cb(void* arg)
{
    ESP_LOGI(TAG, "long press (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_LONG_PRESS_PUSH, (void *)arg);
}

static void button_long_release_cb(void* arg)
{
    ESP_LOGI(TAG, "long resease (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_LONG_PRESS_RELEASE, (void *)arg);
}

static void button_long_long_press_cb(void* arg)
{
    ESP_LOGI(TAG, "long long press (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_LONG_LONG_PRESS_PUSH, (void *)arg);
}

static void button_long_long_release_cb(void* arg)
{
    ESP_LOGI(TAG, "long long resease (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_LONG_LONG_PRESS_RELEASE, (void *)arg);
}

static void button_invalid_press_cb(void* arg)
{
    ESP_LOGI(TAG, "invalid press (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_LONG_PRESS_INVAILD, (void *)arg);
}

static void button_invalid_release_cb(void* arg)
{
    ESP_LOGI(TAG, "invalid resease (%d), heap: %d\n", (uint32_t)arg, esp_get_free_heap_size());
    mdf_event_loop_send(MDF_EVENT_BUTTON_KEY_NONE, (void *)arg);
}

mdf_err_t button_driver_init()
{
    mdf_err_t ret = MDF_OK;
	int i;
	
	iot_button_init();

	for ( i=0; i<BUTTON_KEY_NUM; i++ ) {
	    btn_handle[i] = iot_button_create((gpio_num_t)gpio_num[i], (button_active_t)BUTTON_ACTIVE_LEVEL);
		
    	iot_button_set_evt_cb(btn_handle[i], BUTTON_CB_PUSH, button_press_cb, (void *)i);
		iot_button_set_evt_cb(btn_handle[i], BUTTON_CB_RELEASE, button_release_cb, (void *)i);

		#ifdef __TEST
		iot_button_add_on_press_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_LONG_PRESS_SEC, button_long_press_cb, (void *)i);
		#endif
	}
	
	for ( i=BUTTON_KEY_NUM-2; i<BUTTON_KEY_NUM; i++ ) {
		iot_button_add_on_press_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_LONG_PRESS_SEC, button_long_press_cb, (void *)i);
		iot_button_add_on_release_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_LONG_PRESS_SEC, button_long_release_cb, (void *)i);
		iot_button_add_on_press_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_LONG_LONG_PRESS_SEC, button_long_long_press_cb, (void *)i);
		iot_button_add_on_release_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_LONG_LONG_PRESS_SEC, button_long_long_release_cb, (void *)i);
		
		iot_button_add_on_press_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_INVALID_SEC, button_invalid_press_cb, (void *)i);
		iot_button_add_on_release_cb(btn_handle[i], MDF_EVENT_BUTTON_KEY_INVALID_SEC, button_invalid_release_cb, (void *)i);
	}

    MDF_LOGI("Power hold");

    return MDF_OK;
}

mdf_err_t button_driver_deinit()
{
    MDF_LOGI("Power down");
	iot_button_deinit();
	int i;
	for ( i=0; i<BUTTON_KEY_NUM; i++ ) {
	    iot_button_delete(btn_handle[i]);
	}

    return MDF_OK;
}


