#include "v8datamodel/GameSettings.h"

namespace RBX
{
	GameSettings::GameSettings()
		: soundEnabled(true),
		  softwareSound(false),
	{
		setName("Game Options");
	}
}
