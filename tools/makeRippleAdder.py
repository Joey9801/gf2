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
for i in range(n):
    c = dict();
    c['type'] = 'includes.adder'
    c['inputs'] = {
        'a' : 'inputs.a[' + str(i) + ']',
        'b' : 'inputs.b[' + str(i) + ']',
        'c_in' : 'adder' + str(i-1) + '.c_out'
    }
    components['adder' + str(i)] = c

components['adder0']['inputs']['c_in'] = 'inputs.carry'

definition['components'] = components


# Create the definition string - just json without the quotes
def_string = json.dumps(definition, indent=2)
def_string = def_string.translate(None, '"')
#def_string = def_string.translate(None, '\n ')
print(def_string)
