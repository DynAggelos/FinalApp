/**************************************************************************
 * Name:      TetherFrame.cpp
 * Purpose:   Code for Application Frame
 * Author:    Joshua Gerrity (comments@dynaggelos.com)
 * Created:   2018-03-04
 * Copyright: Joshua Gerrity (biblicalwiki.com)
 * License:
 *************************************************************************/

#include <string>

#include "TetherFrame.h"

#include "imgs/UndoImage.xpm"
#include "imgs/RedoImage.xpm"

/* Constructor: TetherFrame() *********************************************
 * Creates and binds all the elements of the main window frame in the app.
 *-------------------------------------------------------------------------
 * Parameters: void
 * Returns: void
 *************************************************************************/
TetherFrame::TetherFrame()
    : wxFrame(NULL, -1, _("Tether"))
{
    /* Position and Size Frame */
    this->SetSize(wxSize(700, 600));
    this->SetMinSize(wxSize(400, 400));
    this->Center(wxBOTH);

    /* Create Menu Bar Above */
    this->createMenu();

    /* Create Status Bar Below */
    this->CreateStatusBar(2);

    /* Create Toolbar */
    this->createToolbar();

    /* Set up textBox1 */
    textBox1 = new TetherTextCtrl(
        this);

    /* Bind Events */
    // Frame Binding
    Bind(wxEVT_CLOSE_WINDOW, &TetherFrame::OnClose, this, wxID_CLOSE);

    // Menu Bindings
    Bind(wxEVT_MENU, &TetherFrame::onNewFile, this, wxID_NEW);
    Bind(wxEVT_MENU, &TetherFrame::onOpenFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &TetherFrame::onSaveFile, this, wxID_SAVE);
    Bind(wxEVT_MENU, &TetherFrame::onSaveAsFile, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &TetherFrame::onSettings, this, wxID_PREFERENCES);
    Bind(wxEVT_MENU, &TetherFrame::onUndoTextAction, this, wxID_UNDO);
    Bind(wxEVT_MENU, &TetherFrame::onRedoTextAction, this, wxID_REDO);
}

/* Destructor: ~TetherFrame() *********************************************
 * An empty destructor.
 *-------------------------------------------------------------------------
 * Parameters: void
 * Returns: void
 *************************************************************************/
TetherFrame::~TetherFrame()
{}

/* Member Function: createMenu() ******************************************
 * Handles the creation of the menubar and all of its objects.
 *-------------------------------------------------------------------------
 * Parameters: void
 * Returns: void
 *************************************************************************/
void TetherFrame::createMenu()
{
    menuBar = new wxMenuBar();

    /* Create File Menu */
    fileMenu = new wxMenu();
    fileMenu->SetTitle(_("Files"));

    fileMenu->Append(wxID_NEW, _("New"), _("Start a new file"));
    fileMenu->Append(wxID_OPEN, _("Open"), _("Open an existing file"));
    fileMenu->Append(wxID_SAVE, _("Save"), _("Save an open file"));
    fileMenu->Append(
        wxID_SAVEAS,
        _("Save As"),
        _("Specify how to save an open file"));

    /* Create Tools Menu */
    toolsMenu = new wxMenu();
    toolsMenu->SetTitle(_("Tools"));
    toolsMenu->Append(
        wxID_PREFERENCES, _("Settings"), _("Open up the settings window"));

    /* Append Menus to Menubar */
    menuBar->Append(fileMenu, fileMenu->GetTitle());
    menuBar->Append(toolsMenu, toolsMenu->GetTitle());

    /* Associate Menubar with Frame */
    this->SetMenuBar(menuBar);
}

/* Member Function: createToolbar() ***************************************
 * Handles the creation of the toolbar and all of its objects.
 *-------------------------------------------------------------------------
 * Parameters: void
 * Returns: void
 *************************************************************************/
void TetherFrame::createToolbar()
{
    /* Instantiate Toolbar Bitmaps */
    //toolBitmap = new wxBitmap(RedoImage_xpm);
    wxBitmap undoBitmap(UndoImage_xpm);      // Only declared here, not in H
    wxBitmap redoBitmap(RedoImage_xpm);      // Only declared here, not in H

    /* Instantiate Toolbar */
    toolbar = this->CreateToolBar(
        wxTB_DEFAULT_STYLE
        | wxTB_TOP
        | wxTB_HORZ_TEXT);

    toolbar->AddTool(
        wxID_UNDO,
        _("Undo"),
        undoBitmap,
        _("Undo operation"));
    toolbar->AddTool(
        wxID_REDO,
        _("Redo"),
        redoBitmap,
        _("Redo operation"));

    /* Display Toolbar */
    toolbar->Realize();
}

