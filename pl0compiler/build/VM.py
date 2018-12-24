import sys
import os

STACK_SIZE = 256
MEMORY_SIZE = 1024
CODEHEAP_SIZE = 1024
MAX_HISTORY = 1000000

def main():
#################################################################################
    if len(sys.argv) != 2:
        raise_err("Missing input file name")
    input_file_name = sys.argv[1]
    if os.path.exists(input_file_name) == False:
        raise_err("Input file '"+ input_file_name +"' does not exist")
    if os.access(input_file_name, os.R_OK) == False:
        raise_err("Input file '"+ input_file_name +"' is not accessible to read")
    input_file = open(input_file_name,'r')
    del input_file_name
    codeheap = input_file.readlines()
    codeheap = list(map(lambda x:x.strip(),codeheap))
    if len(codeheap) > CODEHEAP_SIZE:
        raise_err("The length of code is out of range")
    input_file.close()
    del input_file
#################################################################################
    pc = 0
    stack = []
    memory = []
    for i in range(MEMORY_SIZE):
        memory.append(sys.maxsize)
    codeheap_size = len(codeheap)
    for i in range(codeheap_size,CODEHEAP_SIZE):
        codeheap.append("")
    history = 0
#################################################################################
    while(True):
        print("stack: ",stack)
        print("memory: ",memory[:20])
        print("reg: ",memory[1014:1024])
        history = history + 1
        if history > MAX_HISTORY:
            time_out()
            break
        if pc > CODEHEAP_SIZE or codeheap[pc] == "":
            illegal_code_heap_access(codeheap[pc],pc)
        ins = codeheap[pc]
        print(ins)
#print("execute '"+ins+"' at line "+str(pc))
        ins_items = ins.split(" ")
        if len(ins_items) == 2:
            first = ins_items[0]
            second = ins_items[1]
            if is_int(second) == False:
                illegal_instruction(ins,pc)
            second = int(second)
            if(first == "LD"):
                if second >= MEMORY_SIZE or memory[second] == sys.maxsize:
                    illegal_memory_access(ins,pc)
                if(len(stack) >= STACK_SIZE):
                    illegal_stack_access(ins,pc)
                stack.append(memory[second])
                pc = pc + 1
            elif(first == "ST"):
                if second >= MEMORY_SIZE:
                    illegal_memory_access(ins,pc)
                if(len(stack) == 0):
                    illegal_stack_access(ins,pc)
                memory[second] = stack.pop()
                pc = pc + 1           
            elif(first == "PUT"):
                stack.append(second)
                pc = pc + 1
            elif(first == "JMP"):
                if second >= CODEHEAP_SIZE or codeheap[second] == "":
                    illegal_code_heap_access(ins,pc)
                pc = second
            elif(first == "JPC"):
                if second >= CODEHEAP_SIZE or codeheap[second] == "":
                    illegal_code_heap_access(ins,pc)
                if(len(stack) == 0):
                    illegal_stack_access(ins,pc)
                b = stack.pop()
                if b == 0:
                    pc = pc + 1
                elif b == 1:
                    pc = second
                else:
                    illegal_stack_access(ins,pc)
            elif(first == "INT"):
                if second == 0:
                    if(len(stack) == 0):
                        illegal_stack_access(ins,pc)
                    print(stack[-1])
                    pc = pc + 1
                elif second == 1:
                    exit()
                # TODO: debug
                else:
                    illegal_instruction(ins,pc)
            else:
                illegal_instruction(ins,pc)
        elif len(ins_items) == 1:
            op = ins_items[0]
            if(op == "JMP"):
                if(len(stack) == 0):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                if l > CODEHEAP_SIZE or codeheap[l] == "":
                    illegal_code_heap_access(ins,pc)
                pc = l
            elif(op == "LD"):
                if(len(stack) == 0):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                if l >= MEMORY_SIZE or memory[l] == sys.maxsize:
                    illegal_memory_access(ins,pc)
                stack.append(memory[l])
                pc = pc + 1
            elif(op == "ST"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if l >= MEMORY_SIZE:
                    illegal_memory_access(ins,pc)
                memory[l] = r
                pc = pc + 1
            elif(op == "ADD"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                stack.append(l+r)
                pc = pc + 1
            elif(op == "SUB"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                stack.append(l-r)
                pc = pc + 1
            elif(op == "MUL"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                stack.append(l*r)
                pc = pc + 1
            elif(op == "DIV"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(r == 0):
                    illegal_stack_access(ins,pc)
                stack.append(int(l/r))
                pc = pc + 1
            elif(op == "EQ"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(l == r):
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "LE"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(l <= r):
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "LT"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(l < r):
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "GE"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(l >= r):
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "GT"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if(l > r):
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "AND"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if is_bool(l) == False or is_bool(r) == False:
                    illegal_stack_access(ins,pc)
                if l == 1 and r == 1:
                    stack.append(1)
                else:
                    stack.append(0)
                pc = pc + 1
            elif(op == "OR"):
                if(len(stack) <= 1):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                r = stack.pop()
                if is_bool(l) == False or is_bool(r) == False:
                    illegal_stack_access(ins,pc)
                if l == 0 and r == 0:
                    stack.append(0)
                else:
                    stack.append(1)
                pc = pc + 1
            elif(op == "NOT"):
                if(len(stack) <= 0):
                    illegal_stack_access(ins,pc)
                l = stack.pop()
                if is_bool(l) == False:
                    illegal_stack_access(ins,pc)
                if l == 1:
                    stack.append(0)
                else:
                    stack.append(1)
                pc = pc + 1
            else:
                illegal_instruction(ins,pc)
        else:
            illegal_instruction(ins,pc)

def is_bool(i):
    if i == 0 or i == 1:
        return True
    else:
        return False

def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def time_out():
    raise_err("Timeout")

def illegal_code_heap_access(ins,pc):
    raise_err("Illegal code heap access from instruction '"+ins+"' at line "+str(pc))

def illegal_stack_access(ins,pc):
    raise_err("Illegal stack access from instruction '"+ins+"' at line "+str(pc))

def illegal_memory_access(ins,pc):
    raise_err("Illegal memory access from instruction '"+ins+"' at line "+str(pc))

def illegal_instruction(ins,pc):
    raise_err("Illegal instruction '"+ins+"' at line "+str(pc))

def raise_err(s):
    print("ERROR: "+s+".")
    exit()

if __name__=="__main__":
    main()
