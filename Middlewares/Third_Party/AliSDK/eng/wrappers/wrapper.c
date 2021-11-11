/**
 * NOTE:
 *
 * HAL_TCP_xxx API reference implementation: wrappers/os/ubuntu/HAL_TCP_linux.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

#include "infra_types.h"
#include "infra_defs.h"
#include "infra_compat.h"
#include "wrappers_defs.h"

/* usart include */
#include "usart.h"

/* socket include */
#include "socket.h"
#include "lwip/netdb.h"

char g_DeviceName[] = {"stm32_led_test"};
char g_DeviceSecret[] = {"dca0ad80ed0327a7c173ca4785bae014"};
char g_ProductKey[] = {"a10xBYWjgoc"};

/* function */
void HAL_Printf(const char *fmt, ...);

/* USER CODE BEGIN EM */
#ifndef PRINT_LEVEL
#define PRINT_LEVEL   DEBUG_LEVEL
#endif

#define ERROR_LEVEL   1
#define WARN_LEVEL 	  2
#define INFO_LEVEL 	  3
#define DEBUG_LEVEL   4

#if (PRINT_LEVEL >= DEBUG_LEVEL)
#define DEBUG_LOG(fmt, args...) \
				HAL_Printf("\r\n[D] (%s,%d) "fmt"\r\n", __func__, __LINE__, ##args)
#else 
#define DEBUG_LOG(fmt, args...)
#endif

#if (PRINT_LEVEL >= INFO_LEVEL)
#define INFO_LOG(fmt, args...) \
				HAL_Printf("\r\n[I] (%s,%d) "fmt"\r\n", __func__, __LINE__, ##args)
#else 
#define INFO_LOG(fmt, args...) 
#endif

#if (PRINT_LEVEL >= WARN_LEVEL)
#define WARN_LOG(fmt, args...) \
				HAL_Printf("\r\n[W] (%s,%d) "fmt"\r\n", __func__, __LINE__, ##args)
#else 
#define WARN_LOG(fmt, args...) 
#endif

#if (PRINT_LEVEL >= ERROR_LEVEL)
#define ERROR_LOG(fmt, args...) \
				HAL_Printf("\r\n[E] (%s,%d) "fmt"\r\n", __func__, __LINE__, ##args)
#else 
#define ERROR_LOG(fmt, args...) 
#endif
/* USER CODE END EM */

/**
 *
 * 函数 HAL_Free() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_Free() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_Free(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_Free, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Deallocate memory block
 *
 * @param[in] ptr @n Pointer to a memory block previously allocated with platform_malloc.
 * @return None.
 * @see None.
 * @note None.
 */
extern void HAL_Free(void *ptr)
{
	vPortFree(ptr);
}


/**
 *
 * 函数 HAL_GetDeviceName() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_GetDeviceName() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_GetDeviceName(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_GetDeviceName, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Get device name from user's system persistent storage
 *
 * @param [ou] device_name: array to store device name, max length is IOTX_DEVICE_NAME_LEN
 * @return the actual length of device name
 */
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN])
{
    int len = strlen(g_DeviceName);
    memset(device_name, 0x0, IOTX_DEVICE_NAME_LEN + 1);

    strncpy(device_name, g_DeviceName, len);

    return strlen(device_name);
}


/**
 *
 * 函数 HAL_GetDeviceSecret() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_GetDeviceSecret() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_GetDeviceSecret(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_GetDeviceSecret, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Get device secret from user's system persistent storage
 *
 * @param [ou] device_secret: array to store device secret, max length is IOTX_DEVICE_SECRET_LEN
 * @return the actual length of device secret
 */
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN])
{
    int len = strlen(g_DeviceSecret);
    memset(device_secret, 0x0, IOTX_DEVICE_SECRET_LEN + 1);

    strncpy(device_secret, g_DeviceSecret, len);

    return len;
}


/**
 *
 * 函数 HAL_GetFirmwareVersion() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_GetFirmwareVersion() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_GetFirmwareVersion(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_GetFirmwareVersion, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Get firmware version
 *
 * @param [ou] version: array to store firmware version, max length is IOTX_FIRMWARE_VER_LEN
 * @return the actual length of firmware version
 */
int HAL_GetFirmwareVersion(char *version)
{
	return (int)1;
}


