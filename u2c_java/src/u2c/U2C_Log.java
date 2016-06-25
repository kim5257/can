package u2c;

import java.io.*;

public class U2C_Log
{
	public enum OpenMode
	{
		FILE_READ, FILE_WRITE,
	}

	private FileReader fileReader = null;
	private BufferedReader bufReader = null;

	private FileWriter fileWriter = null;
	private BufferedWriter bufWriter = null;

	private OpenMode mode;

	public U2C_Log()
	{
	}

	public boolean open(String path, OpenMode mode)
	{
		boolean ret = false;

		try
		{
			this.mode = mode;

			switch (mode)
			{
			case FILE_READ:
				fileReader = new FileReader(path);
				bufReader = new BufferedReader(fileReader);
				break;
			case FILE_WRITE:
				fileWriter = new FileWriter(path);
				bufWriter = new BufferedWriter(fileWriter);
				break;
			}

			ret = true;
		} catch (FileNotFoundException e)
		{
			e.printStackTrace();
		} catch (IOException e)
		{
			e.printStackTrace();
		}

		return ret;
	}

	public void close()
	{
		try
		{
			switch (mode)
			{
			case FILE_READ:
				bufReader.close();
				fileReader.close();
				break;
			case FILE_WRITE:
				bufWriter.close();
				fileWriter.close();
				break;
			}
		} catch (IOException e)
		{
			e.printStackTrace();
		}
	}

	public CanFrame read()
	{
		CanFrame frame = null;

		String readData;
		String readDataList[];

		try
		{
			readData = bufReader.readLine();

			if (readData != null)
			{
				readDataList = readData.split(" ");

				int ext = Integer.parseInt(readDataList[0], 16);
				int id = Integer.parseInt(readDataList[1], 16);
				int len = Integer.parseInt(readDataList[2], 16);
				byte[] data = new byte[len];
				for (int off = 0; off < len; ++off)
				{
					data[off] = (byte) Integer.parseInt(readDataList[3 + off], 16);
				}

				frame = new CanFrame(ext, id, len, data);
			}

		} catch (IOException e)
		{
			e.printStackTrace();
		}

		return frame;
	}

	public boolean write(final CanFrame frame)
	{
		boolean ret = false;
		String frameFormat[] =
		{ "%x %03x %02x ", "%x %08x %02x " };
		String data;

		data = String.format(frameFormat[frame.getExt()], frame.getExt(), frame.getId(), frame.getLen());

		for (int cnt = 0; cnt < frame.getLen(); ++cnt)
		{
			data += String.format("%02x ", ((int) frame.getData()[cnt]) & 0xFF);
		}
		data += '\n';

		try
		{
			fileWriter.write(data);
			ret = true;
		} catch (IOException e)
		{
			e.printStackTrace();
		}

		return ret;
	}
}
