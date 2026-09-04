modded class RaG_BB_Base
{
    override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
    {
        // Verifica se o nosso mod está ativado e barra o dano fora do horário de raid
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();
        if (manager && manager.IsEnabled() && !manager.IsSDN_RaidManager())
        {
            return false;
        }

        // Se for horário de Raid, restaura a mecânica original do RaG via super call
        return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
    }
};