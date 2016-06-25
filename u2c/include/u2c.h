/*
 * u2c.h
 *
 *  Created on: 2015. 12. 18.
 *      Author: arun
 */

#ifndef INCLUDE_U2C_H_
#define INCLUDE_U2C_H_

#include <ftdi.h>
#include <structure/streamqueue.h>
#include <error/err_msg.h>
#include <u2c_typedef.h>

namespace u2c
{

class U2C
{
private:
	ftdi_context* mContext;
	ftdi_device_list* mList;
	uint32_t mListCnt;
	datastruct::StreamQueue mDataFifo;
	char* mReadBuf;
public:
	U2C (void);
	~U2C (void);
private:
	void
	init (void);
	void
	deinit (void);
	void
	send (	char buf[],
			size_t len);
	size_t
	recv (	char buf[],
			size_t len);
	void
	moveToHdr (void);
	size_t
	findTail (void);
	int
	readIdType (void);
	int
	readDataLen (int idType);
	int
	charToInt (	char val[],
				size_t len);
public:
	void
	open (uint32_t index);
	void
	close (void);
public:
	void
	setSpeed (	int bs1,
				int bs2,
				int swj,
				int prescaler);
	void
	setFilter (	int active,
				int index,
				int idType,
				int mask,
				int filter);
	void
	setMode (int mode);
	void
	readData (data_vector_t& data);
	void
	sendData (const CanFrame& data);
public:
	inline size_t
	getRecvFifoUsage (void);
};

size_t
U2C::getRecvFifoUsage (void)
{
	return (mDataFifo.getRemainSize() * 100) / mDataFifo.getQueueSize ();
}

}

#endif /* INCLUDE_U2C_H_ */
