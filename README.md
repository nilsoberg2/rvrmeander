# rvrmeander

Open-source RVR Meander 2.0 alpha

Currently the code must be downloaded and compiled.  We have no releases at this
time although we expect a release by the end of April 2016.

## Building the Engine

### Dependencies

Download DotSpatial-1.8 (newer versions may or may not work) and put the binaries in
contrib/DotSpatial-1.8.

Download GSL 1.10 source (newer version may or may not work) and put the source in
contrib/gsl-1.10.  Build cblaslib and gsllib in Release Win32 and x64 builds.
http://git.savannah.gnu.org/cgit/gsl.git/snapshot/release-1-10.tar.gz

### Build the engine

engine/apps/BUILD/RVRMeander.sln  build in Debug mode

### Build the GUI

gui/RVRMeander-Gui.sln
