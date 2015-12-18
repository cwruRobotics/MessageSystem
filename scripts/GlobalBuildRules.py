
# BUILT IN MODULES
import inspect
import os
import platform
import shutil
import subprocess
import sys
import traceback
# import _winreg

# PYTHON PROJECT MODULES
import LocalEnvironment


def failExecution(errorMsg):
    print("Failed Build Stack:")
    traceback.print_stack(file=sys.stdout)

    print("********************\n* FAILURE %s\n********************\n" % errorMsg)
    sys.exit(1)


def PFork(appToExecute=None, argsForApp=[], wd=None, failOnError=False, environment={}):

    # a list that will contain all arguments with the application to
    # execute as the first element. This is to invoke Popen with,
    # which takes as an argument the commands that will be executed.
    app_and_args = list(argsForApp)
    if appToExecute is not None:
        app_and_args.insert(0, appToExecute)

    # get the current environment and update it with custom environment
    # variables injected into this method.
    real_environment = dict(os.environ)
    real_environment.update(environment)

    # execute the code. This method does not use the shell for safety
    # reasons. It will store the result in a pipe and default write all
    # errors to the console (later we can ignore the pipe and safe time
    # on the builds). Tidbit: "Popen" stands for "process open" also known as "fork."
    # note that this DOES NOT create a new console process (no new window will pop up)
    # when this executes.
    print("Executing %s" % ' '.join(app_and_args))
    childProcess = subprocess.Popen(app_and_args, cwd=wd, shell=False, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, env=real_environment, bufsize=1)

    # (for now) write the messages of the process execution to the console.
    for line in iter(childProcess.stdout.readline, b''):
        print line.rstrip() + '\n',

    # wait for childProcess and let all childProcess values settle
    # (like the returncode variable)
    childProcess.communicate()

    if childProcess.returncode < 0 and failOnError:
        failExecution("PFork failed to execute [%s]" % ' '.join(app_and_args))
    return childProcess.returncode


def mkdir(dir):

    # only create the directory if it doesn't exist.
    # we should only run into the directory already existing
    # DURING a build, as all created directories will be
    # deleted at the beginning of each build.
    if not os.path.exists(dir):
        os.makedirs(dir)


def rmTree(path):
    if os.path.isdir(path):
        shutil.rmtree(path)
    else:
        os.unlink(path)  # same as os.remove(path)


def copyTree(srcPath, destPath):
    if os.path.isdir(srcPath):
        if os.path.isdir(destPath):
            failExecution("cannot copy directory [%s]: %s exists!" %
                          (srcPath, destPath))
        elif os.path.isfile(destPath):
            failExecution("Cannot copy directory [%s]: %s is a File!" %
                          (srcPath, destPath))
        shutil.copytree(srcPath, destPath)
    else:
        shutil.copy2(srcPath, destPath)  # copy2() copies file metaData


# this is no longer windows specific
def getProcessorInfo():
    machine = platform.machine().lower()
    if "x86 64" in machine:
        machine = "x86"
    elif "x86" in machine:
        machine = "x86_arm"
    elif "x64" in machine:
        machine = "x64"
    elif "amd64" not in machine:
        failExecution("unsupported processor: %s" % machine)
    return machine
    '''
    # this is windows specific
    if platform.system() == "Windows":
        return getWindowsRegistryValue(
            "HKEY_LOCAL_MACHINE",
            "SYSTEM\\CurrentControlSet\Control\\Session Manager\\Environment",
            "PROCESSOR_ARCHITECTURE"
        ).lower()
    else:
        failExecution("Cannot get processor info on Unix based system")
    '''

'''
# this is windows specific
def getWindowsRegistryValue(key, subkey, value):
    if platform.system() == "Windows":
        key = getattr(_winreg, key)
        handle = _winreg.OpenKey(key, subkey)

        # this returns (value, type) where the value
        # is the value of the processor and the type
        # is the registry type of the value
        return _winreg.QueryValueEx(handle, value)[0]
    else:
        failExecution("Cannot get registry values on Unix based system")
'''

def getMachineBits():
    if platform.machine().endswith("64"):
        return "x64"
    else:
        return "x86"


def parseCommandLine(commandLine):
    parsedCommandedValues = {}
    parsedCommandedMethods = []
    key = None
    for command in commandLine:
        if '-' in command:
            key = command.replace('-', '', 1)
        elif key is not None:
            parsedCommandedValues[key] = command
            key = None
        else:
            parsedCommandedMethods.append(command)
    return (parsedCommandedMethods, parsedCommandedValues)


