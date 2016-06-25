package u2c_java_example;

import javax.swing.table.DefaultTableModel;

import u2c.CanFrame;
import u2c.U2C_Log;

public class DataReadService implements Runnable
{
	private DefaultTableModel sendTableModel = null;
	private boolean exitFlag = false;
	private U2C_Log u2cLog = null;
	private int cnt = 0;

	public DataReadService(String fileName, DataTable sendTable)
	{
		this.sendTableModel = (DefaultTableModel) sendTable.getModel();

		u2cLog = new U2C_Log();
		u2cLog.open(fileName, U2C_Log.OpenMode.FILE_READ);

		init();
	}

	public void init()
	{
		cnt = 0;
		exitFlag = false;
	}

	public void exit()
	{
		exitFlag = true;
	}

	@Override
	public void run()
	{
		while (!exitFlag)
		{
			CanFrame frame = u2cLog.read();

			if (frame == null)
			{
				break;
			}
			
			sendTableModel.setRowCount(cnt+1);
			sendTableModel.setValueAt(Integer.toString(cnt), cnt, 0);
			sendTableModel.setValueAt(Integer.toString(frame.getExt(), 16), cnt, 1);
			sendTableModel.setValueAt(Integer.toString(frame.getId(), 16), cnt, 2);
			sendTableModel.setValueAt(Integer.toString(frame.getLen(), 16), cnt, 3);
			for(int off=0;off<frame.getLen();++off)
			{
				sendTableModel.setValueAt(Integer.toString(((int)frame.getData()[off]) & 0xFF, 16), cnt, 4+off);
			}
			
			++cnt;
			
			try
			{
				Thread.sleep(0, 1);
			} catch (InterruptedException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
