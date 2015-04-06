//
// Created by wspeirs on 4/3/15.
//

#ifndef FLATTENER_ANF_H
#define FLATTENER_ANF_H

#define BOOST_POOL_NO_MT // disable mutex

#include <boost/pool/pool_alloc.hpp>

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <vector>
//#include <boost/dynamic_bitset.hpp>
#include <bitset>

//using boost::dynamic_bitset;
using namespace std;

template<typename Symbol, size_t MaxVars>
class basic_anf {

    class Clause {
        typedef unordered_map<Symbol, size_t> VariableLookup;

    protected:
        static VariableLookup var_lookup;
        static size_t var_count;
        bitset<MaxVars> bits;
        size_t hash;

        inline Clause(bitset<MaxVars> bits, size_t hash) : bits(bits), hash(hash) { }

        template<typename V, size_t S>
        friend bool operator==(const typename basic_anf<V, S>::Clause &lhs, const typename basic_anf<V, S>::Clause &rhs);

        inline Clause() : bits(), hash(0) { }

        inline Clause(const Symbol &var) {
            auto bit_it = var_lookup.find(var);

            if(bit_it == var_lookup.end()) {
                bit_it = var_lookup.emplace(var, var_count++)->first;
            }

            bits.set(bit_it->second);

            hash = std::hash<Symbol>()(var);
        }
    };


    typedef unordered_set<Clause> Equation;

    Equation equation;

    inline basic_anf(Equation &equation) : equation(equation) { }

public:
    inline basic_anf() {
        equation.insert(Clause());
    }

    basic_anf(Symbol &var) {
        equation.insert(Clause(var));
    }

    basic_anf* XOR(const basic_anf &eq) const {
        Equation ret(size() + eq.size());

        ret.insert(equation.begin(), equation.end());
        ret.insert(eq.equation.begin(), eq.equation.end());

        return new basic_anf<Symbol, MaxVars>(ret);
    }

    basic_anf* AND(const basic_anf &eq) const {
        Equation ret(size() * eq.size());

        for(auto clause1 : equation) {
            for(auto clause2 : eq.equation) {
                ret.emplace(Clause(clause1.bits | clause2.bits, clause1.hash ^ clause2.hash));
            }
        }

        return new basic_anf<Symbol, MaxVars>(ret);
    }

    inline size_t size() const {
        return equation.size();
    }
};

template<typename V, size_t S>
inline bool operator==(const typename basic_anf<V, S>::Clause &lhs, const typename basic_anf<V, S>::Cause &rhs) {
    return lhs.bits == rhs.bits;
}

namespace std {
    template<typename S, size_t V>
    class hash<typename basic_anf<S, V>::Clause> {
    public:
        inline size_t operator()(const basic_anf<V, S>::Clause &clause) const {
            return clause.hash;
        }
    };
}




typedef basic_anf<char, 32> anf32;



#endif //FLATTENER_ANF_H
