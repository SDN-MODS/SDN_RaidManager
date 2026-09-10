modded class ActionDestroyCodeLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

        // If code lock protection is enabled in JSON and it is NOT raid time, hide the action entirely
        if (manager && manager.IsCodeLockProtectionEnabled() && !manager.IsSDN_RaidManager())
        {
            return false;
        }

        return super.ActionCondition(player, target, item);
    }
}