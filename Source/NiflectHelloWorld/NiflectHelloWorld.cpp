#include "NiflectHelloWorld.h"
#include <stdio.h>
#include "NiflectHelloWorld_private.h"

int main()
{
	Niflect::CNiflectTable table;
	Niflect::GeneratedRegisterTypes(&table);
	Niflect::GeneratedInitTypes();
	table.BuildTypesMeta();

	auto type = Niflect::StaticGetType<CHelloWorld>();
	ASSERT(type == table.GetTypeByIndex(0));
	printf("Type name: %s\n", type->GetTypeName().c_str());

	return 0;
}