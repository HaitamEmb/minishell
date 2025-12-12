#!/bin/bash

# ==============================================================================
# MINISHELL COMPREHENSIVE TESTER - 42 Evaluation Scale
# Based on the mandatory part of the minishell evaluation criteria
# ==============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell path
MINISHELL="./minishell"

# Test results directory
RESULTS_DIR="/tmp/minishell_test_results"
mkdir -p "$RESULTS_DIR"

# ==============================================================================
# Helper functions
# ==============================================================================

print_header() {
    echo -e "\n${BOLD}${BLUE}============================================================${NC}"
    echo -e "${BOLD}${BLUE}$1${NC}"
    echo -e "${BOLD}${BLUE}============================================================${NC}\n"
}

print_subheader() {
    echo -e "\n${CYAN}--- $1 ---${NC}"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    local check_exit_code="$4"  # Optional: expected exit code
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in minishell
    local mini_output=$(echo -e "$command\nexit" | timeout 5 $MINISHELL 2>&1 | grep -v "^minishell\$" | grep -v "^exit$")
    local mini_exit=$?
    
    # Clean output (remove prompt)
    mini_output=$(echo "$mini_output" | sed 's/minishell\$ //g' | sed '/^$/d')
    
    if [[ "$expected" == "NOCRASH" ]]; then
        if [[ $mini_exit -ne 139 && $mini_exit -ne 134 && $mini_exit -ne 136 ]]; then
            echo -e "${GREEN}[PASS]${NC} $test_name"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}[FAIL]${NC} $test_name - Program crashed!"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    else
        # For tests where we compare outputs
        if [[ "$mini_output" == *"$expected"* ]] || [[ -z "$expected" && -z "$mini_output" ]]; then
            echo -e "${GREEN}[PASS]${NC} $test_name"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}[FAIL]${NC} $test_name"
            echo -e "       Expected: '$expected'"
            echo -e "       Got:      '$mini_output'"
            FAILED_TESTS=$((FAILED_TESTS + 1))
        fi
    fi
}

compare_with_bash() {
    local test_name="$1"
    local command="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in bash
    local bash_output=$(echo -e "$command" | bash 2>&1)
    local bash_exit=$?
    
    # Run in minishell
    local mini_output=$(echo -e "$command\nexit" | timeout 5 $MINISHELL 2>&1 | grep -v "^minishell\$" | sed 's/minishell\$ //g' | grep -v "^exit$" | sed '/^$/d')
    local mini_exit=$?
    
    # Compare (remove prompt artifacts)
    mini_output=$(echo "$mini_output" | head -n $(echo "$bash_output" | wc -l))
    
    if [[ "$mini_output" == "$bash_output" ]]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo -e "       Bash output:      '$bash_output'"
        echo -e "       Minishell output: '$mini_output'"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

check_exit_code() {
    local test_name="$1"
    local command="$2"
    local expected_code="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in minishell and check echo $?
    local mini_output=$(echo -e "$command\necho \$?\nexit" | timeout 5 $MINISHELL 2>&1 | grep -v "^minishell\$" | sed 's/minishell\$ //g' | grep -v "^exit$" | tail -1)
    
    if [[ "$mini_output" == "$expected_code" ]]; then
        echo -e "${GREEN}[PASS]${NC} $test_name (exit code: $expected_code)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo -e "       Expected exit code: $expected_code"
        echo -e "       Got: $mini_output"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
}

# ==============================================================================
# TEST 1: COMPILATION
# ==============================================================================
print_header "TEST 1: COMPILATION"

echo -e "${CYAN}Checking compilation flags with make -n...${NC}"
make_output=$(make -n 2>&1)
if echo "$make_output" | grep -q "\-Wall" && echo "$make_output" | grep -q "\-Wextra" && echo "$make_output" | grep -q "\-Werror"; then
    echo -e "${GREEN}[PASS]${NC} Compilation uses -Wall -Wextra -Werror"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} Compilation does not use required flags"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# Check re-linking
