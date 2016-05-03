

Repository Functions Implemented
=================================

- GetFiles
  Gets a list of all packages open/closed in the server

- Check-In
  New packages can be checked In.
	- If the package doesn't already exist, server creates a version 1 of the package.
	- If the package exists and the latest version is still open, it overwrite the existing latest version.
	- If the package exists and the latest version is closed, it creates a new version of the package.
  User can select the packages which the check-In package depends on. Only the packages "closed" in the repository
  can be selected.

- GetOpenCheck-Ins
  When a package is checked-In, it is initially given an "open" status.
  User can retrieve all the open check-In packages in the server.

- CloseOpenCheck-In
  User can select an "open" package and close it.

- Check-Out
  User can get the list of all packages(open/closed) and select one of them to download it. Optionally, the user
  can decide to include the dependencies along with the downloaded packages. All dependencies are downloaded in the 
  dependency graph.


Directory Structure
==============================
(Note: All paths are relative to the Debug folder where all exe files reside)
Server:
 
1) Metadata files (stores metadata of all packages)
	../root/Metadata/

2) Package files
        ../root/<PackageName>_<Ver>/<PackageName>.cpp
        ../root/<PackageName>_<Ver>/<PackageName>.h

3) socket uploads - This is where all the file uploads are done initially and then the repository 
                    moves it to the repository directories
        ../root/socket_uploads/
     