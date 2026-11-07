
---
`PowerView` utilizes PowerShell AD hooks and Win32 API functions, and, among other functions, replaces a variety of net commands called by the built-in Windows tools. `SharpView` is a .NET port that provides all of the `PowerView` functions and arguments in a .NET assembly. One major difference between `PowerView` and `SharpView` is the ability to pipe commands. `SharpView` uses strings instead of PowerShell objects. Therefore we cannot specify properties using `Select` or `Select-Object`, to parse the output or select specific AD objects as easily.
