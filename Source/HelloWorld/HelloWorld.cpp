#include "HelloWorld.h"
#include "HelloWorld_private.h"
#include "Niflect/Serialization/JsonFormat.h"

static void InitForTest(CHelloWorld& instance)
{
	instance.m_value = 1.23f;
}
static bool operator==(const CHelloWorld& lhs, const CHelloWorld& rhs)
{
	return lhs.m_value == rhs.m_value
		;
}

int main()
{
	//Container for registered types
	Niflect::CNiflectTable table;

	//Initialize types in stages
	{
		Niflect::GeneratedRegisterTypes(&table);
		Niflect::GeneratedInitTypes();
		table.BuildTypesMeta();
	}

	//Verify initialization succeeded
	{
		auto type = Niflect::StaticGetType<CHelloWorld>();
		printf("Registered type name: %s\n", type->GetTypeName().c_str());
	}

	//Saving, loading and serialization
	{
		using namespace RwTree;
		Niflect::CStringStream ss;
		{
			CRwNode rwDst;
			{
				CRwNode rwSrc;
				CHelloWorld src;
				CHelloWorld dst;
				InitForTest(src);
				auto type = Niflect::StaticGetType<CHelloWorld>();
				type->SaveInstanceToRwNode(&src, &rwSrc);
				type->LoadInstanceFromRwNode(&dst, &rwSrc);
				assert(src == dst);
				type->SaveInstanceToRwNode(&dst, &rwDst);
			}
			CJsonFormat::Write(&rwDst, ss);
		}
		printf("%s\n", ss.str().c_str());
	}

	return 0;
}