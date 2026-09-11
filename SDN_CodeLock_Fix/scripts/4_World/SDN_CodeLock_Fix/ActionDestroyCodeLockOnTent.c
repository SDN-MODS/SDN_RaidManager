modded class ActionDestroyCodeLockOnTent
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
                TentBase tentBase = TentBase.Cast(targetObject);
                if (tentBase)
                {
                    // Checa especificamente se há um CodeLock plugado
                    EntityAI lockAttach = tentBase.FindAttachmentBySlotName("Att_CombinationLock");
                    if (lockAttach) return true;

                    // Fallback para varrer inventário caso o nome do slot mude
                    for (int i = 0; i < tentBase.GetInventory().AttachmentCount(); i++)
                    {
                        EntityAI attachment = tentBase.GetInventory().GetAttachmentFromIndex(i);
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