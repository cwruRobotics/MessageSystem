
import os
import shutil
import subprocess
import sys
import traceback


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
