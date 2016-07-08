
# SYSTEM IMPORTS
import os
import platform
import sys


# PYTHON PROJECT IMPORTS
currentDir = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(currentDir, "scripts"))  # now we can import modules from <currentDirectory>/scripts
import LocalBuildRules
import GlobalBuildRules
import Utilities
import FileSystem



if __name__ == "__main__":
    localBuild = None
    customCommands = Utilities.parseCommandLine(sys.argv[1:])
    print(customCommands)
    if "projects" not in customCommands[1]:
        customCommands[1]["projects"] = ["Logging", "Utilities", "Async", "Robos"]

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