/**
 *
 * 函数 HAL_GetProductKey() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_GetProductKey() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_GetProductKey(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_GetProductKey, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Get product key from user's system persistent storage
 *
 * @param [ou] product_key: array to store product key, max length is IOTX_PRODUCT_KEY_LEN
 * @return  the actual length of product key
 */
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN])
{
    int len = strlen(g_ProductKey);
    memset(product_key, 0x0, IOTX_PRODUCT_KEY_LEN + 1);

    strncpy(product_key, g_ProductKey, len);

    return len;
}


/**
 *
 * 函数 HAL_GetProductSecret() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_GetProductSecret() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_GetProductSecret(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_GetProductSecret, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
int HAL_GetProductSecret(char *product_secret)
{
	return (int)1;
}


/**
 *
 * 函数 HAL_Malloc() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_Malloc() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_Malloc(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_Malloc, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
 *
 * @param [in] size @n specify block size in bytes.
 * @return A pointer to the beginning of the block.
 * @see None.
 * @note Block value is indeterminate.
 */
extern void *HAL_Malloc(uint32_t size)
{
	return pvPortMalloc(size);
}


/**
 *
 * 函数 HAL_MutexCreate() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_MutexCreate() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_MutexCreate(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_MutexCreate, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Create a mutex.
 *
 * @retval NULL : Initialize mutex failed.
 * @retval NOT_NULL : The mutex handle.
 * @see None.
 * @note None.
 */
extern void *HAL_MutexCreate(void)
{
	QueueHandle_t sem;

    sem = xSemaphoreCreateMutex();
    if (0 == sem) {
        return NULL;
    }

    return sem;
}


/**
 *
 * 函数 HAL_MutexDestroy() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_MutexDestroy() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_MutexDestroy(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_MutexDestroy, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Destroy the specified mutex object, it will release related resource.
 *
 * @param [in] mutex @n The specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
extern void HAL_MutexDestroy(void *mutex)
{
    QueueHandle_t sem;
    if (mutex == NULL) {
        return;
    }
    sem = (QueueHandle_t)mutex;
    vSemaphoreDelete(sem);
}


/**
 *
 * 函数 HAL_MutexLock() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_MutexLock() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_MutexLock(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_MutexLock, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Waits until the specified mutex is in the signaled state.
 *
 * @param [in] mutex @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
extern void HAL_MutexLock(void *mutex)
{
	BaseType_t ret;
    QueueHandle_t sem;
    if (mutex == NULL) {
        return;
    }

    sem = (QueueHandle_t)mutex;
    ret = xSemaphoreTake(sem, portMAX_DELAY);  //0xffffffffUL
    while (pdPASS != ret) {
        ret = xSemaphoreTake(sem, portMAX_DELAY);
    }
}


/**
 *
 * 函数 HAL_MutexUnlock() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_MutexUnlock() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_MutexUnlock(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_MutexUnlock, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Releases ownership of the specified mutex object..
 *
 * @param [in] mutex @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
extern void HAL_MutexUnlock(void *mutex)
{
    QueueHandle_t sem;
    if (mutex == NULL) {
        return;
    }
    sem = (QueueHandle_t)mutex;
    (void)xSemaphoreGive(sem);
}


/**
 *
 * 函数 HAL_Printf() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_Printf() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_Printf(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_Printf, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Writes formatted data to stream.
 *
 * @param [in] fmt: @n String that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_Printf(const char *fmt, ...)
{
	int32_t txCnt;
	
    va_list args;
    va_start(args, fmt);
	txCnt = vsprintf((char*)g_DebUsartTxBuff, fmt, args);
	va_end(args);

	//Start Send
	HAL_UART_Transmit(&huart1, g_DebUsartTxBuff, (uint16_t)txCnt, USART_TX_TIMEOUT);
    
}


/**
 *
 * 函数 HAL_SleepMs() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_SleepMs() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_SleepMs(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_SleepMs, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Sleep thread itself.
 *
 * @param [in] ms @n the time interval for which execution is to be suspended, in milliseconds.
 * @return None.
 * @see None.
 * @note None.
 */
extern void HAL_SleepMs(uint32_t ms)
{
	vTaskDelay(ms);
}


