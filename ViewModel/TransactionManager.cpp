#include "TransactionManager.h"
#include <algorithm>
#include <set>
#include <iostream>

TransactionManager::TransactionManager(const std::string& dbPath) {
    dbHandler_ = std::make_unique<DatabaseHandler>(dbPath);
    if (dbHandler_->Initialize()) {
        LoadTransactions();
    } else {
        std::cerr << "Failed to initialize database" << std::endl;
    }
}

bool TransactionManager::AddTransaction(const std::string& description, double amount,
                                       const std::string& category, TransactionType type) {
    if (!dbHandler_ || description.empty() || category.empty() || amount <= 0) {
        return false;
    }
    
    Transaction transaction(GetNextId(), description, amount, category, type);
    
    if (dbHandler_->AddTransaction(transaction)) {
        LoadTransactions(); // Refresh from database to get the actual ID
        NotifyObservers();
        return true;
    }
    
    return false;
}

bool TransactionManager::UpdateTransaction(int id, const std::string& description, double amount,
                                         const std::string& category, TransactionType type) {
    if (!dbHandler_ || description.empty() || category.empty() || amount <= 0) {
        return false;
    }
    
    Transaction transaction(id, description, amount, category, type);
    
    if (dbHandler_->UpdateTransaction(transaction)) {
        LoadTransactions();
        NotifyObservers();
        return true;
    }
    
    return false;
}

bool TransactionManager::DeleteTransaction(int id) {
    if (!dbHandler_) {
        return false;
    }
    
    if (dbHandler_->DeleteTransaction(id)) {
        LoadTransactions();
        NotifyObservers();
        return true;
    }
    
    return false;
}

TransactionManager::TransactionList TransactionManager::GetTransactionsByCategory(const std::string& category) {
    if (!dbHandler_) {
        return {};
    }
    
    return dbHandler_->GetTransactionsByCategory(category);
}

TransactionManager::TransactionList TransactionManager::GetTransactionsByType(TransactionType type) {
    if (!dbHandler_) {
        return {};
    }
    
    return dbHandler_->GetTransactionsByType(type);
}

double TransactionManager::GetTotalIncome() const {
    if (!dbHandler_) {
        return 0.0;
    }
    
    return dbHandler_->GetTotalByType(TransactionType::Income);
}

double TransactionManager::GetTotalExpenses() const {
    if (!dbHandler_) {
        return 0.0;
    }
    
    return dbHandler_->GetTotalByType(TransactionType::Expense);
}

double TransactionManager::GetBalance() const {
    return GetTotalIncome() - GetTotalExpenses();
}

double TransactionManager::GetTotalByCategory(const std::string& category) const {
    if (!dbHandler_) {
        return 0.0;
    }
    
    return dbHandler_->GetTotalByCategory(category);
}

std::vector<std::string> TransactionManager::GetCategories() const {
    std::set<std::string> uniqueCategories;
    
    for (const auto& transaction : transactions_) {
        uniqueCategories.insert(transaction.category);
    }
    
    return std::vector<std::string>(uniqueCategories.begin(), uniqueCategories.end());
}

void TransactionManager::RegisterObserver(Observer observer) {
    observers_.push_back(observer);
}

void TransactionManager::UnregisterObserver(Observer observer) {
    // Note: This is a simplified implementation. In a real application,
    // you might want to use a more sophisticated approach to manage observers
    // observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
}

void TransactionManager::RefreshData() {
    LoadTransactions();
    NotifyObservers();
}

void TransactionManager::NotifyObservers() {
    for (const auto& observer : observers_) {
        observer();
    }
}

void TransactionManager::LoadTransactions() {
    if (dbHandler_) {
        transactions_ = dbHandler_->GetAllTransactions();
    }
}

int TransactionManager::GetNextId() const {
    if (transactions_.empty()) {
        return 1;
    }
    
    int maxId = 0;
    for (const auto& transaction : transactions_) {
        if (transaction.id > maxId) {
            maxId = transaction.id;
        }
    }
    
    return maxId + 1;
} 