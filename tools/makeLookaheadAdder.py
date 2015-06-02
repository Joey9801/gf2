#!/usr/bin/python
"""
Creates a logic definition file for an N bit ripple adder
"""

import json
import sys

if len(sys.argv) == 2:
    n = int(sys.argv[1])
else:
    n = 4

definition = dict()

definition['includes'] = { 'example_defs/full_adder.def' : 'adder' }

#Create input vectors
definition['inputs'] = dict()
definition['inputs']['a[' + str(n) + ']'] = False
definition['inputs']['b[' + str(n) + ']'] = False
definition['inputs']['carry'] = False

#define outputs
definition['outputs'] = dict()
definition['outputs']['carry'] = 'adder' + str(n-1) + '.c_out'
outVec = dict()
for i in range(n):
    outVec[ str(i) ] = 'adder' + str(i) + '.s'

definition['outputs']['out[' + str(n) + ']'] = outVec

#Create the components
components = dict()

#First make all the adders
for i in range(n):
    c = dict();
    c['type'] = 'includes.adder'
    c['inputs'] = {
        'a' : 'inputs.a[' + str(i) + ']',
        'b' : 'inputs.b[' + str(i) + ']',
        'c_in' : 'c' + str(i)
    }
    components['adder' + str(i)] = c

components['adder0']['inputs']['c_in'] = 'inputs.carry'

#Then make the 'generate' bits
for i in range(n):
    c = dict()
    c['type'] = 'and'
    c['inputs'] = {
        'i1' : 'inputs.a[' + str(i) + ']',
        'i2' : 'inputs.b[' + str(i) + ']'
    }
    components['g' + str(i)] = c

#Then make the 'propagate' bits
for i in range(n):
    c = dict()
    c['type'] = 'xor'
    c['inputs'] = {
        'i1' : 'inputs.a[' + str(i) + ']',
        'i2' : 'inputs.b[' + str(i) + ']'
    }
    components['p' + str(i)] = c

#Make all the bits we OR together to make the carry bits
for i in range(n):
    """The components in the form C0.P0.P1.Pn"""
    c = {
        'type' : 'and',
        'inputs' : {
            'i1' : 'inputs.carry'
        }
    }

    for j in range(i+1):
        c['inputs']['i' + str(j+2)] = 'p' + str(j)

    components['int' + str(i)] = c

for i in range(n):
    """The components in the form Gn.Pn+1.Pn+2"""
    for j in range(n-i):
        c = {
            'type' : 'and',
            'inputs' : {
                'i1' : 'g' +  str(i)
            }
        }
        for k in range(j):
            c['inputs']['i' + str(k+2)] = 'p' + str(k+i+1)

        components['int' + str(i) + '-' + str(i+j)] = c

#Finally, make the carry bits
for i in range(1, n):
    c = dict()
    c['type'] = 'or'
    c['inputs'] = dict()

    c['inputs']['i1'] = 'int' + str(i-1)
    for j in range(i):
        c['inputs']['i' + str(j+2)] = 'int' + str(j) + '-' + str(i-1)

    components['c' + str(i)] = c



definition['components'] = components


# Create the definition string - just json without the quotes
def_string = json.dumps(definition, indent=2)
def_string = def_string.translate(None, '"')
def_string = def_string.translate(None, '\n ')
print(def_string)
