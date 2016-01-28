
# SYSTEM MODULES
import platform

# BUILT IN MODULES
import FileSystem
from GlobalBuildRules import GlobalBuild
import Utilities

class LocalBuild(GlobalBuild):
    def __init__(self, projectName):
        super(LocalBuild, self).__init__()

        # we can override member variables defined in GlobalBuildRules
        self._project_name = projectName
        self._project_namespace = "CWRUBotix"
        self._build_steps = ["build",
                             "runUnitTests",
                             "package",
                             "uploadPackagedVersion"]  # MessageFramework specific default build steps

        self._tests_to_run = ["%sUnitTests" % self._project_name]

    # this method will launch CMake.
    # CMake is handling all of our compiling and linking.
    def cmake(self):
        # make directory that CMake will dump output to
        wd = FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name)
        Utilities.mkdir(wd)

        CMakeArgs = self.getCMakeArgs("", wd)
        if platform.system() == "Windows":
            CMakeArgs.extend(["-G", "\"NMake Makefiles\""])
            Utilities.PForkWithVisualStudio(appToExecute="cmake",
                                            argsForApp=CMakeArgs,
                                            wd=wd)
        else:
            CMakeArgs.extend(["-G", "Unix Makefiles"])
            Utilities.PFork(appToExecute="cmake", argsForApp=CMakeArgs, wd=wd, failOnError=True)

    def makeTarget(self, targets):
        # make directory that CMake will dump all output to
        wd = FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name)

        if platform.system() == "Windows":
            Utilities.PForkWithVisualStudio(appToExecute="nmake",
                                            argsForApp=targets,
                                            wd=wd)
        else:
            Utilities.PFork(appToExecute="make", argsForApp=targets, wd=wd, failOnError=True)

    def makeVisualStudioProjects(self):
        wd = FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name)
        Utilities.mkdir(wd)
        CMakeArgs = self.getCMakeArgs("", wd)
        if platform.system() == "Windows":
            CMakeArgs.extend(["-G", "\"Visual Studio: %s\"" % Utilities.getMachineBits()])
            Utilities.PForkWithVisualStudio(appToExecute="cmake",
                                            argsForApp=CMakeArgs,
                                            wd=wd)
        

    def make(self):
        self.makeTarget(["all"])
        self.makeTarget(["install"])

    def build(self):
        print("Building project [%s]" % self._project_name)
        self.executeBuildSteps(["preBuild", "cmake", "make"])

    def uploadPackagedVersion(self):
        print("Uploading project [%s]" % self._project_name)

    def help(self):
        print "command specific to project [%s]" % self._project_name
        print "     [%s] specific build steps" % self._project_name
        print "         cmake                       generates build files for all C++ source and tests."
        print "         make                        makes all binaries."
        print "         build                       runs cmake and make to build all binaries."
        print "         makeVisualStudioProjects    generates visual studio projects."
        print "         uploadPackagedVersion       uploads built and tested binaries for distribution."
        print "     [%s] specific custom variables" % self._project_name
        print ""
