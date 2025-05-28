#include "MainWindow.h"
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/font.h>
#include <sstream>
#include <iomanip>

// Financial Freedom Color Palette
const wxColour EMERALD_GREEN(46, 204, 113);    // #2ECC71 - Growth, prosperity, wealth
const wxColour MIDNIGHT_BLUE(44, 62, 80);      // #2C3E50 - Trust, stability, financial control
const wxColour SKY_BLUE(93, 173, 226);         // #5DADE2 - Clarity, planning, peace of mind
const wxColour SUNSHINE_YELLOW(244, 208, 63);  // #F4D03F - Optimism, energy, positive outlook
const wxColour SOFT_MINT(169, 223, 191);       // #A9DFBF - Calm, balance, budgeting peace
const wxColour SLATE_GRAY(149, 165, 166);      // #95A5A6 - Neutrality, professionalism
const wxColour PURE_WHITE(255, 255, 255);      // #FFFFFF - Simplicity, cleanliness, minimalism
const wxColour BLACK_CHARCOAL(28, 28, 28);     // #1C1C1C - Focus, strength, modern elegance

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_BUTTON(ID_ADD_TRANSACTION, MainWindow::OnAddTransaction)
    EVT_BUTTON(ID_EDIT_TRANSACTION, MainWindow::OnEditTransaction)
    EVT_BUTTON(ID_DELETE_TRANSACTION, MainWindow::OnDeleteTransaction)
    EVT_BUTTON(ID_REFRESH, MainWindow::OnRefresh)
    EVT_MENU(wxID_EXIT, MainWindow::OnExit)
    EVT_MENU(wxID_ABOUT, MainWindow::OnAbout)
    EVT_LIST_ITEM_SELECTED(ID_TRANSACTION_LIST, MainWindow::OnTransactionSelected)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(TransactionManager& manager)
    : wxFrame(nullptr, wxID_ANY, "Personal Finance Tracker", wxDefaultPosition, wxSize(1000, 700))
    , manager_(manager)
    , selectedTransactionId_(-1)
    , transactionList_(nullptr)
    , descriptionText_(nullptr)
    , amountText_(nullptr)
    , categoryChoice_(nullptr)
    , typeChoice_(nullptr)
    , datePicker_(nullptr)
    , addButton_(nullptr)
    , editButton_(nullptr)
    , deleteButton_(nullptr)
    , refreshButton_(nullptr)
    , totalIncomeLabel_(nullptr)
    , totalExpensesLabel_(nullptr)
    , balanceLabel_(nullptr) {
    
    // Set window background to Pure White
    SetBackgroundColour(PURE_WHITE);
    
    // Register as observer (using the correct method name)
    manager_.RegisterObserver([this]() {
        RefreshTransactionList();
        RefreshSummary();
    });
    
    CreateMenuBar();
    CreateControls();
    CreateStatusBar();
    SetStatusText("Ready");
    
    // Center the window
    Center();
    
    // Initial data load
    RefreshTransactionList();
    RefreshSummary();
}

void MainWindow::CreateMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar;
    
    // File menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(ID_REFRESH, "&Refresh\tF5", "Refresh the transaction list");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X", "Quit this program");
    
    // Help menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About\tF1", "Show about dialog");
    
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    
    SetMenuBar(menuBar);
}

