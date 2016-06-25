package u2c_java_example;

import javax.swing.JButton;
import javax.swing.table.DefaultTableModel;

import u2c.CanFrame;

public class DataSendService implements Runnable
{
	private DefaultTableModel sendTableModel = null;
	private boolean exitFlag = false;
	private JButton btnSend = null;
	
	public DataSendService(DataTable sendTable, JButton btnSend)
	{
		sendTableModel = (DefaultTableModel)sendTable.getModel();
		this.btnSend = btnSend;
	}
	
	public void init()
	{
		exitFlag = false;
	}
	
	public void exit()
	{
		exitFlag = true;
	}
	
	private int getSendCnt()
	{
		int idx = 0;
		int row = sendTableModel.getRowCount();
		
		for(idx=0;idx<row;++idx)
		{
			if( ((String)sendTableModel.getValueAt(idx, 1)) == null )
			{
				break;
			}
			else if(((String)sendTableModel.getValueAt(idx, 1)).equals(""))
			{
				break;
			}
		}
		
		return idx;
	}
	
	@Override
	public void run()
	{
		int row = getSendCnt();
		
		CtrlHandler.getInstance().getPgSendProgress().setMaximum(row);
		
		for(int idx=0;idx<row;++idx)
		{
			if( ((String)sendTableModel.getValueAt(idx, 1)) == null )
			{
				break;
			}
			else if(((String)sendTableModel.getValueAt(idx, 1)).equals(""))
			{
				break;
			}
			
			int ext =  Integer.parseInt((String)sendTableModel.getValueAt(idx, 1), 16);
			int id =  Integer.parseInt((String)sendTableModel.getValueAt(idx, 2), 16);
			int len =  Integer.parseInt((String)sendTableModel.getValueAt(idx, 3), 16);
			
			byte data[] = new byte[len];
			for(int off=0;off<len;++off)
			{
				data[off] = (byte)Integer.parseInt((String)sendTableModel.getValueAt(idx, 4+off), 16);
			}
			
			CanFrame	frame = new CanFrame(ext, id, len, data);
			
			U2C_Mgr.getInstance().sendData(frame);
			
			if( exitFlag )
			{
				break;
			}
			
			try
			{
				Thread.sleep(100, 0);
				//Thread.sleep(1000);
			} catch (InterruptedException e)
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			CtrlHandler.getInstance().getPgSendProgress().setValue(idx + 1);
		}
		
		btnSend.setText(Res.send);
	}

}
