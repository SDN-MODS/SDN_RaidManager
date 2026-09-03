modded class BaseBuildingBase
{
	private static ref map<string, int> s_RaidBlockedMsgCooldown = new map<string, int>();
	private ref Timer m_SDN_RaidManagerStateTimer;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		SDN_RaidManagerRefreshDamageState();

		if (!m_SDN_RaidManagerStateTimer)
		{
			m_SDN_RaidManagerStateTimer = new Timer(CALL_CATEGORY_SYSTEM);
		}

		// Keep damage state synced with schedule transitions.
		m_SDN_RaidManagerStateTimer.Run(15.0, this, "SDN_RaidManagerRefreshDamageState", NULL, true);
	}

	void SDN_RaidManagerRefreshDamageState()
	{
		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
		if (!manager || !manager.IsEnabled())
		{
			SetAllowDamage(true);
			return;
		}

		SetAllowDamage(manager.IsSDN_RaidManager());
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetGame() && GetGame().IsServer())
		{
			SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
			if (manager && manager.IsEnabled() && !manager.IsSDN_RaidManager())
			{
				SetAllowDamage(false);
				SDN_RaidManagerNotifyBlockedHit(source, manager);
				return;
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	private void SDN_RaidManagerNotifyBlockedHit(EntityAI source, SDN_RaidManagerManager manager)
	{
		if (!source || !manager)
		{
			return;
		}

		PlayerBase player = PlayerBase.Cast(source);
		if (!player)
		{
			player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		}

		if (!player)
		{
			return;
		}

		string message = manager.GetRaidUnavailableMessage();
		if (message == "")
		{
			return;
		}

		int cooldownMs = manager.GetPlayerMessageCooldownSeconds() * 1000;
		int nowMs = GetGame().GetTime();

		string playerKey = "unknown";
		PlayerIdentity identity = player.GetIdentity();
		if (identity)
		{
			playerKey = identity.GetPlainId();
		}

		int nextAllowedMs = 0;
		if (s_RaidBlockedMsgCooldown.Find(playerKey, nextAllowedMs))
		{
			if (nowMs < nextAllowedMs)
			{
				return;
			}
		}

		s_RaidBlockedMsgCooldown.Set(playerKey, nowMs + cooldownMs);
		player.MessageStatus(message);
	}
}
