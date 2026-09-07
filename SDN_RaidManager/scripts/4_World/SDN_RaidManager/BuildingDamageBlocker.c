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
		SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
		if (manager)
		{
			manager.RegisterBase(this);
			SetAllowDamage(manager.IsSDN_RaidManager());
		}
		else
		{
			SetAllowDamage(true);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetGame() && GetGame().IsServer())
		{
			SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
			if (manager)
			{
				manager.UnregisterBase(this);
			}
		}
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

			if (manager && manager.IsEnabled() && !manager.IsSDN_RaidManager())
			{
				// Not raid time: Block damage outright and notify
				SDN_RaidManagerNotifyBlockedHit(source, manager);
				return false;
			}
		}

		// RaG Specific Door-Only damage enforcement during Raid
		if (this.IsKindOf("RaG_BB_Base"))
		{
			bool isExplosion = damageType == DamageType.EXPLOSION;
			if (isExplosion)
			{
				string doorZone = SDN_GetPrimaryDoorDamageZone();
				if (doorZone != "")
				{
					float damage = damageResult.GetHighestDamage("Health");
					AddHealth(doorZone, "Health", -damage);
				}
				return false;
			}

			if (SDN_IsDoorDamageZone(dmgZone))
			{
				return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
			}

			return false;
		}

		return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	string SDN_GetPrimaryDoorDamageZone()
	{
		array<string> doors = {"Wall_Gate_1", "Wall_Gate_2", "Wall_Gate_3", "Door_1", "Door_2"};
		foreach (string zoneName : doors)
		{
			float health = GetHealth(zoneName, "Health");
			if (health >= 0)
				return zoneName;
		}
		return "";
	}

	bool SDN_IsDoorDamageZone(string dmgZone)
	{
		string lowered = dmgZone;
		lowered.ToLower();
		if (lowered.Contains("door") || lowered.Contains("gate"))
		{
			return true;
		}
		return false;
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