def PForkWithVisualStudio(appToExecute=None, argsForApp=[], wd=None, environment={}):
    appAndArgs = list(argsForApp)
    if appToExecute is not None:
        appAndArgs.insert(0, appToExecute)

    # there will be an environment variable set externally called
    # VS_VERSION that will declare the version of Visual Studio's Compiler
    # you want this build to use. Defaults to 2014
    visualStudioPathBase = os.environ.get('VS%s0COMNTOOLS' % (
                                          os.environ.get('VS_VERSION')
                                          if os.environ.get('VS_VERSION') is not None else '14'
                                          ))

    # this is the fully qualified path to vcvarsall.bat
    visualStudioUtilsPath = (
        os.path.join(visualStudioPathBase, '..', '..', 'VC', 'vcvarsall.bat')
        if visualStudioPathBase is not None else
        (
            failExecution('No Visual Studio Compiler present on system')
        )
    )

    # we want to execute the command in the appropriate working directory.
    # so we will change into that directory at the beginning of the command
    # execution.
    if wd is not None:
        wd = os.path.abspath(wd)
        appAndArgs.insert(0, ' cd "%s" && ' % wd)

    # this is important. We want to execute vcvarsall.bat so that the following
    # commands execute in the visual studio environment. vcvarsall.bat prints lots
    # of information and is optimized if we tell it what the processor architecture
    # is. So, we give it the processor architecture and redirect all output to 'nul'
    # which means we won't see it. Then, we add all the commands we want to execute.
    cmdString = '("%s" %s>nul)&&%s' % (visualStudioUtilsPath,
                                       getProcessorInfo(),
                                       ' '.join(appAndArgs))

    # allows additional environment variables to be set / altered.
    realEnv = dict(os.environ)
    realEnv.update(environment)
    print cmdString

    # fork a child process to execute out commands, piping the output to stdout (eventually)
    # and applying the environment given.
    childProcess = subprocess.Popen(cmdString, cwd=wd, shell=True, stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT, env=realEnv, bufsize=1)

    for line in iter(childProcess.stdout.readline, b''):
        print line.rstrip() + '\n'

    # this waits for the childProcess to finish its execution.
    # this is necessary to allow the childProcess variables to settle,
    # most importantly the returncode variable.
    childProcess.communicate()

    # fail execution if there was an error.
    if childProcess.returncode < 0:
        failExecution('execute with visual studio failed with returncode [%s]' % childProcess.returncode)


# this is a nifty function to attempt to call any callable object with its desired arguments.
def call(callable, args):
    # we need to build a dictionary of all the arguments that the callable object requires
    # to execute
    callableArgs = {}

    # inspect is a cool module that is used to analyze a callable object at runtime.
    # here we are getting the set of parameters that "callable" has in its method/function
    # signature. inspect has converted the parameter names to strings.
    for arg in inspect.getargspec(callable).args:
        if arg in args:
            callableArgs[arg] = args[arg]

    # note that at this point, callableArgs may NOT contain all arguments in "callables"
    # method/function signature. This is ok. Python allows default values to be specified
    # so we will hope that the arguments NOT contained in "callableArgs" are default parameters.
    # An example of this is if "callable" is a method. Methods in Python require the first
    # parameter to be "self" referring to the object that the method belongs to. inspect will
    # return "self" as a parameter, which we cannot provide, but will be provided by the Python
    # interpreter when executed.

    try:
        # this is known as variable unpacking. We are taking the dictionary, and unpacking
        # it, which the interpreter treats as a parameter list in this context. It will build
        # a final parameter list combining the default values of the method/function and the ones
        # we have provided and call the method/function with it. This throws an error if the total
        # parameter list does not match the callable's signature EXACTLY. This is why we needed
        # to construct "callableArgs" in the first place, to filter out excess arguments.
        callable(**callableArgs)
        return True
    except:
        failExecution("Error: %s" % sys.exc_info()[1])


class FileSystemDirectory():
    ROOT = 1                    # the absolute path to the top level of the project
    WORKING = 2                 # the absolute path to the build directory of the project
    # the absolute path to the directory where all static scripts are kept
    #   (a static script is abstract enough that it can be shared between
    #    projects, this script is a good example)
    SCRIPT_ROOT = 3

    PROJECT_ROOT = 4            # the absolute path to the top level CMAKE directory of the project
    MANUAL_DIR = 5              # the absolute path to the documentation directory of the project
    CPP_SOURCE_DIR = 6          # the absolute path to the top level directory of c++ source directories
    TEST_ROOT = 7               # the absolute path to the top level directory of c++ unit test directories
    TEST_REPORT_DIR = 8         # the absolute path to the directory containing test reports
    CMAKE_BASE_DIR = 9          # the absolute path to the top level directory of CMAKE utilities

    # the absolute path to the CMAKE toolchain directory
    #   - toolchain files are used for certain platforms to separate
    #     CMAKE functionality
    CMAKE_TOOLCHAIN_DIR = 10
    CMAKE_MODULE_DIR = 11       # the absolute path to the CMAKE modules directory
    OUT_ROOT = 12               # the absolute path to the directory where built code goes
    INSTALL_ROOT = 13           # the absolute path to the top level directory where all built code goes
    INSTALL_DIR = 14            # the absolute path to the directory where public c++ headers go when built
    LOG_DIR = 15                # the absolute path to the directory where all log files will be written


