package main

import (
  "os"
  "os/exec"
  "path/filepath"
  "runtime"
)

func main() {
  targetName := "SalixEDA"

  exe, _ := os.Executable()
  dir := filepath.Dir(exe)

  if runtime.GOOS == "windows" {
    targetName += ".exe"
    }

  cmd := exec.Command(filepath.Join(dir, targetName), "--delete")
  cmd.Start()
  }
