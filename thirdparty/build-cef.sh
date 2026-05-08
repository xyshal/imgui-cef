# This is apparently arduous.  I got the steps from
# https://chromiumembedded.github.io/cef/master_build_quick_start
# and this is the Mac OS steps.

# Note that I couldn't get this to work on Mac OS so I switched to the prebuilt
# installation on Linux.

ROOT_DIR=$PWD
mkdir -p $ROOT_DIR/code/automate
mkdir -p $ROOT_DIR/code/chromium_git
cd $ROOT_DIR/code
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH=$ROOT_DIR/code/depot_tools:$PATH
cd $ROOT_DIR/code/automate
wget https://raw.githubusercontent.com/chromiumembedded/cef/refs/heads/master/tools/automate/automate-git.py
echo "#!/bin/bash" > $ROOT_DIR/code/chromium_git/update.sh
echo "python3 ../automate/automate-git.py --download-dir=$ROOT_DIR/code/chromium_git --depot-tools-dir=$ROOT_DIR/code/depot_tools --no-distrib --no-build --arm64-build" >> $ROOT_DIR/code/chromium_git/update.sh
chmod u+x $ROOT_DIR/code/chromium_git/update.sh
cd $ROOT_DIR/code/chromium_git
./update.sh
export GN_DEFINES=is_component_build=true
cd $ROOT_DIR/code/chromium_git/chromium/src/cef
./cef_create_projects.sh
cd $ROOT_DIR/code/chromium_git/chromium/src
autoninja -C out/Release_GN_arm64 cef
