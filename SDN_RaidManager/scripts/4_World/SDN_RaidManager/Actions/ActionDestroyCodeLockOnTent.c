modded class ActionDestroyCodeLockOnTent
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

        // To safely bypass the flawed super.ActionCondition (which blocks arbitrarily),
        // we must manually enforce DayZ's native lock validation so we don't return
        // true on empty tents, causing null pointers and server crashes.

        Object targetObject = target.GetObject();
        if (targetObject)
        {
            TentBase tentBase = TentBase.Cast(targetObject);
            if (tentBase)
            {
                // Verify the tent has a lock attached to it
                bool hasCodeLock = false;
                for (int i = 0; i < tentBase.GetInventory().AttachmentCount(); i++)
                {
                    EntityAI attachment = tentBase.GetInventory().GetAttachmentFromIndex(i);
                    if (attachment)
                    {
                        string attType = attachment.GetType();
                        attType.ToLower();
                        if (attType.Contains("codelock"))
                        {
                            hasCodeLock = true;
                            break;
                        }
                    }
                }

                if (hasCodeLock)
                {
                    return true;
                }
            }
        }

        return false;
    }
}
