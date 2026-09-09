modded class RaG_BB_Base
{
	// Override the flawed method from original RaG code which disabled
	// the invincibility configuration for solid walls (walls without gates).
	override bool ShouldUseDoorOnlyDamageMode()
	{
		if (!GetRaGConfig().DisableDamageButDoors)
			return false;

		// ORIGINAL RAG BUG: if (!HasBBGatePart()) return false;
		// By removing the line above, we ensure SOLID WALLS are also protected
		// by the "Door Only Damage" rule, effectively making them invincible.

		if (IsBBWindow())
			return false;

		return true;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (GetRaGConfig().DisableAllDamage)
		{
			return false; // Absolute Godmode
		}

		if (!ShouldUseDoorOnlyDamageMode())
		{
			return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}

		// At this point: DoorOnlyDamageMode is ON.
		// If it's a solid wall (no gate), it should take NO damage at all.
		if (!HasBBGatePart())
		{
			return false;
		}

		// If it HAS a gate, redirect explosion damage to the door.
		bool isExplosion = damageType == DamageType.EXPLOSION;
		if (isExplosion)
		{
			string doorZone = GetPrimaryDoorDamageZone();
			if (doorZone != "")
			{
				float damage = damageResult.GetHighestDamage("Health");
				AddHealth(doorZone, "Health", -damage);
			}
			return false; // Block explosion from hitting the wall frame
		}

		// If it's a bullet/melee, allow damage only if it hit the door specifically.
		if (IsDoorDamageZone(dmgZone))
		{
			return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		}

		return false; // Bullets hitting the wall frame are blocked
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		// C++ Engine Damage Bleed Fix
		// SetAllowDamage(true) forces the C++ engine to apply damage before scripts can say 'return false;'.
		// We must instantly heal the wall if it was hit and should be protected.
		if (damageResult && GetRaGConfig() && GetRaGConfig().DisableDamageButDoors)
		{
			// If it's a solid wall OR the hit landed on the wall frame (not the door)
			if (!HasBBGatePart() || !IsDoorDamageZone(dmgZone))
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

				// Return to prevent visual destruction particles if blocked
				return;
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
}
