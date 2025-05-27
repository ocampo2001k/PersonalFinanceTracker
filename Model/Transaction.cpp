#include "Transaction.h"
#include <sstream>
#include <iomanip>

std::string Transaction::GetDateString() const {
    std::tm* timeinfo = std::localtime(&date);
    std::ostringstream oss;
    oss << std::put_time(timeinfo, "%Y-%m-%d");
    return oss.str();
} 