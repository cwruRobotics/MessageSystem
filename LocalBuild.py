
# BUILT IN MODULES
# import os
# import platform
import sys
import platform


# PYTHON PROJECT MODULES
sys.path.append("scripts")  # now we can import modules from <currentDirectory>/scripts
import GlobalBuildRules


class LocalBuild(GlobalBuildRules.GlobalBuild):
    def __init__(self):
        super(LocalBuild, self).__init__()

        # we can override member variables defined in GlobalBuildRules
        self._project_name = "MessageFramework"
        self._project_namespace = "CWRUBotix"
        self._build_steps = ["build",
                             "runUnitTests",
                             "package",
                             "uploadPackagedVersion"]  # MessageFramework specific default build steps

    # this method will launch CMake.
    # CMake is handling all of our compiling and linking.
    def cmake(self):
        # make directory that CMake will dump output to
        wd = GlobalBuildRules.getDirectory(GlobalBuildRules.FileSystemDirectory.WORKING, self._config)
        GlobalBuildRules.mkdir(wd)

        CMakeArgs = self.getCMakeArgs("../../", wd)
        if platform.system() == "Windows":
            CMakeArgs.extend(["-G", "\"NMake Makefiles\""])
            GlobalBuildRules.PForkWithVisualStudio(appToExecute="cmake",
                                                   argsForApp=CMakeArgs,
                                                   wd=wd)
        else:
            CMakeArgs.extend(["-G", "\"Unix Makefiles\""])
            GlobalBuildRules.PFork(appToExecute="cmake", argsForApp=CMakeArgs, wd=wd)

    def make(self):
        # make directory that CMake will dump all output to
        wd = GlobalBuildRules.getDirectory(GlobalBuildRules.FileSystemDirectory.WORKING, self._config)
        GlobalBuildRules.mkdir(wd)

        makeArgs = ["all"]
        if platform.system() == "Windows":
            GlobalBuildRules.PForkWithVisualStudio(appToExecute="nmake",
                                                   argsForApp=makeArgs,
                                                   wd=wd)
        else:
            GlobalBuildRules.PFork(appToExecute="make", argsForApp=makeArgs, wd=wd)

    def build(self):
        print("Building project [%s]" % self._project_name)
        self.executeBuildSteps(["preBuild", "cmake", "make"])

    def uploadPackagedVersion(self):
        print("Uploading project [%s]" % self._project_name)

if __name__ == "__main__":
    localBuild = LocalBuild()
    customCommands = GlobalBuildRules.parseCommandLine(sys.argv[1:])
    localBuild.run(customCommands)
