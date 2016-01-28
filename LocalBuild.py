
# SYSTEM IMPORTS
import os
import platform
import sys


# PYTHON PROJECT IMPORTS
sys.path.append("scripts")  # now we can import modules from <currentDirectory>/scripts
import LocalBuildRules
import Utilities
import FileSystem



if __name__ == "__main__":
    localBuild = None
    customCommands = Utilities.parseCommandLine(sys.argv[1:])
    print(customCommands)
    if "projects" not in customCommands[1]:
        customCommands[1]["projects"] = ["Logging", "Async"] #, "Robos"]

    help = False
    for projectName in customCommands[1]["projects"]:
        localBuild = LocalBuildRules.LocalBuild(projectName)
        if "help" in customCommands[0]:
            localBuild.help()
            help = True
        else:
            localBuild.run(customCommands)
    if help:
        GlobalBuildRules.help()
