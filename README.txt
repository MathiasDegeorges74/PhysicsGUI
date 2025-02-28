Help for linking
https://www.youtube.com/watch?v=qvg8BXXWpCE

*** New project *** 
Crate a new project (console application)
Launch the default main once in Release and once in Debug to create folders in x64


*** Setup project ***

in Solution > Property: 

* All configuration
-Solution > Property > General > Set language to <C++20>
-Solutoin > C/C++ > General > other included repository> add <%SFML_ROOT%/include>
-Solutoin > C/C++ > General > additional library > add <%SFML_ROOT%/lib>

* Debug 
-Solution > C/C++ > input > additional dependency > add <sfml*-d.lib>

* Release
-Solution > C/C++ > input > additional dependency > add <sfml*.lib>


* from <%SFML_ROOT%/bin>
-Copy all needed <sfml*-d-3.dll> into <%PROJECT_ROOT%/x64/Debug>
-Copy all needed <sfml*-3.dll> into <%PROJECT_ROOT%/x64/Release>

