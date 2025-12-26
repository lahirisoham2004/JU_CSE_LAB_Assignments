#!/usr/bin/env python3
strrule = lambda rule: " | ".join((" ".join(subRule) if len(subRule) > 0 else "?") for subRule in rule)
strset = lambda seq: "{"+(",".join(str(x) for x in seq))+"}"
"""
Bullock:
LL(1) Predictive Parser Generator
Naming: Yacc, Bison, Bullock, etc...
"""
terminalArray = tuple(input("Enter terminal symbols separated by space.\nThese will be matched as-is\n-> ").split())
terminals = frozenset(terminalArray)
nonterminalArray = list(input("Enter nonterminal symbols separated by space.\nThese must be entered in the correct precedence order desired (low to high); starting with the start symbol.\n-> ").split())
nonterminals = frozenset(nonterminalArray)
startSymbol = nonterminalArray[0]; print("Taking start symbol "+startSymbol)

rules = dict()
"""
Structure for rules:
rules[nonterminal] = frozenset(tuple(sequences of terminal and nonterminal), tuple(sequences of terminal and nonterminal), ...)
Each sub-tuple in value indicates a possible option for expansion. Each such subsequence will be separated in input by |.
sub-tuples are sequences of terminals and nonterminals; and will be empty if epsilon is an option.
"""
print("""
Rules for nonterminals should be sequences of terminals and nonterminals separated by space. Multiple possible options for
a nonterminal should be separated by |. Spaces around '|' are not required. An empty string option is indicated by '?'.
""")
for nonterminal in nonterminalArray: rules[nonterminal] = frozenset(tuple(x for x in subRule.split() if x != '?') for subRule in input("Enter rules for %s: " % nonterminal).split("|"))

#Left-recursion removal

def removeImmediateLeftRecursion(nonterminal: str) -> str:
    "Remove immediate left-recursion. Returns new terminal if present"
    global rules
    global nonterminals
    global terminals
    nameGen = 1; newNonterminal = ''
    while (newNonterminal := "%s_R%d" % (nonterminal, nameGen)) in (nonterminals | terminals): nameGen += 1
    rule = rules[nonterminal]
    print("\nRemoving immediate-left recursion for %s. New nonterminal will be %s." % (nonterminal, newNonterminal))
    #Step 1: separate into rules which result in left-recursion and rules which don't.
    leftRecursionRule = frozenset(subRule for subRule in rule if len(subRule) > 0 and subRule[0] == nonterminal)
    if len(leftRecursionRule) == 0:
        print("This is not left-recursive."); return None
    nonLeftRecursionRule: frozenset = rule - leftRecursionRule #The ones which start with symbols other than nonterminal; including epsilon if present.
    print("New rules:")
    rules[nonterminal] = frozenset(subRule + (newNonterminal, ) for subRule in nonLeftRecursionRule) #Step 2
    print("%s ::= %s"  % (nonterminal, strrule(rules[nonterminal])))
    rules[newNonterminal] = frozenset(subRule[1:]+(newNonterminal, ) for subRule in leftRecursionRule) | frozenset({tuple()}) #Step 3
    print("%s ::= %s"  % (newNonterminal, strrule(rules[newNonterminal])))
    nonterminals |= frozenset({newNonterminal})
    return newNonterminal

newNonterminals = dict() #index to nonterminal
#Remove chained left recursion
for i in range(len(nonterminalArray)):
    print("\nRemoving chained-left recursion for "+nonterminalArray[i])
    for j in range(i):
        print("Trying substituting in "+nonterminalArray[j])
        newRuleSet = set()
        for subRule in rules[nonterminalArray[i]]:
            if len(subRule) > 0 and subRule[0] == nonterminalArray[j]:
                for jSubRule in rules[nonterminalArray[j]]:
                    newRuleSet.add(jSubRule+subRule[1:])
            else: newRuleSet.add(subRule)
        rules[nonterminalArray[i]] = frozenset(newRuleSet)
        print("%s ::= %s" % (nonterminalArray[i], strrule(rules[nonterminalArray[i]])))
    newNonterminal = removeImmediateLeftRecursion(nonterminalArray[i])
    if newNonterminal is not None:
        newNonterminals[i] = newNonterminal #insert newNonterminal after index i
        nonterminals |= frozenset({newNonterminal})

#Regenerate nonterminalArray
newNonterminalArray = list()
for i in range(len(nonterminalArray)):
    newNonterminalArray.append(nonterminalArray[i])
    if i in newNonterminals: newNonterminalArray.append(newNonterminals[i])
nonterminalArray = newNonterminalArray

print("\nRules after left recursion removal:")
for nonterminal in nonterminalArray:
    print("%s ::= %s" % (nonterminal, strrule(rules[nonterminal])))

from itertools import combinations

#Left-factoring

def begins_with(seq: tuple, subseq: tuple) -> bool:
    "Checks if subseq begins with seq."
    if len(subseq) > len(seq): return False
    for i in range(len(subseq)):
        if seq[i] != subseq[i]: return False
    return True