/**
 *
 * 函数 HAL_Snprintf() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_Snprintf() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_Snprintf(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_Snprintf, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Writes formatted data to string.
 *
 * @param [out] str: @n String that holds written text.
 * @param [in] len: @n Maximum length of character will be written
 * @param [in] fmt: @n Format that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return bytes of character successfully written into string.
 * @see None.
 * @note None.
 */
int HAL_Snprintf(char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);
    return rc;
}

int HAL_Vsnprintf(char *str, const int len, const char *format, va_list ap)
{
	return vsnprintf(str, len, format, ap);
}

/**
 * @brief Destroy the specific TCP connection.
 *
 * @param [in] fd: @n Specify the TCP connection by handle.
 *
 * @return The result of destroy TCP connection.
 * @retval < 0 : Fail.
 * @retval   0 : Success.
 */
int HAL_TCP_Destroy(uintptr_t fd)
{
	int rc;

	/* Shutdown both send and receive operations. */
	rc = shutdown((int) fd, 2);
	if (0 != rc) {
		printf("shutdown error");
		return -1;
	}

	rc = closesocket((int) fd);
	if (0 != rc) {
		printf("closesocket error");
		return -1;
	}

	return 0;
}


/**
 * @brief Establish a TCP connection.
 *
 * @param [in] host: @n Specify the hostname(IP) of the TCP server
 * @param [in] port: @n Specify the TCP port of TCP server
 *
 * @return The handle of TCP connection.
   @retval   0 : Fail.
   @retval > 0 : Success, the value is handle of this TCP connection.
 */
uintptr_t HAL_TCP_Establish(const char *host, uint16_t port)
{
	uintptr_t sockfd;
	struct hostent *hp = NULL;
	struct sockaddr_in addrServer;

	DEBUG_LOG("host : %s, port : %u", host, port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);   /* socket */
	if (sockfd == (uintptr_t)-1) {
	  ERROR_LOG("Socket error");
      return sockfd;
	} else {
		hp = lwip_gethostbyname(host);

		memset(&addrServer, 0, sizeof(addrServer));
		memcpy(&(addrServer.sin_addr), hp->h_addr, hp->h_length);

		addrServer.sin_family = AF_INET;
		addrServer.sin_port = htons((unsigned short)1883);

		DEBUG_LOG("connecting to %s", host);
		if (connect(sockfd, (struct sockaddr *)&addrServer, sizeof(struct sockaddr))) {
			ERROR_LOG("connect failed!\n");
			return (uintptr_t)-1;
		}

		DEBUG_LOG("connect successfully!\n");

		return sockfd;
	}
}


/**
 * @brief Read data from the specific TCP connection with timeout parameter.
 *        The API will return immediately if 'len' be received from the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a TCP connection.
 * @param [out] buf @n A pointer to a buffer to receive incoming data.
 * @param [out] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval       -2 : TCP connection error occur.
 * @retval       -1 : TCP connection be closed by remote server.
 * @retval        0 : No any data be received in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be received in 'timeout_ms' timeout period.

 * @see None.
 */
int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
	int ret, err_code;
	uint32_t len_recv;
	uint64_t t_end, t_left;
	fd_set sets;
	struct timeval timeout;

	t_end = HAL_GetTick() + timeout_ms;
	len_recv = 0;
	err_code = 0;

	do {
		t_left = t_end - HAL_GetTick();

		FD_ZERO(&sets);
		FD_SET(fd, &sets);

		timeout.tv_sec = t_left / 1000;
		timeout.tv_usec = (t_left % 1000) * 1000;

		ret = select(fd+1, &sets, NULL, NULL, &timeout);
		if (ret > 0) {
			ret = recv(fd, buf + len_recv, len - len_recv, 0);
			if (ret > 0) {
				len_recv += ret;
			} else if (0 == ret) {
				DEBUG_LOG("connection is closed");
				err_code = -1;
				break;
			} else {
				DEBUG_LOG("recv fail");
				err_code = -2;
				break;
			}
		} else if (0 == ret) {
			break;
		} else {
			DEBUG_LOG("select-read fail");
			err_code = -2;
			break;
		}
	} while ((len_recv < len) && (t_end - HAL_GetTick() > 0));

	/* priority to return data bytes if any data be received from TCP connection. */
	/* It will get error code on next calling */
	return (0 != len_recv) ? len_recv : err_code;
}


