modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		SDN_RaidManagerManager.GetInstance().Init();
	}
}
