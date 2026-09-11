class CfgPatches
{
	class SDN_CodeLock_Fix
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] =
		{
			"DZ_Data",
			"CodeLock",
			"SDN_RaidManager"
		};
	};
};

class CfgMods
{
	class SDN_CodeLock_Fix
	{
		dir = "SDN_CodeLock_Fix";
		name = "SDN_CodeLock_Fix";
		author = "SafeDoNordeste";
		credits = "SafeDoNordeste";
		type = "mod";
		dependencies[] = { "World" };

		class defs
		{
			class worldScriptModule
			{
				files[] = { "SDN_CodeLock_Fix/scripts/4_World" };
			};
		};
	};
};