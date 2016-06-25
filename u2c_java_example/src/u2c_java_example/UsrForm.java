package u2c_java_example;

import javax.swing.*;

public class UsrForm extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = -7727695552957218655L;
	
	private JLabel title = null;
	private JTextField value = null;
	
	public UsrForm (String name, int col)
	{
		title = new JLabel(name);
		value = new JTextField(col);
		
		add(title);
		add(value);
	}
	
	public void setValue(String val)
	{
		value.setText(val);
	}
	
	public String getValue()
	{
		return value.getText();
	}
}
