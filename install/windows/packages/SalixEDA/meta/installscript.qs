
  function Component()
  {
      // default constructor
  }

  Component.prototype.createOperations = function()
  {
      // call default implementation to actually install
      component.createOperations();

      if( systemInfo.kernelType === "winnt" ) {
        //Create file association
        component.addOperation("RegisterFileType", "salixeda", "@TargetDir@/SalixEDA.exe '%1'", "SalixEDA project file" );
        //Create menu
        component.addOperation("CreateShortcut", "@TargetDir@/SalixEDA.exe", "@StartMenuDir@/SalixEDA.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/SalixEDA.exe", "description=Start SalixEDA schematic and pcb layout EDA" );
        component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/RemoveSalixEDA.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/maintenancetool.exe", "description=Remove SalixEDA from computer" );
        }


  }

