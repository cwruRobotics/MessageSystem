'''
    This is a python module to compute a factorial. The goal
    is for the argument to be received by C++, packaged
    and delivered to Python, and for the result to be
    packaged and delivered back to C++.

    Take a look at Nodes/PyFactorialNode.<hpp|cpp> for more info
    on the C++ side of this language bridge.
'''

def main(numToFactorial):
    result = 1
    while(numToFactorial > 1):
        result *= numToFactorial
        numToFactorial -= 1
    return result
