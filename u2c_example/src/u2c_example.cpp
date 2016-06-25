/*
 * u2c_example.cpp
 *
 *  Created on: 2015. 12. 18.
 *      Author: arun
 */

#include <stdio.h>
#include <u2c.h>
#include <u2c_log.h>
#include <error/err_msg.h>

int _kbhit()
{
	struct timeval tv = {0,};

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0,&fds);

	return select(1, &fds, NULL, NULL, &tv);
}

int
main (	int argc,
		char* argv[])
{
	try
	{
		u2c::U2C device;
		u2c::U2C_Log log;

		log.open ("./log.txt", u2c::U2C_Log::OPEN_WRITE);

		u2c::CanFrame frame;
		u2c::CanFrame frame1;

		frame.mExt = 0;
		frame.mId = 0x123;
		frame.mLen = 0x8;
		frame.mData[0] = 0x30;
		frame.mData[1] = 0x31;
		frame.mData[2] = 0x32;
		frame.mData[3] = 0x33;
		frame.mData[4] = 0x34;
		frame.mData[5] = 0x35;
		frame.mData[6] = 0x36;
		frame.mData[7] = 0x37;

		log.write (frame);

		log.close();

		log.open("./log.txt", u2c::U2C_Log::OPEN_WRITE);

		//log.read(frame1);

		//log.close();

		device.open (0);

		device.setSpeed (5, 2, 1, 18);
		device.setFilter (1, 0, 0, 0, 0);
		device.setMode (1);

		while( !_kbhit() )
		{
			u2c::data_vector_t data;

			device.readData (data);

			for(size_t cnt = 0;cnt < data.size ();++cnt)
			{
				log.write(data[cnt]);
			}
			usleep (10);
		}

		device.setMode (0);
		device.close ();

		log.close ();
	}
	catch( error::ErrMsg* err )
	{
		printf ("Error(%d) : %s\n", err->getErrCode (), err->getErrString ());

		error::ErrMsg::destroyErrMsg (err);
	}

	return 0;
}

