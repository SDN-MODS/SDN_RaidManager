modded class RaG_BB_Base
{
    override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        // Integration with SDN_RaidManager
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
        if (manager && manager.IsEnabled() && !manager.IsSDN_RaidManager())
        {
            return false; // Blocks damage if it's not raid time
        }

        // If it's raid time, pass back to the original engine/mod rules
        return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }
};