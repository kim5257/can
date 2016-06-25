/*
 * u2c_log.cpp
 *
 *  Created on: 2015. 12. 21.
 *      Author: arun
 */

#include "u2c_log.h"
#include <error/err_msg.h>

namespace u2c
{

U2C_Log::U2C_Log (void)
		: mFile (NULL)
{
	init ();
}

U2C_Log::~U2C_Log (void)
{
	deinit ();
}

void
U2C_Log::init (void)
{

}

void
U2C_Log::deinit (void)
{
	close ();
}

void
U2C_Log::open (	const char fileName[],
				OpenMode mode)
{
	switch( mode )
	{
	case OPEN_READ:
		mFile = fopen (fileName, "r");
		break;
	case OPEN_WRITE:
		mFile = fopen (fileName, "w");
		break;
	}

	if ( mFile == NULL )
	{
		error::ErrMsg::createErrMsg ("파일 열기 실패");
	}
}

void
U2C_Log::close (void)
{
	if ( mFile != NULL )
	{
		fclose (mFile);
		mFile = NULL;
	}
}

void
U2C_Log::read (CanFrame& frame)
{
	const char* frameFormat = "%x %x %x ";

	fscanf (mFile, frameFormat, &frame.mExt, &frame.mId, &frame.mLen);

	for(int cnt = 0;cnt < frame.mLen;++cnt)
	{
		int tmp = 0;
		fscanf (mFile, "%x", &tmp);
		frame.mData[cnt] = (char)tmp;
	}
}

void
U2C_Log::write (const CanFrame& frame)
{
	const char* frameFormat[2] =
	{ "%x %03x %02x ", "%x %08x %02x " };
	char buf[CAN_FILE_FRAME_SIZE] =
	{ 0, };
	size_t idx = 0;

	idx = snprintf (buf,
					sizeof(buf),
					frameFormat[frame.mExt],
					frame.mExt,
					frame.mId,
					frame.mLen);

	for(int cnt = 0;cnt < frame.mLen;++cnt)
	{
		idx += snprintf (	&buf[idx],
							sizeof(buf) - idx,
							"%02x ",
							(unsigned char)frame.mData[cnt]);
	}

	fprintf (mFile, "%s\n", buf);
}

}
