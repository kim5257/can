package u2c_java_example;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.*;

public class U2C_Example extends JFrame
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -1004676735954256060L;

	public U2C_Example()
	{
		setSize(960, 600);
		setTitle(Res.title);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//this.addWindowListener(new WindowCloser());

		add(new MenuPanel(), BorderLayout.NORTH);
		add(new DataPanel(), BorderLayout.CENTER);

		CtrlHandler.getInstance().setParent(this);
		
		setVisible(true);
	}

	public static void main(String[] args)
	{
		new U2C_Example();
	}

}

class WindowCloser extends WindowAdapter
{
	@Override
	public void windowClosing(WindowEvent e)
	{
		super.windowClosing(e);
		
		//CtrlHandler.getInstance().endService();
		
		System.exit(0);
	}
}

class MenuPanel extends JPanel
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -4390966440793787833L;

	public MenuPanel()
	{
		add(CtrlHandler.getInstance().getBtnOpen());
		add(CtrlHandler.getInstance().getBtnStart());
		add(CtrlHandler.getInstance().getCbSpeed());
		add(CtrlHandler.getInstance().getBtnExtFlag());
		add(CtrlHandler.getInstance().getFormMask());
		add(CtrlHandler.getInstance().getFormFilter());
	}
}

class DataPanel extends JPanel
{
	/**
	 * 
	 */
	private static final long serialVersionUID = 4385693131279446488L;
	
	public DataPanel()
	{
		this.setLayout(new GridLayout(1,2));
		add(new SendDataPanel());
		add(new RecvDataPanel());
	}
}

class SendDataPanel extends JPanel
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -308341374650783814L;

	public SendDataPanel()
	{
		setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));
		
		add(CtrlHandler.getInstance().getBtnSend());
		add(CtrlHandler.getInstance().getBtnRead());
		add(CtrlHandler.getInstance().getSpSendData());
		add(CtrlHandler.getInstance().getPgSendProgress());
	}
	
}

class RecvDataPanel extends JPanel
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -4134128432648757077L;
	
	public RecvDataPanel()
	{
		setLayout(new FlowLayout(FlowLayout.LEFT, 5, 5));
		
		add(CtrlHandler.getInstance().getBtnSave());
		add(CtrlHandler.getInstance().getSpRecvData());
		add(CtrlHandler.getInstance().getPgRecvBufUsage());
	}
}