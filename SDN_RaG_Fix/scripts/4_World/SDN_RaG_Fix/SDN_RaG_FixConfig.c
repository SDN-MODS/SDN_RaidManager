class SDN_RaG_FixConfigData
{
	bool AllowWindowDestruction = false;
}

class SDN_RaG_FixConfig
{
	private static ref SDN_RaG_FixConfig s_Instance;

	private const string PROFILE_DIR = "$profile:SDN_MODS/SDN_RaG_Fix";
	private const string CONFIG_PATH = "$profile:SDN_MODS/SDN_RaG_Fix/SDN_RaG_FixConfig.json";

	private ref SDN_RaG_FixConfigData m_Data;

	static SDN_RaG_FixConfig GetInstance()
	{
		if (!s_Instance)
		{
			s_Instance = new SDN_RaG_FixConfig();
		}
		return s_Instance;
	}

	void SDN_RaG_FixConfig()
	{
		Load();
	}

	void Load()
	{
		MakeDirectory("$profile:SDN_MODS");
		MakeDirectory(PROFILE_DIR);

		if (!FileExist(CONFIG_PATH))
		{
			m_Data = new SDN_RaG_FixConfigData();
			Save();
			return;
		}

		SDN_RaG_FixConfigData loadedData = new SDN_RaG_FixConfigData();
		string errorMessage = "";
		if (!JsonFileLoader<SDN_RaG_FixConfigData>.LoadFile(CONFIG_PATH, loadedData, errorMessage))
		{
			m_Data = new SDN_RaG_FixConfigData();
			Save();
			return;
		}

		m_Data = loadedData;
	}

	void Save()
	{
		if (m_Data)
		{
			JsonFileLoader<SDN_RaG_FixConfigData>.JsonSaveFile(CONFIG_PATH, m_Data);
		}
	}

	bool IsWindowDestructionAllowed()
	{
		if (m_Data)
		{
			return m_Data.AllowWindowDestruction;
		}
		return false;
	}
}