def leftFactoring(nonterminal: str) -> str:
    "Perform left-factoring. Returns new nonterminal if present"
    global rules
    global nonterminals
    global terminals
    nameGen = 1; newNonterminal = ''
    while (newNonterminal := "%s_F%d" % (nonterminal, nameGen)) in (nonterminals | terminals): nameGen += 1
    rule = rules[nonterminal]
    print("\nPerforming left-factoring for %s. New nonterminal will be %s." % (nonterminal, newNonterminal))

    #Step 1: find longest common subsequence
    longestCommonSubsequence = tuple()
    for seq1, seq2 in combinations(rule, 2):
        commonSubsequence = list()
        for e1, e2 in zip(seq1, seq2):
            if e1 == e2: commonSubsequence.append(e1)
            else: break
        if len(commonSubsequence) > len(longestCommonSubsequence): longestCommonSubsequence = tuple(commonSubsequence)
    if len(longestCommonSubsequence) == 0:
        print("Left-factoring not required."); return None
    print("Largest common subsequence selected: "+(" ".join(longestCommonSubsequence)))
    
    leftFactoringRule = frozenset(subRule for subRule in rule if begins_with(subRule, longestCommonSubsequence))
    nonLeftFactoringRule = rule - leftFactoringRule
    print("New rules:")
    rules[nonterminal] = nonLeftFactoringRule | frozenset({longestCommonSubsequence + (newNonterminal, )}) #Step 2
    print("%s ::= %s"  % (nonterminal, strrule(rules[nonterminal])))
    rules[newNonterminal] = frozenset(subRule[len(longestCommonSubsequence):] for subRule in leftFactoringRule) #Step 3
    print("%s ::= %s"  % (newNonterminal, strrule(rules[newNonterminal])))
    nonterminals |= frozenset({newNonterminal})
    return newNonterminal

print("\nMultiple scans will be performed for left-factoring.")
runScan = True; run = 0
while runScan:
    run += 1; runScan = False
    print("\nRun %d" % run)
    newNonterminals = dict()
    for i in range(len(nonterminalArray)):
        nonterminal = nonterminalArray[i] 
        newNonterminal = leftFactoring(nonterminal)
        if newNonterminal is not None:
            newNonterminals[i] = newNonterminal #insert newNonterminal after index i
            nonterminals |= frozenset({newNonterminal})
            runScan = True
    #Regenerate nonterminalArray
    newNonterminalArray = list()
    for i in range(len(nonterminalArray)):
        newNonterminalArray.append(nonterminalArray[i])
        if i in newNonterminals: newNonterminalArray.append(newNonterminals[i])
    nonterminalArray = newNonterminalArray

nonterminalArray = tuple(nonterminalArray) #Freeze array
print("\nFinal rules:")
for nonterminal in nonterminalArray:
    print("%s ::= %s" % (nonterminal, strrule(rules[nonterminal])))

#Generate first and follow sets
from enum import Enum, unique
@unique
class SpecialSymbols(Enum):
    EPSILON = '?'
    END = 'eoe'

    def __str__(self): return self.value

first = dict(); follow = dict()
#Generating first sets... requires multple runs
print("\nGenerating first sets...")
#For terminals this is simple.
first[SpecialSymbols.END] = frozenset({SpecialSymbols.END})
for terminal in terminals: first[terminal] = frozenset({terminal})
#For nonterminals; multiple scans are required.
runScan = True
while runScan:
    runScan = False
    for nonterminal in nonterminalArray:
        newSet = first[nonterminal] if nonterminal in first else frozenset({})
        rule = rules[nonterminal]
        for subRule in rule:
            allNullable = True
            for subRuleSymbol in subRule:
                subRuleSymbolFirst = first[subRuleSymbol] if subRuleSymbol in first else frozenset({})
                newSet |= (subRuleSymbolFirst - frozenset({SpecialSymbols.EPSILON}))
                if SpecialSymbols.EPSILON not in subRuleSymbolFirst: allNullable = False; break
            if allNullable: newSet |= frozenset({SpecialSymbols.EPSILON})
        if nonterminal not in first or newSet != first[nonterminal]: runScan = True
        first[nonterminal] = newSet

for nonterminal in nonterminalArray:
    print("%s -> %s" % (nonterminal, strset(first[nonterminal])))

def compositeFirstSet(seq) -> frozenset:
    "Return first set for multiple symbols"
    global first
    result = frozenset({})
    for symbol in seq:
        result |= first[symbol]
        if SpecialSymbols.EPSILON not in first[symbol]: 
            result -= frozenset({SpecialSymbols.EPSILON})
            break
    return result

