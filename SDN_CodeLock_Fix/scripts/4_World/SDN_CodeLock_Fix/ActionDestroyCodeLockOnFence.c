modded class ActionDestroyCodeLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_CodeLock_FixConfig fixConfig = SDN_CodeLock_FixConfig.GetInstance();

        if (fixConfig && (!fixConfig.IsCodeLockProtectionEnabled() || fixConfig.IsSDN_RaidManager()))
        {
            return super.ActionCondition(player, target, item);
        }

        return false;
    }
}