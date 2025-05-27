#include <wx/wx.h>
#include "View/MainWindow.h"
#include "ViewModel/TransactionManager.h"
#include <iostream>

class PersonalFinanceApp : public wxApp {
public:
    virtual bool OnInit() override;
    virtual int OnExit() override;

private:
    std::unique_ptr<TransactionManager> transactionManager_;
};

bool PersonalFinanceApp::OnInit() {
    // Initialize the transaction manager with database
    std::string dbPath = "finance_tracker.db";
    transactionManager_ = std::make_unique<TransactionManager>(dbPath);
    
    if (!transactionManager_->IsInitialized()) {
        wxMessageBox("Failed to initialize database. The application will exit.", 
                     "Database Error", wxOK | wxICON_ERROR);
        return false;
    }
    
    // Create and show the main window
    MainWindow* mainWindow = new MainWindow(*transactionManager_);
    mainWindow->Show(true);
    
    return true;
}

int PersonalFinanceApp::OnExit() {
    // Cleanup will be handled automatically by smart pointers
    return wxApp::OnExit();
}

// Implement the application
wxIMPLEMENT_APP(PersonalFinanceApp); 