#pragma once
#include "../Model/Transaction.h"
#include <string>
#include <vector>
#include <memory>

// Forward declaration to avoid including sqlite3.h in header
struct sqlite3;

class DatabaseHandler {
public:
    explicit DatabaseHandler(const std::string& dbPath);
    ~DatabaseHandler();
    
    // Disable copy constructor and assignment operator
    DatabaseHandler(const DatabaseHandler&) = delete;
    DatabaseHandler& operator=(const DatabaseHandler&) = delete;
    
    // Database operations
    bool Initialize();
    bool AddTransaction(const Transaction& transaction);
    bool UpdateTransaction(const Transaction& transaction);
    bool DeleteTransaction(int id);
    std::vector<Transaction> GetAllTransactions();
    std::vector<Transaction> GetTransactionsByCategory(const std::string& category);
    std::vector<Transaction> GetTransactionsByType(TransactionType type);
    
    // Analytics
    double GetTotalByType(TransactionType type);
    double GetTotalByCategory(const std::string& category);
    
    bool IsConnected() const { return db_ != nullptr; }

private:
    sqlite3* db_;
    std::string dbPath_;
    
    bool CreateTables();
    bool ExecuteSQL(const std::string& sql);
}; 