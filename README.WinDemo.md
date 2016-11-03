# libofa - windows Build Instructions

LibOFA (Library Open Fingerprint Architecture) is an open-source audio fingerprint created and provided by MusicIP Corporation
605 E. Huntington Dr. Suite 201, Monrovia CA 91016 USA
Ph +1-626-359-9702  Fax +1-626-359-9827

Original sources from [musicip-libofa](https://code.google.com/archive/p/musicip-libofa/)

For this project we choose GNU GENERAL PUBLIC LICENSE Version 2

## Microsoft Visual Studio 2015 - Visual C++ 2015

To build Windows Demo solutions, you need libofa library wich could be build and referenced from the other project, [libfftw](http://www.fftw.org/) is a C subroutine library for computing the discrete Fourier transform


Directory structure may look like this to keep the solution and project references

```
   libofa
      |
      |---lib          ; contains libofa headers and sources files
      |    |---AFLIB
      |    |---JAMA
      |    . . .
      |
      |---include
      |    |---ofa1    ; contains ofa.h
      |
      |---WinDemo      ; contains visual studio solution and project files
      |    |---Debug   ; after build contains the WindDemo.exe (in debug mode)
      |    |---Release ; after build contains the WindDemo.exe (in release mode)
      |    . . .
      |
      |---Windows      ; contains visual studio solution and project files
      |    |---Debug   ; after build contains the libofa.dll (in debug mode)
      |    |---Release ; after build contains the libofa.dll (in release mode)
      |    . . .
      |
      |---libfftw3     ; contains libfftw3-3.def, libfftw3-3.dll, libfftw3.h
      |      
```

Open .\WinDemo\WinDemo.sln in Microsoft Visual Studio and build.
If needed copy libfft*.dll to Debug/ or /Release folders
If all dependencies were installed and placed into the right structure, libofa should build.

