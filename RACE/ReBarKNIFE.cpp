#include "stdafx.h"

HWND CreateKNIFEToolbar( HWND hwnd )
{
    HWND toolBar;
    HFONT defaultFont;
    RECT recToolBar;
    
    
    TBADDBITMAP tbab;

    int imageOffset = 0;

    defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    toolBar = CreateWindowEx( 0,//TBSTYLE_EX_DRAWDDARROWS,
                              TOOLBARCLASSNAME,
                              NULL,
                              WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TRANSPARENT,
                              0, 0, 0, 0,
                              hwnd,
                              (HMENU)CONTORLS_TOOLBAR,
                              GetModuleHandle(0),
                              NULL);
    
    if( toolBar == NULL )
        return NULL;
    
    HBITMAP hBitmap = LoadBitmap( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP2) );
    HBITMAP hBitmap2 = LoadBitmap( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP3) );
    HIMAGELIST hImageList = ImageList_Create( 24, 24, ILC_COLOR32 | ILC_MASK, 5, 5 ); //ImageList_LoadImage( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP2), 24, 10, RGB(0,0,0), IMAGE_BITMAP, LR_DEFAULTCOLOR | LR_LOADTRANSPARENT );
    ImageList_Add( hImageList, hBitmap, hBitmap2 );
    //ImageList_AddMasked( hImageList, hBitmap, RGB(0,0,0) );

    DeleteObject( hBitmap );

    DWORD lastError = ::GetLastError();

    SendMessage( toolBar, TB_SETIMAGELIST, 0, (LPARAM)hImageList );

    TBBUTTON tbb[5] = 
    {
        { MAKELONG(4, 0), ID_FILE_NEWSTUDY, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"New Study" },
        { MAKELONG(3, 0), ID_FILE_LOADSTUDY, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Load Study" },
        { MAKELONG(0, 0), ONESERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"One Series" },
        { MAKELONG(1, 0), TWOSERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Two Series" },
        { MAKELONG(2, 0), FOURSERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Four Series" }
    };

    SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(toolBar, TB_ADDBUTTONS, 5, (LPARAM)&tbb );

    SendMessage( toolBar, TB_AUTOSIZE, 0, 0 );

    SendMessage(toolBar, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE, 0));

    ShowWindow( toolBar, TRUE );

    return toolBar;
}

HWND CreateKNIFERebar( HWND hwnd, HWND toolbar )
{
    HWND rebar;
    REBARINFO rbi = { 0 };
    REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };

    rbBand.cbSize = sizeof( REBARBANDINFO );

    rebar = CreateWindowEx( WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL, WS_CHILD 
																	| WS_VISIBLE 
																	| WS_CLIPSIBLINGS 
																	| RBS_AUTOSIZE 
																	| WS_CLIPCHILDREN 
																	| RBS_VARHEIGHT 
																	| CCS_NODIVIDER 
																	| RBS_BANDBORDERS,
																	  0,0,0,0, hwnd, (HMENU)CONTROLS_REBAR, GetModuleHandle(0), NULL );

    if( !rebar )
        return NULL;

    rbi.cbSize = sizeof( REBARINFO );

    SendMessage( rebar, RB_SETBARINFO, 0, (LPARAM)&rbi );


    rbBand.fMask = RBBIM_STYLE |
                   RBBIM_TEXT  |
                   RBBIM_CHILD |
                   RBBIM_CHILDSIZE |
                   RBBIM_SIZE;
    rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;

    unsigned long toolbarSize = (unsigned long)SendMessage( toolbar, TB_GETBUTTONSIZE, 0, 0 );

    rbBand.lpText = TEXT("");
    rbBand.hwndChild = toolbar;
    rbBand.cyChild = HIWORD(toolbarSize);
    rbBand.cxMinChild = 3 * LOWORD(toolbarSize);
    rbBand.cyMinChild = HIWORD(toolbarSize);
    rbBand.cx = 0;

    SendMessage( rebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand );

    return rebar;
}

