modded class RaG_BB_Base
{
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetRaGConfig().DisableAllDamage)
		{
			return false;
		}

		if (!ShouldUseDoorOnlyDamageMode())
		{
			return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}

		bool isExplosion = damageType == DamageType.EXPLOSION;

		if (isExplosion)
		{
			string doorZone = GetPrimaryDoorDamageZone();
			if (doorZone != "")
			{
				float damage = damageResult.GetHighestDamage("Health");
				AddHealth(doorZone, "Health", -damage);
			}

			return false;
		}

		if (IsDoorDamageZone(dmgZone))
		{
			return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}

		return false;
	}
}
