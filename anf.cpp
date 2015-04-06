#include "anf.h"

template<typename Symbol, size_t MaxVars>
size_t Clause<Symbol, MaxVars>::var_count = 0;

template<typename Symbol, size_t MaxVars>
typename Clause<Symbol, MaxVars>::VariableLookup Clause<Symbol, MaxVars>::var_lookup;