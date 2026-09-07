modded class BaseBuildingBase
{
	private static ref map<string, int> s_RaidBlockedMsgCooldown = new map<string, int>();

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		// Sets initial state to allow damage tracking. Actual raid checks
		// are now handled instantly at EEOnDamageCalculated and EEHitBy for performance optimization.
		SetAllowDamage(true);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetGame() && GetGame().IsServer())
		{
			SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
			if (manager && manager.IsEnabled() && !manager.IsSDN_RaidManager())
			{
				SDN_RaidManagerNotifyBlockedHit(source, manager);
				return;
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetGame() && GetGame().IsServer())
		{
			SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

			if (manager && manager.IsEnabled())
			{
				if (!manager.IsSDN_RaidManager())
				{
					// Not raid time: Block damage outright and notify
					SDN_RaidManagerNotifyBlockedHit(source, manager);
					return false;
				}
				else
				{
					// Raid is active
					if (manager.IsRagCompatibilityEnabled() && this.IsKindOf("RaG_BB_Base"))
					{
						// Dynamic compatibility bypasses further logic overrides of this class
						// and returns control straight back to RaG base via super logic
						return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
					}
				}
			}
		}

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
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