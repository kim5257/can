package u2c_java_example;

public class Res
{
	static public final String title = "U2C Example";
	static public final String open = "열기";
	static public final String close = "닫기";
	static public final String start = "시작";
	static public final String stop = "중지";
	static public final String send = "전송";
	static public final String load = "읽기";
	static public final String save = "저장";
	static public final String setting = "설정";
	static public final String ext = "EXT";
	static public final String mask = "마스크";
	static public final String filter = "필터";
	static public final String sendProgress = "전송 진행 상황";
	static public final String recvBufUsage = "수신 버퍼 사용률";

	static public final String[] dataHdr =
	{ "NO", "EXT", "ID", "LEN", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7" };

	static public final SpeedInfo speedInfo[] =
	{ new SpeedInfo("100K", 5, 4, 1, 72), new SpeedInfo("125K", 5, 2, 1, 72), new SpeedInfo("200K", 5, 3, 1, 40),
			new SpeedInfo("250K", 5, 2, 1, 36), new SpeedInfo("500K", 5, 2, 1, 18), new SpeedInfo("1M", 5, 3, 1, 8), };

	static public class SpeedInfo
	{
		String name;
		int bs1;
		int bs2;
		int swj;
		int prescaler;

		public SpeedInfo(String name, int bs1, int bs2, int swj, int prescaler)
		{
			super();
			this.name = name;
			this.bs1 = bs1;
			this.bs2 = bs2;
			this.swj = swj;
			this.prescaler = prescaler;
		}

		public String toString()
		{
			return name;
		}

		public int getBs1()
		{
			return bs1;
		}

		public int getBs2()
		{
			return bs2;
		}

		public int getSwj()
		{
			return swj;
		}

		public int getPrescaler()
		{
			return prescaler;
		}
	}
}
