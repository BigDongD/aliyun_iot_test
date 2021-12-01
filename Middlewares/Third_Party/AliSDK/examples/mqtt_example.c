#include "dev_sign_api.h"
#include "mqtt_api.h"
#include "cmsis_os.h"
#include "led_task.h"
#include "beep_task.h"

#define LED_PARAM_STATUS "Status"
#define LED_PARAM_COLORARR "ColorArr"

char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};

void *HAL_Malloc(uint32_t size);
void HAL_Free(void *ptr);
void HAL_Printf(const char *fmt, ...);
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
uint64_t HAL_UptimeMs(void);
int HAL_Snprintf(char *str, const int len, const char *fmt, ...);
uintptr_t HAL_TCP_Establish(const char *host, uint16_t port);

#define EXAMPLE_TRACE(fmt, ...)  \
    do { \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__); \
        HAL_Printf("%s", "\r\n"); \
    } while(0)

extern osMutexId_t ledCtrlMutex;
extern osMutexId_t beepCtrlMutex;

/* get jason item's value */
int32_t core_json_value(const char *input, uint32_t input_len, const char *key, uint32_t key_len, char **value,
                        uint32_t *value_len)
{
    uint32_t idx = 0;

    for (idx = 0; idx < input_len; idx++) {
        if (idx + key_len >= input_len) {
            return -1;
        }
        if ((memcmp(&input[idx], key, key_len) == 0) &&
            ((idx > 0) && (input[idx - 1] == '"')) &&
            ((idx + key_len < input_len) && (input[idx + key_len] == '"'))) {
            idx += key_len;
            /* shortest ":x, or ":x} or ":x] */
            if ((idx + 2 >= input_len) ||
                (input[idx + 1] != ':')) {
                return -1;
            }
            idx += 2;
            if (input[idx] == '"') {
                *value = (char *)&input[++idx];
                for (; idx < input_len; idx++) {
                    if ((input[idx] == '"')) {
                        *value_len = (uint32_t)(idx - (*value - input));
                        return 0;
                    }
                }
            } else if (input[idx] == '{' || input[idx] == '[') {
                char start = input[idx];
                char end = (start == '{') ? ('}') : (']');
                uint8_t count = 0;
                *value = (char *)&input[idx];
                for (; idx < input_len; idx++) {
                    if ((input[idx] == start)) {
                        count++;
                    } else if ((input[idx] == end)) {
                        if (--count == 0) {
                            *value_len = (uint32_t)(idx - (*value - input) + 1);
                            return 0;
                        }
                    }
                }
            } else {
                *value = (char *)&input[idx];
                for (; idx < input_len; idx++) {
                    if ((input[idx] == ',' || input[idx] == ']' || input[idx] == '}')) {
                        *value_len = (uint32_t)(idx - (*value - input));
                        return 0;
                    }
                }
            }
        }
    }

    return -1;
}

/* string convert to uint */
int32_t core_str2uint(char *input, uint8_t input_len, uint32_t *output)
{
    uint8_t index = 0;
    uint32_t temp = 0;

    for (index = 0; index < input_len; index++) {
        if (input[index] < '0' || input[index] > '9') {
            return -1;
        }
        temp = temp * 10 + input[index] - '0';
    }
    *output = temp;

    return 0;
}

void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;
    char *temp = NULL;
	char *value = NULL;
    uint32_t value_len = 0;
    switch (msg->event_type) {
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            /* print topic name and topic message */
            EXAMPLE_TRACE("Message Arrived:");
            EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
		    temp = strrchr(topic_info->ptopic, '/');
            EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
            EXAMPLE_TRACE("\n");
		    if (memcmp(temp + 1, "set", strlen("set")) == 0) {
				if (core_json_value(topic_info->payload, topic_info->payload_len, LED_PARAM_STATUS, strlen(LED_PARAM_STATUS), &value,
                        &value_len) == 0) {
					core_str2uint(value, value_len, (uint32_t*)&g_LedInfo.ledStatus);
				}
				if (core_json_value(topic_info->payload, topic_info->payload_len, LED_PARAM_COLORARR, strlen(LED_PARAM_COLORARR), &value,
                        &value_len) == 0) {
					core_str2uint(value, value_len, (uint32_t*)&g_LedInfo.ledColor);
				}
			}
            break;
        default:
            break;
    }
}

int example_subscribe(void *handle)
{
    int res = 0;
    const char *fmt = "/%s/%s/user/get";
    char topic[128] = {0};
    int topic_len = 0;

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
    if (res < 0) {
        EXAMPLE_TRACE("subscribe failed");
        return -1;
    }

	/* Subscribe module set property*/
	fmt = "/sys/%s/%s/thing/service/property/set";
    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;

    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
    if (res < 0) {
        EXAMPLE_TRACE("subscribe failed");
        return -1;
    }

    return 0;
}

