class CfgPatches
{
	class SDN_RaG_BB
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class SDN_RaG_BB
	{
		dir="SDN_RaG_BB";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="";
		credits="";
		author="UN Square";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
		dependencies[]=
		{

			"World"

		};

		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"SDN_RaG_BB/scripts/4_world"
				};
			};
		};
	};
};