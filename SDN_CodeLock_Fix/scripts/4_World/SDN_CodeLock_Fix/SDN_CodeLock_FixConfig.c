class SDN_CodeLock_RaidDayWindow
{
	string DayOfWeek = "Saturday";
	int StartHour = 18;
	int StartMinute = 0;
	int Duration = 180;
}

class SDN_CodeLock_RaidManagerConfigData
{
	ref array<ref SDN_CodeLock_RaidDayWindow> RaidDays = new array<ref SDN_CodeLock_RaidDayWindow>();
	int AutoReloadSeconds = 15;
	bool EnableCodeLockProtection = true;
}

class SDN_CodeLock_FixConfig
{
	private static ref SDN_CodeLock_FixConfig s_Instance;

	private const string CONFIG_PATH = "$profile:SDN_MODS/SDN_RaidManager/SDN_RaidManagerConfig.json";

	private ref SDN_CodeLock_RaidManagerConfigData m_Data;
	private ref Timer m_AutoReloadTimer;

	static SDN_CodeLock_FixConfig GetInstance()
	{
		if (!s_Instance)
		{
			s_Instance = new SDN_CodeLock_FixConfig();
		}
		return s_Instance;
	}

	void SDN_CodeLock_FixConfig()
	{
		Load();
		StartAutoReload();
	}

	void Load()
	{
		if (!FileExist(CONFIG_PATH))
		{
			m_Data = new SDN_CodeLock_RaidManagerConfigData();
			return;
		}

		SDN_CodeLock_RaidManagerConfigData loadedData = new SDN_CodeLock_RaidManagerConfigData();
		string errorMessage = "";
		if (!JsonFileLoader<SDN_CodeLock_RaidManagerConfigData>.LoadFile(CONFIG_PATH, loadedData, errorMessage))
		{
			return; // keep existing state if load fails
		}

		m_Data = loadedData;
	}

	private void StartAutoReload()
	{
		if (!m_AutoReloadTimer)
		{
			m_AutoReloadTimer = new Timer(CALL_CATEGORY_SYSTEM);
		}

		int reloadTime = 15;
		if (m_Data && m_Data.AutoReloadSeconds >= 5)
		{
			reloadTime = m_Data.AutoReloadSeconds;
		}

		m_AutoReloadTimer.Stop();
		m_AutoReloadTimer.Run(reloadTime, this, "Load", NULL, true);
	}

	bool IsCodeLockProtectionEnabled()
	{
		if (m_Data)
		{
			return m_Data.EnableCodeLockProtection;
		}
		return true;
	}

	bool IsSDN_RaidManager()
	{
		if (!m_Data || !m_Data.RaidDays || m_Data.RaidDays.Count() == 0)
		{
			return false;
		}

		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);

		int dayOfWeek = GetDayOfWeek(year, month, day);
		int currentMinuteOfWeek = dayOfWeek * 1440 + hour * 60 + minute;
		const int minutesInWeek = 7 * 1440;

		foreach (SDN_CodeLock_RaidDayWindow window : m_Data.RaidDays)
		{
			if (!window)
			{
				continue;
			}

			int startDayIndex = ParseDayOfWeek(window.DayOfWeek);
			if (startDayIndex < 0)
			{
				continue;
			}

			int duration = window.Duration;
			if (duration <= 0)
			{
				continue;
			}

			if (duration >= minutesInWeek)
			{
				return true;
			}

			int startMinute = startDayIndex * 1440 + window.StartHour * 60 + window.StartMinute;
			int endMinute = startMinute + duration;

			if (endMinute < minutesInWeek)
			{
				if (currentMinuteOfWeek >= startMinute && currentMinuteOfWeek < endMinute)
				{
					return true;
				}
			}
			else
			{
				int wrappedEnd = endMinute - minutesInWeek;
				if (currentMinuteOfWeek >= startMinute || currentMinuteOfWeek < wrappedEnd)
				{
					return true;
				}
			}
		}

		return false;
	}

	private int ParseDayOfWeek(string value)
	{
		string lowered = value;
		lowered.ToLower();

		switch (lowered)
		{
			case "sunday": return 0;
			case "monday": return 1;
			case "tuesday": return 2;
			case "wednesday": return 3;
			case "thursday": return 4;
			case "friday": return 5;
			case "saturday": return 6;
		}

		return -1;
	}

	private int GetDayOfWeek(int year, int month, int day)
	{
		if (month < 1 || month > 12)
		{
			return 0;
		}

		int offset = 0;
		switch (month)
		{
			case 1:  offset = 0; break;
			case 2:  offset = 3; break;
			case 3:  offset = 2; break;
			case 4:  offset = 5; break;
			case 5:  offset = 0; break;
			case 6:  offset = 3; break;
			case 7:  offset = 5; break;
			case 8:  offset = 1; break;
			case 9:  offset = 4; break;
			case 10: offset = 6; break;
			case 11: offset = 2; break;
			case 12: offset = 4; break;
		}

		if (month < 3)
		{
			year = year - 1;
		}

		return (year + year / 4 - year / 100 + year / 400 + offset + day) % 7;
	}
}