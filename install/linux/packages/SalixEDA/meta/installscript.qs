
  function Component()
  {
      // default constructor
  }

  Component.prototype.createOperations = function()
  {
      // call default implementation to actually install
      component.createOperations();

      if (systemInfo.kernelType === "linux") {
          //Create desktop entry for SalixEDA
          component.addOperation("CreateDesktopEntry", "salixeda.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SalixEDA\nExec=@TargetDir@/SalixEDA.sh");
          //Create desktop entry for delete SalixEDA
          component.addOperation("CreateDesktopEntry", "salixedadel.desktop",
             "Version=@ProductVersion@\nType=Application\nCategories=Application;\nName=SalixEDA Uninstall\nExec=@TargetDir@/maintenancetool");
      }
  }

