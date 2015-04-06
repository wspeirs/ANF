//
// Created by wspeirs on 4/3/15.
//

#ifndef FLATTENER_ANF_H
#define FLATTENER_ANF_H

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <vector>
#include <bitset>
#include <sstream>

using namespace std;


template<typename Symbol, size_t MaxVars>
struct Clause {
    typedef unordered_map<Symbol, size_t> VariableLookup;

    static VariableLookup var_lookup;
    static size_t var_count;
    bitset<MaxVars> bits;
    size_t hash;

    inline Clause(bitset<MaxVars> bits, size_t hash) : bits(bits), hash(hash) { }

    template<typename S, size_t M>
    friend bool operator==(const Clause<S, M> &lhs, const Clause<S, M> &rhs);

    inline Clause() : bits(), hash(0) { }

    inline Clause(const Symbol &var) {
        auto bit_it = var_lookup.find(var);

        if(bit_it == var_lookup.end()) {
            bit_it = var_lookup.emplace(var, var_count++).first;
        }

        bits.set(bit_it->second);

        hash = std::hash<Symbol>()(var);
    }

    inline string to_string() const {
        stringstream ss;

        ss << '(';

        for(size_t i=0; i < bits.size(); ++i) {
            ss << var_lookup[bits[i]];
        }

        ss << ')';

        return ss.str();
    }
};

template<typename Symbol, size_t MaxVars>
size_t Clause<Symbol, MaxVars>::var_count = 0;

template<typename Symbol, size_t MaxVars>
typename Clause<Symbol, MaxVars>::VariableLookup Clause<Symbol, MaxVars>::var_lookup;

template<typename Symbol, size_t MaxVars>
class basic_anf {
    friend class Clause<Symbol, MaxVars>;

    template<typename S, size_t M>
    friend ostream& operator<<(ostream &output, const basic_anf<S, M> &eq);

    typedef unordered_set<Clause<Symbol, MaxVars> > Equation;

    Equation equation;

    inline basic_anf(Equation &equation) : equation(equation) { }

public:
    inline basic_anf() {
        equation.emplace(Clause<Symbol, MaxVars>());
    }

    basic_anf(const Symbol &var) {
        equation.emplace(Clause<Symbol, MaxVars>(var));
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
                ret.emplace(Clause<Symbol, MaxVars>(clause1.bits | clause2.bits, clause1.hash ^ clause2.hash));
            }
        }

        return new basic_anf<Symbol, MaxVars>(ret);
    }

    inline size_t size() const {
        return equation.size();
    }
};

template<typename S, size_t M>
inline bool operator==(const Clause<S, M> &lhs, const Clause<S, M> &rhs) {
    return lhs.bits == rhs.bits;
}

template<typename S, size_t M>
inline ostream& operator<<(ostream &output, const basic_anf<S, M> &eq) {
    for(auto c : eq.equation) {
        output << c.to_string() << " ^ ";
    }

    // output.seekp(output.tellp()-3); // remove the last ' ^ '

    return output;
}


namespace std {
    template <typename Symbol, size_t MaxVars>
    struct hash<Clause<Symbol, MaxVars> > {
        inline size_t operator()(const Clause<Symbol, MaxVars> &clause) const {
            return clause.hash;
        }
    };
}




typedef basic_anf<char, 32> anf32;



#endif //FLATTENER_ANF_H
