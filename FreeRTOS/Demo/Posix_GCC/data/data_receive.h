/******************************************************************************
* File:             data_receive.h
*
* Author:           Nemausa
* Created:          08/13/24 
* Description:      
*****************************************************************************/

#ifndef DATA_RECEIVE_H
#define DATA_RECEIVE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

void DataTaskInit();
void DataTaskAdd(uint8_t *msg);
void DataTaskProcess(uint8_t *msg);

#ifdef __cplusplus
}
#endif


#endif /* ifndef DATA_RECEIVE_H */

