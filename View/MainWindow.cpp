#include "MainWindow.h"
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/msgdlg.h>
#include <sstream>
#include <iomanip>

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT, MainWindow::OnExit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
    EVT_BUTTON(ID_ADD_TRANSACTION, MainWindow::OnAddTransaction)
    EVT_BUTTON(ID_EDIT_TRANSACTION, MainWindow::OnEditTransaction)
    EVT_BUTTON(ID_DELETE_TRANSACTION, MainWindow::OnDeleteTransaction)
    EVT_BUTTON(ID_REFRESH, MainWindow::OnRefresh)
    EVT_LIST_ITEM_SELECTED(ID_TRANSACTION_LIST, MainWindow::OnTransactionSelected)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(TransactionManager& manager)
    : wxFrame(nullptr, wxID_ANY, "Personal Finance Tracker", wxDefaultPosition, wxSize(800, 600)),
      manager_(manager), selectedTransactionId_(-1) {
    
    CreateMenuBar();
    CreateControls();
    
    // Register as observer for data changes
    manager_.RegisterObserver([this]() {
        RefreshTransactionList();
        RefreshSummary();
    });
    
    // Initial data load
    RefreshTransactionList();
    RefreshSummary();
    
    Centre();
}

void MainWindow::CreateMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar;
    
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_EXIT, "E&xit\tCtrl-Q", "Quit this program");
    
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
    
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar(menuBar);
    
    CreateStatusBar(2);
    SetStatusText("Ready", 0);
}

