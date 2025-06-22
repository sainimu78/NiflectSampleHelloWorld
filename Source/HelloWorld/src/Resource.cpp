#include "Resource.h"

using CSharedResourceFactory = Niflect::TSharedPtr<CResourceFactory>;

static CSharedResourceFactory* s_addr = NULL;

CResourceFactory* GetResourceFactory()
{
	static CSharedResourceFactory s_singleton = Niflect::MakeShared<CResourceFactory>();
	if (s_addr == NULL)
		s_addr = &s_singleton;
	return s_singleton.Get();
}
void DestroyResourceFactory()
{
	*s_addr = NULL;
	s_addr = NULL;
}