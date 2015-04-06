#include "anf.h"

template<typename Symbol, size_t MaxVars>
size_t basic_anf<Symbol, MaxVars>::Clause::var_count = 0;

template<typename Symbol, size_t MaxVars>
typename basic_anf<Symbol, MaxVars>::Clause::VariableLookup basic_anf<Symbol, MaxVars>::Clause::var_lookup;