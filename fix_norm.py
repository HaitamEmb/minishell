#!/usr/bin/env python3
import re
import sys
import os

def fix_norminette_errors(filepath):
    """Fix common norminette errors in a C file."""
    with open(filepath, 'r') as f:
        lines = f.readlines()
    
    original_lines = lines.copy()
    fixed_lines = []
    i = 0
    
    while i < len(lines):
        line = lines[i]
        
        # Fix: Consecutive newlines (max 1 empty line)
        if i > 0 and line.strip() == '' and i < len(lines) - 1:
            if lines[i-1].strip() == '':
                i += 1
                continue
        
        # Fix: Space before newline
        line = re.sub(r' +\n$', '\n', line)
        
        # Fix: Missing space after keyword
        line = re.sub(r'\bif\(', 'if (', line)
        line = re.sub(r'\bwhile\(', 'while (', line)
        line = re.sub(r'\breturn\(', 'return (', line)
        
        # Fix: Space before function name (function declarations)
        if re.match(r'^(static )?(void|int|char|bool|t_\w+\s*\*?) +(\w+)\s*\(', line):
            line = re.sub(r'^(static )?(void|int|char|bool|t_\w+\s*\*?) +(\w+)\s*\(',
                         r'\1\2\t\3(', line)
        
        # Fix: BRACE_SHOULD_EOL
        if line.rstrip().endswith('{') and not line.strip().startswith('{'):
            # This should be on next line
            pass  # Complex, skip for now
        
        # Fix: Close brace should have no space before it
        if re.match(r'^\s*\}\s*else', line):
            # }else should be } else or }\nelse
            pass  # Skip this pattern
        
        fixed_lines.append(line)
        i += 1
    
    # Write back
    content = ''.join(fixed_lines)
    original_content = ''.join(original_lines)
    
    if content != original_content:
        with open(filepath, 'w') as f:
            f.write(content)
        return True
    return False

def main():
    parsing_dir = '/home/isingara/Documents/minishell/parsing'
    
    if not os.path.exists(parsing_dir):
        print(f"Directory {parsing_dir} not found")
        return
    
    for filename in os.listdir(parsing_dir):
        if filename.endswith('.c'):
            filepath = os.path.join(parsing_dir, filename)
            print(f"Processing {filename}...")
            if fix_norminette_errors(filepath):
                print(f"  Fixed {filename}")
            else:
                print(f"  No changes needed for {filename}")

if __name__ == '__main__':
    main()
