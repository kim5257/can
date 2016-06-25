package u2c;

public class CanFrame
{
	private int ext = 0;
	private int id = 0;
	private int len = 0;
	private byte data[] = null;

	public CanFrame()
	{
		super();
	}
	
	public CanFrame(int ext, int id, int len, byte[] data)
	{
		super();
		this.ext = ext;
		this.id = id;
		this.len = len;
		this.data = data;
	}

	public int getExt()
	{
		return ext;
	}

	public void setExt(int ext)
	{
		this.ext = ext;
	}

	public int getId()
	{
		return id;
	}

	public void setId(int id)
	{
		this.id = id;
	}

	public int getLen()
	{
		return len;
	}

	public void setLen(int len)
	{
		this.len = len;
	}

	public byte[] getData()
	{
		return data;
	}

	public void setData(byte[] data)
	{
		this.data = data;
	}
}
