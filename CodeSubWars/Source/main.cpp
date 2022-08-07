// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#include "PrecompiledHeader.h"
#include "CodeSubwarsMainWindow.h"
#include "CSWSilentApplication.h"
#include "CSWLog.h"
#include "CSWWorld.h"


#include "CSWPyActuators.h"
#include "CSWPyCommands.h"
#include "CSWPyCommon.h"
#include "CSWPyEvents.h"
#include "CSWPyEquipment.h"
#include "CSWPyMath.h"
#include "CSWPySensors.h"
#include "CSWPySubmarine.h"
#include "CSWPyUtilities.h"
#include "CSWPyWeapons.h"

using namespace CodeSubWars;

int main(int argc, char** argv)
{
  QDir().mkdir("log");
  try
  {
    //Register the modules with the interpreter
    if (PyImport_AppendInittab("CodeSubWars_Actuators", &PyInit_CodeSubWars_Actuators) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Actuators to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Commands", &PyInit_CodeSubWars_Commands) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Commands to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Common", &PyInit_CodeSubWars_Common) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Common to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Events", &PyInit_CodeSubWars_Events) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Events to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Equipment", &PyInit_CodeSubWars_Equipment) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Equipment to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Math", &PyInit_CodeSubWars_Math) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Math to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Sensors", &PyInit_CodeSubWars_Sensors) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Sensors to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Submarine", &PyInit_CodeSubWars_Submarine) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Submarine to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Utilities", &PyInit_CodeSubWars_Utilities) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Utilities to the interpreter's builtin modules");
    if (PyImport_AppendInittab("CodeSubWars_Weapons", &PyInit_CodeSubWars_Weapons) == -1)
      throw std::runtime_error("Failed to add CodeSubWars_Weapons to the interpreter's builtin modules");

    if (argc > 1)
    {
      CSWSilentApplication app(argc, argv);
      app.run();
    }
    else
    {
      QCoreApplication::addLibraryPath("plugins");

      QApplication app(argc, argv);
      CodeSubwarsMainWindow mainWnd;
      mainWnd.show();
      app.exec();
    }
  }
  catch (std::exception& e)
  {
    CSWLog::getInstance()->log(std::string("exception caught: ") + e.what());
  }
  catch (...)
  {
    CSWLog::getInstance()->log("unknown exception caught");
  }
  return 0;
}

