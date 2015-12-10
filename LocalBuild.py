
import os
import platform
import sys

sys.path.append("scripts")
import GlobalBuildRules

class LocalBuild(GlobalBuildRules.GlobalBuild):
    def __init__(self):
        super(LocalBuild, self).__init__()
