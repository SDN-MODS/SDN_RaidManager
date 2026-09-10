modded class ActionDestroyCodeLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

        // Block action out of raid hours if protection is enabled
        if (manager && manager.IsCodeLockProtectionEnabled())
        {
            if (!manager.IsSDN_RaidManager())
            {
                return false;
            }
        }

        // Return control to standard DayZ/Mod validation to avoid bypassing critical
        // engine checks (like tool durability, player status, and strict target casting).
        return super.ActionCondition(player, target, item);
    }
}