print("\nGenerating follow sets...")
follow[startSymbol] = frozenset({SpecialSymbols.END})
#Also multiple scans are required.
runScan = True
while runScan:
    runScan = False
    for nonterminal in nonterminalArray:
        rule = rules[nonterminal]
        for subRule in rule:
            for i in range(len(subRule)):
                targetSymbol = subRule[i]
                if targetSymbol not in nonterminals: continue
                newSet = follow[targetSymbol] if targetSymbol in follow else frozenset({})
                betaSet = compositeFirstSet(subRule[i+1:])
                newSet |= (betaSet - frozenset({SpecialSymbols.EPSILON}))
                alphaSet = follow[nonterminal] if nonterminal in follow else frozenset({})
                if SpecialSymbols.EPSILON in betaSet or len(betaSet) == 0: newSet |= alphaSet
                if targetSymbol not in follow or newSet != follow[targetSymbol]: runScan = True
                follow[targetSymbol] = newSet

for nonterminal in nonterminalArray:
    print("%s -> %s" % (nonterminal, strset(follow[nonterminal])))

#Checking if LL(1)
print()
for nonterminal in nonterminalArray:
    if tuple() in rules[nonterminal] and not first[nonterminal].isdisjoint(follow[nonterminal]):
        print("Error: given grammar is not LL(1).")
        print("For nonterminal %s, first set = %s and follow set %s" % (nonterminal, strset(first[nonterminal]), strset(follow[nonterminal])))
        print("The given nonterminal produces an empty string and first and follow sets are not disjoint.")
        exit(1)
    for prod1, prod2 in combinations(rules[nonterminal] - {tuple()}, 2):
        prod1First = compositeFirstSet(prod1); prod2First = compositeFirstSet(prod2)
        if prod1First.isdisjoint(prod2First): continue
        print("Error: given grammar is not LL(1).")
        print("For productions:")
        print("%s ::= %s; first set = %s" % (nonterminal, " ".join(prod1) if len(prod1) > 0 else '?', strset(prod1First)))
        print("%s ::= %s; first set = %s" % (nonterminal, " ".join(prod2) if len(prod2) > 0 else '?', strset(prod2First)))
        print("First sets are not disjoint.")
        exit(1)
        

print("Generating parsing table...")

#Fill error-recovery entries as well
@unique
class ErrorRecovery(Enum):
    POP = 'pop'
    SCAN = 'scan'

    def __str__(self): return self.value

M = dict()
for nonterminal in nonterminalArray:
    for terminal in (terminals | {SpecialSymbols.END}): 
        if not(terminal in first[nonterminal] or (SpecialSymbols.EPSILON in first[nonterminal] and terminal in follow[nonterminal])):
            M[(nonterminal, terminal)] = ErrorRecovery.POP if terminal == SpecialSymbols.END or terminal in follow[nonterminal] else ErrorRecovery.SCAN
        else: M[(nonterminal, terminal)] = None 

for nonterminal in nonterminalArray:
    rule = rules[nonterminal]
    for subRule in rule:
        firstSet = compositeFirstSet(subRule)
        for terminal in firstSet: M[(nonterminal, terminal)] = subRule
        if SpecialSymbols.EPSILON in firstSet or len(firstSet) == 0:
            for terminal in follow[nonterminal]: M[(nonterminal, terminal)] = subRule

fieldLength = max(len(terminal) for terminal in (terminals | {SpecialSymbols.END.value}))
for nonterminal in nonterminalArray:
    print("\nWhen parsing %s:" % nonterminal)
    for terminal in (terminalArray + (SpecialSymbols.END,)):
        print(("If current token is %"+str(fieldLength)+"s, ") % terminal, end='')
        subRule = None
        if type(subRule := M[(nonterminal, terminal)]) == ErrorRecovery: print("do %s for error recovery" % str(subRule))
        else: print("do %s ::= %s" % (nonterminal, " ".join(subRule) if len(subRule) > 0 else '?'))

inputArray: tuple = None
try:inputArray = tuple(input("\nEnter sequences for terminals (separated by space) to evaluate.\nEnter EOF (Ctrl-D on UNIX-based systems, Ctrl-Z on Windows) to exit here.\n-> ").split()) + (SpecialSymbols.END,)
except EOFError: exit()

from collections import deque
stack = deque((SpecialSymbols.END, startSymbol))
i = 0
errorFlag = False
while len(stack) > 0:
    symbol = stack[-1] #I need only to peek
    print("\nStack is %s; current input terminal is %s at position %d" % (strset(stack), inputArray[i], i+1))
    if symbol == inputArray[i]: #We have a direct match
        print("We have a match. Going over to next input terminal."); i += 1; stack.pop(); continue
    elif symbol in terminals: #We should have expected a terminal but we got something else.
        print("Unrecoverable error; expected %s; found %s" % (symbol, inputArray[i])); errorFlag = True; break
    action = M[(symbol, inputArray[i])]
    if type(action) == ErrorRecovery:
        print("Error; doing %s to recover" % str(action))
        if action == ErrorRecovery.POP: stack.pop()
        else: i += 1
        errorFlag = True; continue
    stack.pop()
    #Directly push the rule on stack. symbol matching will take care of the rest.
    print("Doing %s ::= %s" % (symbol, " ".join(action) if len(action) > 0 else '?'))
    stack.extend(reversed(action))

if errorFlag or i != len(inputArray): print("Given input string of terminals was erroneous.")