/**
 * @brief Write data into the specific TCP connection.
 *        The API will return immediately if 'len' be written into the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a connection.
 * @param [in] buf @n A pointer to a buffer containing the data to be transmitted.
 * @param [in] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval      < 0 : TCP connection error occur..
 * @retval        0 : No any data be write into the TCP connection in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be written in 'timeout_ms' timeout period.

 * @see None.
 */
int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
{
	int ret, err_code;
	uint32_t len_sent;
	uint64_t t_end, t_left;
	fd_set sets;

	t_end = HAL_GetTick() + timeout_ms;
	len_sent = 0;
	err_code = 0;
	ret = 1; /* send one time if timeout_ms is value 0 */

	do {
		t_left = t_end - HAL_GetTick();

		if (0 != t_left) {
			struct timeval timeout;

			FD_ZERO(&sets);
			FD_SET(fd, &sets);

			timeout.tv_sec = t_left / 1000;
			timeout.tv_usec = (t_left % 1000) * 1000;

			ret = select(fd+1, NULL, &sets, NULL, &timeout);
			if (ret > 0) {
				if (0 == FD_ISSET(fd, &sets)) {
					/* TODO */
					/* not this fd, continue; */
					/* If timeout in next loop, it will not sent any data */
					ret = 0;
					continue;
				}
			} else if (0 == ret) {
				DEBUG_LOG("select-write timeout");
				break;
			} else {
				DEBUG_LOG("select-write fail");
				err_code = -1;
				break;
			}
		}

		if (ret > 0) {
			ret = send(fd, buf + len_sent, len - len_sent, 0);
			if (ret > 0) {
				len_sent += ret;
			} else if (0 == ret) {
				DEBUG_LOG("No any data be sent");
			} else {
				/* socket error occur */
				DEBUG_LOG("send fail");
				err_code = -1;
				break;
			}
		}
	} while ((len_sent < len) && (t_end - HAL_GetTick() > 0));

	/* Priority to return data bytes if any data be sent to TCP connection. */
	/* It will get error code on next calling */
	return (0 == len_sent) ? err_code : len_sent;
}
//int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
//{
//	int ret, err_code;
//	uint32_t len_sent;
//	uint64_t t_end, t_left;
//	fd_set sets;

//	t_end = HAL_GetTick() + timeout_ms;
//	len_sent = 0;
//	err_code = 0;
//	ret = 1; /* send one time if timeout_ms is value 0 */

//	do {
//		t_left = t_end - HAL_GetTick();

//		ret = send(fd, buf + len_sent, len - len_sent, 0);
//		if (ret > 0) {
//			len_sent += ret;
//		} else if (0 == ret) {
//			DEBUG_LOG("No any data be sent");
//		} else {
//			/* socket error occur */
//			DEBUG_LOG("send fail");
//			err_code = -1;
//			break;
//		}
//	} while ((len_sent < len) && (t_end - HAL_GetTick() > 0));

//	/* Priority to return data bytes if any data be sent to TCP connection. */
//	/* It will get error code on next calling */
//	return (0 == len_sent) ? err_code : len_sent;
//}

/**
 *
 * 函数 HAL_UptimeMs() 需要SDK的使用者针对SDK将运行的硬件平台填充实现, 供SDK调用
 * ---
 * Interface of HAL_UptimeMs() requires to be implemented by user of SDK, according to target device platform
 *
 * 如果需要参考如何实现函数 HAL_UptimeMs(), 可以查阅SDK移植到 Ubuntu Linux 上时的示例代码
 * ---
 * If you need guidance about how to implement HAL_UptimeMs, you can check its reference implementation for Ubuntu platform
 *
 * https://code.aliyun.com/linkkit/c-sdk/blob/v3.0.1/wrappers/os/ubuntu/HAL_OS_linux.c
 *
 *
 * 注意! HAL_XXX() 系列的函数虽然有阿里提供的对应参考实现, 但不建议您不做任何修改/检视的应用于您的商用设备!
 * 
 * 注意! 参考示例实现仅用于解释各个 HAL_XXX() 系列函数的语义!
 * 
 */
/**
 * @brief Retrieves the number of milliseconds that have elapsed since the system was boot.
 *
 * @return the number of milliseconds.
 * @see None.
 * @note None.
 */
uint64_t HAL_UptimeMs(void)
{
	return (uint64_t)xTaskGetTickCount();
}
