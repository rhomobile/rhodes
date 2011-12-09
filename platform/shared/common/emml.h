#include "ruby/ext/rho/rhoruby.h"
#include <AtlBase.h>
#include <AtlConv.h>

using namespace std;

namespace rho 
{
	namespace emml 
	{
		wstring rubyValueToEMML(rho_param* p)
		{
			wstring emml;

			if (p && p->type == RHO_PARAM_HASH) {
				for (int i = 0; i < p->v.hash->size; i++) {
					emml.append(CA2W(p->v.hash->name[i]));
					emml.append(L":");
					emml.append(CA2W(p->v.hash->value[i]->v.string));
					if(i < p->v.hash->size - 1)
						emml.append(L";");
				}
			}
			rho_param_free(p);
			return emml;
		}

		void addEMML(wstring& dest, wchar_t* src)
		{
			dest.append(L";");
			dest.append(src);		
		}
	}
}