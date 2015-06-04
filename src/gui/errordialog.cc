#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "errordialog.h"

ErrorDialog::ErrorDialog(wxWindow *parent, wxWindowID id, const wxString & title)
  : wxDialog(parent, id, title)
{
  _errorlist = new wxSimpleHtmlListBox(this, -1, wxDefaultPosition, wxSize(800, 200));

  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(_errorlist, 1, wxEXPAND, 0);
  this->SetSizer(sizer);
  sizer->SetSizeHints(this);
}

void ErrorDialog::showErrorList(void)
{
  LOG_DEBUG;
  if(_list->anyErrors()){
    std::vector<std::string> errors = _list->formatErrors();
    for(std::vector<std::string>::size_type i = 0; i != errors.size(); i++) {
      _errorlist->Append(wxString(errors[i]));
    }
  }
}

void ErrorDialog::clearErrorList()
{
  _errorlist->Clear();
}

void ErrorDialog::setErrorList(ErrorList * list) {
  _list = list;
  return;
}

ErrorList * ErrorDialog::getErrorList(void) {
  return _list;
}
