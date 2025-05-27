#pragma once
#include "../Model/Transaction.h"
#include "../Database/DatabaseHandler.h"
#include <vector>
#include <memory>
#include <functional>
#include <string>

class TransactionManager {
public:
    using TransactionList = std::vector<Transaction>;
    using Observer = std::function<void()>;
    
    explicit TransactionManager(const std::string& dbPath);
    ~TransactionManager() = default;
    
    // Transaction operations
    bool AddTransaction(const std::string& description, double amount, 
                       const std::string& category, TransactionType type);
    bool UpdateTransaction(int id, const std::string& description, double amount,
                          const std::string& category, TransactionType type);
    bool DeleteTransaction(int id);
    
    // Data retrieval
    const TransactionList& GetTransactions() const { return transactions_; }
    TransactionList GetTransactionsByCategory(const std::string& category);
    TransactionList GetTransactionsByType(TransactionType type);
    
    // Analytics
    double GetTotalIncome() const;
    double GetTotalExpenses() const;
    double GetBalance() const;
    double GetTotalByCategory(const std::string& category) const;
    
    // Categories management
    std::vector<std::string> GetCategories() const;
    
    // Observer pattern for UI updates
    void RegisterObserver(Observer observer);
    void UnregisterObserver(Observer observer);
    
    // Data refresh
    void RefreshData();
    
    bool IsInitialized() const { return dbHandler_ && dbHandler_->IsConnected(); }

private:
    std::unique_ptr<DatabaseHandler> dbHandler_;
    TransactionList transactions_;
    std::vector<Observer> observers_;
    
    void NotifyObservers();
    void LoadTransactions();
    int GetNextId() const;
}; 