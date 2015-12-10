
# BUILT IN MODULES
# import os
# import platform
import sys


# PYTHON PROJECT MODULES
sys.path.append("scripts")  # now we can import modules from <currentDirectory>/scripts
import GlobalBuildRules


class LocalBuild(GlobalBuildRules.GlobalBuild):
    def __init__(self):
        super(LocalBuild, self).__init__()

        # we can override member variables defined in GlobalBuildRules
        self._project_name = "MessageFramework"
        self._project_namespace = "CWRUBotix"
        self._build_steps = ["setupWorkspace",
                             "build",
                             "runUnitTests",
                             "package",
                             "uploadPackagedVersion"]  # MessageFramework specific default build steps

    def build(self):
        print("Building project [%s]" % self._project_name)

    def uploadPackagedVersion(self):
        print("Uploading project [%s]" % self._project_name)

if __name__ == "__main__":
    localBuild = LocalBuild()
    customCommands = GlobalBuildRules.parseCommandLine(sys.argv[1:])
    localBuild.run(customCommands)
