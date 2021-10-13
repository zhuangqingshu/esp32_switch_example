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
#include "mlink.h"

#include "switch_driver.h"
#include "light_handle.h"
#include "DriverPM.h"

static const char *TAG = "light_handle";


mdf_err_t light_show_layer(mlink_handle_data_t *handle_data)
{
    switch (esp_mesh_get_layer()) {
        default:
            break;
    }

    return MDF_OK;
}

mdf_err_t light_get_tsf_time(mlink_handle_data_t *handle_data)
{
    char tsf_time_str[16] = {0x0};

    sprintf(tsf_time_str, "%lld", esp_mesh_get_tsf_time());
    mlink_json_pack(&handle_data->resp_data, "tsf_time", tsf_time_str);

    handle_data->resp_size = strlen(handle_data->resp_data);

    return MDF_OK;
}

mdf_err_t mlink_set_value(uint16_t cid, void *arg)
{
    int value = *((int *)arg);

    switch (cid) {
        case DEVICE_CID_SWITCH_0:
            switch (value) {
                case LIGHT_STATUS_ON:
                case LIGHT_STATUS_OFF:
                    switch_driver_set_switch(CHANNEL_ID_SWITCH_0 ,value);
                    break;

                case LIGHT_STATUS_SWITCH:
                    switch_driver_set_switch(CHANNEL_ID_SWITCH_0, !switch_driver_get_switch(CHANNEL_ID_SWITCH_0));
                    break;

                default:
                    break;
            }

            break;
#if (defined CONFIG_SWITCH_CHANNEL_4) || (defined CONFIG_SWITCH_CHANNEL_3) || (defined CONFIG_SWITCH_CHANNEL_2)
		case DEVICE_CID_SWITCH_1:
			switch (value) {
				case LIGHT_STATUS_ON:
				case LIGHT_STATUS_OFF:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_1 ,value);
					break;
		
				case LIGHT_STATUS_SWITCH:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_1, !switch_driver_get_switch(CHANNEL_ID_SWITCH_1));
					break;
		
				default:
					break;
			}
		
			break;
#endif
#if (defined CONFIG_SWITCH_CHANNEL_4) || (defined CONFIG_SWITCH_CHANNEL_3)

		case DEVICE_CID_SWITCH_2:
			switch (value) {
				case LIGHT_STATUS_ON:
				case LIGHT_STATUS_OFF:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_2 ,value);
					break;
		
				case LIGHT_STATUS_SWITCH:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_2, !switch_driver_get_switch(CHANNEL_ID_SWITCH_2));
					break;
		
				default:
					break;
			}
		
			break;
#endif
#if (defined CONFIG_SWITCH_CHANNEL_4)

		case DEVICE_CID_SWITCH_3:
			switch (value) {
				case LIGHT_STATUS_ON:
				case LIGHT_STATUS_OFF:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_3 ,value);
					break;
		
				case LIGHT_STATUS_SWITCH:
					switch_driver_set_switch(CHANNEL_ID_SWITCH_3, !switch_driver_get_switch(CHANNEL_ID_SWITCH_3));
					break;
		
				default:
					break;
			}
		
			break;
#endif
        case DEVICE_CID_MODE:
			switch_driver_set_mode(value);
            break;

        default:
            MDF_LOGE("No support cid: %d", cid);
            return MDF_FAIL;
    }

    MDF_LOGD("cid: %d, value: %d", cid, value);

    return MDF_OK;
}

mdf_err_t mlink_get_value(uint16_t cid, void *arg)
{
    int *value = (int *)arg;

    switch (cid) {
        case DEVICE_CID_SWITCH_0:
            *value = switch_driver_get_switch(CHANNEL_ID_SWITCH_0);
            break;
#if (defined CONFIG_SWITCH_CHANNEL_4) || (defined CONFIG_SWITCH_CHANNEL_3) || (defined CONFIG_SWITCH_CHANNEL_2)
        case DEVICE_CID_SWITCH_1:
            *value = switch_driver_get_switch(CHANNEL_ID_SWITCH_1);
            break;
#endif
#if (defined CONFIG_SWITCH_CHANNEL_4) || (defined CONFIG_SWITCH_CHANNEL_3)
        case DEVICE_CID_SWITCH_2:
            *value = switch_driver_get_switch(CHANNEL_ID_SWITCH_2);
            break;
#endif
#if (defined CONFIG_SWITCH_CHANNEL_4)
		case DEVICE_CID_SWITCH_3:
			*value = switch_driver_get_switch(CHANNEL_ID_SWITCH_3);
			break;
#endif		
        case DEVICE_CID_MODE:
			*value = switch_driver_get_mode();
            break;
        case DEVICE_CID_POWER:
			*value = (int)(DriverPM_getActivePower()*100);
            break;
        case DEVICE_CID_ENERGY:
			*value = (int)(DriverPM_getEnergy()*100);
            break;
        case DEVICE_CID_VOLTAGE:
			*value = (int)(DriverPM_getVoltage()*100);
            break;
        case DEVICE_CID_CURRENT:
			*value = (int)(DriverPM_getCurrent()*100);
            break;
        case DEVICE_CID_TIME_OF_DAY:{
			time_t now;
			struct tm timeinfo;
			time(&now);
			localtime_r(&now, &timeinfo);

			*value = timeinfo.tm_hour*3600+timeinfo.tm_min*60+timeinfo.tm_sec;

        	}
            break;

        default:
            MDF_LOGE("No support cid: %d", cid);
            return MDF_FAIL;
    }

    MDF_LOGV("cid: %d, value: %d", cid, *value);

    return MDF_OK;
}
