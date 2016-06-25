package u2c_java_example;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;

public class UsrProgressBar extends JPanel
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -586110843999335755L;

	private JLabel title = null;
	private JProgressBar value = null;

	public UsrProgressBar(String name)
	{
		title = new JLabel(name);
		value = new JProgressBar(0, 100);

		add(title);
		add(value);
	}

	public void setMaximum(int val)
	{
		value.setMaximum(val);
	}
	
	public int getMaximum()
	{
		return value.getMaximum();
	}
	
	public void setMinimum(int val)
	{
		value.setMinimum(val);
	}
	
	public int getMinimum()
	{
		return value.getMinimum();
	}

	public void setValue(int val)
	{
		value.setValue(val);
	}

	public int getValue()
	{
		return value.getValue();
	}
}
