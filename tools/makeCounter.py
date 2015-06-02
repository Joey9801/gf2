#!/usr/bin/python

"""
Creates an n-bit synchronous counter with JK flip flops + and gates.
"""

import json
import sys

if len(sys.argv) == 2:
    n = int(sys.argv[1])
else:
    n = 4

definition = dict()

definition['inputs'] = {'clock' : False}
definition['outputs'] = {'count[' + str(n) + ']' : dict()}
for i in range(n):
    definition['outputs']['count[' + str(n) + ']'][str(i)] = 'jk' + str(i) + '.q'

components = dict()

#Create all the JK flip flops
for i in range(n):
    c = {
        'type' : 'jk',
        'inputs' : {
            'clock' : 'inputs.clock',
            'j' : 'and' + str(i),
            'k' : 'and' + str(i)
            }
        }

    components['jk' + str(i)] = c

components['jk0']['inputs']['j'] = 'const.true'
components['jk0']['inputs']['k'] = 'const.true'

#Create the And gates
for i in range(1, n):
    c = {
        'type' : 'and',
        'inputs' : dict()
        }

    for j in range(i):
        c['inputs']['i' + str(j+1)] = 'jk' + str(j) + '.q'

    components['and' + str(i)] = c

definition['components'] = components


# Create the definition string - just json without the quotes
def_string = json.dumps(definition, indent=2)
def_string = def_string.translate(None, '"')
#def_string = def_string.translate(None, '\n ')
print(def_string)
