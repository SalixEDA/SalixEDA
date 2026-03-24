package main

// SgString is a slice of runes representing a Unicode string
type SgString []rune



//!
//! \brief NewSgString creates an empty SgString
//! \return             Empty SgString
//!
//! Returns a new, empty SgString ready for use.
func NewSgString() SgString {
  return SgString{}
  }




//!
//! \brief SgStringFromUtf8 creates a new SgString from a UTF-8 string
//! \param str          UTF-8 string to convert
//! \return             SgString containing the Unicode code points from the input string
//!
//! Converts a standard Go UTF-8 string to a slice of runes for proper Unicode handling.
func SgStringFromUtf8(str string) SgString {
  // Convert UTF-8 string to runes (Unicode code points)
  return SgString([]rune(str))
  }




//!
//! \brief SgStringAsUChars creates a new SgString consisting of count copies of the same character
//! \param ch           Unicode code point to repeat (as int)
//! \param count        Number of times to repeat the character
//! \return             SgString containing count copies of the specified character
//!
//! Creates a string filled with the same character repeated count times.
//! If count <= 0, returns an empty string.
func SgStringAsUChars(ch int, count int) SgString {
  // Handle invalid count
  if count <= 0 {
    return SgString{}
    }

  // Create slice with specified length
  result := make(SgString, count)

  // Fill with the character
  for i := 0; i < count; i++ {
    result[i] = rune(ch)
    }

  return result
  }




//!
//! \brief AppendString converts a UTF-8 string to runes and appends them to the end of current SgString
//! \param str          UTF-8 string to append
//! \return             Updated SgString with appended runes
//!
//! The method creates a new slice by appending the converted runes to the existing ones.
//! Original SgString remains unchanged if not reassigned.
func (s *SgString) AppendString(str string) SgString {
  // Convert UTF-8 string to runes and delegate to AppendSgString
  return s.AppendSgString(SgStringFromUtf8(str))
  }



//!
//! \brief AppendSgString appends another SgString to the end of current SgString
//! \param str          SgString to append
//! \return             Updated SgString with appended runes
//!
//! Appends all runes from the provided SgString to the current one.
//! Both slices remain valid after operation.
func (s *SgString) AppendSgString(str SgString) SgString {
  // Append all runes from str to current slice
  *s = append(*s, str...)
  return *s
  }



//!
//! \brief AppendUChar appends a single Unicode character to the end of current SgString
//! \param ch           Unicode code point to append (as int)
//! \return             Updated SgString with appended character
//!
//! Appends a single rune represented by the integer code point.
func (s *SgString) AppendUChar(ch int) SgString {
  // Convert int to rune and append to slice
  *s = append(*s, rune(ch))
  return *s
  }



//!
//! \brief At returns the Unicode code point at the specified index
//! \param index        Position in the SgString (0-based)
//! \return             Unicode code point as int, or 0 if index out of range
//!
//! Provides safe access to individual runes with bounds checking.
func (s SgString) At(index int) int {
  // Check if index is within valid range
  if index >= len(s) {
    return 0 // Return 0 (null character) for invalid index
    }
  if index < 0 {
    if -index >= s.Length() {
      return 0
      }
    return int(s[s.Length() + index])
    }
  // Return the rune at specified index as int
  return int(s[index])
  }



//!
//! \brief IsEmpty checks if the SgString contains no runes
//! \return             true if string is empty, false otherwise
//!
//! Returns true when the slice has zero length.
func (s SgString) IsEmpty() bool {
  // Check if slice length is zero
  return len(s) == 0
  }




//!
//! \brief InsertString converts a UTF-8 string to runes and inserts them at the specified position
//! \param pos          Position to insert at (0-based)
//! \param str          UTF-8 string to insert
//! \return             Updated SgString with inserted runes
//!
//! Inserts runes at position pos. If pos is out of range, returns original string.
func (s *SgString) InsertString(pos int, str string) SgString {
  // Delegate to InsertSgString after converting string to SgString
  return s.InsertSgString(pos, SgStringFromUtf8(str))
  }




