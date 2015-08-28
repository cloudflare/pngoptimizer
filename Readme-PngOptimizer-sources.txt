* The PngOptimizer application in the "Projects" directory 
  is distributed under the terms of the GNU General Public Licence.

* The PngOptimizerCL application in the "Projects" directory 
  is distributed under the terms of the GNU General Public Licence.

* The PngOptimizerDll in the "Projects" directory 
  is distributed under the terms of the GNU Lesser General Public Licence.

* The TestPODll in the "Projects" directory 
  is distributed under the terms of the GNU General Public Licence.

* The poeng library in the "SDK" directory
  is distributed under the terms of the GNU Lesser General Public Licence.

* The chuwin32 library in the "SDK" directory
  is distributed under the terms of the Zlib licence.

* The chustd library in the "SDK" directory
  is distributed under the terms of the Zlib licence.

* The chustd uses the Zlib library, distributed under the terms of the 
  ZLib licence.

* The UnitTests directory uses the Google Test framework, distributed under
  the terms of the new BSD license.
  
--------------------------------------------------------------------------
How to build PngOptimizer?

For Windows:

1. Use Microsoft Visual Studio 2012 and open Projects/PngOptimizer/PngOptimizer.sln

2. Build the solution, in either Debug or Release mode, and for Win32 (x86) or x64.

The library build results are located in each library directory of the SDK directory:
 * Win32-Debug for the Windows 32 bits debug version
 * Win32-Release for the Windows 32 bits release version
 * x64-Debug for the Windows 64 bits debug version
 * x64-Release for the Windows 64 bits release version

The final build result, PngOptimizer.exe, is located into the Win32-Debug, Win32-Release, 
x64-Debug or x64-Release sub-directory of the PngOptimizer directory.

--------------------------------------------------------------------------
How to build PngOptimizerCL?

For Windows: same as PngOptimizer except you need to open
             Projects/PngOptimizerCL/PngOptimizerCL.sln

For Linux:

1. Open a terminal
2. Ensure you have build tools. To get the build tools on Ubuntu,
   open a terminal and execute this command:
   
   $ sudo apt-get install build-essential

3. Change directory to Projects/PngOptimizerCL/
5. Execute command:
   
   $ make
   
   or:
    
   $ make CONFIG=release
   
   These commands build in debug and release configuration respectively.
   The build results are located in the linux-debug/ or linux-release/ directories.
