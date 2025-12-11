#!/bin/bash
echo "Testing minishell..."
echo ""
echo "=== Test 1: Basic commands ==="
echo -e "ls\necho hello\npwd\nexit" | ./minishell

echo ""
echo "=== Test 2: Echo with quotes ==="
echo -e 'echo "hello world"\nexit' | ./minishell

echo ""
echo "=== Test 3: Pipes ==="
echo -e "ls | grep mini\nexit" | ./minishell

echo ""
echo "=== Test 4: Environment variables ==="
echo -e 'echo $HOME\necho $USER\nexit' | ./minishell

echo ""
echo "=== Test 5: Exit status ==="
echo -e 'ls\necho $?\nexit' | ./minishell

