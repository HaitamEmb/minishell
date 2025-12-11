#!/bin/bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell << 'INPUT' 2>&1 | head -80
cat << END
hello
END
exit
INPUT
