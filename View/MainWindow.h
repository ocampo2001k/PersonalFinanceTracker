#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/choice.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include "../ViewModel/TransactionManager.h"

class MainWindow : public wxFrame {
public:
    explicit MainWindow(TransactionManager& manager);

private:
    // Event handlers
    void OnAddTransaction(wxCommandEvent& event);
    void OnEditTransaction(wxCommandEvent& event);
    void OnDeleteTransaction(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTransactionSelected(wxListEvent& event);
    
    // UI update methods
    void RefreshTransactionList();
    void RefreshSummary();
    void ClearInputFields();
    void PopulateInputFields(const Transaction& transaction);
    void ShowNotification(const wxString& message, bool isSuccess = true);
    
    // UI Creation methods
    void CreateMenuBar();
    void CreateControls();
    wxPanel* CreateSummaryPanel(wxWindow* parent);
    wxPanel* CreateInputPanel(wxWindow* parent);
    wxPanel* CreateListPanel(wxWindow* parent);
    
    // Member variables
    TransactionManager& manager_;
    int selectedTransactionId_;
    
    // UI Controls
    wxListCtrl* transactionList_;
    wxTextCtrl* descriptionText_;
    wxTextCtrl* amountText_;
    wxChoice* categoryChoice_;
    wxChoice* typeChoice_;
    wxDatePickerCtrl* datePicker_;
    
    wxButton* addButton_;
    wxButton* editButton_;
    wxButton* deleteButton_;
    wxButton* refreshButton_;
    
    // Summary labels
    wxStaticText* totalIncomeLabel_;
    wxStaticText* totalExpensesLabel_;
    wxStaticText* balanceLabel_;
    
    // Event IDs
    enum {
        ID_ADD_TRANSACTION = 1000,
        ID_EDIT_TRANSACTION,
        ID_DELETE_TRANSACTION,
        ID_REFRESH,
        ID_TRANSACTION_LIST
    };
    
    wxDECLARE_EVENT_TABLE();
}; 