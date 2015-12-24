
# BUILT IN MODULES
import os
import platform
import sys


# PYTHON PROJECT MODULES
sys.path.append("scripts")  # now we can import modules from <currentDirectory>/scripts
import GlobalBuildRules
import Utilities
import FileSystem


class LocalBuild(GlobalBuildRules.GlobalBuild):
    def __init__(self, projectName):
        super(LocalBuild, self).__init__()

        # we can override member variables defined in GlobalBuildRules
        self._project_name = projectName
        self._project_namespace = "CWRUBotix"
        self._build_steps = ["build",
                             "runUnitTests",
                             "package",
                             "uploadPackagedVersion"]  # MessageFramework specific default build steps

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
            CMakeArgs.extend(["-G", "\"Unix Makefiles\""])
            Utilities.PFork(appToExecute="cmake", argsForApp=CMakeArgs, wd=wd)

    def makeTarget(self, targets):
        # make directory that CMake will dump all output to
        wd = FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name)

        if platform.system() == "Windows":
            Utilities.PForkWithVisualStudio(appToExecute="nmake",
                                            argsForApp=targets,
                                            wd=wd)
        else:
            Utilities.PFork(appToExecute="make", argsForApp=targets, wd=wd)

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

    def build(self):
        print("Building project [%s]" % self._project_name)
        self.executeBuildSteps(["preBuild", "cmake", "make"])

    def uploadPackagedVersion(self):
        print("Uploading project [%s]" % self._project_name)

if __name__ == "__main__":
    localBuild = None
    customCommands = Utilities.parseCommandLine(sys.argv[1:])
    print(customCommands)
    if "projects" not in customCommands[1]:
        customCommands[1]["projects"] = ["gtest", "Logging", "FuturesFramework", "MessageFramework"]

    for projectName in customCommands[1]["projects"]:
        localBuild = LocalBuild(projectName)
        localBuild.run(customCommands)
