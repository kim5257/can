package u2c_java_example;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import u2c_java_example.Res.SpeedInfo;

public class CtrlHandler implements ActionListener
{
	private static CtrlHandler inst = new CtrlHandler();

	private JFrame parent = null;

	private JFileChooser readFileSel = new JFileChooser();
	private JFileChooser saveFileSel = new JFileChooser();

	private JButton btnOpen = new JButton(Res.open);
	private JButton btnStart = new JButton(Res.start);
	private JButton btnSend = new JButton(Res.send);
	private JButton btnRead = new JButton(Res.load);
	private JButton btnSave = new JButton(Res.save);

	private JComboBox<Res.SpeedInfo> cbSpeed = new JComboBox<Res.SpeedInfo>(Res.speedInfo);
	private JCheckBox btnExtFlag = new JCheckBox(Res.ext);
	private UsrForm formMask = new UsrForm(Res.mask, 10);
	private UsrForm formFilter = new UsrForm(Res.filter, 10);

	private DataTable tbSendData = new DataTable();
	private DataTable tbRecvData = new DataTable();

	private JScrollPane spSendData = new JScrollPane(tbSendData);
	private JScrollPane spRecvData = new JScrollPane(tbRecvData);
	
	private UsrProgressBar pgSendProgress = new UsrProgressBar(Res.sendProgress);
	private UsrProgressBar pgRecvBufUsage = new UsrProgressBar(Res.recvBufUsage);

	private Thread dataRecvThread = null;
	private DataRecvService dataRecvService = null;

	private Thread dataSendThread = null;
	private DataSendService dataSendService = null;

	private Thread dataReadThread = null;
	private DataReadService dataReadService = null;

	private Thread dataSaveThread = null;
	private DataSaveService dataSaveService = null;

	public static CtrlHandler getInstance()
	{
		return inst;
	}

	private CtrlHandler()
	{
		btnOpen.addActionListener(this);
		btnStart.addActionListener(this);
		btnSend.addActionListener(this);
		btnRead.addActionListener(this);
		btnSave.addActionListener(this);

		init();
	}

	void init()
	{
		btnStart.setEnabled(false);

		cbSpeed.setSelectedIndex(4);

		// 파일 열기,저장 설정
		readFileSel.setFileFilter(new FileNameExtensionFilter("txt", "txt"));
		readFileSel.setMultiSelectionEnabled(false);

		saveFileSel.setFileFilter(new FileNameExtensionFilter("txt", "txt"));
		saveFileSel.setMultiSelectionEnabled(false);
	}

	public void setParent(JFrame parent)
	{
		this.parent = parent;
	}

	public JButton getBtnOpen()
	{
		return btnOpen;
	}

	public JButton getBtnStart()
	{
		return btnStart;
	}

	public JButton getBtnSend()
	{
		return btnSend;
	}

	public JButton getBtnRead()
	{
		return btnRead;
	}

	public JButton getBtnSave()
	{
		return btnSave;
	}

	public JComboBox<Res.SpeedInfo> getCbSpeed()
	{
		return cbSpeed;
	}

	public JCheckBox getBtnExtFlag()
	{
		return btnExtFlag;
	}

	public UsrForm getFormMask()
	{
		return formMask;
	}

	public UsrForm getFormFilter()
	{
		return formFilter;
	}

	public DataTable getTbSendData()
	{
		return tbSendData;
	}

	public DataTable getTbRecvData()
	{
		return tbRecvData;
	}

	public JScrollPane getSpSendData()
	{
		return spSendData;
	}

	public JScrollPane getSpRecvData()
	{
		return spRecvData;
	}
	
	public UsrProgressBar getPgSendProgress()
	{
		return pgSendProgress;
	}
	
	public UsrProgressBar getPgRecvBufUsage()
	{
		return pgRecvBufUsage;
	}

	@Override
	public void actionPerformed(ActionEvent e)
	{
		if (e.getSource() == btnOpen)
		{
			openHandle();
		} else if (e.getSource() == btnStart)
		{
			startHandle();
		} else if (e.getSource() == btnSend)
		{
			sendHandle();
		} else if (e.getSource() == btnRead)
		{
			readHandle();
		} else if (e.getSource() == btnSave)
		{
			saveHandle();
		} else
		{
			System.out.println(e.toString());
		}
	}

	private void openHandle()
	{
		if (!U2C_Mgr.getInstance().isOpen())
		{
			if (!U2C_Mgr.getInstance().open(0))
			{
				JOptionPane.showMessageDialog(null, "열기 실패", "에러", JOptionPane.ERROR_MESSAGE);
			}
		} else
		{
			if (U2C_Mgr.getInstance().getMode())
			{
				startHandle();
			}

			if (!U2C_Mgr.getInstance().close())
			{
				JOptionPane.showMessageDialog(null, "닫기 실패", "에러", JOptionPane.ERROR_MESSAGE);
			}
		}

		if (U2C_Mgr.getInstance().isOpen())
		{
			btnOpen.setText(Res.close);
			btnStart.setEnabled(true);
		} else
		{
			btnOpen.setText(Res.open);
			btnStart.setEnabled(false);
		}
	}

