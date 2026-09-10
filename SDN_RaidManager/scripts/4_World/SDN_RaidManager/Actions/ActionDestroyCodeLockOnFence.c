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

        // To safely bypass the flawed super.ActionCondition (which blocks arbitrarily),
        // we must manually enforce DayZ's native lock validation so we don't return
        // true on empty fences, causing null pointers and server crashes.

        Object targetObject = target.GetObject();
        if (targetObject && targetObject.CanUseConstruction())
        {
            BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(targetObject);
            if (baseBuilding)
            {
                // Verify the fence has a lock attached to it
                bool hasCodeLock = false;
                for (int i = 0; i < baseBuilding.GetInventory().AttachmentCount(); i++)
                {
                    EntityAI attachment = baseBuilding.GetInventory().GetAttachmentFromIndex(i);
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
