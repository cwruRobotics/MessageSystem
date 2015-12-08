
import inspect
import os
import shutil
import subprocess
import sys
import traceback
import _winreg


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
        os.mkdir(dir)


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


# this is windows specific
def getProcessor():
    return getRegistryValue(
        "HKEY_LOCAL_MACHINE",
        "SYSTEM\\CurrentControlSet\Control\\Session Manager\\Environment",
        "PROCESSOR_ARCHITECTURE"
    ).lower()


# this is windows specific
def getRegistryValue(key, subkey, value):
    key = getattr(_winreg, key)
    handle = _winreg.OpenKey(key, subkey)

    # this returns (value, type) where the value
    # is the value of the processor and the type
    # is the registry type of the value
    return _winreg.QueryValueEx(handle, value)[0]


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
        appAndArgs.append(appToExecute)

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
        appAndArgs.insert(0, 'cd "%s" &&' % wd)

    # this is important. We want to execute vcvarsall.bat so that the following
    # commands execute in the visual studio environment. vcvarsall.bat prints lots
    # of information and is optimized if we tell it what the processor architecture
    # is. So, we give it the processor architecture and redirect all output to 'nul'
    # which means we won't see it. Then, we add all the commands we want to execute.
    cmdString = '("%s" %s > nul) && %s' % (visualStudioUtilsPath,
                                            getProcessor(),
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


def call(callable, args):
    callableArgs = {}
    for arg in inspect.getargspec(callable).args:
        if arg in args:
            callableArgs[arg] = args[arg]

    try:
        callable(**callableArgs)
        return True
    except:
        failExecution("Error: %s" % sys.exc_info()[0])


class FileSystemDirectory():
    ROOT = 1
    WORKING = 2
    SCRIPT_ROOT = 3
    PROJECT_ROOT = 4
    MANUAL_DIR = 5
    CPP_INCLUDE_DIR = 6
    CPP_SOUCE_DIR = 7
    TEST_ROOT = 8
    TEST_REPORT_DIR = 9
    CMAKE_BASE_DIR = 10
    CMAKE_TOOLCHAIN_DIR = 11
    CMAKE_MODULE_DIR = 12
    OUT_ROOT = 13
    INSTALL_ROOT = 14
    INSTALL_DIR = 15


def getDirectory(directoryEnum):
    if directoryEnum == FileSystemDirectory.ROOT:
        return os.path.join(os.getcwd(), '..')
    elif directoryEnum == FileSystemDirectory.WORKING:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'build')
    elif directoryEnum == FileSystemDirectory.SCRIPT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'scripts')
    elif directoryEnum == FileSystemDirectory.PROJECT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'projects')
    elif directoryEnum == FileSystemDirectory.MANUAL_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'manual')
    elif directoryEnum == FileSystemDirectory.CPP_INCLUDE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'cpp', 'include')
    elif directoryEnum == FileSystemDirectory.CPP_SOUCE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'cpp', 'source')
    elif directoryEnum == FileSystemDirectory.TEST_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.CPP_SOUCE_DIR), 'unittest')
    elif directoryEnum == FileSystemDirectory.TEST_REPORT_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.WORKING), 'testReports')
    elif directoryEnum == FileSystemDirectory.CMAKE_BASE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.ROOT), 'cmake')
    elif directoryEnum == FileSystemDirectory.CMAKE_TOOLCHAIN_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.CMAKE_BASE_DIR), 'toolchains')
    elif directoryEnum == FileSystemDirectory.CMAKE_MODULE_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.CMAKE_BASE_DIR), 'modules')
    elif directoryEnum == FileSystemDirectory.OUT_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.WORKING), 'out')
    elif directoryEnum == FileSystemDirectory.INSTALL_ROOT:
        return os.path.join(getDirectory(FileSystemDirectory.OUT_ROOT), 'installRoot')
    elif directoryEnum == FileSystemDirectory.INSTALL_DIR:
        return os.path.join(getDirectory(FileSystemDirectory.OUT_ROOT), 'install')
    else:
        failExecution("Unknown directoryEnum: [%s]" % directoryEnum)


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

    def testMethod(self):
        print("Test complete!")

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

    def executeBuildSteps(self, buildSteps):
        for buildStep in buildSteps:
            self.executeStep(buildStep)

    def run(self, parsedCommandLine):
        (buildSteps, self._custom_args) = parsedCommandLine

        # if self._project_name == "":
        #    failExecution("Project name not set")

        if len(buildSteps) == 0:
            buildSteps = self._build_steps

        if "configuration" in self._custom_args:
            print("\n[%s]\n" % self._custom_args["configuration"])
            self._config = self._custom_args["configuration"]
            self.executeBuildSteps(buildSteps)
        else:
            for configuration in self._configurations:
                print("\n[%s]\n" % configuration)
                self._config = configuration
                self.executeBuildSteps(buildSteps)

        print("********************")
        print("*     COMPLETE     *")
        print("********************")
