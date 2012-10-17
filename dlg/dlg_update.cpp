/*
** Taiga, a lightweight client for MyAnimeList
** Copyright (C) 2010-2012, Eren Okka
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../std.h"

#include "dlg_main.h"
#include "dlg_update.h"

#include "../common.h"
#include "../http.h"
#include "../resource.h"
#include "../taiga.h"

#include "../win32/win_gdi.h"
#include "../win32/win_taskdialog.h"

class UpdateDialog UpdateDialog;

// =============================================================================

UpdateDialog::UpdateDialog() {
  RegisterDlgClass(L"TaigaUpdateW");
}

BOOL UpdateDialog::OnInitDialog() {
  // Set icon
  SetIconLarge(IDI_MAIN);
  SetIconSmall(IDI_MAIN);

  // Set title
  SetText(APP_TITLE);

  // Set progress text
  SetDlgItemText(IDC_STATIC_UPDATE_PROGRESS, L"Checking updates...");
  // Set progress bar
  progressbar.Attach(GetDlgItem(IDC_PROGRESS_UPDATE));
  progressbar.SetMarquee(true);

  // Check updates
//#ifdef _DEBUG
  PostMessage(WM_CLOSE);
//#else
//  Taiga.Updater.Check(L"taiga.erengy.com/update.php", Taiga, HTTP_UpdateCheck);
//#endif

  // Success
  return TRUE;
}

INT_PTR UpdateDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CTLCOLORSTATIC: {
      return ::GetSysColor(COLOR_WINDOW);
      break;
    }
  }

  return DialogProcDefault(hwnd, uMsg, wParam, lParam);
}

BOOL UpdateDialog::OnDestroy() {
  // Clean up
  Taiga.Updater.client.Cleanup();
  
  if (Taiga.Updater.update_available) {
    // Restart application
    if (Taiga.Updater.RestartApplication(L"UpdateHelper.exe", L"Taiga.exe", L"Taiga.exe.new")) {
      return TRUE;
    } else {
      // Load data again
      Taiga.LoadData();
    }
  } else {
    if (g_hMain) {
      win32::TaskDialog dlg(APP_TITLE, TD_ICON_INFORMATION);
      dlg.SetMainInstruction(L"No updates available. Taiga is up to date!");
      dlg.SetExpandedInformation(L"Current version: " APP_VERSION);
      dlg.AddButton(L"OK", IDOK);
      dlg.Show(g_hMain);
    }
  }

  // Create/activate main window
  ExecuteAction(L"MainDialog");
  return TRUE;
}

void UpdateDialog::OnPaint(HDC hdc, LPPAINTSTRUCT lpps) {
  // Paint background
  win32::Dc dc = hdc;
  dc.FillRect(lpps->rcPaint, ::GetSysColor(COLOR_WINDOW));
}