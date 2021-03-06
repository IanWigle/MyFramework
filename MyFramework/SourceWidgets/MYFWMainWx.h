//
// Copyright (c) 2012-2015 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __MYFWMainWx_H__
#define __MYFWMainWx_H__

#include "wx/wx.h"
#include "wx/glcanvas.h"

class MainFrame;
class MainGLCanvas;
class CommandStack;
class MainApp;

enum EditorWindowTypes
{
    EditorWindow_Game,
    EditorWindow_PanelObjectList,
    EditorWindow_PanelWatch,
    EditorWindow_PanelMemory,
    EditorWindow_NumTypes,
};

enum MenuIDs
{
    myID_View_SavePerspective = wxID_HIGHEST + 1,
    myID_View_LoadPerspective,
    myID_View_ResetPerspective,
    myID_View_EditorWindows,
    myID_EditorWindow_FirstWindow,
    myID_EditorWindow_Game = myID_EditorWindow_FirstWindow,
    myID_EditorWindow_PanelObjectList,
    myID_EditorWindow_PanelWatch,
    myID_EditorWindow_PanelMemory,
    myID_GLViewType_Fill,
    myID_GLViewType_Tall,
    myID_GLViewType_Square,
    myID_GLViewType_Wide,
    myID_Undo,
    myID_Redo,
    myID_LastID,
};

struct EditorInputEvent
{
    int keychar;
    int keyaction;
    int keycode;
    int mouseaction;
    int mousebuttonid;
    float x;
    float y;
    float pressure;

    EditorInputEvent(int keychar, int keyaction, int keycode, int mouseaction, int id, float x, float y, float pressure)
    {
        this->keychar = keychar;
        this->keyaction = keyaction;
        this->keycode = keycode;
        this->mouseaction = mouseaction;
        this->mousebuttonid = id;
        this->x = x;
        this->y = y;
        this->pressure = pressure;
    }
};

void WinMain_CreateGameCore();
MainFrame* WinMain_CreateMainFrame();
void WinMain_GetClientSize(int* width, int* height, GLViewTypes* viewtype);

extern unsigned int g_GLCanvasIDActive;
extern wxDataFormat* g_pMyDataFormat;

void SetMouseLock(bool lock);
bool IsMouseLocked();

extern MainApp* g_pMainApp;

// Main application class
class MainApp : public wxApp
{
public:
    MainFrame* m_pMainFrame;

    bool m_HasFocus;

public:
    MainApp();
    ~MainApp();

    virtual bool OnInit();
    virtual int FilterEvent(wxEvent& event);
};

// Main frame class
class MainFrame : public wxFrame
{
public:
    wxMenuBar* m_MenuBar;
    wxMenu* m_File;
    wxMenu* m_Edit;
    wxMenu* m_View;
    wxMenu* m_Aspect;

    wxMenu* m_EditorWindows;
    wxMenuItem* m_EditorWindowOptions[EditorWindow_NumTypes];

    wxMenuItem* m_AspectMenuItems[GLView_NumTypes];

    wxAuiManager m_AUIManager;
    MainGLCanvas* m_pGLCanvas;

    CommandStack* m_pCommandStack;

    wxString m_DefaultPerspectiveString;
    wxString m_SavedPerspectiveString;

    int m_WindowX;
    int m_WindowY;
    int m_ClientWidth;
    int m_ClientHeight;
    bool m_Maximized;

public:
    MainFrame(wxWindow* parent);
    ~MainFrame();

    MainGLCanvas* GetGLCanvas() { return m_pGLCanvas; }

    virtual void InitFrame();
    virtual void AddPanes();
    virtual bool UpdateAUIManagerAndLoadPerspective(); // returns true if valid window layout was set.
    virtual void OnPostInit() {}
    virtual bool OnClose();

    virtual bool FilterGlobalEvents(wxEvent& event) { return false; }
    virtual void OnGLCanvasShownOrHidden(bool shown) {}
    
    void OnMove(wxMoveEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnCloseWindow(wxCloseEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnMenu(wxCommandEvent& event);
    //void OnKeyPressed(wxKeyEvent& event);
    //void OnKeyReleased(wxKeyEvent& event);

    virtual void ResizeViewport();

    virtual void UpdateMenuItemStates();

    virtual void ProcessAllGLCanvasInputEventQueues();
};

// Main gl canvas class
class MainGLCanvas : public wxGLCanvas
{
public:
    std::vector<EditorInputEvent> m_InputEventQueue;

    unsigned int m_GLCanvasID;

    bool m_KeysDown[512];
    int m_MouseCaptured_ButtonsHeld;
    int m_MouseButtonStates;
    Vector3 m_MousePosition; // x, y and wheel
    double m_LastTimeTicked;

    bool m_TickGameCore;
 
    int m_CurrentGLViewWidth;
    int m_CurrentGLViewHeight;

    // Vars for mouse lock.
    bool m_RawMouseInputInitialized;

    bool m_GameWantsLockedMouse;
    bool m_SystemMouseIsLocked;

    int m_MouseXPositionWhenLocked;
    int m_MouseYPositionWhenLocked;

public:
    MainGLCanvas(wxWindow* parent, int* args, unsigned int ID, bool tickgamecore);
    virtual ~MainGLCanvas();

#if MYFW_WINDOWS
    void RequestRawMouseAccess();
    // Override from wxWindowMSW class to handle WM_INPUT messages (higher res mouse)
    virtual bool MSWHandleMessage(WXLRESULT* result, WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif

    void MakeContextCurrent();
 
    void Resized(wxSizeEvent& evt);
    void ResizeViewport(bool clearhack = true);

    void Idle(wxIdleEvent& evt);
    void Render(wxPaintEvent& evt);

    void ProcessInputEventQueue();

    void Draw();

    // Mouse Lock Methods
    void LockMouse(bool lock);
    void RequestMouseLock();
    void ReleaseMouseLock();
    bool IsMouseLocked();

    // events
    DECLARE_EVENT_TABLE()
    void MouseMoved(wxMouseEvent& event);
    void MouseWheelMoved(wxMouseEvent& event);
    void MouseLeftDown(wxMouseEvent& event);
    void MouseLeftDoubleClick(wxMouseEvent& event);
    void MouseLeftUp(wxMouseEvent& event);
    void MouseRightDown(wxMouseEvent& event);
    void MouseRightUp(wxMouseEvent& event);
    void MouseRightDoubleClick(wxMouseEvent& event);
    void MouseMiddleDown(wxMouseEvent& event);
    void MouseMiddleUp(wxMouseEvent& event);
    void MouseMiddleDoubleClick(wxMouseEvent& event);
    void MouseLeftWindow(wxMouseEvent& event);
    void MouseCaptureLost(wxMouseCaptureLostEvent& event);
    void KeyPressed(wxKeyEvent& event);
    void KeyReleased(wxKeyEvent& event);
    void KeyChar(wxKeyEvent& event);
};

#endif //__MYFWMainWx_H__
