modded class RaG_BB_Base
{
	// Standalone script to resolve the architectural flaw in RaG_BaseBuild
	// where explosive and bullet damage passes to walls because SetAllowDamage(true)
	// resolves C++ native damage prior to script-level boolean returns.

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		// To fix the C++ engine damage bleed, we heal the wall instantaneously
		// if the impact zone is NOT a door while the base is unlocked for raids.
		if (damageResult)
		{
			// If it hit the wall (not a door), heal the wall regardless of damage type
			// (Bullets, explosives, melee)
			if (!SDN_Fix_IsDoorDamageZone(dmgZone))
			{
				float damageDealt = damageResult.GetDamage("", "Health");
				if (damageDealt > 0)
				{
					AddHealth("", "Health", damageDealt); // Revert Global Health loss

					if (dmgZone != "")
					{
						float zoneDamage = damageResult.GetDamage(dmgZone, "Health");
						if (zoneDamage > 0)
						{
							AddHealth(dmgZone, "Health", zoneDamage); // Revert Zone Health loss
						}
					}
				}
				// We halt further execution so no visual/audio break effects play for a blocked impact
				return;
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	bool SDN_Fix_IsDoorDamageZone(string dmgZone)
	{
		string lowered = dmgZone;
		lowered.ToLower();
		if (lowered.Contains("door") || lowered.Contains("gate"))
		{
			return true;
		}
		return false;
	}
}