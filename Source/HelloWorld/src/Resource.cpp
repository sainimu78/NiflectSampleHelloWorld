#include "Resource.h"

using CSharedResourceFactory = Niflect::TSharedPtr<CResourceFactory>;

static CSharedResourceFactory* s_pointeer = NULL;

CResourceFactory* GetResourceFactory()
{
	static CSharedResourceFactory s_singleton = Niflect::MakeShared<CResourceFactory>();
	if (s_pointeer == NULL)
		s_pointeer = &s_singleton;
	return s_singleton.Get();
}
void DestroyResourceFactory()
{
	*s_pointeer = NULL;
	s_pointeer = NULL;
}