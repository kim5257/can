package u2c_java_example;

import u2c.U2C;

public class U2C_Mgr {
	private static U2C_Mgr inst = null;
	
	private U2C u2c = null;
	
	private U2C_Mgr()
	{
		try {
			u2c = new U2C();
		} catch (Exception e) {
			System.err.println(e.getMessage());
		}
	}
	
	public static U2C getInstance()
	{
		if( inst == null )
		{
			inst = new U2C_Mgr();
		}
		
		return inst.u2c;
	}
}
