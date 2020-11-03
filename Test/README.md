# Manual of Test

The test framework has been repaired and integrated into the main program through CMake, it will compile together with the application and generate an executable file called detiq-t_Test in your BUILD directory. 

In order to run the test, you have to manually copy the res folder (where this file is located) to the same location of the executable. (Note: better to enable the "Run in terminal" option (found at Projects--Build & Run--Run Settings--Run) if you decide to run the test through Qt Creator.


## Introduction of test content

The test is mainly written for the ImageIn library, to test several low-level actions provided by this library, such as reading/writing operations towards different image formats, copy & crop operations, format conversion and serval algorithm tests including the MorphoMats.


## Development suggestions

If you want to add other tests as a new developer, please refer to the Test.h and Tester.h files, they define a basic test framework to be inherited, including the common functions of a test such as constructor, init, test cleanup and info (to point out where the error occurred). 

You can add the content and method you want to test in your own test file and start the test by creating a tester file that can execute multiple tests (Testers need to be added to the main function for the purpose of unification).

## Copyright

DETIQ-T is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DETIQ-T is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.