# a method to get the absolute path to a directory within the project based
# on the FileSystemDirectory enums described above
def getDirectory(directoryEnum, configuration='', projectName=''):
    if directoryEnum == FileSystemDirectory.ROOT:
        return os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    elif directoryEnum == FileSystemDirectory.WORKING:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'build', configuration, projectName)
    elif directoryEnum == FileSystemDirectory.SCRIPT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'scripts')
    elif directoryEnum == FileSystemDirectory.PROJECT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'projects')
    elif directoryEnum == FileSystemDirectory.MANUAL_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'manual')
    elif directoryEnum == FileSystemDirectory.CPP_SOURCE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'cpp')
    elif directoryEnum == FileSystemDirectory.TEST_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.CPP_SOURCE_DIR), 'src', 'unitTest')
    elif directoryEnum == FileSystemDirectory.TEST_REPORT_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.WORKING, configuration, projectName), 'testReports')
    elif directoryEnum == FileSystemDirectory.CMAKE_BASE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'cmake')
    elif directoryEnum == FileSystemDirectory.CMAKE_TOOLCHAIN_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.CMAKE_BASE_DIR), 'toolchains')
    elif directoryEnum == FileSystemDirectory.CMAKE_MODULE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.CMAKE_BASE_DIR), 'modules')
    elif directoryEnum == FileSystemDirectory.OUT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.WORKING, configuration, projectName), 'out')
    elif directoryEnum == FileSystemDirectory.INSTALL_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.OUT_ROOT, configuration, projectName), 'installRoot')
    elif directoryEnum == FileSystemDirectory.INSTALL_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.OUT_ROOT, configuration, projectName), 'install')
    elif directoryEnum == FileSystemDirectory.LOG_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.WORKING, configuration, projectName), 'logs')
    else:
        failExecution("Unknown directoryEnum: [%s]" % directoryEnum)


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
        self._build_directory = getDirectory(FileSystemDirectory.WORKING)

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
        buildDirectory = getDirectory(FileSystemDirectory.WORKING, self._config)
        if os.path.exists(buildDirectory):
            rmTree(buildDirectory)

    def setupWorkspace(self):
        print("Setting up workspaces for project [%s]" % self._project_name)
        self.cleanBuildWorkspace()
        mkdir(getDirectory(FileSystemDirectory.WORKING, self._config, self._project_name))

    def generateProjectVersion(self):
        outIncludeDir = os.path.join(
            getDirectory(FileSystemDirectory.OUT_ROOT),
            'include'
        )
        print("making directory %s" % outIncludeDir)
        mkdir(outIncludeDir)
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
            getDirectory(FileSystemDirectory.OUT_ROOT, self._config, self._project_name), 'dummy')

        # projectWorkingDir = getDirectory(FileSystemDirectory.ROOT, self._config, self._project_name)
        installRootDir = getDirectory(FileSystemDirectory.INSTALL_ROOT, self._config, self._project_name)

        # all of these are relative paths that are used by CMake
        # to place the appropriate build components in the correct
        # directories.
        binDir = os.path.relpath(
            os.path.join(getDirectory(FileSystemDirectory.INSTALL_ROOT, self._config, self._project_name), "bin"),
            dummyDir
        )

        includeDir = os.path.relpath(
            os.path.join(getDirectory(FileSystemDirectory.INSTALL_DIR, self._config, self._project_name), "include"),
            dummyDir
        )

        libDir = os.path.relpath(
            os.path.join(getDirectory(FileSystemDirectory.INSTALL_ROOT, self._config, self._project_name), "lib"),
            dummyDir
        )
        outIncludeDir = os.path.join(getDirectory(FileSystemDirectory.OUT_ROOT, self._config, self._project_name),
                                     "include")

        toolchainDir = os.path.relpath(getDirectory(FileSystemDirectory.CMAKE_TOOLCHAIN_DIR), workingDirectory)
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
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_windows_%s.cmake" % getMachineBits())
        elif platform.system() == "Linux":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_linux_%s.cmake" % getMachineBits())
        elif platform.system() == "OSX":
            fullToolchainPath = os.path.join(toolchainDir, "toolchain_osx_%s.cmake" % getMachineBits())
        else:
            failExecution("Unsupported OS: %s" % platform.system())

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
            "-DPROCESSOR=%s" % getProcessorInfo(),
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
            success = call(method, self._custom_args)
            if not success:
                failExecution("Build step [%s] failed" % buildStep)
        else:
            failExecution("Project %s does not have build step [%s]" %
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
            failExecution("Project name not set")

        # if the user has not specified any build steps, run the default
        if len(buildSteps) == 0:
            buildSteps = self._build_steps

        # run the build for the user specified configuration else run for
        # all configurations (the user can restrict this to build for
        # debug or release versions)
        if "configuration" in self._custom_args:
            self._config = self._custom_args["configuration"]
            if self._config != "release" and self._config != "debug":
                failExecution("Unknown configuration [%s]" % self._config)
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