void MainWindow::CreateControls() {
    wxPanel* mainPanel = new wxPanel(this);
    mainPanel->SetBackgroundColour(PURE_WHITE);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Create panels
    wxPanel* summaryPanel = CreateSummaryPanel(mainPanel);
    wxPanel* inputPanel = CreateInputPanel(mainPanel);
    wxPanel* listPanel = CreateListPanel(mainPanel);
    
    // Add panels to main sizer
    mainSizer->Add(summaryPanel, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(inputPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    mainSizer->Add(listPanel, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    mainPanel->SetSizer(mainSizer);
    
    // Create main frame sizer
    wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
    frameSizer->Add(mainPanel, 1, wxEXPAND);
    SetSizer(frameSizer);
}

wxPanel* MainWindow::CreateSummaryPanel(wxWindow* parent) {
    wxPanel* panel = new wxPanel(parent);
    panel->SetBackgroundColour(SLATE_GRAY);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Create font for headers (18px bold)
    wxFont headerFont = wxFontInfo(18).Bold().FaceName("Segoe UI");
    
    // Create font for balance (32px bold)
    wxFont balanceFont = wxFontInfo(32).Bold().FaceName("Segoe UI");
    
    // Create font for subheadings (20px bold)
    wxFont subheadingFont = wxFontInfo(20).Bold().FaceName("Segoe UI");
    
    // Income section
    wxBoxSizer* incomeSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* incomeHeader = new wxStaticText(panel, wxID_ANY, "INCOME");
    incomeHeader->SetFont(headerFont);
    incomeHeader->SetForegroundColour(PURE_WHITE);
    
    totalIncomeLabel_ = new wxStaticText(panel, wxID_ANY, "$0.00");
    totalIncomeLabel_->SetFont(subheadingFont);
    totalIncomeLabel_->SetForegroundColour(EMERALD_GREEN);
    
    incomeSizer->Add(incomeHeader, 0, wxALIGN_CENTER);
    incomeSizer->Add(totalIncomeLabel_, 0, wxALIGN_CENTER | wxTOP, 5);
    
    // Expenses section
    wxBoxSizer* expensesSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* expensesHeader = new wxStaticText(panel, wxID_ANY, "EXPENSES");
    expensesHeader->SetFont(headerFont);
    expensesHeader->SetForegroundColour(PURE_WHITE);
    
    totalExpensesLabel_ = new wxStaticText(panel, wxID_ANY, "$0.00");
    totalExpensesLabel_->SetFont(subheadingFont);
    totalExpensesLabel_->SetForegroundColour(BLACK_CHARCOAL);
    
    expensesSizer->Add(expensesHeader, 0, wxALIGN_CENTER);
    expensesSizer->Add(totalExpensesLabel_, 0, wxALIGN_CENTER | wxTOP, 5);
    
    // Balance section
    wxBoxSizer* balanceSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* balanceHeader = new wxStaticText(panel, wxID_ANY, "BALANCE");
    balanceHeader->SetFont(headerFont);
    balanceHeader->SetForegroundColour(PURE_WHITE);
    
    balanceLabel_ = new wxStaticText(panel, wxID_ANY, "$0.00");
    balanceLabel_->SetFont(balanceFont);
    balanceLabel_->SetForegroundColour(EMERALD_GREEN);
    
    balanceSizer->Add(balanceHeader, 0, wxALIGN_CENTER);
    balanceSizer->Add(balanceLabel_, 0, wxALIGN_CENTER | wxTOP, 5);
    
    // Add sections to main sizer
    sizer->Add(incomeSizer, 1, wxEXPAND | wxALL, 20);
    sizer->Add(expensesSizer, 1, wxEXPAND | wxALL, 20);
    sizer->Add(balanceSizer, 1, wxEXPAND | wxALL, 20);
    
    panel->SetSizer(sizer);
    return panel;
}

wxPanel* MainWindow::CreateInputPanel(wxWindow* parent) {
    wxPanel* panel = new wxPanel(parent);
    panel->SetBackgroundColour(MIDNIGHT_BLUE);
    
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Create fonts
    wxFont headerFont = wxFontInfo(18).Bold().FaceName("Segoe UI");
    wxFont labelFont = wxFontInfo(16).Bold().FaceName("Segoe UI");
    wxFont inputFont = wxFontInfo(14).FaceName("Segoe UI");
    wxFont buttonFont = wxFontInfo(15).Bold().FaceName("Segoe UI");
    
    // Header
    wxStaticText* headerLabel = new wxStaticText(panel, wxID_ANY, "TRANSACTION DETAILS");
    headerLabel->SetFont(headerFont);
    headerLabel->SetForegroundColour(PURE_WHITE);
    mainSizer->Add(headerLabel, 0, wxALIGN_CENTER | wxALL, 10);
    
    // Input fields
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 5, 10);
    gridSizer->AddGrowableCol(1, 1);
    
    // Description
    wxStaticText* descLabel = new wxStaticText(panel, wxID_ANY, "Description:");
    descLabel->SetFont(labelFont);
    descLabel->SetForegroundColour(PURE_WHITE);
    descriptionText_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    descriptionText_->SetFont(inputFont);
    descriptionText_->SetBackgroundColour(SOFT_MINT);
    descriptionText_->SetForegroundColour(BLACK_CHARCOAL);
    
    // Amount
    wxStaticText* amountLabel = new wxStaticText(panel, wxID_ANY, "Amount:");
    amountLabel->SetFont(labelFont);
    amountLabel->SetForegroundColour(PURE_WHITE);
    amountText_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0);
    amountText_->SetFont(inputFont);
    amountText_->SetBackgroundColour(SOFT_MINT);
    amountText_->SetForegroundColour(BLACK_CHARCOAL);
    
    // Category
    wxStaticText* categoryLabel = new wxStaticText(panel, wxID_ANY, "Category:");
    categoryLabel->SetFont(labelFont);
    categoryLabel->SetForegroundColour(PURE_WHITE);
    categoryChoice_ = new wxChoice(panel, wxID_ANY);
    categoryChoice_->SetFont(inputFont);
    categoryChoice_->SetBackgroundColour(SOFT_MINT);
    categoryChoice_->SetForegroundColour(BLACK_CHARCOAL);
    categoryChoice_->Append("Food");
    categoryChoice_->Append("Transportation");
    categoryChoice_->Append("Entertainment");
    categoryChoice_->Append("Utilities");
    categoryChoice_->Append("Healthcare");
    categoryChoice_->Append("Shopping");
    categoryChoice_->Append("Salary");
    categoryChoice_->Append("Investment");
    categoryChoice_->Append("Other");
    categoryChoice_->SetSelection(0);
    
    // Type
    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Type:");
    typeLabel->SetFont(labelFont);
    typeLabel->SetForegroundColour(PURE_WHITE);
    typeChoice_ = new wxChoice(panel, wxID_ANY);
    typeChoice_->SetFont(inputFont);
    typeChoice_->SetBackgroundColour(SOFT_MINT);
    typeChoice_->SetForegroundColour(BLACK_CHARCOAL);
    typeChoice_->Append("Income");
    typeChoice_->Append("Expense");
    typeChoice_->SetSelection(1);
    
    // Date
    wxStaticText* dateLabel = new wxStaticText(panel, wxID_ANY, "Date:");
    dateLabel->SetFont(labelFont);
    dateLabel->SetForegroundColour(PURE_WHITE);
    datePicker_ = new wxDatePickerCtrl(panel, wxID_ANY);
    datePicker_->SetFont(inputFont);
    datePicker_->SetBackgroundColour(SOFT_MINT);
    datePicker_->SetForegroundColour(BLACK_CHARCOAL);
    
    gridSizer->Add(descLabel, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(descriptionText_, 1, wxEXPAND);
    gridSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(amountText_, 1, wxEXPAND);
    gridSizer->Add(categoryLabel, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(categoryChoice_, 1, wxEXPAND);
    gridSizer->Add(typeLabel, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(typeChoice_, 1, wxEXPAND);
    gridSizer->Add(dateLabel, 0, wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(datePicker_, 1, wxEXPAND);
    
    mainSizer->Add(gridSizer, 0, wxEXPAND | wxALL, 20);
    
    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    
    addButton_ = new wxButton(panel, ID_ADD_TRANSACTION, "ADD");
    addButton_->SetFont(buttonFont);
    addButton_->SetBackgroundColour(EMERALD_GREEN);
    addButton_->SetForegroundColour(PURE_WHITE);
    
    editButton_ = new wxButton(panel, ID_EDIT_TRANSACTION, "EDIT");
    editButton_->SetFont(buttonFont);
    editButton_->SetBackgroundColour(SKY_BLUE);
    editButton_->SetForegroundColour(PURE_WHITE);
    editButton_->Enable(false);
    
    deleteButton_ = new wxButton(panel, ID_DELETE_TRANSACTION, "DELETE");
    deleteButton_->SetFont(buttonFont);
    deleteButton_->SetBackgroundColour(wxColour(231, 76, 60)); // Red for delete
    deleteButton_->SetForegroundColour(PURE_WHITE);
    deleteButton_->Enable(false);
    
    refreshButton_ = new wxButton(panel, ID_REFRESH, "REFRESH");
    refreshButton_->SetFont(buttonFont);
    refreshButton_->SetBackgroundColour(SOFT_MINT);
    refreshButton_->SetForegroundColour(BLACK_CHARCOAL);
    
    buttonSizer->Add(addButton_, 0, wxRIGHT, 10);
    buttonSizer->Add(editButton_, 0, wxRIGHT, 10);
    buttonSizer->Add(deleteButton_, 0, wxRIGHT, 10);
    buttonSizer->Add(refreshButton_, 0);
    
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);
    
    panel->SetSizer(mainSizer);
    return panel;
}

wxPanel* MainWindow::CreateListPanel(wxWindow* parent) {
    wxPanel* panel = new wxPanel(parent);
    panel->SetBackgroundColour(PURE_WHITE);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // Create font for header
    wxFont headerFont = wxFontInfo(18).Bold().FaceName("Segoe UI");
    
    // Header
    wxStaticText* headerLabel = new wxStaticText(panel, wxID_ANY, "TRANSACTIONS");
    headerLabel->SetFont(headerFont);
    headerLabel->SetForegroundColour(BLACK_CHARCOAL);
    sizer->Add(headerLabel, 0, wxALIGN_CENTER | wxALL, 10);
    
    // Transaction list
    transactionList_ = new wxListCtrl(panel, ID_TRANSACTION_LIST, wxDefaultPosition, wxDefaultSize, 
                                     wxLC_REPORT | wxLC_SINGLE_SEL);
    
    // Create font for list
    wxFont listFont = wxFontInfo(14).FaceName("Segoe UI");
    transactionList_->SetFont(listFont);
    transactionList_->SetBackgroundColour(PURE_WHITE);
    transactionList_->SetForegroundColour(BLACK_CHARCOAL);
    
    // Add columns
    transactionList_->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    transactionList_->AppendColumn("Date", wxLIST_FORMAT_LEFT, 100);
    transactionList_->AppendColumn("Description", wxLIST_FORMAT_LEFT, 200);
    transactionList_->AppendColumn("Category", wxLIST_FORMAT_LEFT, 120);
    transactionList_->AppendColumn("Type", wxLIST_FORMAT_LEFT, 80);
    transactionList_->AppendColumn("Amount", wxLIST_FORMAT_RIGHT, 100);
    
    sizer->Add(transactionList_, 1, wxEXPAND | wxALL, 10);
    
    panel->SetSizer(sizer);
    return panel;
}

void MainWindow::OnAddTransaction(wxCommandEvent& event) {
    wxString description = descriptionText_->GetValue().Trim();
    wxString amountStr = amountText_->GetValue().Trim();
    
    if (description.IsEmpty() || amountStr.IsEmpty()) {
        ShowNotification("Please fill in all fields", false);
        return;
    }
    
    double amount;
    if (!amountStr.ToDouble(&amount) || amount <= 0) {
        ShowNotification("Please enter a valid positive amount", false);
        return;
    }
    
    std::string category = categoryChoice_->GetStringSelection().ToStdString();
    TransactionType type = (typeChoice_->GetSelection() == 0) ? TransactionType::Income : TransactionType::Expense;
    
    // Use the correct method signature
    if (manager_.AddTransaction(description.ToStdString(), amount, category, type)) {
        ShowNotification("Transaction added successfully!");
        ClearInputFields();
    } else {
        ShowNotification("Failed to add transaction", false);
    }
}

void MainWindow::OnEditTransaction(wxCommandEvent& event) {
    if (selectedTransactionId_ == -1) {
        ShowNotification("Please select a transaction to edit", false);
        return;
    }
    
    wxString description = descriptionText_->GetValue().Trim();
    wxString amountStr = amountText_->GetValue().Trim();
    
    if (description.IsEmpty() || amountStr.IsEmpty()) {
        ShowNotification("Please fill in all fields", false);
        return;
    }
    
    double amount;
    if (!amountStr.ToDouble(&amount) || amount <= 0) {
        ShowNotification("Please enter a valid positive amount", false);
        return;
    }
    
    std::string category = categoryChoice_->GetStringSelection().ToStdString();
    TransactionType type = (typeChoice_->GetSelection() == 0) ? TransactionType::Income : TransactionType::Expense;
    
    // Use the correct method signature
    if (manager_.UpdateTransaction(selectedTransactionId_, description.ToStdString(), amount, category, type)) {
        ShowNotification("Transaction updated successfully!");
        ClearInputFields();
        selectedTransactionId_ = -1;
        editButton_->Enable(false);
        deleteButton_->Enable(false);
    } else {
        ShowNotification("Failed to update transaction", false);
    }
}

void MainWindow::OnDeleteTransaction(wxCommandEvent& event) {
    if (selectedTransactionId_ == -1) {
        ShowNotification("Please select a transaction to delete", false);
        return;
    }
    
    int result = wxMessageBox("Are you sure you want to delete this transaction?", 
                             "Confirm Delete", wxYES_NO | wxICON_QUESTION);
    
    if (result == wxYES) {
        if (manager_.DeleteTransaction(selectedTransactionId_)) {
            ShowNotification("Transaction deleted successfully!");
            ClearInputFields();
            selectedTransactionId_ = -1;
            editButton_->Enable(false);
            deleteButton_->Enable(false);
        } else {
            ShowNotification("Failed to delete transaction", false);
        }
    }
}

void MainWindow::OnRefresh(wxCommandEvent& event) {
    manager_.RefreshData();
    ShowNotification("Data refreshed!");
}

void MainWindow::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainWindow::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Personal Finance Tracker v1.0\n\nA simple application to track your income and expenses.",
                 "About Personal Finance Tracker", wxOK | wxICON_INFORMATION);
}

void MainWindow::OnTransactionSelected(wxListEvent& event) {
    long selectedIndex = event.GetIndex();
    if (selectedIndex >= 0) {
        wxString idStr = transactionList_->GetItemText(selectedIndex, 0);
        long id;
        if (idStr.ToLong(&id)) {
            selectedTransactionId_ = static_cast<int>(id);
            
            // Get transaction details and populate fields
            const auto& transactions = manager_.GetTransactions();
            for (const auto& transaction : transactions) {
                if (transaction.id == selectedTransactionId_) {
                    PopulateInputFields(transaction);
                    editButton_->Enable(true);
                    deleteButton_->Enable(true);
                    break;
                }
            }
        }
    }
}

void MainWindow::RefreshTransactionList() {
    if (!transactionList_) return;
    
    transactionList_->DeleteAllItems();
    
    const auto& transactions = manager_.GetTransactions();
    
    for (size_t i = 0; i < transactions.size(); ++i) {
        const auto& transaction = transactions[i];
        
        long index = transactionList_->InsertItem(i, wxString::Format("%d", transaction.id));
        transactionList_->SetItem(index, 1, transaction.GetDateString());
        transactionList_->SetItem(index, 2, transaction.description);
        transactionList_->SetItem(index, 3, transaction.category);
        transactionList_->SetItem(index, 4, transaction.GetTypeString());
        transactionList_->SetItem(index, 5, wxString::Format("$%.2f", transaction.amount));
        
        // Color coding for transaction types
        if (transaction.type == TransactionType::Income) {
            transactionList_->SetItemTextColour(index, EMERALD_GREEN);
        } else {
            transactionList_->SetItemTextColour(index, BLACK_CHARCOAL);
        }
        
        // Alternating row colors
        if (i % 2 == 1) {
            transactionList_->SetItemBackgroundColour(index, wxColour(248, 249, 250));
        }
    }
    
    // Refresh the list control
    transactionList_->Refresh();
}

void MainWindow::RefreshSummary() {
    if (!totalIncomeLabel_ || !totalExpensesLabel_ || !balanceLabel_) return;
    
    // Use the individual methods instead of GetAnalytics()
    double totalIncome = manager_.GetTotalIncome();
    double totalExpenses = manager_.GetTotalExpenses();
    double balance = manager_.GetBalance();
    
    // Update income label
    totalIncomeLabel_->SetLabel(wxString::Format("$%.2f", totalIncome));
    totalIncomeLabel_->Refresh();
    
    // Update expenses label  
    totalExpensesLabel_->SetLabel(wxString::Format("$%.2f", totalExpenses));
    totalExpensesLabel_->Refresh();
    
    // Update balance label with dynamic coloring
    balanceLabel_->SetLabel(wxString::Format("$%.2f", balance));
    
    if (balance >= 0) {
        balanceLabel_->SetForegroundColour(EMERALD_GREEN);
    } else {
        balanceLabel_->SetForegroundColour(wxColour(231, 76, 60)); // Red for negative balance
    }
    balanceLabel_->Refresh();
}

void MainWindow::ClearInputFields() {
    if (descriptionText_) descriptionText_->Clear();
    if (amountText_) amountText_->Clear();
    if (categoryChoice_) categoryChoice_->SetSelection(0);
    if (typeChoice_) typeChoice_->SetSelection(1); // Default to Expense
    if (datePicker_) datePicker_->SetValue(wxDateTime::Now());
}

void MainWindow::PopulateInputFields(const Transaction& transaction) {
    if (descriptionText_) descriptionText_->SetValue(transaction.description);
    if (amountText_) amountText_->SetValue(wxString::Format("%.2f", transaction.amount));
    
    if (categoryChoice_) {
        int categoryIndex = categoryChoice_->FindString(transaction.category);
        if (categoryIndex != wxNOT_FOUND) {
            categoryChoice_->SetSelection(categoryIndex);
        }
    }
    
    if (typeChoice_) {
        typeChoice_->SetSelection(transaction.type == TransactionType::Income ? 0 : 1);
    }
    
    if (datePicker_) {
        wxDateTime date;
        date.Set(static_cast<time_t>(transaction.date));
        datePicker_->SetValue(date);
    }
}

void MainWindow::ShowNotification(const wxString& message, bool isSuccess) {
    if (isSuccess) {
        SetStatusText(message);
    } else {
        wxMessageBox(message, "Error", wxOK | wxICON_ERROR);
        SetStatusText("Error occurred");
    }
}