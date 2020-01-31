#include "stdafx.h"
#include "Menu.h"

DWORD InitializeMenuBar(HWND hParent)
{
	HMENU hMenu;
	HMENU hSubMenu;

	hMenu = GetMenu(hParent);

	//Disable 'New' and 'SaveAs' Initally
		//--New should not be disabled
		//--SaveAs disabled because there is no available data
	hSubMenu = GetSubMenu(hMenu, MP_ROOT_FILE);
	EnableMenuItem(hSubMenu, MP_FILE_NEW, MF_BYPOSITION | MF_ENABLED);
	EnableMenuItem(hSubMenu, MP_FILE_SAVEAS, MF_BYPOSITION|MF_ENABLED);

	//Disable 'Cascade' and 'Tile' Initally
		//--No Open Windows
	hSubMenu = GetSubMenu(hMenu, MP_ROOT_WINDOW);
	EnableMenuItem(hSubMenu, MP_WINDOW_CASCADE, MF_BYPOSITION|MF_ENABLED);
	EnableMenuItem(hSubMenu, MP_WINDOW_TILE_HORZ, MF_BYPOSITION|MF_ENABLED);
	EnableMenuItem(hSubMenu, MP_WINDOW_TILE_VERT, MF_BYPOSITION|MF_ENABLED);

	return TRUE;
}