int example_publish(void *handle)
{
    int             res = 0;
    const char     *fmt = "/%s/%s/user/get";
    char            topic[128] = {0};
    int             topic_len = 0;
    char            payload[256] = {0};
	int             payload_len = 0;
    static float    humi = 15.0f;
	static float    temp = 20.0f;

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);
	payload_len = strlen("{\"Message\":Hello World!}");
    memcpy(payload, "{\"Message\":Hello World!}", payload_len);
    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, payload_len);
    if (res < 0) {
        EXAMPLE_TRACE("publish failed, res = %d", res);
        return -1;
    }

	/* report temp-hum sensor data */
	fmt = "/sys/%s/%s/thing/event/property/post/";
	topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);
	/* payload vsnprintf */
	fmt = "{\"sys\":{\"ack\":0},\"params\":{\"TempHumSensor:Humidity\":%f,\"TempHumSensor:Temperature\":%f},\"method\":\"thing.event.property.post\"}";
	payload_len = HAL_Snprintf(payload, sizeof(payload), fmt, humi++, temp++);
    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, payload_len);
    if (res < 0) {
        EXAMPLE_TRACE("publish failed, res = %d", res);
        return -1;
    }
	if (humi>100.0f) {
		humi = 0;
	}
	if (temp>100.0f) {
		temp = 0;
	}
    return 0;
}

void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
}

/*
 *  NOTE: About demo topic of /${productKey}/${deviceName}/user/get
 *
 *  The demo device has been configured in IoT console (https://iot.console.aliyun.com)
 *  so that its /${productKey}/${deviceName}/user/get can both be subscribed and published
 *
 *  We design this to completely demonstrate publish & subscribe process, in this way
 *  MQTT client can receive original packet sent by itself
 *
 *  For new devices created by yourself, pub/sub privilege also requires being granted
 *  to its /${productKey}/${deviceName}/user/get for successfully running whole example
 */

void MqttTask(void *argument)
{
    void                   *pclient = NULL;
    int                     res = 0;
    int                     loop_cnt = 0;
    iotx_mqtt_param_t       mqtt_params;

    HAL_GetProductKey(DEMO_PRODUCT_KEY);
    HAL_GetDeviceName(DEMO_DEVICE_NAME);
    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

    EXAMPLE_TRACE("mqtt example");

    /* Initialize MQTT parameter */
    /*
     * Note:
     *
     * If you did NOT set value for members of mqtt_params, SDK will use their default values
     * If you wish to customize some parameter, just un-comment value assigning expressions below
     *
     **/
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    /**
     *
     *  MQTT connect hostname string
     *
     *  MQTT server's hostname can be customized here
     *
     *  default value is ${productKey}.iot-as-mqtt.cn-shanghai.aliyuncs.com
     */
    mqtt_params.host = "something.iot-as-mqtt.cn-shanghai.aliyuncs.com";

    /**
     *
     *  MQTT connect port number
     *
     *  TCP/TLS port which can be 443 or 1883 or 80 or etc, you can customize it here
     *
     *  default value is 1883 in TCP case, and 443 in TLS case
     */
    mqtt_params.port = 1883;

    /**
     *
     * MQTT request timeout interval
     *
     * MQTT message request timeout for waiting ACK in MQTT Protocol
     *
     * default value is 2000ms.
     */
    mqtt_params.request_timeout_ms = 2000;

    /**
     *
     * MQTT clean session flag
     *
     * If CleanSession is set to 0, the Server MUST resume communications with the Client based on state from
     * the current Session (as identified by the Client identifier).
     *
     * If CleanSession is set to 1, the Client and Server MUST discard any previous Session and Start a new one.
     *
     * default value is 0.
     */
    mqtt_params.clean_session = 0;

    /**
     *
     * MQTT keepAlive interval
     *
     * KeepAlive is the maximum time interval that is permitted to elapse between the point at which
     * the Client finishes transmitting one Control Packet and the point it starts sending the next.
     *
     * default value is 60000.
     */
    mqtt_params.keepalive_interval_ms = 60000;

    /**
     *
     * MQTT write buffer size
     *
     * Write buffer is allocated to place upstream MQTT messages, MQTT client will be limitted
     * to send packet no longer than this to Cloud
     *
     * default value is 1024.
     *
     */
    mqtt_params.write_buf_size = 1024;

    /**
     *
     * MQTT read buffer size
     *
     * Write buffer is allocated to place downstream MQTT messages, MQTT client will be limitted
     * to recv packet no longer than this from Cloud
     *
     * default value is 1024.
     *
     */
    mqtt_params.read_buf_size = 1024;


    /**
     *
     * MQTT event callback function
     *
     * Event callback function will be called by SDK when it want to notify user what is happening inside itself
     *
     * default value is NULL, which means PUB/SUB event won't be exposed.
     *
     */
	osDelay(4000);
    mqtt_params.handle_event.h_fp = example_event_handle;

    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) {
        EXAMPLE_TRACE("MQTT construct failed");
    }

    res = example_subscribe(pclient);
    if (res < 0) {
        IOT_MQTT_Destroy(&pclient);
    }

    while (1) {
        if (0 == loop_cnt % 20) {
            example_publish(pclient);
        }

        IOT_MQTT_Yield(pclient, 200);
		osDelay(200);
        loop_cnt += 1;
    }
}

