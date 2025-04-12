#pragma once

namespace TestDefinition
{
#define CONCAT_CONST_CHAR_2(a, b) a"" b
#define CONCAT_CONST_CHAR_3(a, b, c) CONCAT_CONST_CHAR_2(a, b)"" c
#define CONCAT_CONST_CHAR_4(a, b, c, d) CONCAT_CONST_CHAR_3(a, b, c)"" d
#define ROOT_TEST_PATH "../../../../../../Source/Niflect/src/Niflect/Test"
	namespace FilePath
	{
		constexpr const char* InputJson_JsonFormat = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Input/JsonFormat.json");
		constexpr const char* OutputJson_JsonFormat = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/JsonFormat.json");
		constexpr const char* InputJson_AnimGraphEditorData = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Input/mhlike.animgraph.meta");
		constexpr const char* OutputJson_AnimGraphEditorData = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/mhlike.animgraph.meta");
		constexpr const char* OutputJson_0 = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/JsonFormat_0.json");
		constexpr const char* OutputBinary_0 = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/BinaryFormat_0.bin");
		constexpr const char* OutputJson_1 = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/JsonFormat_1.json");
		constexpr const char* OutputBinary_1 = CONCAT_CONST_CHAR_2(ROOT_TEST_PATH, "/RwTree/Output/BinaryFormat_1.bin");
	}
}
