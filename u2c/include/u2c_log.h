/*
 * u2c_log.h
 *
 *  Created on: 2015. 12. 21.
 *      Author: arun
 */

#ifndef INCLUDE_U2C_LOG_H_
#define INCLUDE_U2C_LOG_H_

#include <u2c_typedef.h>
#include <stdio.h>

namespace u2c
{

class U2C_Log
{
public:
	enum OpenMode
	{
		OPEN_WRITE,
		OPEN_READ,
	};
private:
	FILE* mFile;
public:
	U2C_Log (void);
	~U2C_Log (void);
private:
	void
	init (void);
	void
	deinit (void);
public:
	void
	open (	const char fileName[],
			OpenMode mode);
	void
	close (void);
public:
	void
	read (CanFrame& frame);
	void
	write (const CanFrame& frame);
};

}

#endif /* INCLUDE_U2C_LOG_H_ */
