package u2c_java_example;

import javax.swing.table.DefaultTableModel;

import u2c.CanFrame;
import u2c.U2C_Log;

public class DataSaveService implements Runnable
{
	private DefaultTableModel recvTableModel = null;
	private boolean exitFlag = false;
	private U2C_Log u2cLog = null;

	public DataSaveService(String fileName, DataTable recvTable)
	{
		this.recvTableModel = (DefaultTableModel) recvTable.getModel();

		u2cLog = new U2C_Log();
		u2cLog.open(fileName, U2C_Log.OpenMode.FILE_WRITE);
		
		init();
	}

	public void init()
	{
		exitFlag = false;
	}

	public void exit()
	{
		exitFlag = true;
	}

	@Override
	public void run()
	{
		int row = recvTableModel.getRowCount();

		for (int idx = 0; idx < row; ++idx)
		{
			if( ((String)recvTableModel.getValueAt(idx, 1)) == null )
			{
				break;
			}
			
			int ext =  Integer.parseInt((String)recvTableModel.getValueAt(idx, 1), 16);
			int id =  Integer.parseInt((String)recvTableModel.getValueAt(idx, 2), 16);
			int len =  Integer.parseInt((String)recvTableModel.getValueAt(idx, 3), 16);
			
			byte data[] = new byte[len];
			for(int off=0;off<len;++off)
			{
				data[off] = (byte)Integer.parseInt((String)recvTableModel.getValueAt(idx, 4+off), 16);
			}
			
			CanFrame	frame = new CanFrame(ext, id, len, data);
			
			u2cLog.write(frame);
			
			if( exitFlag )
			{
				break;
			}
			
			try
			{
				Thread.sleep(0, 1);
			} catch (InterruptedException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		u2cLog.close();
	}

}
