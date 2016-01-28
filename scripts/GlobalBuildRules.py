
# SYSTEM IMPORTS
import os
import platform
# import _winreg

# PYTHON PROJECT IMPORTS
import LocalEnvironment
import Utilities
import FileSystem


# the parent class of the build process. It contains methods common to all build processes as well
# as the ability to run methods in user specified order with user given arguments.
class GlobalBuild(object):
    def __init__(self):
        self._project_name = ""
        self._project_namespace = ""
        self._source_dirs = ["cpp"]
        self._build_steps = []
        self._project_build_number = "0.0.0.0"
        self._configurations = ["debug", "release"]
        self._build_directory = FileSystem.getDirectory(FileSystem.WORKING)

        # execute local environment
        projectSpecificEnvVars = {
            # project specific static environment variables
        }
        localEnv = LocalEnvironment.LocalEnvironment(projectSpecificEnvVars)
        localEnv.injectAllEnvironmentVariables()

    # removes previous builds so that this build
    # is a fresh build (on this machine). This
    # guarentees that this build uses the most recent
    # source files.
    def cleanBuildWorkspace(self):
        print("Cleaning build directory for project [%s]" % self._project_name)
        buildDirectory = FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name)
        if os.path.exists(buildDirectory):
            Utilities.rmTree(buildDirectory)

    def parseDependencyFile(self):
        dependencyFilePath = os.path.join(FileSystem.getDirectory(FileSystem.DEPENDENCIES), "dependencies.txt")
        if not os.path.exists(dependencyFilePath):
            Utilities.failExecution("dependency file [%s] does not exist" % dependencyFilePath)
        requiredProjects = []
        with open(dependencyFilePath, 'r') as file:
            flag = False
            lineNum = 0
            splitLine = None
            for line in file:
                splitLine = line.strip().split(None)
                if len(splitLine) == 0 or splitLine[0] == '#':
                    continue
                if splitLine[0] == '-' + self._project_name:
                    flag = True
                elif flag and '-' not in splitLine[0]:
                    requiredProjects.append(splitLine[0])
                elif flag and '-' in splitLine[0] and ('-' + self._project_name) not in splitLine[0]:
                    flag = False
                elif not flag:
                    continue
                else:
                    Utilities.failExecution("Parse error in dependency file [%s] at line [%s]"
                                            % (dependencyFilePath, lineNum))
                lineNum += 1
        print("Required projects for project [%s] are %s" % (self._project_name, requiredProjects))
        return requiredProjects

    def moveDependentProjectsToWorkingDir(self, projectToCopyFrom, rootToCopyFrom):
        destinationDir = FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name)

        destLibDir = os.path.join(destinationDir, "lib")
        if not os.path.exists(destLibDir):
                Utilities.mkdir(destLibDir)
        if platform.system() == "Windows":
            destBinDir = os.path.join(destinationDir, "bin")
            if not os.path.exists(destBinDir):
                Utilities.mkdir(destBinDir)
            # copy dll and lib
            Utilities.copyTree(os.path.join(rootToCopyFrom, "bin", projectToCopyFrom + ".dll"), destBinDir)
            Utilities.copyTree(os.path.join(rootToCopyFrom, "lib", projectToCopyFrom + ".lib"), destLibDir)
        else:
            # copy .so
            Utilities.copyTree(os.path.join(rootToCopyFrom, "lib", "lib" + projectToCopyFrom + ".so"), destLibDir)
        Utilities.copyTree(os.path.join(rootToCopyFrom, "include", projectToCopyFrom),
                           os.path.join(destinationDir, "include", projectToCopyFrom))

    def buildAndLoadDependencies(self):
        rootBuildDirectory = FileSystem.getDirectory(FileSystem.WORKING, self._config)
        dependentProjects = self.parseDependencyFile()
        print("dependentProjects %s" % dependentProjects)
        projectBuild = None
        
        Utilities.mkdir(FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name))
        allBuiltOutDir = FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config)
        for project in dependentProjects:
            dependentProjectPath = FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config,  project)

            # build the project if necessary
            if not os.path.exists(dependentProjectPath):
                projectBuild = LocalBuildRules.LocalBuild(project)
                projectBuild.run(([], {'configuration': self._config}))

            # copy over necessary objects to link
            self.moveDependentProjectsToWorkingDir(project, allBuiltOutDir)
            

    def setupWorkspace(self):
        print("Setting up workspaces for project [%s]" % self._project_name)
        self.cleanBuildWorkspace()
        Utilities.mkdir(FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name))
        self.buildAndLoadDependencies()

    def generateProjectVersion(self):
        outIncludeDir = os.path.join(
            FileSystem.getDirectory(FileSystem.OUT_ROOT),
            'include'
        )
        print("making directory %s" % outIncludeDir)
        Utilities.mkdir(outIncludeDir)
        with open(os.path.join(outIncludeDir, 'Version.hpp'), 'w') as file:
            file.write("#pragma once\n"
                       "#ifndef VERSION_H\n"
                       "#define VERSION_H\n\n"
                       "#define VERSION       " + self._project_build_number + "\n"
                       "#define VERSION_STR  \"" + self._project_build_number + "\"\n\n"
                       "#endif  // VERSION_H\n\n")

    def generateLoggingConfig(self):
        outIncludeDir = os.path.join(FileSystem.getDirectory(FileSystem.OUT_ROOT),
                                     "include")
        projectLogDir = FileSystem.getDirectory(FileSystem.LOG_DIR, self._config, self._project_name)
        Utilities.mkdir(outIncludeDir)

        if os.path.exists(projectLogDir):
            Utilities.rmTree(projectLogDir)
        Utilities.mkdir(projectLogDir)

        with open(os.path.join(outIncludeDir, self._project_name + "LoggingConfig.hpp"), 'w') as file:
            file.write("#pragma once\n"
                       "#ifndef " + self._project_name.upper() + "_LOGGING_LOGGINGCONFIG_HPP\n"
                       "#define " + self._project_name.upper() + "_LOGGING_LOGGINGCONFIG_HPP\n\n"
                       "// SYSTEM INCLUDES\n"
                       "#include <string>\n\n"
                       "// C++ PROJECT INCLUDES\n\n"
                       "namespace " + self._project_name + "\n"
                       "{\n"
                       "namespace Logging\n"
                       "{\n\n"
                       "    extern const std::string LOGGING_ROOT;\n\n"
                       "} // end of namespace Logging\n"
                       "} // end of namespace " + self._project_name + "\n"
                       "#endif // end of " + self._project_name + "_LOGGING_LOGGINGCONFIG_HPP\n")
        with open(os.path.join(outIncludeDir, self._project_name + "LoggingConfig.cpp"), 'w') as file:
            file.write("// SYSTEM INCLUDES\n\n"
                       "// C++ PROJECT INCLUDES\n"
                       "#include \"" + self._project_name + "LoggingConfig.hpp\"\n\n"
                       "namespace Async\n"
                       "{\n"
                       "namespace Logging\n"
                       "{\n\n"
                       "    const std::string LOGGING_ROOT = \"" + projectLogDir.replace("\\", "/") + "\";\n\n"
                       "} // end of namespace Logging\n"
                       "} // end of namespace Async\n")

    def preBuild(self):
        self.setupWorkspace()
        self.generateProjectVersion()
        self.generateLoggingConfig()

    def getCMakeArgs(self, pathPrefix, workingDirectory):
        CMakeProjectDir = "projects"
        relCMakeProjectDir = os.path.relpath(CMakeProjectDir,
                                             workingDirectory)

        dummyDir = os.path.join(
            FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config, self._project_name), 'dummy')

        # projectWorkingDir = getDirectory(FileSystemDirectory.ROOT, self._config, self._project_name)
        installRootDir = FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config,  self._project_name)

        # all of these are relative paths that are used by CMake
        # to place the appropriate build components in the correct
        # directories.
        binDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name), "bin"),
            dummyDir
        )

        includeDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_DIR, self._config), "include"),
            dummyDir
        )

        libDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name), "lib"),
            dummyDir
        )
        outIncludeDir = os.path.join(FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config, self._project_name),
                                     "include")

        toolchainDir = os.path.relpath(FileSystem.getDirectory(FileSystem.CMAKE_TOOLCHAIN_DIR), workingDirectory)
        allBuiltOutDir = FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config)
        if platform.system() == "Windows":
            installRootDir = "\"%s\"" % installRootDir.replace("\\", "/")
            outIncludeDir = "\"%s\"" % outIncludeDir.replace("\\", "/")
            toolchain = "\"%s\"" % toolchainDir.replace("\\", "/")

        if self._config == "release":
            cmake_config = "Release"
        else:
            cmake_config = "Debug"

        fullToolchainPath = None
        if platform.system() == "Windows":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_windows_%s.cmake" % Utilities.getMachineBits())
        else:
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_unix_%s.cmake" % Utilities.getMachineBits())

        # remember CMake paths need to be relative to the top level
        # directory that CMake is called (in this case projects/<project_name>)
        CMakeArgs = [
            relCMakeProjectDir,
            # "--build \"%s\"" % (workingDirectory),
            "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, binDir),
            # "-DCMAKE_INCLUDE_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, includeDir),
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, libDir),
            "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, libDir),
            "-DCMAKE_PREFIX_PATH=%s" % (installRootDir),  # absolute path
            # "-DOUT_INCLUDE_DIRECTORY=%s" % (outIncludeDir),  # absolute path
            "-DCMAKE_BUILD_TYPE=%s" % cmake_config,
            "-DPROCESSOR=%s" % Utilities.getProcessorInfo(),
            "-DCMAKE_TOOLCHAIN_FILE=%s" % fullToolchainPath,  # toolchain file path (relative)
            "-DBUILD_%s=ON" % self._project_name.upper(),
            "-DCMAKE_INSTALL_PREFIX=%s" % allBuiltOutDir, # install root dir
            "-DRUN_UNIT_TESTS=ON",
            # "-DINSTALL_ROOT=%s" %  # install root dir (absolute)
            # "-DCMAKE_PREFIX_PATH=%s" %  # out include dir path (absolute)
            # "-DCMAKE_INCLUDE_PATH=%s" % (dependencyIncludeDir)  # absolute path
        ]
        return CMakeArgs
        

    # this method will generate documentation
    # of the project. We are using Doxygen
    # to fulfill this.
    def document(self):
        print("generating documentation for project [%s]" % self._project_name)

    # this method will package the project into
    # a gzipped tarball (tar.gz) file.
    def package(self):
        print("packaging project [%s]" % self._project_name)

    def runUnitTests(self, numTimesToRun=1):
        print("Running unit tests for project [%s]" % self._project_name)
        installRoot = FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config,  self._project_name)
        for iteration in range(0, int(numTimesToRun)):
            print("Running unit tests [%s/%s]" % (iteration + 1, numTimesToRun))
            for testToRun in self._tests_to_run:
                if platform.system() == "Windows":
                    testToRun += ".exe"
                executablePath = os.path.join(installRoot, "bin", testToRun)
                if os.path.exists(executablePath):
                    Utilities.PFork(appToExecute=executablePath, failOnError=True)
                else:
                    print("%s does NOT exist!" % executablePath)
            if numTimesToRun > 1:
                print("\n\n")

    # executes a particular part of the build process and fails the build
    # if that build step fails.
    def executeStep(self, buildStep):
        if hasattr(self, buildStep):
            print("-Executing build step [%s]" % buildStep)
            method = getattr(self, buildStep)
            success = Utilities.call(method, self._custom_args)
            if not success:
                Utilities.failExecution("Build step [%s] failed" % buildStep)
        else:
            Utilities.failExecution("Project %s does not have build step [%s]" %
                                    (self._project_name, buildStep))

    # executes all build steps
    def executeBuildSteps(self, buildSteps):
        for buildStep in buildSteps:
            self.executeStep(buildStep)

    # entry point into the build process. At this point, user supplied
    # methods and arguments will have been entered and parsed. If no methods
    # are present, then the default build steps will be run.
    # (note that the default steps will be left to the child (LocalBuild) of
    #  GlobalBuild to define so that the default build steps can be unique
    #  for each project).
    def run(self, parsedCommandLine):
        (buildSteps, self._custom_args) = (parsedCommandLine[0], parsedCommandLine[1])

        # this build MUST have a project name to run
        if self._project_name == "":
            Utilities.failExecution("Project name not set")

        # if the user has not specified any build steps, run the default
        if len(buildSteps) == 0:
            buildSteps = self._build_steps

        # run the build for the user specified configuration else run for
        # all configurations (the user can restrict this to build for
        # debug or release versions)
        if "configuration" in self._custom_args:
            self._config = self._custom_args["configuration"]
            if self._config != "release" and self._config != "debug":
                Utilities.failExecution("Unknown configuration [%s]" % self._config)
            print("\nbuilding configuration [%s]\n" % self._config)
            self.executeBuildSteps(buildSteps)
        else:
            for configuration in self._configurations:
                print("\nbuilding configuration [%s]\n" % configuration)
                self._config = configuration
                self.executeBuildSteps(buildSteps)

        print("********************")
        print("*     COMPLETE     *")
        print("********************")


def help():
    print "global commands:"
    print "     global build steps:"
    print "         setupWorkspace              removed previous project build files and directories."
    print "         document                    generates documentation for all project source files."
    print "         package                     packages up project binaries and public headers for"
    print "                                     distribution."
    print "         runUnitTests                runs the project unit tests."
    print "     global custom variables:"
    print "         -configuration <project>    the configuration of the build (debug or release)."
    print "         -projects  <projects...>    the projects that will be built and the order in which"
    print "                                     they are built."
    print "         -numTimesToRun <num>        the number of times that unit tests will be run as part"
    print "                                     or the build process."
    print ""
    print ""

import LocalBuildRules