//!
//! \brief InsertSgString inserts another SgString at the specified position
//! \param pos          Position to insert at (0-based)
//! \param str          SgString to insert
//! \return             Updated SgString with inserted runes
//!
//! Inserts all runes from str at position pos. This is the core insertion method
//! used by all other Insert* methods.
func (s *SgString) InsertSgString(pos int, str SgString) SgString {
  // Validate insertion position
  if pos < 0 || pos > s.Length() {
    return *s // Invalid position, return original
    }

  // If the string to insert is empty, return original
  if len(str) == 0 {
    return *s
    }

  // Create new slice with capacity for all elements
  result := make(SgString, 0, s.Length() + len(str) )

  // Build result by concatenating parts:
  // 1. Elements before insertion point
  result = append(result, (*s)[:pos]...)
  // 2. New elements to insert
  result = append(result, str...)
  // 3. Remaining elements after insertion point
  *s = append(result, (*s)[pos:]...)

  return *s
  }




//!
//! \brief InsertUChar inserts a single Unicode character at the specified position
//! \param pos          Position to insert at (0-based)
//! \param ch           Unicode code point to insert (as int)
//! \return             Updated SgString with inserted character
//!
//! Inserts a single rune at position pos by delegating to InsertSgString.
func (s *SgString) InsertUChar(pos int, ch int) SgString {
  // Create a temporary SgString with the single character and delegate
  return s.InsertSgString(pos, SgString{rune(ch)})
  }




//!
//! \brief Mid returns a substring starting at position pos with specified length
//! \param pos          Starting position (0-based)
//! \param len          Length of substring. If len < 0, returns from pos to end
//! \return             SgString containing the requested substring
//!
//! Extracts a portion of the SgString. Returns empty string if pos is invalid.
func (s SgString) Mid(pos int, length int) SgString {
  // Check if starting position is valid
  if pos < 0 || pos >= len(s) {
    return SgString{} // Return empty string for invalid position
    }

  // If length is negative, return from pos to end
  if length < 0 {
    return s[pos:]
    }

  // Calculate end position
  end := pos + length
  if end > len(s) {
    end = len(s) // Adjust if length exceeds available characters
    }

  // Return substring
  return s[pos:end]
  }



//!
//! \brief Length returns the number of runes in the SgString
//! \return             Count of Unicode characters
//!
//! Returns the length of the rune slice, which equals the number of Unicode code points.
func (s SgString) Length() int {
  // Return slice length (number of runes)
  return len(s)
  }



//!
//! \brief First returns a substring from the beginning with specified length
//! \param len          Length of substring. If len < 0, returns entire string
//! \return             SgString containing the first len characters
//!
//! Returns the first len characters. Equivalent to Mid(0, len).
func (s SgString) First(length int) SgString {
  // Delegate to Mid with position 0
  return s.Mid(0, length)
  }



//!
//! \brief Last returns a substring from the end with specified length
//! \param len          Length of substring. If len < 0, returns entire string
//! \return             SgString containing the last len characters
//!
//! Returns the last len characters. For negative len, returns entire string.
func (s SgString) Last(length int) SgString {
  // Handle negative length - return entire string
  if length < 0 {
    return s.Mid(0, -1) // -1 means to end in Mid
    }

  // Calculate start position for last 'length' characters
  start := s.Length() - length
  if start < 0 {
    start = 0 // Adjust if length exceeds string length
    }

  // Return substring from start to end
  return s.Mid(start, -1) // -1 means to end
  }







//!
//! \brief ToUtf8 converts the SgString to a UTF-8 encoded string
//! \return             UTF-8 string representation
//!
//! Converts the slice of runes back to a standard Go UTF-8 string.
//! This is useful for output, file I/O, or interfacing with string APIs.
func (s SgString) ToUtf8() string {
  // Convert rune slice to UTF-8 string
  return string(s)
  }




//!
//! \brief Remove removes characters from the SgString starting at position pos
//! \param pos          Starting position for removal (0-based)
//! \param len          Number of characters to remove. If len < 0, removes from pos to end
//! \return             Updated SgString after removal
//!
//! Removes a portion of the string. If pos is invalid, returns original string unchanged.
//! If len <= 0 and len >= 0, removes exactly len characters.
//! If len < 0, removes from pos to the end of string.
func (s *SgString) Remove(pos int, length int) SgString {
  // Validate starting position
  if pos < 0 || pos >= s.Length() {
    return *s // Invalid position, return original unchanged
    }

  // Handle removal to the end
  if length < 0 {
    // Remove from pos to end
    *s = (*s)[:pos]
    return *s
    }

  // Calculate end position
  end := pos + length
  if end > s.Length() {
    end = s.Length() // Adjust if length exceeds available characters
    }

  // Remove the specified range
  // s[:pos] - keep characters before pos
  // s[end:] - keep characters after end
  *s = append( (*s)[:pos], (*s)[end:]...)

  return *s
  }


