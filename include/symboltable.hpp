#pragma once
#include <iostream>
#include <optional>
#include <stdexcept>
#include <unordered_map>
//R3
template <typename Key, typename Value> class SymbolTable
{

  public:
    std::unordered_map<Key, Value> table;
    bool add(const Key &key,const Value &value)
    {
        auto [it, inserted] = table.emplace(key, value);
        return inserted;
    }

    Value &get(const Key &key)
    {
        auto it = table.find(key);
        if (it == table.end())
        {
            throw std::runtime_error("Symbol not found: " + key);
        }
        return it->second;
    }

    std::optional<Value> get_if(const Key &key) const
    {
        auto it = table.find(key);
        if (it != table.end())
            return it->second;
        return std::nullopt;
    }
    bool contains(std::string name) {
        auto found = table.find(name);
        return found != table.end();
    }
    void print() const
    { //R2
        for (const auto &[k, v] : table)
            std::cout << k << " => {\n" << v << "\n}" << std::endl;
    }
};
