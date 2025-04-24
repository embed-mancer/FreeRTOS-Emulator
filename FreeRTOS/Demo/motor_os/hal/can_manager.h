/**
 * @file can_manager.h
 *
 * @brief This message displayed in Doxygen Files index
 *
 * @ingroup PackageName
 * (note: this needs exactly one @defgroup somewhere)
 *
 * @date 2025-04-24-15-25
 * @author Nemausa
 *
 */
#ifndef CAN_MANAGER_H_
#define CAN_MANAGER_H_

#include "../middleware/parser/protocol_frame.h"

typedef void (*frame_handler_t)(const protocol_frame_t *frame);

/**
 * @brief 初始化 CAN 管理器，打开接口
 * @param ifname CAN 接口名称，如 "can0"
 */
void can_manager_init(const char *ifname);

/**
 * @brief 注册指定 ID 的回调
 */
void can_manager_register(uint32_t id, frame_handler_t handler);

/**
 * @brief 调度：读取一帧并分发给对应回调
 */
void can_manager_dispatch(const protocol_frame_t *msg);

#endif  // CAN_MANAGER_H_