//!
//! \brief IndexOf finds the first occurrence of substring starting from position 'from'
//! \param from         Starting position to search from (0-based)
//! \param str          Substring to search for
//! \return             Index of first occurrence, or -1 if not found or invalid parameters
//!
//! Searches for the substring 'str' within the SgString starting at index 'from'.
//! Returns the starting index of the first match, or -1 if:
//! - 'from' is out of bounds
//! - 'str' is empty
//! - 'str' is longer than the remaining string
//! - no match is found
func (s SgString) IndexOf(from int, str SgString) int {
  // Validate parameters
  if from < 0 || from >= s.Length() {
    return -1 // Invalid starting position
    }

  if str.Length() == 0 {
    return -1 // Empty substring
    }

  if str.Length() > s.Length() - from {
    return -1 // Substring longer than remaining string
    }

  lastPos := s.Length() - str.Length()

  // Search for the substring
  for i := from; i <= lastPos; i++ {
    match := true
    for j := 0; j < str.Length(); j++ {
      if s[i+j] != str[j] {
        match = false
        break
        }
      }
    if match {
      return i // Found match at position i
      }
    }

  return -1 // No match found
  }



//!
//! \brief IndexOfUChar finds the first occurrence of a Unicode character starting from position 'from'
//! \param from         Starting position to search from (0-based)
//! \param ch           Unicode code point to search for
//! \return             Index of first occurrence, or -1 if not found or invalid parameters
//!
//! Searches for the character 'ch' within the SgString starting at index 'from'.
//! Returns the starting index of the first match, or -1 if:
//! - 'from' is out of bounds
//! - no match is found
func (s SgString) IndexOfUChar(from int, ch int) int {
  return s.IndexOf( from, SgStringAsUChars( ch, 1 ) )
  }




//!
//! \brief IndexOfString finds the first occurrence of a UTF-8 string starting from position 'from'
//! \param from         Starting position to search from (0-based)
//! \param str          UTF-8 string to search for
//! \return             Index of first occurrence, or -1 if not found or invalid parameters
//!
//! Searches for the substring 'str' within the SgString starting at index 'from'.
//! Converts the UTF-8 string to SgString and delegates to IndexOf.
//! Returns -1 if:
//! - 'from' is out of bounds
//! - 'str' is empty
//! - no match is found
func (s SgString) IndexOfString(from int, str string) int {
  // Convert string to SgString and delegate to IndexOf
  return s.IndexOf(from, SgStringFromUtf8(str))
  }



func (s SgString) Trimmed() SgString {
  slen := s.Length()
  i := 0
  // Skip all delimiters
  for i < slen && s[i] == 0x20 {
    i++
    }
  if i < slen {
    return s[i:]
    }
  return SgString{}
  }




//!
//! \brief splitUChar splits the SgString by the specified Unicode character
//! \param ch           Unicode code point to use as delimiter
//! \return             Slice of SgString substrings between delimiters
//!
//! Splits the string by the given character. If the string is empty, returns a slice with one empty string.
//! If the delimiter is not found, returns a slice containing the original string.
//! Consecutive delimiters create empty strings in the result.
//!
func (s SgString) SplitUChar(ch int) []SgString {
  // Handle empty string
  if s.IsEmpty() {
    return []SgString{}
    }

  result := make([]SgString, 0)
  start := 0
  delim := rune(ch)
  slen := s.Length()
  i := 0

  // Iterate through the string looking for delimiter
  for i < slen {
    start = i
    // Skip all delimiters
    for i < slen && s[i] == delim {
      i++
      }
    // Skip all non-delimiters
    for i < slen && s[i] != delim {
      i++
      }
    // If segment not empty then append to result
    if start != i {
      result = append(result, s.Mid(start, i-start))
      }
    }
  return result
  }



//!
//! \brief join concatenates a slice of SgString into a single SgString
//! \param list         Slice of SgString to join
//! \return             SgString containing all elements joined with the current string as separator
//!
//! Joins the elements of the list using the current SgString as a separator.
//! If the list is empty, returns an empty string.
//! If the list has one element, returns that element without the separator.
func (s *SgString) Join(list []SgString) SgString {
  // Handle empty list
  if len(list) == 0 {
    return SgString{}
    }

  // Handle single element
  if len(list) == 1 {
    *s = list[0]
    return *s
    }

  // Calculate total length needed
  for _, str := range list {
    s.AppendSgString( str )
    }
  return *s
  }
