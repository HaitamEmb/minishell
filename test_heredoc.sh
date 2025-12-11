#!/bin/bash

echo "========================================="
echo "Test 1: Basic heredoc"
echo "========================================="
echo -e "cat << END\nhello world\nEND\nexit" | ./minishell

echo ""
echo "========================================="
echo "Test 2: Heredoc with variable expansion"
echo "========================================="
echo -e "cat << EOF\n\$USER is \$HOME\nEOF\nexit" | ./minishell

echo ""
echo "========================================="
echo "Test 3: Empty environment variable (should not segfault)"
echo "========================================="
echo -e 'echo $NONEXISTENT\nexit' | ./minishell

echo ""
echo "========================================="
echo "Test 4: Multiple heredocs"
echo "========================================="
echo -e "cat << A << B\ntest\nA\ndata\nB\nexit" | ./minishell

