def VoidFunctionNoArgs():
    print("Calling VoidFunctionNoArgs")
    x = 5

def VoidFunctionOneArg(arg):
    print("Calling VoidFunctionOneArg")
    x = arg + 5

def VoidFunctionTwoArgs(arg1, arg2):
    print("Calling VoidFunctionTwoArgs")
    x = arg1 + arg2

def ReturnFunctionNoArgs():
    print("Calling ReturnFunctionNoArgs")
    c = 10 * 5
    return c

def ReturnFunctionOneArg(arg):
    print("Calling ReturnFunctionOneArg")
    return arg + 5

def ReturnFunctionTwoArgs(arg1, arg2):
    print("Calling ReturnFunctionTwoArgs")
    return arg1 + arg2
