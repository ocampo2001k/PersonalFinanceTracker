#pragma once
#include <string>
#include <ctime>

enum class TransactionType {
    Income,
    Expense
};

struct Transaction {
    int id;
    std::string description;
    double amount;
    std::string category;
    TransactionType type;
    std::time_t date;
    
    // Constructor
    Transaction() : id(0), amount(0.0), type(TransactionType::Expense), date(std::time(nullptr)) {}
    
    Transaction(int id, const std::string& desc, double amt, const std::string& cat, 
                TransactionType t, std::time_t d = std::time(nullptr))
        : id(id), description(desc), amount(amt), category(cat), type(t), date(d) {}
    
    // Helper methods
    std::string GetTypeString() const {
        return (type == TransactionType::Income) ? "Income" : "Expense";
    }
    
    std::string GetDateString() const;
}; 