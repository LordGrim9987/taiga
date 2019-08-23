/*
** Taiga
** Copyright (C) 2010-2019, Eren Okka
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

#pragma once

#include <windows/win/application.h>

#include "taiga/command_line.h"
#include "taiga/update.h"

namespace taiga {

class App : public win::App {
public:
  App();
  ~App();

  BOOL InitInstance();
  void Uninitialize();

  void LoadData();

  class Updater : public UpdateHelper {
  public:
    void OnCheck();
    void OnCRCCheck(const std::wstring& path, std::wstring& crc);
    void OnDone();
    void OnProgress(int file_index);
    bool OnRestartApp();
    void OnRunActions();
  } Updater;

  CommandLineOptions options;
};

}  // namespace taiga

extern taiga::App Taiga;
