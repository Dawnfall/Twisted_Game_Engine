#include "Project.h"

namespace Twisted
{
	Project& Project::GetInstance()
	{
		static Project project;
		return project;
	}
}
