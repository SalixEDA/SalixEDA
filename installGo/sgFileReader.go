package main

import (
  "embed"
  "errors"
  "fmt"
  "io"
  "os"
  "strings"
)

// Global embedded filesystem for qrc: prefix
var embeddedFS *embed.FS

//!
//! \brief SgSetEmbeddedFS sets the embedded filesystem for qrc: prefix
//! \param fs              Embedded filesystem to use for loading resources with qrc: prefix
//!
func SgSetEmbeddedFS(fs *embed.FS) {
  embeddedFS = fs
  }





//!
//! \brief SgFileRead   reads a file from either OS filesystem or embedded FS
//! \param resourcePath Path to the resource
//! \return []byte      File contents, error if reading fails
//! \note If path starts with "qrc:" or "qrc:/", reads from embedded filesystem
//!       Otherwise reads from OS filesystem
//!
func SgFileRead(resourcePath string) ([]byte, error) {
  // Check if this is an embedded resource (qrc: prefix)
  if strings.HasPrefix(resourcePath, "qrc:") {
    return readEmbeddedFile(resourcePath)
    }

  // Regular OS file
  return readOSFile(resourcePath)
  }





// readEmbeddedFile reads a file from embedded filesystem
func readEmbeddedFile(resourcePath string) ([]byte, error) {
  if embeddedFS == nil {
    return nil, errors.New("embedded filesystem not set. Call SgSetEmbeddedFS first")
    }

  // Remove qrc: prefix and clean the path
  cleanPath := strings.TrimPrefix(resourcePath, "qrc:")
  cleanPath = strings.TrimPrefix(cleanPath, "/")

  // Read from embedded FS
  data, err := embeddedFS.ReadFile(cleanPath)
  if err != nil {
    return nil, fmt.Errorf("failed to read embedded file %s: %w", cleanPath, err)
    }

  return data, nil
  }



// readOSFile reads a file from OS filesystem
func readOSFile(filePath string) ([]byte, error) {
  file, err := os.Open(filePath)
  if err != nil {
    return nil, fmt.Errorf("failed to open file %s: %w", filePath, err)
    }
  defer file.Close()

  data, err := io.ReadAll(file)
  if err != nil {
    return nil, fmt.Errorf("failed to read file %s: %w", filePath, err)
    }

  return data, nil
  }



//!
//! \brief SgFileExist  checks if a resource exists
//! \param resourcePath Path to the resource
//! \return bool        True if resource exists and is accessible, false otherwise
//!
func SgFileExist(resourcePath string) bool {
  if strings.HasPrefix(resourcePath, "qrc:") {
    return embeddedFileExists(resourcePath)
    }
  return osFileExists(resourcePath)
  }


// embeddedFileExists checks if a file exists in embedded filesystem
func embeddedFileExists(resourcePath string) bool {
  if embeddedFS == nil {
    return false
    }
  cleanPath := strings.TrimPrefix(resourcePath, "qrc:")
  cleanPath = strings.TrimPrefix(cleanPath, "/")

  file, err := embeddedFS.Open(cleanPath)
  if err != nil {
    return false
    }
  file.Close()
  return true
  }


// osFileExists checks if a file exists in OS filesystem
func osFileExists(filePath string) bool {
  _, err := os.Stat(filePath)
  return !os.IsNotExist(err)
  }
