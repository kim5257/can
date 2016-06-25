/*
 * u2c_log_creater.cpp
 *
 *  Created on: 2016. 1. 4.
 *      Author: arun
 */

#include <stdio.h>
#include <string>
#include <u2c.h>
#include <u2c_log.h>
#include <error/err_msg.h>

#define	PREFIX		(0)

int
main (	int argc,
		char* argv[])
{
	try
	{
		char name[255] = {0,};
		u2c::U2C_Log log;

		//for(int cnt2=0x0;cnt2<=0xF;++cnt2)
		for(int cnt2=0x0;cnt2<=0x0;++cnt2)
		{
			sprintf(name, "./log5xx.txt", cnt2);
			//sprintf(name, "./log5%Xx.txt", cnt2);

			log.open (name, u2c::U2C_Log::OPEN_WRITE);

			u2c::CanFrame frame;

			frame.mExt = 0;
			frame.mId = 0x0;
			frame.mLen = 0x8;
			for(int off=0;off<8;++off)
			{
				frame.mData[off] = 0xFF;
			}

			//for(int cnt=(cnt2 << 4);cnt<=((cnt2 << 4) | 0xF);++cnt)
			for(int cnt=0x0;cnt<=0xFF;++cnt)
			{
				frame.mId = 0x500 + cnt;
				log.write(frame);
			}

			log.close();
		}

	}
	catch( error::ErrMsg* err )
	{
		printf ("Error(%d) : %s\n", err->getErrCode (), err->getErrString ());

		error::ErrMsg::destroyErrMsg (err);
	}

	return 0;
}
