/*
 * u2c_typedef.h
 *
 *  Created on: 2015. 12. 18.
 *      Author: arun
 */

#ifndef INCLUDE_U2C_TYPEDEF_H_
#define INCLUDE_U2C_TYPEDEF_H_

#include <vector>

#define	U2C_VENDER		(0x403)
#define	U2C_PRODUCT	(0x6014)

#define	U2C_CMD_SET_SPEED_LEN		(11)
#define	U2C_CMD_SET_FILTER_LEN		(14)
#define	U2C_CMD_SET_EXFILTER_LEN		(24)
#define	U2C_CMD_SET_MODE_LEN			(5)

#define	U2C_CMD_HEAD			";"
#define	U2C_CMD_TAIL			"\r"

#define	U2C_CMD_TX_DATA		"0"
#define	U2C_CMD_SET_SPEED		"11"
#define	U2C_CMD_SET_FILTER 	"12"
#define	U2C_CMD_SET_MODE 		"13"

#define	U2C_MODE_CONFING		"0"
#define	U2C_MODE_NORMAL		"1"

#define	U2C_ID_TYPE_NORMAL	'0'
#define	U2C_ID_TYPE_EXT		'1'

#define	CAN_DATA_LEN	(8)
#define	CAN_RAW_DATA_LEN	(CAN_DATA_LEN*2)

// CAN 통신 최대속도 = 1 Mbit/s
// 데이터 읽기 주기 = 1 msec
// 데이터 버퍼 크기 = 1 msec 당 읽는 데이터 크기 * 2
// 1 msec 당 읽는 데이터 크기 = 125 KByte/s / 1000 = 125 Byte/ms
// 데이터 버퍼 크기 = 125 Byte/ms * 2 = 256 Byte
#define	CAN_DATA_FIFO_SIZE	(32*1024*1024)

#define	CAN_FILE_FRAME_SIZE	(38)

namespace u2c
{

struct CanHdr
{
	char mHead;
	char mCmd;
	char mExt;
};

struct CanRawFrame
{
	CanHdr	mHdr;

	char mId[3];
	char mDlc[2];
	char mData[CAN_RAW_DATA_LEN + 1];
};

struct CanExtRawFrame
{
	CanHdr	mHdr;

	char mId[8];
	char mDlc[2];
	char mData[CAN_RAW_DATA_LEN + 1];
};

struct CanFrame
{
	int mExt;
	int mId;
	int mLen;
	char mData[CAN_DATA_LEN];
};

typedef std::vector<CanFrame> data_vector_t;

}

#endif /* INCLUDE_U2C_TYPEDEF_H_ */