void MainWindow::CreateControls() {
    wxPanel* panel = new wxPanel(this);
    
    // Main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Create and add panels
    wxPanel* summaryPanel = CreateSummaryPanel();
    wxPanel* inputPanel = CreateInputPanel();
    wxPanel* listPanel = CreateTransactionList();
    
    mainSizer->Add(summaryPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(inputPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(listPanel, 1, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
}

wxPanel* MainWindow::CreateSummaryPanel() {
    wxPanel* summaryPanel = new wxPanel(this);
    wxStaticBoxSizer* summarySizer = new wxStaticBoxSizer(wxHORIZONTAL, summaryPanel, "Financial Summary");
    
    // Create summary labels
    totalIncomeLabel_ = new wxStaticText(summaryPanel, wxID_ANY, "Total Income: $0.00");
    totalExpensesLabel_ = new wxStaticText(summaryPanel, wxID_ANY, "Total Expenses: $0.00");
    balanceLabel_ = new wxStaticText(summaryPanel, wxID_ANY, "Balance: $0.00");
    
    // Style the balance label
    wxFont font = balanceLabel_->GetFont();
    font.SetWeight(wxFONTWEIGHT_BOLD);
    balanceLabel_->SetFont(font);
    
    summarySizer->Add(totalIncomeLabel_, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    summarySizer->Add(totalExpensesLabel_, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    summarySizer->Add(balanceLabel_, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    
    summaryPanel->SetSizer(summarySizer);
    return summaryPanel;
}

wxPanel* MainWindow::CreateInputPanel() {
    wxPanel* inputPanel = new wxPanel(this);
    wxStaticBoxSizer* inputSizer = new wxStaticBoxSizer(wxVERTICAL, inputPanel, "Transaction Details");
    
    // Create input controls
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 5, 5);
    gridSizer->AddGrowableCol(1);
    
    // Description
    gridSizer->Add(new wxStaticText(inputPanel, wxID_ANY, "Description:"), 0, wxALIGN_CENTER_VERTICAL);
    descriptionText_ = new wxTextCtrl(inputPanel, wxID_ANY);
    gridSizer->Add(descriptionText_, 1, wxEXPAND);
    
    // Amount
    gridSizer->Add(new wxStaticText(inputPanel, wxID_ANY, "Amount:"), 0, wxALIGN_CENTER_VERTICAL);
    amountText_ = new wxTextCtrl(inputPanel, wxID_ANY);
    gridSizer->Add(amountText_, 1, wxEXPAND);
    
    // Category
    gridSizer->Add(new wxStaticText(inputPanel, wxID_ANY, "Category:"), 0, wxALIGN_CENTER_VERTICAL);
    categoryChoice_ = new wxChoice(inputPanel, wxID_ANY);
    categoryChoice_->Append("Salary");
    categoryChoice_->Append("Rent");
    categoryChoice_->Append("Groceries");
    categoryChoice_->Append("Transportation");
    categoryChoice_->Append("Entertainment");
    categoryChoice_->Append("Utilities");
    categoryChoice_->Append("Healthcare");
    categoryChoice_->Append("Other");
    categoryChoice_->SetSelection(0);
    gridSizer->Add(categoryChoice_, 1, wxEXPAND);
    
    // Type
    gridSizer->Add(new wxStaticText(inputPanel, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL);
    typeChoice_ = new wxChoice(inputPanel, wxID_ANY);
    typeChoice_->Append("Income");
    typeChoice_->Append("Expense");
    typeChoice_->SetSelection(1);
    gridSizer->Add(typeChoice_, 1, wxEXPAND);
    
    // Date
    gridSizer->Add(new wxStaticText(inputPanel, wxID_ANY, "Date:"), 0, wxALIGN_CENTER_VERTICAL);
    datePicker_ = new wxDatePickerCtrl(inputPanel, wxID_ANY);
    gridSizer->Add(datePicker_, 1, wxEXPAND);
    
    inputSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 5);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton_ = new wxButton(inputPanel, ID_ADD_TRANSACTION, "Add");
    editButton_ = new wxButton(inputPanel, ID_EDIT_TRANSACTION, "Edit");
    deleteButton_ = new wxButton(inputPanel, ID_DELETE_TRANSACTION, "Delete");
    refreshButton_ = new wxButton(inputPanel, ID_REFRESH, "Refresh");
    
    editButton_->Enable(false);
    deleteButton_->Enable(false);
    
    buttonSizer->Add(addButton_, 0, wxALL, 5);
    buttonSizer->Add(editButton_, 0, wxALL, 5);
    buttonSizer->Add(deleteButton_, 0, wxALL, 5);
    buttonSizer->Add(refreshButton_, 0, wxALL, 5);
    
    inputSizer->Add(buttonSizer, 0, wxALIGN_CENTER);
    
    inputPanel->SetSizer(inputSizer);
    return inputPanel;
}

wxPanel* MainWindow::CreateTransactionList() {
    wxPanel* listPanel = new wxPanel(this);
    wxStaticBoxSizer* listSizer = new wxStaticBoxSizer(wxVERTICAL, listPanel, "Transactions");
    
    transactionList_ = new wxListCtrl(listPanel, ID_TRANSACTION_LIST, wxDefaultPosition, wxDefaultSize,
                                     wxLC_REPORT | wxLC_SINGLE_SEL);
    
    // Add columns
    transactionList_->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    transactionList_->AppendColumn("Description", wxLIST_FORMAT_LEFT, 200);
    transactionList_->AppendColumn("Amount", wxLIST_FORMAT_RIGHT, 100);
    transactionList_->AppendColumn("Category", wxLIST_FORMAT_LEFT, 120);
    transactionList_->AppendColumn("Type", wxLIST_FORMAT_LEFT, 80);
    transactionList_->AppendColumn("Date", wxLIST_FORMAT_LEFT, 100);
    
    listSizer->Add(transactionList_, 1, wxEXPAND | wxALL, 5);
    
    listPanel->SetSizer(listSizer);
    return listPanel;
}

void MainWindow::OnAddTransaction(wxCommandEvent& event) {
    wxString description = descriptionText_->GetValue();
    wxString amountStr = amountText_->GetValue();
    wxString category = categoryChoice_->GetStringSelection();
    TransactionType type = (typeChoice_->GetSelection() == 0) ? TransactionType::Income : TransactionType::Expense;
    
    if (description.IsEmpty() || amountStr.IsEmpty()) {
        wxMessageBox("Please fill in all fields", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    double amount;
    if (!amountStr.ToDouble(&amount) || amount <= 0) {
        wxMessageBox("Please enter a valid amount", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (manager_.AddTransaction(description.ToStdString(), amount, category.ToStdString(), type)) {
        ClearInputFields();
        SetStatusText("Transaction added successfully", 0);
    } else {
        wxMessageBox("Failed to add transaction", "Error", wxOK | wxICON_ERROR);
    }
}

void MainWindow::OnEditTransaction(wxCommandEvent& event) {
    if (selectedTransactionId_ == -1) {
        wxMessageBox("Please select a transaction to edit", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    wxString description = descriptionText_->GetValue();
    wxString amountStr = amountText_->GetValue();
    wxString category = categoryChoice_->GetStringSelection();
    TransactionType type = (typeChoice_->GetSelection() == 0) ? TransactionType::Income : TransactionType::Expense;
    
    if (description.IsEmpty() || amountStr.IsEmpty()) {
        wxMessageBox("Please fill in all fields", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    double amount;
    if (!amountStr.ToDouble(&amount) || amount <= 0) {
        wxMessageBox("Please enter a valid amount", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (manager_.UpdateTransaction(selectedTransactionId_, description.ToStdString(), 
                                 amount, category.ToStdString(), type)) {
        ClearInputFields();
        selectedTransactionId_ = -1;
        editButton_->Enable(false);
        deleteButton_->Enable(false);
        SetStatusText("Transaction updated successfully", 0);
    } else {
        wxMessageBox("Failed to update transaction", "Error", wxOK | wxICON_ERROR);
    }
}

void MainWindow::OnDeleteTransaction(wxCommandEvent& event) {
    if (selectedTransactionId_ == -1) {
        wxMessageBox("Please select a transaction to delete", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    int result = wxMessageBox("Are you sure you want to delete this transaction?", 
                             "Confirm Delete", wxYES_NO | wxICON_QUESTION);
    
    if (result == wxYES) {
        if (manager_.DeleteTransaction(selectedTransactionId_)) {
            ClearInputFields();
            selectedTransactionId_ = -1;
            editButton_->Enable(false);
            deleteButton_->Enable(false);
            SetStatusText("Transaction deleted successfully", 0);
        } else {
            wxMessageBox("Failed to delete transaction", "Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainWindow::OnRefresh(wxCommandEvent& event) {
    manager_.RefreshData();
    SetStatusText("Data refreshed", 0);
}

void MainWindow::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Personal Finance Tracker v1.0\n\nA C++ application using MVVM architecture",
                 "About Personal Finance Tracker", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnTransactionSelected(wxListEvent& event) {
    long selectedIndex = event.GetIndex();
    if (selectedIndex >= 0) {
        wxString idStr = transactionList_->GetItemText(selectedIndex, 0);
        long id;
        if (idStr.ToLong(&id)) {
            selectedTransactionId_ = static_cast<int>(id);
            
            // Find the transaction and populate fields
            const auto& transactions = manager_.GetTransactions();
            for (const auto& transaction : transactions) {
                if (transaction.id == selectedTransactionId_) {
                    PopulateInputFields(transaction);
                    break;
                }
            }
            
            editButton_->Enable(true);
            deleteButton_->Enable(true);
        }
    }
}

void MainWindow::RefreshTransactionList() {
    transactionList_->DeleteAllItems();
    
    const auto& transactions = manager_.GetTransactions();
    for (size_t i = 0; i < transactions.size(); ++i) {
        const auto& transaction = transactions[i];
        
        long index = transactionList_->InsertItem(i, wxString::Format("%d", transaction.id));
        transactionList_->SetItem(index, 1, transaction.description);
        transactionList_->SetItem(index, 2, wxString::Format("$%.2f", transaction.amount));
        transactionList_->SetItem(index, 3, transaction.category);
        transactionList_->SetItem(index, 4, transaction.GetTypeString());
        transactionList_->SetItem(index, 5, transaction.GetDateString());
    }
}

void MainWindow::RefreshSummary() {
    double totalIncome = manager_.GetTotalIncome();
    double totalExpenses = manager_.GetTotalExpenses();
    double balance = manager_.GetBalance();
    
    totalIncomeLabel_->SetLabel(wxString::Format("Total Income: $%.2f", totalIncome));
    totalExpensesLabel_->SetLabel(wxString::Format("Total Expenses: $%.2f", totalExpenses));
    balanceLabel_->SetLabel(wxString::Format("Balance: $%.2f", balance));
    
    // Color code the balance
    if (balance >= 0) {
        balanceLabel_->SetForegroundColour(wxColour(0, 128, 0)); // Green
    } else {
        balanceLabel_->SetForegroundColour(wxColour(255, 0, 0)); // Red
    }
    
    SetStatusText(wxString::Format("Transactions: %zu", manager_.GetTransactions().size()), 1);
}

void MainWindow::ClearInputFields() {
    descriptionText_->Clear();
    amountText_->Clear();
    categoryChoice_->SetSelection(0);
    typeChoice_->SetSelection(1);
    datePicker_->SetValue(wxDateTime::Now());
}

void MainWindow::PopulateInputFields(const Transaction& transaction) {
    descriptionText_->SetValue(transaction.description);
    amountText_->SetValue(wxString::Format("%.2f", transaction.amount));
    
    // Find and set category
    int categoryIndex = categoryChoice_->FindString(transaction.category);
    if (categoryIndex != wxNOT_FOUND) {
        categoryChoice_->SetSelection(categoryIndex);
    }
    
    typeChoice_->SetSelection(transaction.type == TransactionType::Income ? 0 : 1);
    
    // Set date
    wxDateTime date;
    date.Set(static_cast<time_t>(transaction.date));
    datePicker_->SetValue(date);
} 