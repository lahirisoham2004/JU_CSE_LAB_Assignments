#!/usr/bin/python3
strrule = lambda rule: " | ".join((" ".join(subRule) if len(subRule) > 0 else "?") for subRule in rule)
strset = lambda seq: "{"+(",".join(str(x) for x in seq))+"}"
"""
Zebu:
Canonical LR(1) Bottom-Up Parser Generator
Naming: Yacc, Bison, Bullock, Cow, Zebu etc...
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
Please avoid using '.'.
""")
for nonterminal in nonterminalArray: rules[nonterminal] = frozenset(tuple(x for x in subRule.split() if x != '?') for subRule in input("Enter rules for %s: " % nonterminal).split("|"))

if len(rules[startSymbol]) > 1:
    newStartSymbol = input("New start symbol required. Please enter new start symbol: ").strip()
    rules[newStartSymbol] = frozenset({(startSymbol,)})
    nonterminals |= frozenset({newStartSymbol})
    nonterminalArray.insert(0, newStartSymbol)
    startSymbol = newStartSymbol

from collections import deque
from enum import Enum, unique
from itertools import combinations

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

print("\nMultiple scans will be performed for left-factoring to remove ambiguity.")
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

@unique
class SpecialSymbols(Enum):
    EPSILON = '?'
    END = 'eoe'
    DOT = '.'

    def __str__(self): return self.value
    def __repr__(self): return self.value

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

class Item:
    def __init__(self, nonterminal: str, rule: tuple, currentIndex: int, lookahead):
        self.__nonterminal = nonterminal
        self.__rule = rule
        self.__currentIndex = len(rule) if currentIndex > len(rule) else currentIndex #clamp value
        self.__lookahead = lookahead

    @property
    def nonterminal(self) -> str: return self.__nonterminal
    @property
    def rule(self) -> tuple: return self.__rule
    @property
    def currentIndex(self) -> int: return self.__currentIndex
    @property
    def lookahead(self): return self.__lookahead
    @property
    def at_end(self) -> bool: return self.__currentIndex >= len(self.__rule)
    @property
    def expected_symbol(self) -> str: return self.__rule[self.__currentIndex]

    def __eq__(self, other) -> bool: return self.__nonterminal == other.nonterminal and\
        self.__rule == other.rule and self.__currentIndex == other.currentIndex and self.__lookahead == other.lookahead
    def __ne__(self, other) -> bool: return self.__nonterminal != other.nonterminal or\
        self.__rule != other.rule or  self.__currentIndex != other.currentIndex or  self.__lookahead != other.lookahead
    def __hash__(self) -> int: return hash((self.__nonterminal, self.__rule, self.__currentIndex, self.__lookahead))
    def __repr__(self) -> str: return "Item(%s, %s, %s, %s)" % (repr(self.__nonterminal),repr(self.__rule),repr(self.__currentIndex),repr(self.__lookahead))

    def __str__(self) -> str:
        l = deque()
        for i in range(len(self.__rule)):
            if i == self.__currentIndex: l.append('.')
            l.append(self.__rule[i])
        if self.at_end: 
            l.append('.')
        return self.__nonterminal + " ::= " + " ".join(l) + "|" + str(self.__lookahead)

def sortedItems(itemSet) -> tuple:
    "Get a sorted item set on the basis of nonterminalArray. This is an expensive operation."
    global nonterminalArray
    keys = dict()
    for i in range(len(nonterminalArray)): keys[nonterminalArray[i]] = i
    return tuple(sorted(itemSet, key=lambda item: keys[item.nonterminal]))

def closure(i: Item) -> frozenset:
    "Compute closure of an item"
    global terminals
    global rules
    q = deque([i])
    result = set()
    while len(q) > 0:
        item = q.pop()
        if item in result: continue
        result.add(item)
        if item.at_end: continue
        newNonterminal = item.expected_symbol
        if newNonterminal not in nonterminals: continue
        lookaheadSet = compositeFirstSet(item.rule[item.currentIndex+1:]+(item.lookahead,))
        for newRule in rules[newNonterminal]: 
            for b in lookaheadSet:
                q.appendleft(Item(newNonterminal, newRule, 0, b))
    return frozenset(result)

def compositeClosure(itemSet) -> frozenset:
    "Compute closure for an item set"
    result = frozenset({})
    for item in itemSet: result |= closure(item)
    return result

def computeGoto(itemSet, symbol: str) -> frozenset:
    "Compute GOTO(itemSet, symbol)"
    targetItemSet = set()
    item: Item = None
    for item in itemSet:
        if item.at_end: continue #This is a termination state.
        if item.expected_symbol == symbol: targetItemSet.add(Item(item.nonterminal, item.rule, item.currentIndex+1, item.lookahead))
    return compositeClosure(targetItemSet)

def listIndexOf(x, l) -> int:
    for i in range(len(l)):
        if l[i] == x: return i
    return -1

EMPTY = frozenset({})
startItem = Item(startSymbol, tuple(rules[startSymbol])[0], 0, SpecialSymbols.END)
states = list(); unmarkedStatesQueue = deque([closure(startItem)]); transitions = dict(); stateIndexes = dict()
while len(unmarkedStatesQueue) > 0:
    state: frozenset = unmarkedStatesQueue.pop()
    if state in stateIndexes: continue
    stateIndexes[state] = len(states); states.append(state)
    for targetSymbol in nonterminalArray:
        targetState = computeGoto(state, targetSymbol)
        if targetState == EMPTY: continue
        unmarkedStatesQueue.appendleft(targetState)
    for targetSymbol in terminalArray:
        targetState = computeGoto(state, targetSymbol)
        if targetState == EMPTY: continue
        unmarkedStatesQueue.appendleft(targetState)

#stateSymbols stores the symbol corresponding to a state (the incoming symbol).
#Incoming arrows to each state are labelled with only one symbol in LR(0).
print("\nIn the following; for items the symbols after the vertical bar '|' are the lookahead symbols.")
stateSymbols = [None] * len(states); stateSymbols[0] = SpecialSymbols.END
for i in range(len(states)):
    print("\nState %d:" % i)
    for item in sortedItems(states[i]): print(item)
    for targetSymbol in nonterminalArray:
        targetState = computeGoto(states[i], targetSymbol)
        if targetState == EMPTY: continue
        targetStateIndex = transitions[(i, targetSymbol)] = stateIndexes[targetState]
        if stateSymbols[targetStateIndex] is None: stateSymbols[targetStateIndex] = targetSymbol
        elif stateSymbols[targetStateIndex] != targetSymbol:
            print("ERROR! Given grammar is not LR(0).")
            print("State %d has multiple incoming symbols (%s and %s)" % (targetStateIndex, targetSymbol, stateSymbols[targetStateIndex]))
            exit(1)
        print("On %s, go to %d" % (targetSymbol, targetStateIndex))
    for targetSymbol in terminalArray:
        targetState = computeGoto(states[i], targetSymbol)
        if targetState == EMPTY: continue
        targetStateIndex = transitions[(i, targetSymbol)] = stateIndexes[targetState]
        if stateSymbols[targetStateIndex] is None: stateSymbols[targetStateIndex] = targetSymbol
        elif stateSymbols[targetStateIndex] != targetSymbol:
            print("ERROR! Given grammar is not LR(0).")
            print("State %d has multiple incoming symbols (%s and %s)" % (targetStateIndex, targetSymbol, stateSymbols[targetStateIndex]))
            exit(1)
        print("On %s, go to %d" % (targetSymbol, targetStateIndex))
print()
for i in range(len(states)): print("State %d has corresponding symbol %s" % (i, stateSymbols[i]))