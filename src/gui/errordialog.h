#ifndef errordialog_h
#define errordialog_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>

#include <string>
#include <vector>

#include "../errors/error.h"

class ErrorDialog: public wxDialog
{
  public:
    ErrorDialog(wxWindow *parent, wxWindowID id, const wxString & title);

    void showErrorList(ErrorList *list);
    void clearErrorList();
  private:
    wxListView *_errorlist;
};

#endif /*errordialog.h*/
