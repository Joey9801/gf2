#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "errordialog.h"

ErrorDialog::ErrorDialog(wxWindow *parent, wxWindowID id, const wxString & title)
  : wxDialog(parent, id, title)
{
  _errorlist = new wxListView(this, -1);

  // Add first column
  wxListItem col0;
  col0.SetId(0);
  col0.SetText( _("Type") );
  col0.SetWidth(100);
  _errorlist->InsertColumn(0, col0);

  // Add second column
  wxListItem col1;
  col1.SetId(1);
  col1.SetText( _("Description") );
  col1.SetWidth(100);
  _errorlist->InsertColumn(1, col1);

  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(_errorlist, 1, wxEXPAND, 0);
  this->SetSizer(sizer);
  sizer->SetSizeHints(this);
}

void ErrorDialog::showErrorList(ErrorList *list)
{
  if(list->anyErrors()){
    std::vector<std::string> errors = list->formatErrors();
    for(std::vector<std::string>::size_type i = 0; i != errors.size(); i++) {
      _errorlist->InsertItem(1, wxString(errors[i]));
    }
  }
}

void ErrorDialog::clearErrorList()
{
  _errorlist->DeleteAllItems();
}
