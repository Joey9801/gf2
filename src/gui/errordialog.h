#ifndef errordialog_h
#define errordialog_h

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
  #include <wx/wx.h>
#endif

#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/htmllbox.h>

#include <string>
#include <vector>

#include "../errors/error.h"

class ErrorDialog: public wxDialog
{
  public:
    ErrorDialog(wxWindow *parent, wxWindowID id, const wxString & title);

    void showErrorList(void);
    void clearErrorList(void);

    void setErrorList(ErrorList * list);
    ErrorList * getErrorList(void);

  private:
    ErrorList * _list;
    wxSimpleHtmlListBox *_errorlist;
};

#endif /*errordialog.h*/
