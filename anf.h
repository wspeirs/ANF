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
#include <iterator>

using namespace std;


template<typename Symbol, size_t MaxVars>
struct Clause {
    typedef unordered_map<Symbol, size_t> VariableLookup;

    static VariableLookup var_lookup;
    static size_t var_count;
    static Symbol reverse_lookup[MaxVars];
    bitset<MaxVars> bits;
    size_t c_hash;


    template<typename S, size_t M>
    friend bool operator==(const Clause<S, M> &lhs, const Clause<S, M> &rhs);

    template<typename S, size_t M>
    friend ostream& operator<<(ostream &output, const Clause<S, M> &clause);


    inline Clause() : bits(), c_hash(0) { }

    inline Clause(const bitset<MaxVars> &bits) : bits(bits), c_hash(hash<bitset<MaxVars> >()(bits)) { }

    inline Clause(const Symbol &var) {
        auto bit_it = var_lookup.find(var);

        if(bit_it == var_lookup.end()) {
            reverse_lookup[var_count] = var;
            bit_it = var_lookup.emplace(var, var_count++).first;
        }

        bits.set(bit_it->second);

        c_hash = std::hash<Symbol>()(var);
    }

    string to_string() const {
        stringstream ss;

        ss << '(';

        if(bits.any()) {
            for(size_t i=0; i < bits.size(); ++i) {
                if(bits[i])
                    ss << reverse_lookup[i];
            }
        } else {
            ss << 1;
        }

        ss << ')';

        return ss.str();
    }
};

template<typename Symbol, size_t MaxVars>
size_t Clause<Symbol, MaxVars>::var_count = 0;

template<typename Symbol, size_t MaxVars>
typename Clause<Symbol, MaxVars>::VariableLookup Clause<Symbol, MaxVars>::var_lookup(MaxVars*2);

template<typename Symbol, size_t MaxVars>
Symbol Clause<Symbol, MaxVars>::reverse_lookup[MaxVars];

template<typename S, size_t M>
inline bool operator==(const Clause<S, M> &lhs, const Clause<S, M> &rhs) {
    return lhs.bits == rhs.bits;
}

template<typename S, size_t M>
inline ostream& operator<<(ostream &output, const Clause<S, M> &clause) {
    return output << clause.to_string();
}

namespace std {
    template <typename Symbol, size_t MaxVars>
    struct hash<Clause<Symbol, MaxVars> > {
        inline size_t operator()(const Clause<Symbol, MaxVars> &clause) const {
            return clause.c_hash;
        }
    };
}


/*
 * The actual class that implements the ANF equation.
 */
template<typename Symbol, size_t MaxVars>
class basic_anf {
    friend class Clause<Symbol, MaxVars>;

    template<typename S, size_t M> friend ostream& operator<<(ostream &output, const basic_anf<S, M> &eq);
    template<typename S, size_t M> friend bool operator==(const basic_anf<S,M> &lhs, const basic_anf<S,M> &rhs);

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

    basic_anf XOR(const basic_anf &rhs) const {
        Equation ret(size() + rhs.size());

        const auto end = equation.end();
        const auto r_end = rhs.equation.end();

        for(auto e : equation) {
            if(rhs.equation.find(e) == r_end)
                ret.emplace(e);
        }

        for(auto e : rhs.equation) {
            if(equation.find(e) == end)
                ret.emplace(e);
        }

        return basic_anf<Symbol, MaxVars>(ret);
    }

    basic_anf AND(const basic_anf &rhs) const {
        Equation ret(size() * rhs.size());

        for(auto clause1 : equation) {
            for(auto clause2 : rhs.equation) {
                const auto new_clause = Clause<Symbol, MaxVars>(clause1.bits | clause2.bits);
                const auto it = ret.find(new_clause);

                if(it == ret.end())
                    ret.emplace(new_clause);
                else
                    ret.erase(it);
            }
        }

        return basic_anf<Symbol, MaxVars>(ret);
    }

    inline basic_anf NOT() const {
        return this->XOR(basic_anf());
    }

    inline basic_anf OR(const basic_anf &rhs) const {
        return this->XOR(rhs).XOR(this->AND(rhs));
    }

    inline size_t size() const {
        return equation.size();
    }
};

template<typename S, size_t M>
inline ostream& operator<<(ostream &output, const basic_anf<S, M> &eq) {
    if(eq.size() == 0)
        return output;

    auto it = eq.equation.begin();

    for(size_t i=0; i < eq.size()-1; ++i) {
        output << *it << " ^ ";
        ++it;
    }

    output << *it;

    return output;
}

template<typename S, size_t M>
inline bool operator==(const basic_anf<S,M> &lhs, const basic_anf<S,M> &rhs) {
    if(lhs.size() != rhs.size())
        return false;

    return lhs.equation == rhs.equation;
}


/*
 * Some specific instances of basic_anf
 */
typedef basic_anf<char, 32> anf_c32;



#endif //FLATTENER_ANF_H
