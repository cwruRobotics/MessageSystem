# MessageSystem


Robos (Robot Operating System)


Requirements:
    To build this project, you need (well I used) Python 2.7.10 and you NEED AT LEAST CMake 3.3 OR IT WILL FAIL BECAUSE I'VE SET IT THAT WAY.
    You will also need either GCC or Visual Studio. I'm using Visual Studio 15 but I think the build process is generic enough
    that it will detect your version of Visual Studio and use it when calling CMake.

    So far, this project has been tested using:
        Visual Studio 15
        GNU 4.9.2

    If you have another system and it successfully builds, please let me know and I will update this list.
    Otherwise, I will continue to branch out for OSX and other compilers.


How to run:
    The build scripts run via python. Therefore, you will need to say "python" (or "python<your_version>") if you are running
    with more than one distribution of python.

    The basic mimimum command to build is:
        "python LocalBuild.py" (remember to not include the quotations when you enter this command in the command line)
                                                    OR
        "python2.7 LocalBuild.py" (this is what I run on my Ubuntu 15 VM).

        NOTE that currently, the Robos project CANNOT BE BUILD with this command, as it has no codebase yet.
        In fact, if you try to build it, it will fail.

    The build scripts accept several arguments.
        After the basic mimimum command, you can specify custom build steps to run. For example:

            "python LocalBuild.py build runUnitTests" will build the "Logging" and "Async" projects while ONLY running
                the "build" and "runUnitTests" build steps. If you would like a list of custom build steps, please
                type "python LocalBuild.py help"

        Also, if you specify a "-" before a command, you can pass in custom variables. You can type in anything, but so far
        the only custom variables that do anything are the "-configuration" and "-projects" variables.

        The "-configuration" command will build projects for the configuration specified...you can specify either "debug"
        or "release":

            "python LocalBuild.py -configuration debug" will build "Logging" and "Async" for the "debug" configuration.
                The default is to build all projects for all configurations.

        The "-projects" command will build the projects specified in the order of specification:

            "python LocalBuild.py -projects Logging Async" This will build "Logging" first, and then "Async." Since multiple
                projects can be specified after the "-projects" command, I highly recommend having that command LAST in your
                input.

    Ultimatley, these can be combined to build various projects for various configurations, consider:

        "python LocalBuild.py -configuration debug -projects Async" This will build "Async" for the "debug" configuration.

Feel free to reach out to me: aew61@case.edu

