/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/artqt.cpp
// Purpose:     stock wxArtProvider instance with native Qt stock icons
// Author:      Kettab Ali
// Created:     2026-02-12
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"


#include "wx/artprov.h"
#include "wx/bitmap.h"
#include "wx/qt/private/converter.h"

#include <QtGui/QIcon>

#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>

namespace // anonymous
{
// This helper function tries to return the standard icon QStyle::StandardPixmap
// corresponding to the wxArtID if it exist.
QIcon GetQtIconFromArtID(const wxArtID& id)
{
    QStyle::StandardPixmap standardIcon;

    if ( id == wxART_GO_BACK )
        standardIcon = QStyle::SP_ArrowBack;
    else if ( id == wxART_GO_FORWARD )
        standardIcon = QStyle::SP_ArrowForward;
    else if ( id == wxART_GO_UP )
        standardIcon = QStyle::SP_ArrowUp;
    else if ( id == wxART_GO_DOWN )
        standardIcon = QStyle::SP_ArrowDown;
    else if ( id == wxART_GO_TO_PARENT )
        standardIcon = QStyle::SP_FileDialogToParent;
    else if ( id == wxART_GO_HOME )
        standardIcon = QStyle::SP_DirHomeIcon;
    else if ( id == wxART_FILE_OPEN )
        standardIcon = QStyle::SP_DialogOpenButton;
    else if ( id == wxART_FILE_SAVE )
        standardIcon = QStyle::SP_DialogSaveButton;
    else if ( id == wxART_HELP )
        standardIcon = QStyle::SP_DialogHelpButton;
    else if ( id == wxART_LIST_VIEW )
        standardIcon = QStyle::SP_FileDialogListView;
    else if ( id == wxART_NEW_DIR )
        standardIcon = QStyle::SP_FileDialogNewFolder;
    else if ( id == wxART_HARDDISK )
        standardIcon = QStyle::SP_DriveHDIcon;
    else if ( id == wxART_FLOPPY )
        standardIcon = QStyle::SP_DriveFDIcon;
    else if ( id == wxART_CDROM )
        standardIcon = QStyle::SP_DriveCDIcon;
    else if ( id == wxART_FOLDER )
        standardIcon = QStyle::SP_DirIcon;
    else if ( id == wxART_FOLDER_OPEN )
        standardIcon = QStyle::SP_DirOpenIcon;
    else if ( id == wxART_NORMAL_FILE )
        standardIcon = QStyle::SP_FileIcon;
    else if ( id == wxART_TICK_MARK )
        standardIcon = QStyle::SP_DialogOkButton;
    else if ( id == wxART_CROSS_MARK )
        standardIcon = QStyle::SP_DialogCancelButton;
    else if ( id == wxART_ERROR )
        standardIcon = QStyle::SP_MessageBoxCritical;
    else if ( id == wxART_QUESTION )
        standardIcon = QStyle::SP_MessageBoxQuestion;
    else if ( id == wxART_WARNING )
        standardIcon = QStyle::SP_MessageBoxWarning;
    else if ( id == wxART_INFORMATION )
        standardIcon = QStyle::SP_MessageBoxInformation;
    else if ( id == wxART_CLOSE )
        standardIcon = QStyle::SP_DialogCloseButton;
    else if ( id == wxART_REFRESH )
        standardIcon = QStyle::SP_BrowserReload;
    else if ( id == wxART_STOP )
        standardIcon = QStyle::SP_BrowserStop;
    else
        standardIcon = QStyle::SP_CustomBase;

    if ( standardIcon != QStyle::SP_CustomBase )
    {
        return QApplication::style()->standardIcon(standardIcon);
    }

    return QIcon();
}
} // anonymous

// ----------------------------------------------------------------------------
// wxQtArtProvider
// ----------------------------------------------------------------------------

class wxQtArtProvider : public wxArtProvider
{
protected:
    virtual wxBitmap CreateBitmap(const wxArtID& id,
                                  const wxArtClient& client,
                                  const wxSize& size) override;
};

wxBitmap wxQtArtProvider::CreateBitmap(const wxArtID& id,
                                       const wxArtClient& client,
                                       const wxSize& size)
{
    wxSize iconSize = size != wxDefaultSize
                    ? size : GetNativeDIPSizeHint(client);

    if ( iconSize != wxDefaultSize )
    {
        const QIcon qtIcon = GetQtIconFromArtID(id);

        if ( !qtIcon.isNull() )
        {
            return wxBitmap(qtIcon.pixmap(wxQtConvertSize(iconSize)));
        }
    }

    return wxNullBitmap;
}

// ----------------------------------------------------------------------------
// wxArtProvider::InitNativeProvider
// ----------------------------------------------------------------------------

/*static*/
void wxArtProvider::InitNativeProvider()
{
    PushBack(new wxQtArtProvider);
}

// ----------------------------------------------------------------------------
// wxArtProvider::GetNativeSizeHint()
// ----------------------------------------------------------------------------

/*static*/
wxSize wxArtProvider::GetNativeDIPSizeHint(const wxArtClient& client)
{
    QStyle* const qtStyle = QApplication::style();

    int pm = -1;

    if ( client == wxART_TOOLBAR )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_ToolBarIconSize);
    }
    else if ( client == wxART_MENU )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_SmallIconSize);
    }
    else if ( client == wxART_FRAME_ICON )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_SmallIconSize);
    }
    else if ( client == wxART_CMN_DIALOG ||
              client == wxART_MESSAGE_BOX )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_MessageBoxIconSize);
    }
    else if ( client == wxART_BUTTON )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_ButtonIconSize);
    }
    else if ( client == wxART_LIST )
    {
        pm = qtStyle->pixelMetric(QStyle::PM_ListViewIconSize);
    }

    return wxSize(pm, pm);
}
