#pragma once
#include <Winstring.h>
#include <assert.h>

namespace App1
{
	class HStringWrap
	{
	public:
		HStringWrap(const wchar_t *str)
		{
			HSTRING s;
			HRESULT hr = WindowsCreateString(str, wcslen(str), &s);
			bool succeeded = SUCCEEDED(hr);
			assert(succeeded);
			hstr = succeeded ? s : nullptr;
		}

		operator HSTRING() const
		{
			return hstr;
		}

		~HStringWrap()
		{
			if (hstr != nullptr)
			{
				HRESULT hr = WindowsDeleteString(hstr);
				assert(SUCCEEDED(hr));
			}
		}

	private:
		HSTRING hstr;
	};
}
