modded class ActionDestroyCodeLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

        // Se a proteção estiver ativa e for fora do raid, não exibe a ação.
        // manager.IsSDN_RaidManager() e manager.IsCodeLockProtectionEnabled() agora respondem
        // de forma client-side com as variáveis cacheadas via RPC!
        if (manager && manager.IsCodeLockProtectionEnabled() && !manager.IsSDN_RaidManager())
        {
            return false;
        }

        // Return control to standard DayZ/Mod validation to avoid bypassing critical
        // engine checks (like tool durability, player status, and strict target casting).
        return super.ActionCondition(player, target, item);
    }
}