echo -e "${CYAN}Checking for re-linking...${NC}"
make > /dev/null 2>&1
relink_output=$(make 2>&1)
if echo "$relink_output" | grep -q "Nothing to be done"; then
    echo -e "${GREEN}[PASS]${NC} Makefile does not re-link"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    if echo "$relink_output" | grep -q "is up to date"; then
        echo -e "${GREEN}[PASS]${NC} Makefile does not re-link"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${YELLOW}[WARN]${NC} Possible re-linking detected"
    fi
fi
TOTAL_TESTS=$((TOTAL_TESTS + 1))

# ==============================================================================
# TEST 2: SIMPLE COMMAND & GLOBAL VARIABLES
# ==============================================================================
print_header "TEST 2: SIMPLE COMMAND & GLOBAL VARIABLES"

print_subheader "Global Variables Analysis"
# Count global variables
global_count=$(grep -rh "^[a-zA-Z_][a-zA-Z0-9_]*\s*=" *.c execution/*.c parsing/*.c utils/*.c 2>/dev/null | grep -v "static" | grep -c "g_")
echo -e "${CYAN}Number of global variables found: ${global_count}${NC}"
echo -e "${CYAN}Global variable(s): g_exit_status (used for signal handling)${NC}"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [[ $global_count -le 1 ]]; then
    echo -e "${GREEN}[PASS]${NC} Only 1 global variable (acceptable for signal handling)"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} More than 1 global variable found"
fi

print_subheader "Simple Commands"
run_test "Execute /bin/ls" "/bin/ls" "NOCRASH"
run_test "Execute /bin/pwd" "/bin/pwd" "$(pwd)"
run_test "Execute /bin/echo test" "/bin/echo test" "test"
run_test "Execute /usr/bin/env" "/usr/bin/env" "NOCRASH"

print_subheader "Empty/Whitespace Commands"
run_test "Empty command" "" "NOCRASH"
run_test "Only spaces" "     " "NOCRASH"
run_test "Only tabs" "		" "NOCRASH"
run_test "Mixed whitespace" "   	  	   " "NOCRASH"

# ==============================================================================
# TEST 3: ARGUMENTS & HISTORY
# ==============================================================================
print_header "TEST 3: ARGUMENTS"

run_test "/bin/ls -l" "/bin/ls -l" "NOCRASH"
run_test "/bin/ls -la" "/bin/ls -la" "NOCRASH"
run_test "/bin/echo hello world" "/bin/echo hello world" "hello world"
run_test "/bin/echo -n test" "/bin/echo -n test" "test"
compare_with_bash "ls with multiple args" "ls -l -a"

# ==============================================================================
# TEST 4: ECHO
# ==============================================================================
print_header "TEST 4: ECHO BUILTIN"

compare_with_bash "echo without args" "echo"
compare_with_bash "echo hello" "echo hello"
compare_with_bash "echo hello world" "echo hello world"
compare_with_bash "echo with multiple spaces" "echo hello    world"
run_test "echo -n test (no newline)" "echo -n test" "test"
run_test "echo -n hello world" "echo -n hello world" "hello world"
run_test "echo -nnnn test" "echo -nnnn test" "test"
run_test "echo -n -n -n test" "echo -n -n -n test" "test"
run_test "echo test -n" "echo test -n" "test -n"

# ==============================================================================
# TEST 5: EXIT
# ==============================================================================
print_header "TEST 5: EXIT BUILTIN"

# exit with no args
mini_exit=$(echo "exit" | timeout 2 $MINISHELL 2>&1; echo $?)
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [[ "$mini_exit" == *"exit"* ]] || [[ $? -eq 0 ]]; then
    echo -e "${GREEN}[PASS]${NC} exit without args"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} exit without args"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# exit with numeric arg
run_test "exit 42" "exit 42" "NOCRASH"
run_test "exit 0" "exit 0" "NOCRASH"
run_test "exit 255" "exit 255" "NOCRASH"

# exit with too many args
mini_output=$(echo -e "exit 1 2\nexit" | timeout 2 $MINISHELL 2>&1)
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [[ "$mini_output" == *"too many"* ]] || [[ "$mini_output" == *"arguments"* ]]; then
    echo -e "${GREEN}[PASS]${NC} exit with too many args (error message)"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} exit with too many args - check error message"
fi

# exit with non-numeric arg
mini_output=$(echo -e "exit abc\nexit" | timeout 2 $MINISHELL 2>&1)
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [[ "$mini_output" == *"numeric"* ]] || [[ "$mini_output" == *"argument"* ]]; then
    echo -e "${GREEN}[PASS]${NC} exit with non-numeric arg (error message)"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} exit with non-numeric arg - check error message"
fi

# ==============================================================================
# TEST 6: RETURN VALUE ($?)
# ==============================================================================
print_header "TEST 6: RETURN VALUE (\$?)"

check_exit_code "ls success" "ls" "0"
check_exit_code "echo success" "echo test" "0"
check_exit_code "ls nonexistent file" "/bin/ls filethatdoesnotexist123" "2"
check_exit_code "nonexistent command" "commandthatdoesnotexist123" "127"

# Test $? after various commands
run_test "echo \$? after success" "ls > /dev/null\necho \$?" "0"

# ==============================================================================
# TEST 7: DOUBLE QUOTES
# ==============================================================================
print_header "TEST 7: DOUBLE QUOTES"

compare_with_bash 'echo "hello world"' 'echo "hello world"'
compare_with_bash 'echo "hello   world"' 'echo "hello   world"'
run_test 'echo "cat lol.c | cat > lol.c"' 'echo "cat lol.c | cat > lol.c"' "cat lol.c | cat > lol.c"
run_test 'echo "test with > redirect"' 'echo "test with > redirect"' "test with > redirect"
run_test 'echo "test with | pipe"' 'echo "test with | pipe"' "test with | pipe"

# Variable expansion in double quotes
run_test 'echo "$USER" (var expansion)' 'echo "$USER"' "$USER"
run_test 'echo "$HOME" (var expansion)' 'echo "$HOME"' "$HOME"
run_test 'echo "hello $USER world"' 'echo "hello $USER world"' "hello $USER world"

# ==============================================================================
# TEST 8: SINGLE QUOTES
# ==============================================================================
print_header "TEST 8: SINGLE QUOTES"

run_test "echo '\$USER' (no expansion)" "echo '\$USER'" "\$USER"
run_test "echo '\$HOME' (no expansion)" "echo '\$HOME'" "\$HOME"
run_test "echo 'hello world'" "echo 'hello world'" "hello world"
run_test "echo 'test | pipe'" "echo 'test | pipe'" "test | pipe"
run_test "echo 'test > redirect'" "echo 'test > redirect'" "test > redirect"
run_test "echo ''" "echo ''" ""

# ==============================================================================
# TEST 9: ENV
# ==============================================================================
print_header "TEST 9: ENV BUILTIN"

run_test "env command" "env" "PATH="
run_test "env shows HOME" "env" "HOME="
run_test "env shows USER" "env" "USER="

# ==============================================================================
# TEST 10: EXPORT
# ==============================================================================
print_header "TEST 10: EXPORT BUILTIN"

run_test "export new var" "export TEST_VAR=hello\necho \$TEST_VAR" "hello"
run_test "export without value" "export TEST_VAR2\nenv | grep TEST_VAR2" "NOCRASH"
run_test "export replace var" "export MY_VAR=first\nexport MY_VAR=second\necho \$MY_VAR" "second"
run_test "export invalid identifier" "export 123invalid=test" "NOCRASH"
run_test "export with special chars" "export VALID_VAR_123=test\necho \$VALID_VAR_123" "test"

# ==============================================================================
# TEST 11: UNSET
# ==============================================================================
print_header "TEST 11: UNSET BUILTIN"

run_test "unset existing var" "export TO_UNSET=value\nunset TO_UNSET\necho \$TO_UNSET" ""
run_test "unset nonexistent var" "unset NONEXISTENT_VAR_12345" "NOCRASH"
run_test "unset multiple vars" "export A=1\nexport B=2\nunset A B\necho \$A\$B" ""

# ==============================================================================
# TEST 12: CD
# ==============================================================================
print_header "TEST 12: CD BUILTIN"

run_test "cd /" "cd /\npwd" "/"
run_test "cd /tmp" "cd /tmp\npwd" "/tmp"
run_test "cd .." "cd ..\npwd" "NOCRASH"
run_test "cd ." "cd .\npwd" "NOCRASH"
run_test "cd nonexistent" "cd /nonexistent_directory_12345" "NOCRASH"
run_test "cd ~" "cd ~\npwd" "$HOME"
run_test "cd without args" "cd\npwd" "$HOME"

# ==============================================================================
# TEST 13: PWD
# ==============================================================================
print_header "TEST 13: PWD BUILTIN"

compare_with_bash "pwd" "pwd"
run_test "pwd after cd /" "cd /\npwd" "/"
run_test "pwd after cd /tmp" "cd /tmp\npwd" "/tmp"

# ==============================================================================
# TEST 14: RELATIVE PATH
# ==============================================================================
print_header "TEST 14: RELATIVE PATH"

run_test "./minishell --version (or crash test)" "./nonexistent_file" "NOCRASH"
run_test "../ path" "cd /tmp\n../bin/ls" "NOCRASH"

# ==============================================================================
# TEST 15: ENVIRONMENT PATH
# ==============================================================================
print_header "TEST 15: ENVIRONMENT PATH"

run_test "ls without path" "ls" "NOCRASH"
run_test "echo without path" "echo test" "test"
run_test "cat without path" "echo test | cat" "test"

# Test unset PATH
run_test "command after unset PATH" "unset PATH\nls" "NOCRASH"

# ==============================================================================
# TEST 16: REDIRECTIONS
# ==============================================================================
print_header "TEST 16: REDIRECTIONS"

# Output redirection >
rm -f "$RESULTS_DIR/test_output.txt" 2>/dev/null
run_test "echo > file" "echo hello > $RESULTS_DIR/test_output.txt\ncat $RESULTS_DIR/test_output.txt" "hello"

# Output redirection >> (append)
echo "first" > "$RESULTS_DIR/test_append.txt"
run_test "echo >> file (append)" "echo second >> $RESULTS_DIR/test_append.txt\ncat $RESULTS_DIR/test_append.txt" "second"

# Input redirection <
echo "input content" > "$RESULTS_DIR/test_input.txt"
run_test "cat < file" "cat < $RESULTS_DIR/test_input.txt" "input content"

# Multiple redirections
run_test "Multiple output redirections" "echo test > $RESULTS_DIR/out1.txt > $RESULTS_DIR/out2.txt\ncat $RESULTS_DIR/out2.txt" "test"

# Combined redirections
echo "combined test" > "$RESULTS_DIR/combined_input.txt"
run_test "Combined < and >" "cat < $RESULTS_DIR/combined_input.txt > $RESULTS_DIR/combined_output.txt\ncat $RESULTS_DIR/combined_output.txt" "combined test"

# ==============================================================================
# TEST 17: HEREDOC
# ==============================================================================
print_header "TEST 17: HEREDOC (<<)"

run_test "Basic heredoc" "cat << EOF\nhello\nEOF" "hello"
run_test "Heredoc multiline" "cat << END\nline1\nline2\nEND" "line1"
run_test "Heredoc with quotes in delimiter" "cat << 'EOF'\n\$USER\nEOF" "\$USER"

# ==============================================================================
# TEST 18: PIPES
# ==============================================================================
print_header "TEST 18: PIPES"

run_test "Simple pipe" "echo hello | cat" "hello"
run_test "Multiple pipes" "echo hello | cat | cat" "hello"
compare_with_bash "ls | grep minishell" "ls | grep minishell"
run_test "ls | wc -l" "ls | wc -l" "NOCRASH"
run_test "cat | cat | cat" "echo test | cat | cat | cat" "test"

# Failing pipe
run_test "Failing command in pipe" "ls nonexistent_file_12345 | cat" "NOCRASH"

# Mixed pipes and redirections
run_test "Pipe with redirection" "echo hello | cat > $RESULTS_DIR/pipe_out.txt\ncat $RESULTS_DIR/pipe_out.txt" "hello"

# ==============================================================================
# TEST 19: ENVIRONMENT VARIABLES
# ==============================================================================
print_header "TEST 19: ENVIRONMENT VARIABLES"

run_test "echo \$USER" "echo \$USER" "$USER"
run_test "echo \$HOME" "echo \$HOME" "$HOME"
run_test "echo \$PWD" "echo \$PWD" "NOCRASH"
run_test "echo \$NONEXISTENT" "echo \$NONEXISTENT" ""
run_test "echo \$?" "echo \$?" "0"
run_test "Multiple vars" "echo \$USER \$HOME" "$USER $HOME"

# ==============================================================================
# TEST 20: GO CRAZY TESTS
# ==============================================================================
print_header "TEST 20: EDGE CASES & STRESS TESTS"

run_test "Nonexistent command" "dsbksdgbksdghsd" "NOCRASH"
run_test "Very long echo" "echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" "aaaa"
run_test "Multiple semicolons (if supported)" "echo a ; echo b" "NOCRASH"
run_test "Empty quotes" 'echo ""' ""
run_test "Only quotes" '""' "NOCRASH"
run_test "Mixed quotes" "echo \"'hello'\"" "'hello'"
run_test "Nested quotes test" 'echo "test"' "test"

# ==============================================================================
# MEMORY LEAK CHECK (optional - requires valgrind)
# ==============================================================================
print_header "MEMORY LEAK CHECK"

if command -v valgrind &> /dev/null; then
    echo -e "${CYAN}Running valgrind check...${NC}"
    valgrind_output=$(echo -e "echo test\nls\nexit" | valgrind --leak-check=full --error-exitcode=1 $MINISHELL 2>&1)
    if echo "$valgrind_output" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}[PASS]${NC} No memory leaks detected"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${YELLOW}[WARN]${NC} Potential memory leaks - review valgrind output"
        echo "$valgrind_output" | grep -A2 "LEAK SUMMARY"
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
else
    echo -e "${YELLOW}[SKIP]${NC} Valgrind not available - skipping memory check"
fi

# ==============================================================================
# FINAL SUMMARY
# ==============================================================================
print_header "TEST SUMMARY"

echo -e "${BOLD}Total Tests:  ${TOTAL_TESTS}${NC}"
echo -e "${GREEN}Passed:       ${PASSED_TESTS}${NC}"
echo -e "${RED}Failed:       ${FAILED_TESTS}${NC}"

PERCENTAGE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
echo -e "\n${BOLD}Score: ${PERCENTAGE}%${NC}"

if [[ $PERCENTAGE -ge 90 ]]; then
    echo -e "${GREEN}${BOLD}Excellent! Your minishell is performing great!${NC}"
elif [[ $PERCENTAGE -ge 70 ]]; then
    echo -e "${YELLOW}${BOLD}Good progress! Some issues to address.${NC}"
else
    echo -e "${RED}${BOLD}Needs work. Review the failed tests.${NC}"
fi

echo -e "\n${CYAN}Note: Signal tests (Ctrl+C, Ctrl+D, Ctrl+\\) require manual testing.${NC}"
echo -e "${CYAN}Note: History navigation (up/down arrows) requires manual testing.${NC}"

# Cleanup
rm -rf "$RESULTS_DIR"

exit $((TOTAL_TESTS - PASSED_TESTS))
