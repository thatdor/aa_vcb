/*
 * crc.h
 *
 *  Created on: Mar 27, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_COMMON_UTILITY_CRC_H_
#define SYSTEM_COMMON_UTILITY_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif


uint16_t crc16_checksum(uint8_t* buf, uint32_t len);


#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_COMMON_UTILITY_CRC_H_ */
