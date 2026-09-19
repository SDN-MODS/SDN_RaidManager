modded class ActionDestroyCodeLockOnTent
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

        if (manager && (!manager.IsCodeLockProtectionEnabled() || manager.IsSDN_RaidManager()))
        {
            return super.ActionCondition(player, target, item);
        }

        return false;
    }
}