	private void startHandle()
	{
		if (!U2C_Mgr.getInstance().getMode())
		{
			do
			{
				Res.SpeedInfo info = (SpeedInfo) cbSpeed.getSelectedItem();
				boolean extFlag = btnExtFlag.isSelected();
				int mask = 0;
				int filter = 0;

				if (!formMask.getValue().equals(""))
				{
					mask = Integer.parseInt(formMask.getValue(), 16);
				}

				if (!formFilter.getValue().equals(""))
				{
					filter = Integer.parseInt(formFilter.getValue(), 16);
				}

				if (!U2C_Mgr.getInstance().setSpeed(info.getBs1(), info.getBs2(), info.getSwj(), info.getPrescaler()))
				{
					JOptionPane.showMessageDialog(null, "속도 설정 실패", "에러", JOptionPane.ERROR_MESSAGE);
					break;
				}

				try
				{
					Thread.sleep(10, 0);
				} catch (InterruptedException e)
				{
					e.printStackTrace();
				}

				if (!U2C_Mgr.getInstance().setFilter(true, 0, extFlag, mask, filter))
				{
					JOptionPane.showMessageDialog(null, "필터 설정 실패", "에러", JOptionPane.ERROR_MESSAGE);
					break;
				}

				try
				{
					Thread.sleep(10, 0);
				} catch (InterruptedException e)
				{
					e.printStackTrace();
				}

				if (!U2C_Mgr.getInstance().setMode(1))
				{
					JOptionPane.showMessageDialog(null, "시작 실패", "에러", JOptionPane.ERROR_MESSAGE);
					break;
				}

				dataRecvService = new DataRecvService(getTbRecvData());
				dataRecvThread = new Thread(dataRecvService);

				dataRecvThread.start();

			} while (false);
		} else
		{
			try
			{
				dataRecvService.exit();
				dataRecvThread.join();

				dataRecvThread = null;
				dataRecvService = null;
			} catch (InterruptedException e)
			{
				e.printStackTrace();
			}

			if (!U2C_Mgr.getInstance().setMode(0))
			{
				JOptionPane.showMessageDialog(null, "중지 실패", "에러", JOptionPane.ERROR_MESSAGE);
			}
		}

		if (U2C_Mgr.getInstance().getMode())
		{
			btnStart.setText(Res.stop);
		} else
		{
			btnStart.setText(Res.start);
		}
	}

	private void sendHandle()
	{
		if (btnSend.getText().equals(Res.send))
		{
			dataSendService = new DataSendService(getTbSendData(), btnSend);
			dataSendThread = new Thread(dataSendService);

			dataSendThread.start();
			btnSend.setText(Res.stop);
		} else
		{
			try
			{
				dataSendService.exit();
				dataSendThread.join();

				dataSendThread = null;
				dataSendService = null;
			} catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}

	private void readHandle()
	{
		int ret = readFileSel.showOpenDialog(parent);

		if (ret == JFileChooser.APPROVE_OPTION)
		{

			dataReadService = new DataReadService(readFileSel.getSelectedFile().getAbsolutePath(),
					this.getTbSendData());
			dataReadThread = new Thread(dataReadService);

			dataReadThread.start();

			try
			{
				dataReadThread.join();

				dataReadThread = null;
				dataReadService = null;
			} catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}

	private void saveHandle()
	{
		int ret = saveFileSel.showSaveDialog(parent);

		// System.out.println(saveFileSel.getSelectedFile().getAbsolutePath());

		if (ret == JFileChooser.APPROVE_OPTION)
		{

			dataSaveService = new DataSaveService(saveFileSel.getSelectedFile().getAbsolutePath(),
					this.getTbRecvData());
			dataSaveThread = new Thread(dataSaveService);

			dataSaveThread.start();

			try
			{
				dataSaveThread.join();
				dataSaveThread = null;
				dataSaveService = null;
			} catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}

	public void endService()
	{
		try
		{
			if (dataRecvService != null)
			{
				dataRecvService.exit();
				dataRecvThread.join();
			}
			
			if (dataSendService != null)
			{
				dataSendService.exit();
				dataSendThread.join();
			}
			
			if (dataSaveService != null)
			{
				dataSaveService.exit();
				dataSaveThread.join();
			}
			
			if (dataReadService != null)
			{
				dataReadService.exit();
				dataReadThread.join();
			}
			
		} catch (InterruptedException e)
		{
			e.printStackTrace();
		}
	}
}
