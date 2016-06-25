/*
 * u2c.cpp
 *
 *  Created on: 2015. 12. 18.
 *      Author: arun
 */

#include "u2c.h"
#include "u2c_typedef.h"

namespace u2c
{

U2C::U2C (void)
		: mContext (NULL),
			mList (NULL),
			mListCnt (0),
			mDataFifo (CAN_DATA_FIFO_SIZE),
			mReadBuf (NULL)
{
	init ();
}

U2C::~U2C (void)
{
	deinit ();
}

void
U2C::init (void)
{
	int ret = 0;

	mContext = ftdi_new ();
	if ( mContext == NULL )
	{
		throw error::ErrMsg::createErrMsg ("FTDI context 생성 실패");
	}

	ret = ftdi_usb_find_all (mContext, &mList, U2C_VENDER, U2C_PRODUCT);
	if ( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg ("FTDI 장치 리스트 가져오기 실패", ret);
	}
	mListCnt = ret;

	mReadBuf = new char[CAN_DATA_FIFO_SIZE];
}

void
U2C::deinit (void)
{
	delete[] mReadBuf;

	if ( mList != NULL )
	{
		ftdi_list_free (&mList);
		mList = NULL;
	}

	if ( mContext != NULL )
	{
		ftdi_free (mContext);
		mContext = NULL;
	}
}

void
U2C::send (	char buf[],
			size_t len)
{
	int ret = 0;

	ret = ftdi_write_data (mContext, (unsigned char*)buf, len);

	if ( ret < 0 || ret != (int)len )
	{
		throw error::ErrMsg::createErrMsg ("장치 쓰기에 실패", ret);
	}
}

size_t
U2C::recv (	char buf[],
			size_t len)
{
	int ret = 0;

	ret = ftdi_read_data (mContext, (unsigned char*)buf, len);
	if ( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg ("장치 읽기에 실패", ret);
	}

	return ret;
}

void
U2C::moveToHdr (void)
{
	char hdrCmd[] = U2C_CMD_HEAD;
	size_t off = mDataFifo.findByte (hdrCmd[0]);

	if ( off == 0 )
	{
		mDataFifo.clear ();
	}
	else if ( 1 < off )
	{
		mDataFifo.flush (off - 1);
	}
}

size_t
U2C::findTail (void)
{
	char tailCmd[] = U2C_CMD_TAIL;
	size_t off = mDataFifo.findByte (tailCmd[0]);

	return off;
}

int
U2C::readIdType (void)
{
	CanHdr* hdr = (CanHdr*)mReadBuf;

	return hdr->mExt;
}

int
U2C::readDataLen (int idType)
{
	int ret = -1;

	switch( idType )
	{
	case U2C_ID_TYPE_NORMAL:
		{
			CanRawFrame* frame = (CanRawFrame*)mReadBuf;

			ret = charToInt (frame->mDlc, sizeof(frame->mDlc));
		}
		break;
	case U2C_ID_TYPE_EXT:
		{
			CanExtRawFrame* frame = (CanExtRawFrame*)mReadBuf;

			ret = charToInt (frame->mDlc, sizeof(frame->mDlc));
		}
		break;
	}

	return ret;
}

int
U2C::charToInt (char val[],
				size_t len)
{
	int ret = 0;

	for(size_t cnt = 0;cnt < len;++cnt)
	{
		ret <<= 4;
		if ( '0' <= val[cnt] && val[cnt] <= '9' )
		{
			ret |= val[cnt] - '0';
		}
		else if ( 'a' <= val[cnt] && val[cnt] <= 'f' )
		{
			ret |= val[cnt] - 'a' + 0xa;
		}
		else if ( 'A' <= val[cnt] && val[cnt] <= 'F' )
		{
			ret |= val[cnt] - 'A' + 0xa;
		}
	}

	return ret;
}

void
U2C::open (uint32_t index)
{
	int ret = 0;

	if ( mListCnt <= index )
	{
		throw error::ErrMsg::createErrMsg ("올바른 장치 번호가 아닙니다.");
	}

	ftdi_device_list* cur = mList;

	// 지정한 장치 번호까지 노드 커서 이동
	for(uint32_t cnt = 0;cnt < index;++index)
	{
		cur = cur->next;
	}

	ret = ftdi_usb_open_dev (mContext, cur->dev);
	if ( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg ("장치 열기에 실패", ret);
	}

	ret = ftdi_usb_reset(mContext);
	if( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg("장치 리셋 실패", ret);
	}

	ret = ftdi_usb_purge_buffers(mContext);
	if( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg("장치 버퍼 비우기 실패", ret);
	}
}

void
U2C::close (void)
{
	int ret = 0;

	ret = ftdi_usb_close (mContext);
	if ( ret < 0 )
	{
		throw error::ErrMsg::createErrMsg ("장치 닫기에 실패");
	}
}

void
U2C::setSpeed (	int bs1,
				int bs2,
				int swj,
				int prescaler)
{
	char buf[U2C_CMD_SET_SPEED_LEN + 1] =
	{ 0, };

	snprintf (buf, sizeof(buf),
	U2C_CMD_HEAD
	U2C_CMD_SET_SPEED
	"%x%x%x%04x"
	U2C_CMD_TAIL,
				bs1, bs2, swj, prescaler);

	send (buf, U2C_CMD_SET_SPEED_LEN);
}

void
U2C::setFilter (int active,
				int index,
				int idType,
				int mask,
				int filter)
{
	char buf[U2C_CMD_SET_EXFILTER_LEN + 1] =
	{ 0, };
	size_t frameLen =
			(idType == 0)? (U2C_CMD_SET_FILTER_LEN):(U2C_CMD_SET_EXFILTER_LEN);

	// ID 타입에 따라 필터와 마스크를 따로 지정
	if ( idType == 0 )
	{
		snprintf (buf, sizeof(buf),
		U2C_CMD_HEAD
		U2C_CMD_SET_FILTER
		"%x%02x%x%03x%03x"
		U2C_CMD_TAIL,
					active, index, idType, mask, filter);
	}
	else
	{
		snprintf (buf, sizeof(buf),
		U2C_CMD_HEAD
		U2C_CMD_SET_FILTER
		"%x%02x%x%08x%08x"
		U2C_CMD_TAIL,
					active, index, idType, mask, filter);
	}

	send (buf, frameLen);
}

void
U2C::setMode (int mode)
{
	char buf[U2C_CMD_SET_MODE_LEN + 1] =
	{ 0, };

	switch( mode )
	{
	case 0:
		snprintf (buf, sizeof(buf),
		U2C_CMD_HEAD
		U2C_CMD_SET_MODE
		"0"
		U2C_CMD_TAIL);
		break;
	case 1:
		snprintf (buf, sizeof(buf),
		U2C_CMD_HEAD
		U2C_CMD_SET_MODE
		"1"
		U2C_CMD_TAIL);
		break;
	}

	send (buf, U2C_CMD_SET_MODE_LEN);
}

void
U2C::readData (data_vector_t& data)
{
	int exitCnt = 64;

	do
	{
		int recvLen = 0;
		char idType = 0;
		int realDataLen = 0;
		int dataLen = 0;

		// 장비로부터 데이터를 읽어 FIFO에 넣기
		recvLen = recv (mReadBuf, mDataFifo.getRemainSize ());
		mDataFifo.push ((uint8_t*)mReadBuf, recvLen);

		moveToHdr ();

		size_t fifoSize = mDataFifo.getSize();

		size_t idx = findTail ();

		if ( idx == 0 )
		{
			break;
		}
		else if (  idx < ((sizeof(CanRawFrame) - (CAN_RAW_DATA_LEN + 1)) )
					|| ( sizeof(CanExtRawFrame) < idx ) )
		{
			// 프레임 길이가 잘못됨
			printf("wrond frame - %lu\n", idx);
			mDataFifo.flush (idx);
			continue;
		}

		if ( !mDataFifo.poll ((uint8_t*)mReadBuf, idx) )
		{
			printf("get data fail - %lu / %lu\n", idx, fifoSize);
			// 데이터 가져오기 실패 (가능성은 없음)
			break;
		}

		idType = readIdType ();
		if ( idType != U2C_ID_TYPE_NORMAL && idType == U2C_ID_TYPE_EXT )
		{
			printf("wrong idType\n");
			break;
		}

		switch( idType )
		{
		case U2C_ID_TYPE_NORMAL:
			realDataLen = (idx - (sizeof(CanRawFrame) - CAN_RAW_DATA_LEN - 1)) >> 1;
			break;
		case U2C_ID_TYPE_EXT:
			realDataLen = (idx - (sizeof(CanExtRawFrame) - CAN_RAW_DATA_LEN - 1)) >> 1;
			break;
		}

		dataLen = readDataLen (idType);
		if ( realDataLen != dataLen )
		{
			// 데이터 길이 값이 잘못됨
			printf("wrond dataLen - %lu: %d / %d\n", idx, realDataLen, dataLen);
			break;
		}

		switch( idType )
		{
		case U2C_ID_TYPE_NORMAL:
			{
				CanRawFrame* rawFrame = (CanRawFrame*)mReadBuf;
				CanFrame frame =
				{ 0, };

				frame.mExt = charToInt (&idType, sizeof(idType));
				frame.mId = charToInt (rawFrame->mId, sizeof(rawFrame->mId));
				frame.mLen = dataLen;

				for(int cnt = 0;cnt < dataLen;++cnt)
				{
					frame.mData[cnt] = (charToInt (&rawFrame->mData[cnt * 2], 2)
										& 0xFF);
				}

				data.push_back(frame);
			}
			break;
		case U2C_ID_TYPE_EXT:
			{
				CanExtRawFrame* rawFrame = (CanExtRawFrame*)mReadBuf;
				CanFrame frame =
				{ 0, };

				frame.mExt = charToInt (&idType, sizeof(idType));
				frame.mId = charToInt (rawFrame->mId, sizeof(rawFrame->mId));
				frame.mLen = dataLen;

				for(int cnt = 0;cnt < dataLen;++cnt)
				{
					frame.mData[cnt] = (charToInt (&rawFrame->mData[cnt * 2], 2)
										& 0xFF);
				}

				data.push_back(frame);
			}
			break;
		}
	}
	while( (--exitCnt) != 0 );
}

void
U2C::sendData (const CanFrame& data)
{
	switch( data.mExt )
	{
	case 0:
		{
			CanRawFrame rawFrame =
			{ 0, };
			size_t sendLen = sizeof(rawFrame) - CAN_RAW_DATA_LEN
								+ (data.mLen << 1);

			snprintf ((char*)&rawFrame.mHdr, sizeof(rawFrame.mHdr) + 1,
			U2C_CMD_HEAD
			U2C_CMD_TX_DATA
			"%c",
						U2C_ID_TYPE_NORMAL);
			snprintf (rawFrame.mId, sizeof(rawFrame.mId) + 1, "%03x", data.mId);
			snprintf (	rawFrame.mDlc,
						sizeof(rawFrame.mDlc) + 1,
						"%02x",
						data.mLen);

			for(int cnt = 0;cnt < data.mLen;++cnt)
			{
				snprintf (&rawFrame.mData[cnt * 2], 3, "%02x", data.mData[cnt]);
			}
			rawFrame.mData[data.mLen * 2] = '\r';

			//printf ("%s\n", (char*)&rawFrame);
			this->send ((char*)&rawFrame, sendLen);
		}
		break;
	case 1:
		{
			CanExtRawFrame rawFrame =
			{ 0, };
			size_t sendLen = sizeof(rawFrame) - CAN_RAW_DATA_LEN
								+ (data.mLen << 1);

			snprintf ((char*)&rawFrame.mHdr, sizeof(rawFrame.mHdr) + 1,
			U2C_CMD_HEAD
			U2C_CMD_TX_DATA
			"%c",
						U2C_ID_TYPE_EXT);

			snprintf (rawFrame.mId, sizeof(rawFrame.mId) + 1, "%08x", data.mId);
			snprintf (	rawFrame.mDlc,
						sizeof(rawFrame.mDlc) + 1,
						"%02x",
						data.mLen);

			for(int cnt = 0;cnt < data.mLen;++cnt)
			{
				snprintf (&rawFrame.mData[cnt * 2], 3, "%02x", data.mData[cnt]);
			}
			rawFrame.mData[data.mLen * 2] = '\r';

			this->send ((char*)&rawFrame, sendLen);
		}
		break;
	}
}

}
