package u2c;

public class U2C
{
	private long u2cObjPtr;
	private boolean openFlag = false;
	private boolean modeFlag = false;

	public U2C() throws Exception
	{
		u2cObjPtr = nativeInit();
		if (u2cObjPtr == 0)
		{
			Exception err = new Exception("Failed create U2C object.");
			throw err;
		}
	}

	public boolean open(int idx)
	{
		boolean ret = nativeOpen(u2cObjPtr, idx);

		openFlag = (ret) ? (true) : (false);

		return ret;
	}

	public boolean close()
	{
		boolean ret = nativeClose(u2cObjPtr);

		openFlag = (ret) ? (false) : (true);

		return ret;
	}

	public boolean setSpeed(int bs1, int bs2, int swj, int prescaler)
	{
		return nativeSetSpeed(u2cObjPtr, bs1, bs2, swj, prescaler);
	}

	public boolean setFilter(boolean active, int index, boolean idType, int mask, int filter)
	{
		return nativeSetFilter(u2cObjPtr, (active) ? (1) : (0), index, (idType) ? (1) : (0), mask, filter);
	}

	public boolean setMode(int mode)
	{
		boolean ret = nativeSetMode(u2cObjPtr, mode);

		if (ret)
		{
			modeFlag = (mode == 0) ? (false) : (true);
		}

		return ret;
	}

	public CanFrame[] readData()
	{
		return nativeReadData(u2cObjPtr);
	}

	public boolean sendData(CanFrame frame)
	{
		return nativeSendData(u2cObjPtr, frame);
	}

	public boolean isOpen()
	{
		return openFlag;
	}

	public boolean getMode()
	{
		return modeFlag;
	}
	
	public long getRecvFifoUsage()
	{
		return nativeGetRecvFifoUsage(u2cObjPtr);
	}

	protected void finalize() throws Throwable
	{
		nativeDeinit(u2cObjPtr);
	}

	static
	{
		try
		{
			System.loadLibrary("u2cjni");
		} catch (UnsatisfiedLinkError err)
		{
			System.out.println("Can not load libraryfile");
		}
	}

	private native long nativeInit();

	private native void nativeDeinit(long u2cObjPtr);

	private native boolean nativeOpen(long u2cObjPtr, int index);

	private native boolean nativeClose(long u2cObjPtr);

	private native boolean nativeSetSpeed(long u2cObjPtr, int bs1, int bs2, int swj, int prescaler);

	private native boolean nativeSetFilter(long u2cObjPtr, int active, int index, int idType, int mask, int filter);

	private native boolean nativeSetMode(long u2cObjPtr, int mode);

	private native CanFrame[] nativeReadData(long u2cObjPtr);

	private native boolean nativeSendData(long u2cObjPtr, CanFrame frame);

	private native long nativeGetRecvFifoUsage(long u2cObjPtr);
}
