#include "DatabaseHandler.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>

DatabaseHandler::DatabaseHandler(const std::string& dbPath) 
    : db_(nullptr), dbPath_(dbPath) {
}

DatabaseHandler::~DatabaseHandler() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool DatabaseHandler::Initialize() {
    int result = sqlite3_open(dbPath_.c_str(), &db_);
    if (result != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    return CreateTables();
}

bool DatabaseHandler::CreateTables() {
    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            description TEXT NOT NULL,
            amount REAL NOT NULL,
            category TEXT NOT NULL,
            type INTEGER NOT NULL,
            date INTEGER NOT NULL
        );
    )";
    
    return ExecuteSQL(createTableSQL);
}

bool DatabaseHandler::ExecuteSQL(const std::string& sql) {
    char* errorMessage = nullptr;
    int result = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errorMessage);
    
    if (result != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    
    return true;
}

bool DatabaseHandler::AddTransaction(const Transaction& transaction) {
    const char* insertSQL = R"(
        INSERT INTO transactions (description, amount, category, type, date)
        VALUES (?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, insertSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, transaction.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, transaction.amount);
    sqlite3_bind_text(stmt, 3, transaction.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, static_cast<int>(transaction.type));
    sqlite3_bind_int64(stmt, 5, static_cast<sqlite3_int64>(transaction.date));
    
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}

bool DatabaseHandler::UpdateTransaction(const Transaction& transaction) {
    const char* updateSQL = R"(
        UPDATE transactions 
        SET description = ?, amount = ?, category = ?, type = ?, date = ?
        WHERE id = ?;
    )";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, updateSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, transaction.description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, transaction.amount);
    sqlite3_bind_text(stmt, 3, transaction.category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, static_cast<int>(transaction.type));
    sqlite3_bind_int64(stmt, 5, static_cast<sqlite3_int64>(transaction.date));
    sqlite3_bind_int(stmt, 6, transaction.id);
    
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}

bool DatabaseHandler::DeleteTransaction(int id) {
    const char* deleteSQL = "DELETE FROM transactions WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, deleteSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}

std::vector<Transaction> DatabaseHandler::GetAllTransactions() {
    std::vector<Transaction> transactions;
    const char* selectSQL = "SELECT id, description, amount, category, type, date FROM transactions ORDER BY date DESC;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return transactions;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction transaction;
        transaction.id = sqlite3_column_int(stmt, 0);
        transaction.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        transaction.amount = sqlite3_column_double(stmt, 2);
        transaction.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        transaction.type = static_cast<TransactionType>(sqlite3_column_int(stmt, 4));
        transaction.date = static_cast<std::time_t>(sqlite3_column_int64(stmt, 5));
        
        transactions.push_back(transaction);
    }
    
    sqlite3_finalize(stmt);
    return transactions;
}

std::vector<Transaction> DatabaseHandler::GetTransactionsByCategory(const std::string& category) {
    std::vector<Transaction> transactions;
    const char* selectSQL = "SELECT id, description, amount, category, type, date FROM transactions WHERE category = ? ORDER BY date DESC;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return transactions;
    }
    
    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction transaction;
        transaction.id = sqlite3_column_int(stmt, 0);
        transaction.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        transaction.amount = sqlite3_column_double(stmt, 2);
        transaction.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        transaction.type = static_cast<TransactionType>(sqlite3_column_int(stmt, 4));
        transaction.date = static_cast<std::time_t>(sqlite3_column_int64(stmt, 5));
        
        transactions.push_back(transaction);
    }
    
    sqlite3_finalize(stmt);
    return transactions;
}

std::vector<Transaction> DatabaseHandler::GetTransactionsByType(TransactionType type) {
    std::vector<Transaction> transactions;
    const char* selectSQL = "SELECT id, description, amount, category, type, date FROM transactions WHERE type = ? ORDER BY date DESC;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return transactions;
    }
    
    sqlite3_bind_int(stmt, 1, static_cast<int>(type));
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction transaction;
        transaction.id = sqlite3_column_int(stmt, 0);
        transaction.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        transaction.amount = sqlite3_column_double(stmt, 2);
        transaction.category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        transaction.type = static_cast<TransactionType>(sqlite3_column_int(stmt, 4));
        transaction.date = static_cast<std::time_t>(sqlite3_column_int64(stmt, 5));
        
        transactions.push_back(transaction);
    }
    
    sqlite3_finalize(stmt);
    return transactions;
}

double DatabaseHandler::GetTotalByType(TransactionType type) {
    const char* selectSQL = "SELECT SUM(amount) FROM transactions WHERE type = ?;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return 0.0;
    }
    
    sqlite3_bind_int(stmt, 1, static_cast<int>(type));
    
    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return total;
}

double DatabaseHandler::GetTotalByCategory(const std::string& category) {
    const char* selectSQL = "SELECT SUM(amount) FROM transactions WHERE category = ?;";
    
    sqlite3_stmt* stmt;
    int result = sqlite3_prepare_v2(db_, selectSQL, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
        return 0.0;
    }
    
    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
    
    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return total;
} 