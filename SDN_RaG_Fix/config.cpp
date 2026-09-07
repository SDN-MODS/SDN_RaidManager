class CfgPatches
{
	class SDN_RaG_Fix
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"RaG_BaseBuild"
		};
	};
};

class CfgMods
{
	class SDN_RaG_Fix
	{
		dir = "SDN_RaG_Fix";
		name = "SDN_RaG_Fix";
		author = "SafeDoNordeste";
		credits = "SafeDoNordeste";
		type = "mod";
		dependencies[] = { "World" };

		class defs
		{
			class worldScriptModule
			{
				files[] = { "SDN_RaG_Fix/scripts/4_World" };
			};
		};
	};
};