/* Member Function: onNewFile() *******************************************
 * Clears textBox1 when the associated menu event takes place.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onNewFile(const wxCommandEvent& event)
{
    /* Initialization ****************************************************/
    std::string noticeBoxDialogue = (
        "Changes are unsaved.\n"
        "Are you sure you want to clear the window of text?");
    int buttonChoice = 0;

    /* Processing ********************************************************/
    /* If Text Modified/Unsaved -----------------------------------------*/
    if (textBox1->IsModified())
    {
        /* Display Warning of Unsaved Content */
        noticeBox = new wxMessageDialog(
            this,
            _(noticeBoxDialogue),
            wxMessageBoxCaptionStr,
            wxYES_NO | wxICON_WARNING | wxCENTRE);

        buttonChoice = noticeBox->ShowModal();

        /* Actions to Take Upon Selection Choice */
        if (buttonChoice == wxID_YES)
        {
            // Clear textBox1
            textBox1->ClearAll();           // Clear textBox1 Text
            textBox1->DiscardEdits();       // Clear Unsaved Edits Flag
            textBox1->EmptyUndoBuffer();    // Clear Undo Buffer

            // Clear Memory of noticeBox
            delete noticeBox;
            noticeBox = nullptr;
        }

        else
        {
            // Clear Memory of noticeBox
            delete noticeBox;
            noticeBox = nullptr;
        }
    }

    /* If Text Unmodified/Saved -----------------------------------------*/
    else
    {
        textBox1->ClearAll();
    }
}

/* Member Function: onOpenFile() ******************************************
 * Creates a new file window frame when the associated menu event takes
 * place. Loads the selected file into textBox1.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onOpenFile(const wxCommandEvent& event)
{
    /* Initialization ****************************************************/
    std::string noticeBoxDialogue = (
        "There are unsaved changes at the moment.");

    /* Processing ********************************************************/
    /* Warning Box if Unsaved Content */
    if (textBox1->IsModified())
    {
        noticeBox = new wxMessageDialog(
            this,
            _(noticeBoxDialogue),
            wxMessageBoxCaptionStr,
            wxOK | wxICON_WARNING | wxCENTRE);
        noticeBox->ShowModal();

        // Clear Memory Once Any Selection Chosen
        delete noticeBox;
        noticeBox = nullptr;
    }

    /* Instantiate File Frame and Process Choice */
    fileFrame = new TetherFileFrame(
        this,
        wxID_AUTO_HIGHEST,
        _("Load"),
        wxT("load"));

    /* Get Chosen File Path for Processing */
    filePath = fileFrame->getFilePath();

    /* If Cancel Was Chosen */
    if (filePath == wxT(""))
    {
        // Do nothing
    }

    /* If File Doesn't Exist */
    else if (wxFileExists(filePath) == false)
    {
        noticeBox = new wxMessageDialog(
            this,
            _("Oops! That file couldn't be loaded."),
            wxMessageBoxCaptionStr,
            wxOK | wxICON_ERROR | wxCENTRE);
        noticeBox->ShowModal();

        // Clear Memory Once Selection Chosen
        delete noticeBox;
        noticeBox = nullptr;
    }

    /* Else Load File into Text Box 1 */
    else
    {
        textBox1->LoadFile(filePath);
        textBox1LoadPath = filePath;
    }
}

/* Member Function: onSaveFile() ******************************************
 * Saves the file from textBox1 with the same path it was opened with.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onSaveFile(const wxCommandEvent& event)
{
    /* Don't Save if No Changes */
    if (textBox1->IsModified() == false)
    {
        // Do nothing
    }

    /* Proceed with Save if Changes */
    else
    {
        textBox1->SaveFile(textBox1LoadPath);
    }
}

/* Member Function: onSaveAsFile() ****************************************
 * Creates a new file window frame when the associated menu event takes
 * place. Saves the file from textBox1 as specified.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onSaveAsFile(const wxCommandEvent& event)
{
    /* Instantiate File Frame and Process Choice */
    fileFrame = new TetherFileFrame(
        this,
        wxID_AUTO_HIGHEST,
        _("Save"),
        wxT("save"));

    /* Get Chosen File Path for Processing */
    filePath = fileFrame->getFilePath();

    /* If Cancel Was Chosen */
    if (filePath == wxT(""))
    {
        // Do nothing
    }

    /* Else Save File */
    else
    {
        textBox1->SaveFile(filePath);
    }
}

/* Member Function: onSettings() ******************************************
 * Creates a settings window that allows the user to manipulate how the
 * program works.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onSettings(const wxCommandEvent& event)
{
    /*optionsFrame = new TetherOptionsDialogue(
        this,
        wxID_ANY,
        _("Settings"),
        wxDefaultPosition,
        wxDefaultSize,
        wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP);*/
}

/* Member Function: onUndoTextAction() ************************************
 * Removes the last change in the buffer from the text.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onUndoTextAction(const wxCommandEvent& event)
{
    /* Processing ********************************************************/
    /* If Undo Possible -------------------------------------------------*/
    if (textBox1->CanUndo())
    {
        textBox1->Undo();
    }
}

/* Member Function: onRedoTextAction() ************************************
 * Adds the next change in the buffer back to the text.
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::onRedoTextAction(const wxCommandEvent& event)
{
    /* Processing ********************************************************/
    /* If Redo Possible -------------------------------------------------*/
    if (textBox1->CanRedo())
    {
        textBox1->Redo();
    }
}

/* Member Function: OnClose() *********************************************
 * Destroys the Tether window frame when the close event takes place (the
 * X button on the window is used).
 *-------------------------------------------------------------------------
 * Parameters:
 *      event -- A parameter required by wxWidgets
 * Returns: void
 *************************************************************************/
void TetherFrame::OnClose(const wxCloseEvent& event)
{
    Destroy();
}