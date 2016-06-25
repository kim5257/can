package u2c_java_example;

import javax.swing.table.DefaultTableModel;
import u2c.CanFrame;

public class DataRecvService implements Runnable
{
	private DefaultTableModel recvTableModel = null;
	private boolean exitFlag = false;
	private int cnt = 0;
	
	public DataRecvService(DataTable recvTable)
	{
		recvTableModel = (DefaultTableModel)recvTable.getModel();
		
		init();
	}
	
	public void init()
	{
		exitFlag = false;
		cnt = 0;
	}
	
	public void exit()
	{
		exitFlag = true;
	}
	
	@Override
	public void run()
	{
		while(!exitFlag)
		{
			CanFrame[] frames = U2C_Mgr.getInstance().readData();
			
			//System.out.println("Recv = "+frames.length);
			
			recvTableModel.setRowCount(cnt + frames.length);
			for(int idx=0;idx<frames.length;++idx)
			{
				recvTableModel.setValueAt(Integer.toString(cnt), cnt, 0);
				recvTableModel.setValueAt(Integer.toString(frames[idx].getExt(), 16), cnt, 1);
				recvTableModel.setValueAt(Integer.toString(frames[idx].getId(), 16), cnt, 2);
				recvTableModel.setValueAt(Integer.toString(frames[idx].getLen(), 16), cnt, 3);
				
				for(int off=0;off<frames[idx].getLen();++off)
				{
					recvTableModel.setValueAt(Integer.toString(((int)frames[idx].getData()[off]) & 0xFF, 16), cnt, 4+off);
				}
				
				cnt++;
			}
			
			CtrlHandler.getInstance().getPgRecvBufUsage().setValue((int)U2C_Mgr.getInstance().getRecvFifoUsage());
			//System.out.println(""+frames.length);
		}
	}

}
