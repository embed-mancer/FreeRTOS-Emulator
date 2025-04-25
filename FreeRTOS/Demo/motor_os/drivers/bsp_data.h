/**
 * @file bsp_data.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-13-56
 * @author Nemausa
 *
 */
#ifndef BSP_DATA_H_
#define BSP_DATA_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool bsp_can_init(const char* device, uint32_t baudrate);
size_t bsp_can_read(uint8_t* buf, size_t buf_size);
void bsp_can_write(uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif  // BSP_DATA_H_