//HWND CreateKNIFEToolbar( HWND hwnd )
//{
//    HWND toolBar;
//    HFONT defaultFont;
//    RECT recToolBar;
//    
//    
//    TBADDBITMAP tbab;
//
//    int imageOffset = 0;
//
//    defaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
//
//    toolBar = CreateWindowEx( 0,//TBSTYLE_EX_DRAWDDARROWS,
//                              TOOLBARCLASSNAME,
//                              NULL,
//                              WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TRANSPARENT,
//                              0, 0, 0, 0,
//                              hwnd,
//                              (HMENU)CONTORLS_TOOLBAR,
//                              GetModuleHandle(0),
//                              NULL);
//    
//    if( toolBar == NULL )
//        return NULL;
//    
//    HBITMAP hBitmap = LoadBitmap( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP2) );
//    HBITMAP hBitmap2 = LoadBitmap( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP3) );
//    HIMAGELIST hImageList = ImageList_Create( 24, 24, ILC_COLOR32 | ILC_MASK, 5, 5 ); //ImageList_LoadImage( GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP2), 24, 10, RGB(0,0,0), IMAGE_BITMAP, LR_DEFAULTCOLOR | LR_LOADTRANSPARENT );
//    ImageList_Add( hImageList, hBitmap, hBitmap2 );
//    //ImageList_AddMasked( hImageList, hBitmap, RGB(0,0,0) );
//
//    DeleteObject( hBitmap );
//
//    DWORD lastError = ::GetLastError();
//
//    SendMessage( toolBar, TB_SETIMAGELIST, 0, (LPARAM)hImageList );
//
//    TBBUTTON tbb[5] = 
//    {
//        { MAKELONG(4, 0), ID_FILE_NEWSTUDY, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"New Study" },
//        { MAKELONG(3, 0), ID_FILE_LOADSTUDY, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Load Study" },
//        { MAKELONG(0, 0), ONESERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"One Series" },
//        { MAKELONG(1, 0), TWOSERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Two Series" },
//        { MAKELONG(2, 0), FOURSERIESBTN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)"Four Series" }
//    };
//
//    SendMessage(toolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
//    SendMessage(toolBar, TB_ADDBUTTONS, 5, (LPARAM)&tbb );
//
//    SendMessage( toolBar, TB_AUTOSIZE, 0, 0 );
//
//    SendMessage(toolBar, WM_SETFONT, (WPARAM)defaultFont, MAKELPARAM(FALSE, 0));
//
//    ShowWindow( toolBar, TRUE );
//
//    return toolBar;
//}
//
//HWND CreateKNIFERebar( HWND hwnd, HWND toolbar )
//{
//    HWND rebar;
//    REBARINFO rbi = { 0 };
//    REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
//
//    rbBand.cbSize = sizeof( REBARBANDINFO );
//
//    rebar = CreateWindowEx( WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | RBS_AUTOSIZE |
//                                                                    WS_CLIPCHILDREN | RBS_VARHEIGHT | CCS_NODIVIDER | RBS_BANDBORDERS,
//                                                                    0,0,0,0, hwnd, (HMENU)CONTROLS_REBAR, GetModuleHandle(0), NULL );
//
//    if( !rebar )
//        return NULL;
//
//    rbi.cbSize = sizeof( REBARINFO );
//
//    SendMessage( rebar, RB_SETBARINFO, 0, (LPARAM)&rbi );
//
//
//    rbBand.fMask = RBBIM_STYLE |
//                   RBBIM_TEXT  |
//                   RBBIM_CHILD |
//                   RBBIM_CHILDSIZE |
//                   RBBIM_SIZE;
//    rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
//
//    unsigned long toolbarSize = (unsigned long)SendMessage( toolbar, TB_GETBUTTONSIZE, 0, 0 );
//
//    rbBand.lpText = TEXT("");
//    rbBand.hwndChild = toolbar;
//    rbBand.cyChild = HIWORD(toolbarSize);
//    rbBand.cxMinChild = 3 * LOWORD(toolbarSize);
//    rbBand.cyMinChild = HIWORD(toolbarSize);
//    rbBand.cx = 0;
//
//    SendMessage( rebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand );
//
//    return rebar;
//}