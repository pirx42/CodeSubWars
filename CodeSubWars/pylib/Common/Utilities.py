# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

import operator

# g(f1)
def make_g_f1(g, f1):
    def tmp_g(*parameters):
        return g(f1(*parameters))
    return tmp_g

# g(f1, f2)
def make_g_f2(g, f1, f2):
    def tmp_g(*parameters):
        return g(f1(*parameters), f2(*parameters))
    return tmp_g

#g(f1, f2, ..., fn)
#does not work, because g does not take necessary *p as parameter
#def make_g_fn(g, *functions):
#    def tmp_g(*parameters):
#        *a = [f(*parameters) for f in functions]
#        return g(a)
#    return tmp_g


#negates the given boolean function
def make_not_f1(f):
    return make_g_f1(operator.__not__, f)


#combines two given boolean function by AND
def make_and_f2(f1, f2):
    return make_g_f2(operator.__and__, f1, f2)


#combines two given boolean function by OR
def make_or_f2(f1, f2):
    return make_g_f2(operator.__or__, f1, f2)

