#pragma once
#include "Niflect/Util/ConcatSymbols.h"

#ifdef _NIFLECTGEN
${GenTimeReplacement}
#else
// Include a redundant semicolon at the end of the generated code block, so that intellisense parsers can start parsing
// a new declaration if the line number/generated code is out of date.
#define _NIFLECTGENTAG_GENERATED_NON_STORAGE_DECLS CONCAT_SYMBOLS_4(CURRENT_FILE_ID,_,__LINE__,_GENERATED_NON_STORAGE_DECLS);

#define _NIFLECTGENTAG_TYPE 
#define _NIFLECTGENTAG_FIELD 
#define _NIFLECTGENTAG_METHOD 
#define _NIFLECTGENTAG_ENUMCONST
#endif