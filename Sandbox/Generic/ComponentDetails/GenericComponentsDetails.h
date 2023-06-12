#pragma once

#include "../Editor/ComponentDetails/ComponentDetailsBase.h"

namespace Generic
{
	struct NameComponent;
}

namespace Editor
{
	class NameComponentDetails : public TComponentDetails<Generic::NameComponent>
	{
	protected:
		void BuildDetailsInternal( Generic::NameComponent & inComponent ) const override;
	};
}
