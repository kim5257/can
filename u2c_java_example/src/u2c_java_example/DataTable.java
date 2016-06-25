package u2c_java_example;

import javax.swing.*;
import javax.swing.table.*;

public class DataTable extends JTable
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -3055909404474077134L;
	
	private DefaultTableModel tmSendData = new DefaultTableModel(Res.dataHdr, 80);
	private DefaultTableCellRenderer trTableSetting = new DefaultTableCellRenderer();

	public DataTable()
	{
		this.setModel(tmSendData);
		
		trTableSetting.setHorizontalAlignment(JLabel.CENTER);
		
		// 정렬
		for(int cnt=0;cnt<Res.dataHdr.length;++cnt)
		{
			this.getColumn(Res.dataHdr[cnt]).setCellRenderer(trTableSetting);
		}
		
		// 크기 조정
		// NO 크기는 60
		this.getColumn(Res.dataHdr[0]).setPreferredWidth(40);
		
		// EXT 크기는 60
		this.getColumn(Res.dataHdr[1]).setPreferredWidth(30);
		
		// LEN 크기는 30
		this.getColumn(Res.dataHdr[3]).setPreferredWidth(30);
		
		// D0~D7 크기는 30
		for(int cnt=4;cnt<=11;++cnt)
		{
			this.getColumn(Res.dataHdr[cnt]).setPreferredWidth(30);
		}
	}
}
