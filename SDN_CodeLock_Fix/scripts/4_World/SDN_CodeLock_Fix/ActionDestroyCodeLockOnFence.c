modded class ActionDestroyCodeLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SDN_RaidManagerManager manager = SDN_RaidManagerManager.GetInstance();

        // Se a proteção estiver ativa e for fora do raid, não exibe a ação.
        if (manager && manager.IsCodeLockProtectionEnabled() && !manager.IsSDN_RaidManager())
        {
            return false;
        }

        // Se for horário de Raid (ou proteção desativada), validamos manualmente.
        Object targetObject = target.GetObject();
        if (targetObject && player && item)
        {
            // Checagem de distância padrão do DayZ
            if (vector.DistanceSq(player.GetPosition(), targetObject.GetPosition()) <= 2.25)
            {
                BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(targetObject);
                if (baseBuilding)
                {
                    // Checa especificamente se há um CodeLock plugado
                    EntityAI lockAttach = baseBuilding.FindAttachmentBySlotName("Att_CombinationLock");
                    if (lockAttach) return true;

                    // Fallback para varrer inventário caso o nome do slot mude
                    for (int i = 0; i < baseBuilding.GetInventory().AttachmentCount(); i++)
                    {
                        EntityAI attachment = baseBuilding.GetInventory().GetAttachmentFromIndex(i);
                        if (attachment)
                        {
                            string attType = attachment.GetType();
                            attType.ToLower();
                            if (attType.Contains("codelock")) return true;
                        }
                    }
                }
            }
        }

        return false;
    }
}