
# BUILT IN MODULES
import os
import platform
# import _winreg

# PYTHON PROJECT MODULES
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
        buildDirectory = FileSystem.getDirectory(FileSystem.WORKING, self._config)
        if os.path.exists(buildDirectory):
            Utilities.rmTree(buildDirectory)

    def setupWorkspace(self):
        print("Setting up workspaces for project [%s]" % self._project_name)
        self.cleanBuildWorkspace()
        Utilities.mkdir(FileSystem.getDirectory(FileSystem.WORKING, self._config, self._project_name))

    def generateProjectVersion(self):
        outIncludeDir = os.path.join(
            FileSystem.getDirectory(FileSystem.OUT_ROOT),
            'include'
        )
        print("making directory %s" % outIncludeDir)
        Utilities.mkdir(outIncludeDir)
        with open(os.path.join(outIncludeDir, 'Version.h'), 'w') as file:
            file.write("#ifndef VERSION_H \n"
                       "#define VERSION_H \n\n"
                       "#define VERSION       " + self._project_build_number + "\n"
                       "#define VERSION_STR  \"" + self._project_build_number + "\"\n\n"
                       "#endif  // VERSION_H\n\n")

    def preBuild(self):
        self.setupWorkspace()
        self.generateProjectVersion()

    def getCMakeArgs(self, pathPrefix, workingDirectory):
        CMakeProjectDir = "projects/%s" % self._project_name
        relCMakeProjectDir = os.path.relpath(CMakeProjectDir,
                                             workingDirectory)

        dummyDir = os.path.join(
            FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config, self._project_name), 'dummy')

        # projectWorkingDir = getDirectory(FileSystemDirectory.ROOT, self._config, self._project_name)
        installRootDir = FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name)

        # all of these are relative paths that are used by CMake
        # to place the appropriate build components in the correct
        # directories.
        binDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name), "bin"),
            dummyDir
        )

        includeDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_DIR, self._config, self._project_name), "include"),
            dummyDir
        )

        libDir = os.path.relpath(
            os.path.join(FileSystem.getDirectory(FileSystem.INSTALL_ROOT, self._config, self._project_name), "lib"),
            dummyDir
        )
        outIncludeDir = os.path.join(FileSystem.getDirectory(FileSystem.OUT_ROOT, self._config, self._project_name),
                                     "include")

        toolchainDir = os.path.relpath(FileSystem.getDirectory(FileSystem.CMAKE_TOOLCHAIN_DIR), workingDirectory)
        if platform.system() == "Windows":
            installRootDir = "\"%s\"" % installRootDir.replace("\\", "/")
            outIncludeDir = "\"%s\"" % outIncludeDir.replace("\\", "/")
            toolchain = "\"%s\"" % toolchainDir.replace("\\", "/")

        print("installRootDir: %s" % installRootDir)
        print("binDir: %s" % binDir)
        print("includeDir: %s" % includeDir)
        print("libDir: %s" % libDir)
        print("outIncludeDir: %s" % outIncludeDir)

        if self._config == "release":
            cmake_config = "Release"
        else:
            cmake_config = "Debug"

        fullToolchainPath = None
        if platform.system() == "Windows":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_windows_%s.cmake" % Utilities.getMachineBits())
        elif platform.system() == "Linux":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_linux_%s.cmake" % Utilities.getMachineBits())
        elif platform.system() == "OSX":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_osx_%s.cmake" % Utilities.getMachineBits())
        else:
            Utilities.failExecution("Unsupported OS: %s" % platform.system())

        # remember CMake paths need to be relative to the top level
        # directory that CMake is called (in this case projects/<project_name>)
        CMakeArgs = [
            relCMakeProjectDir,
            # "--build \"%s\"" % (workingDirectory),
            "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, binDir),
            "-DCMAKE_INCLUDE_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, includeDir),
            "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, libDir),
            "-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=%s%s" % (pathPrefix, libDir),
            "-DCMAKE_PREFIX_PATH=%s" % (installRootDir),  # absolute path
            "-DOUT_INCLUDE_DIRECTORY=%s" % (outIncludeDir),  # absolute path
            "-DCMAKE_BUILD_TYPE=%s" % cmake_config,
            "-DPROCESSOR=%s" % Utilities.getProcessorInfo(),
            "-DCMAKE_TOOLCHAIN_FILE=%s" % fullToolchainPath  # toolchain file path (relative)
            # "-DINSTALL_ROOT=%s" %  # install root dir (absolute)
            # "-DCMAKE_PREFIX_PATH=%s" %  # out include dir path (absolute)
            # "-DGTEST_ROOT=%s" %  # gtest root (absolute)
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

    def runUnitTests(self):
        print("Running unit tests for project [%s]" % self._project_name)

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
