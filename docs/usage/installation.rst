===================
Installing REDCRAFT
===================

Find the current version of REDCRAFT `here <https://bitbucket.org/hvalafar/redcraft>`_. For legacy versions, consult `Ifestos <https://ifestos.cse.sc.edu>`_.

To download the current version of REDCRAFT, open a terminal window and type ``git clone --recursive https://bitbucket.org/hvalafar/redcraft.git``.

Because REDCRAFT uses the GoogleTest submodule, you must pass the ``--recursive`` flag when cloning.

**If you are having problems with running CMake with GoogleTest,** please enter ``git submodule init && git submodule update`` from within the cloned redcraft/ directory.

Once downloaded, your machine must be configured to compile C++/OpenMP projects.

Dependencies
============

* GCC C++ Compiler
* CMake
* OpenMP
* Qt5.12+ with Charts (for GUI support)
* Python 3 and Perl for scripts

Ubuntu/Debian or similar Linux distributions
--------------------------------------------

    First, we must ensure that your repositories are properly synced. 
    To do so, open the Terminal and enter the following command ``sudo apt-get update``.

    Install the GNU C++ compiler with ``sudo apt-get install g++``.

    Install CMake with ``sudo apt-get install cmake``.

    Install OpenMP with ``sudo apt-get install libomp-dev``.

    Install Qt5 with ``sudo apt-get install qt5-default``.

    Install Qt5 charts with ``sudo apt-get install libqt5charts5-dev``.

    Unfortunately, if you are running Ubuntu < 16.10 the charts package is not available for your distribution and it is up to you to get the package ``libqt5charts5-dev``.
    If you are experiencing compilation issues, it is likely that your version of Qt is too old. Please ensure you have Qt5.12+.

Arch/Manjaro or similar Linux distributions
-------------------------------------------

    Ensure that repositories are synced with ``sudo pacman -Syy``.

    Install the GNU C++ compiler with ``sudo pacman -S gcc``.

    Install CMake with ``sudo pacman -S cmake``.

    Install OpenMP with ``sudo pacman -S openmp``.

    Install Qt5 with charts with  ``sudo pacman -S qt5-base qt5-charts``

macOS
-----

    It is highly recommended that you install `Homebrew <https://brew.sh/>`_ to install REDCRAFT's dependencies.

    You will need the GCC compiler (*NOT* the Apple-Clang compiler from the XCode command line tools).
    *WARNING*: The Apple-Clang compiler is automatically aliased as gcc despite being different. If you run into problems
    with OpenMP or with missing headers ensure that you are using the real gcc and not clang. 

    To install GCC, run ``brew install gcc``. This will result in you being able to use the commands ``g++-8`` and ``gcc-8``.

    To install CMake, run ``brew install cmake``.

    To install OpenMP, run ``brew install libomp``.

    To install Qt5, run ``brew install qt``.

    *If you are having trouble with Qt finding the proper headers, Qt5::Core, Qt5::Charts, etc., you may need to run the following command:
    ``sudo ln -s /usr/local/Cellar/qt/5.x.x/mkspecs /usr/local/mkspecs && sudo ln -s /usr/local/Cellar/qt/5.x.x/plugins /usr/local/plugins``

FreeBSD
-------

    FreeBSD, just like macOS, ships with Clang as the default, so you must ensure that gcc is installed and available at ``/usr/local/bin``.

    To install GCC, run ``pkg install gcc``.

    To install CMake, run ``pkg install cmake``.

    To install OpenMP, run ``pkg install openmp``.

    To install Qt5 with all dependencies, run ``pkg install qt5``.

    Although REDCRAFT should work on FreeBSD, it is not a system that is targeted and you may have compilation issues due to differing development headers.

    You may need to add the ``-fpermissive`` flag to CMake by adding the line ``set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \"-fpermissive\"")``.

Windows
-------

Windows Subsystem for Linux
^^^^^^^^^^^^^^^^^^^^^^^^^^^

    To be able to use REDCRAFT to its full potential in a Windows environment, it is currently strongly recommended that you use `Windows Subsystem for Linux <https://docs.microsoft.com/en-us/windows/wsl/install-win10>`_.
    Once installed, if you chose Ubuntu simply follow the steps above the exact same way that you would if you were installing REDCRAFT on an Ubuntu machine.

    To run the REDCRAFT GUI under Windows Subsystem for Linux, first follow `this article <https://www.howtogeek.com/261575/how-to-run-graphical-linux-desktop-applications-from-windows-10s-bash-shell/>`_.
    
    Note that XMing must be running for the GUI to work. Although it does not say in the article, running the command ``echo export DISPLAY=:0 >> ~/.bashrc`` will allow you to run GUI applications via WSL any time
    provided XMing is open on your Windows environment.

    Now, to run REDCRAFT from the folder ``C:\Users\Henry\RDC``, you can run ``redcraft gui /mnt/c/Users/Henry/RDC``

Native Windows
^^^^^^^^^^^^^^

    Native Windows is currently unstable, but it will build and some features work correctly. This port is under active development and once it is working you will be able to simply download an installer.
    
    If you would like, you can install Cygwin to install all of the dependencies at once and keep them in one place.

    Nevertheless, if you are interested in compiling and running REDCRAFT natively on Windows, the dependencies are the same:
        * GCC Compiler, in most cases this should be `MinGW-w64 <https://mingw-w64.org/doku.php>`_ (NOT MinGW, which is restricted to 32-bit Windows)
        * CMake, available `here <https://cmake.org/download/>`_ or via Cygwin.
        * Qt5 for MinGW-w64 + Charts, available `here <https://www.qt.io/download>`_ or via Cygwin.
    
    You will need to ensure the ``bin/`` folder for MinGW-w64, CMake, and Qt5 are in your PATH.

    Then, create the file ``PreLoad.cmake`` and add ``set (CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "" FORCE)``.

    From there, the installation should be the same as below. Note that the default CMake installation directory will be ``C:\Program Files (x86)\Redcraft``.

    Also note that after installing you will not be able to simply run ``redcraft`` in the command line unless you put ``C:\Program Files (x86)\Redcraft\bin\redcraft.exe`` on your PATH.


Installation
============

1. Locate the folder where REDCRAFT was extracted, and navigate to it, i.e. ``cd ~/Downloads/redcraft``

2. Ensure that all dependencies are installed and run ``cmake .``

3. After CMake generates the Makefile, run ``make``.

4. Once finished, run ``sudo make install``. 
This will install the `redcraft` binary to /usr/local/bin and the rest of REDCRAFTs scripts and binaries to /usr/local/redcraft.

5. You may now run any REDCRAFT binary by typing ``redcraft <command> [args]``. 

For example, ``redcraft stage1 nefRDC 2`` will generate **.angles** files with a Ramachandran level of 2 based on files prefixed with **nefRDC**.

Additionally, you can run any REDCRAFT script by typing ``redcraft -S <script> [args]``.

The REDCRAFT and RMSD GUIs are available in the /usr/local/redcraft/bin folder. They can either be accessed directly or executed via the shell, i.e. ``redcraft gui``
