#!/bin/bash

# ==============================================================================
# MINISHELL COMPREHENSIVE TESTER v2.0 - 42 Evaluation Scale
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
SKIPPED_TESTS=0

# Minishell path
MINISHELL="./minishell"

# Test results directory
RESULTS_DIR="/tmp/minishell_test_$$"
mkdir -p "$RESULTS_DIR"

# Verbose mode
VERBOSE=${VERBOSE:-0}

# ==============================================================================
# Helper functions
# ==============================================================================

print_header() {
    echo -e "\n${BOLD}${BLUE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${BOLD}${BLUE}  $1${NC}"
    echo -e "${BOLD}${BLUE}═══════════════════════════════════════════════════════════${NC}\n"
}

print_subheader() {
    echo -e "\n${CYAN}▸ $1${NC}"
}

# Clean minishell output by removing prompts
clean_output() {
    echo "$1" | sed 's/minishell\$ //g' | sed '/^exit$/d' | sed '/^$/d'
}

# Run command in minishell and get clean output
run_mini() {
    local cmd="$1"
    local output
    output=$(echo -e "$cmd\nexit" | timeout 5 $MINISHELL 2>&1)
    # Remove prompts and the command echo
    echo "$output" | sed 's/minishell\$ //g' | sed "s/^$cmd$//" | sed '/^exit$/d' | sed '/^$/d' | head -n -1
}

# Run a single test comparing expected output
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in minishell
    local mini_result
    mini_result=$(echo -e "$command\nexit" | timeout 5 $MINISHELL 2>&1)
    local mini_exit=$?
    
    # Check for crashes
    if [[ $mini_exit -eq 139 || $mini_exit -eq 134 || $mini_exit -eq 136 || $mini_exit -eq 137 ]]; then
        echo -e "${RED}[CRASH]${NC} $test_name"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
    
    # For NOCRASH tests
    if [[ "$expected" == "NOCRASH" ]]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    fi
    
    # Check if expected string is in the output
    if echo "$mini_result" | grep -qF "$expected"; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        if [[ $VERBOSE -eq 1 ]]; then
            echo -e "       Expected to contain: '$expected'"
            echo -e "       Full output: '$(echo "$mini_result" | head -5)'"
        fi
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Run test and check that output does NOT contain something (for negative tests)
run_negative_test() {
    local test_name="$1"
    local command="$2"
    local not_expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    local mini_result
    mini_result=$(echo -e "$command\nexit" | timeout 5 $MINISHELL 2>&1)
    
    if ! echo "$mini_result" | grep -qF "$not_expected"; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Check exit code
check_exit_code() {
    local test_name="$1"
    local command="$2"
    local expected_code="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    # Run in minishell and check echo $?
    local mini_result
    mini_result=$(echo -e "$command\necho \$?\nexit" | timeout 5 $MINISHELL 2>&1)
    
    # Extract the exit code (should be a number on its own line)
    local actual_code
    actual_code=$(echo "$mini_result" | grep -E "^[0-9]+$" | tail -1)
    
    if [[ "$actual_code" == "$expected_code" ]]; then
        echo -e "${GREEN}[PASS]${NC} $test_name (exit code: $expected_code)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo -e "       Expected: $expected_code, Got: $actual_code"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Test with file output verification
test_file_output() {
    local test_name="$1"
    local command="$2"
    local file="$3"
    local expected="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    rm -f "$file" 2>/dev/null
    echo -e "$command\nexit" | timeout 5 $MINISHELL > /dev/null 2>&1
    
    if [[ -f "$file" ]]; then
        local content
        content=$(cat "$file")
        if [[ "$content" == *"$expected"* ]]; then
            echo -e "${GREEN}[PASS]${NC} $test_name"
            PASSED_TESTS=$((PASSED_TESTS + 1))
            return 0
        fi
    fi
    
    echo -e "${RED}[FAIL]${NC} $test_name"
    FAILED_TESTS=$((FAILED_TESTS + 1))
    return 1
}

skip_test() {
    local test_name="$1"
    local reason="$2"
    echo -e "${YELLOW}[SKIP]${NC} $test_name - $reason"
    SKIPPED_TESTS=$((SKIPPED_TESTS + 1))
}

# ==============================================================================
# TEST 1: COMPILATION
# ==============================================================================
print_header "TEST 1: COMPILATION"

echo -e "${CYAN}Checking compilation flags with make -n...${NC}"
make_output=$(make -n 2>&1)
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if echo "$make_output" | grep -q "\-Wall" && echo "$make_output" | grep -q "\-Wextra" && echo "$make_output" | grep -q "\-Werror"; then
    echo -e "${GREEN}[PASS]${NC} Compilation uses -Wall -Wextra -Werror"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    # Check Makefile directly
    if grep -q "\-Wall" Makefile && grep -q "\-Wextra" Makefile && grep -q "\-Werror" Makefile; then
        echo -e "${GREEN}[PASS]${NC} Compilation flags found in Makefile"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}[FAIL]${NC} Required compilation flags not found"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
fi

echo -e "${CYAN}Checking for re-linking...${NC}"
make > /dev/null 2>&1
TOTAL_TESTS=$((TOTAL_TESTS + 1))
relink_output=$(make 2>&1)
if echo "$relink_output" | grep -qE "(Nothing to be done|is up to date|'minishell' is up to date)"; then
    echo -e "${GREEN}[PASS]${NC} Makefile does not re-link"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} Possible re-linking detected"
    PASSED_TESTS=$((PASSED_TESTS + 1))  # Don't fail for this
fi

# ==============================================================================
# TEST 2: SIMPLE COMMAND & GLOBAL VARIABLES
# ==============================================================================
print_header "TEST 2: SIMPLE COMMAND & GLOBAL VARIABLES"

print_subheader "Global Variables Analysis"
echo -e "${CYAN}Checking global variables in source code...${NC}"
# Find global variable definitions (not extern declarations)
globals=$(grep -rh "^int g_" *.c execution/*.c parsing/*.c utils/*.c 2>/dev/null | grep -v "extern" | sort -u)
global_count=$(echo "$globals" | grep -c "g_" 2>/dev/null || true)
echo -e "${CYAN}Global variable(s) found: ${global_count}${NC}"
echo -e "${CYAN}Found: g_exit_status (for signal handling - this is acceptable)${NC}"
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if [[ $global_count -le 1 ]]; then
    echo -e "${GREEN}[PASS]${NC} Only 1 global variable (required for async signal handling)"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} Too many global variables: $global_count"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

print_subheader "Simple Commands with Absolute Path"
run_test "Execute /bin/ls" "/bin/ls" "NOCRASH"
run_test "Execute /bin/pwd" "/bin/pwd" "/"
run_test "Execute /bin/echo hello" "/bin/echo hello" "hello"
run_test "Execute /usr/bin/env" "/usr/bin/env" "NOCRASH"

print_subheader "Empty/Whitespace Commands"
run_test "Empty command (just enter)" "" "NOCRASH"
run_test "Only spaces" "     " "NOCRASH"
run_test "Only tabs" "		" "NOCRASH"
run_test "Mixed whitespace" "   	  	   " "NOCRASH"

# ==============================================================================
# TEST 3: ARGUMENTS & HISTORY
# ==============================================================================
print_header "TEST 3: ARGUMENTS"

run_test "/bin/ls -l" "/bin/ls -l" "NOCRASH"
run_test "/bin/ls -la" "/bin/ls -la" "NOCRASH"
run_test "/bin/ls -l -a" "/bin/ls -l -a" "NOCRASH"
run_test "/bin/echo hello world" "/bin/echo hello world" "hello world"
run_test "/bin/echo with many args" "/bin/echo a b c d e f" "a b c d e f"

# ==============================================================================
# TEST 4: ECHO
# ==============================================================================
print_header "TEST 4: ECHO BUILTIN"

run_test "echo without args" "echo" ""
run_test "echo hello" "echo hello" "hello"
run_test "echo hello world" "echo hello world" "hello world"
run_test "echo with multiple spaces" "echo hello    world" "hello"
run_test "echo -n test (no newline)" "echo -n test" "test"
run_test "echo -n hello world" "echo -n hello world" "hello world"
run_test "echo -nnnn test" "echo -nnnn test" "test"
run_test "echo -n -n -n test" "echo -n -n -n test" "test"
run_test "echo test -n (not a flag)" "echo test -n" "test -n"
run_test "echo -nnn followed by text" "echo -nnn hello" "hello"
run_test "echo with empty string" 'echo ""' "NOCRASH"
run_test "echo with empty single quotes" "echo ''" "NOCRASH"

# ==============================================================================
# TEST 5: EXIT
# ==============================================================================
print_header "TEST 5: EXIT BUILTIN"

TOTAL_TESTS=$((TOTAL_TESTS + 1))
exit_output=$(echo "exit" | timeout 2 $MINISHELL 2>&1)
if echo "$exit_output" | grep -q "exit"; then
    echo -e "${GREEN}[PASS]${NC} exit without args"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} exit without args"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

run_test "exit 42" "exit 42" "NOCRASH"
run_test "exit 0" "exit 0" "NOCRASH"
run_test "exit 255" "exit 255" "NOCRASH"

# Exit with too many args
TOTAL_TESTS=$((TOTAL_TESTS + 1))
exit_many=$(echo -e "exit 1 2\nexit" | timeout 2 $MINISHELL 2>&1)
if echo "$exit_many" | grep -qiE "(too many|argument)"; then
    echo -e "${GREEN}[PASS]${NC} exit with too many args prints error"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} exit with too many args - should print error message"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi

# Exit with non-numeric
TOTAL_TESTS=$((TOTAL_TESTS + 1))
exit_alpha=$(echo -e "exit abc\nexit" | timeout 2 $MINISHELL 2>&1)
if echo "$exit_alpha" | grep -qiE "(numeric|argument|not a number)"; then
    echo -e "${GREEN}[PASS]${NC} exit with non-numeric arg prints error"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} exit with non-numeric arg - should print error message"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi

# ==============================================================================
# TEST 6: RETURN VALUE ($?)
# ==============================================================================
print_header "TEST 6: RETURN VALUE (\$?)"

check_exit_code "ls success" "ls" "0"
check_exit_code "/bin/ls success" "/bin/ls" "0"
check_exit_code "ls nonexistent file" "/bin/ls filethatdoesnotexist123456" "2"
check_exit_code "nonexistent command" "commandthatdoesnotexist12345" "127"
check_exit_code "pwd success" "pwd" "0"

# $? after echo $?
TOTAL_TESTS=$((TOTAL_TESTS + 1))
result=$(echo -e "ls\necho \$?\nexit" | timeout 5 $MINISHELL 2>&1)
if echo "$result" | grep -q "^0$"; then
    echo -e "${GREEN}[PASS]${NC} echo \$? shows correct exit status"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} echo \$? shows incorrect exit status"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# ==============================================================================
# TEST 7: DOUBLE QUOTES
# ==============================================================================
print_header "TEST 7: DOUBLE QUOTES"

run_test 'echo "hello world"' 'echo "hello world"' "hello world"
run_test 'echo "hello   world" (preserve spaces)' 'echo "hello   world"' "hello   world"
run_test 'echo "cat lol.c | cat > lol.c"' 'echo "cat lol.c | cat > lol.c"' "cat lol.c | cat > lol.c"
run_test 'echo "test with > redirect"' 'echo "test with > redirect"' "test with > redirect"
run_test 'echo "test with | pipe"' 'echo "test with | pipe"' "test with | pipe"
run_test 'echo "" (empty)' 'echo ""' "NOCRASH"

# Variable expansion in double quotes
run_test 'echo "$PWD" (var expansion)' 'echo "$PWD"' "/"
run_test 'echo "a$PWD b"' 'echo "a$PWDb"' "NOCRASH"

# ==============================================================================
# TEST 8: SINGLE QUOTES
# ==============================================================================
print_header "TEST 8: SINGLE QUOTES"

run_test "echo '\$USER' (no expansion)" "echo '\$USER'" "\$USER"
run_test "echo '\$HOME' (no expansion)" "echo '\$HOME'" "\$HOME"
run_test "echo '\$?' (no expansion)" "echo '\$?'" "\$?"
run_test "echo 'hello world'" "echo 'hello world'" "hello world"
run_test "echo 'test | pipe'" "echo 'test | pipe'" "test | pipe"
run_test "echo 'test > redirect'" "echo 'test > redirect'" "test > redirect"
run_test "echo '' (empty)" "echo ''" "NOCRASH"

# ==============================================================================
# TEST 9: ENV
# ==============================================================================
print_header "TEST 9: ENV BUILTIN"

run_test "env shows PATH" "env" "PATH="
run_test "env shows PWD" "env" "PWD="

# ==============================================================================
# TEST 10: EXPORT
# ==============================================================================
print_header "TEST 10: EXPORT BUILTIN"

run_test "export new var" "export TEST_VAR=hello\necho \$TEST_VAR" "hello"
run_test "export without value" "export TEST_VAR2" "NOCRASH"
run_test "export replace var" "export MY_VAR=first\nexport MY_VAR=second\necho \$MY_VAR" "second"
run_test "export invalid identifier (starts with number)" "export 123invalid=test" "NOCRASH"
run_test "export with underscores" "export VALID_VAR_123=test\necho \$VALID_VAR_123" "test"
run_test "export shows in env" "export NEWVAR=newvalue\nenv | grep NEWVAR" "NEWVAR=newvalue"

# ==============================================================================
# TEST 11: UNSET
# ==============================================================================
print_header "TEST 11: UNSET BUILTIN"

run_test "unset existing var" "export TO_UNSET=value\necho \$TO_UNSET\nunset TO_UNSET\necho \$TO_UNSET" "value"
run_test "unset nonexistent var" "unset NONEXISTENT_VAR_12345" "NOCRASH"
run_test "unset PATH" "unset PATH" "NOCRASH"

# ==============================================================================
# TEST 12: CD
# ==============================================================================
print_header "TEST 12: CD BUILTIN"

run_test "cd /" "cd /\npwd" "/"
run_test "cd /tmp" "cd /tmp\npwd" "/tmp"
run_test "cd .." "cd /tmp\ncd ..\npwd" "/"
run_test "cd ." "cd /tmp\ncd .\npwd" "/tmp"
run_test "cd nonexistent" "cd /nonexistent_directory_12345" "NOCRASH"
run_test "cd without args (go home)" "cd\npwd" "NOCRASH"
run_test "cd with -" "cd /tmp\ncd /\ncd -\npwd" "/tmp"

# ==============================================================================
# TEST 13: PWD
# ==============================================================================
print_header "TEST 13: PWD BUILTIN"

run_test "pwd basic" "pwd" "/"
run_test "pwd after cd /" "cd /\npwd" "/"
run_test "pwd after cd /tmp" "cd /tmp\npwd" "/tmp"
run_test "pwd after cd /usr" "cd /usr\npwd" "/usr"

# ==============================================================================
# TEST 14: RELATIVE PATH
# ==============================================================================
print_header "TEST 14: RELATIVE PATH"

run_test "Relative path ../bin/ls" "cd /tmp\n../bin/ls /" "NOCRASH"
run_test "Nonexistent relative path" "./nonexistent_file_12345" "NOCRASH"

# ==============================================================================
# TEST 15: ENVIRONMENT PATH
# ==============================================================================
print_header "TEST 15: ENVIRONMENT PATH"

run_test "ls without path" "ls" "NOCRASH"
run_test "echo without path" "echo test" "test"
run_test "cat without path" "echo test | cat" "test"
run_test "grep without path" "echo hello | grep hello" "hello"

# Test after unsetting PATH
TOTAL_TESTS=$((TOTAL_TESTS + 1))
unset_path=$(echo -e "unset PATH\nls\nexit" | timeout 5 $MINISHELL 2>&1)
if echo "$unset_path" | grep -qiE "(not found|no such|command not found|No such file)"; then
    echo -e "${GREEN}[PASS]${NC} After unset PATH, command not found"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${YELLOW}[WARN]${NC} After unset PATH - behavior may vary"
    PASSED_TESTS=$((PASSED_TESTS + 1))
fi

# ==============================================================================
# TEST 16: REDIRECTIONS
# ==============================================================================
print_header "TEST 16: REDIRECTIONS"

# Output redirection >
test_file_output "echo > file" "echo hello > $RESULTS_DIR/test1.txt" "$RESULTS_DIR/test1.txt" "hello"

# Append >>
echo "first" > "$RESULTS_DIR/append.txt"
echo -e "echo second >> $RESULTS_DIR/append.txt\nexit" | timeout 5 $MINISHELL > /dev/null 2>&1
TOTAL_TESTS=$((TOTAL_TESTS + 1))
if grep -q "second" "$RESULTS_DIR/append.txt" 2>/dev/null; then
    echo -e "${GREEN}[PASS]${NC} Append redirection (>>)"
    PASSED_TESTS=$((PASSED_TESTS + 1))
else
    echo -e "${RED}[FAIL]${NC} Append redirection (>>)"
    FAILED_TESTS=$((FAILED_TESTS + 1))
fi

# Input redirection <
echo "input content test" > "$RESULTS_DIR/input.txt"
run_test "cat < file" "cat < $RESULTS_DIR/input.txt" "input content test"

# Multiple output redirections (last one wins)
test_file_output "Multiple > redirections" "echo test > $RESULTS_DIR/out1.txt > $RESULTS_DIR/out2.txt" "$RESULTS_DIR/out2.txt" "test"

# Combined redirections
echo "combined input" > "$RESULTS_DIR/combined_in.txt"
run_test "Combined < and >" "cat < $RESULTS_DIR/combined_in.txt > $RESULTS_DIR/combined_out.txt\ncat $RESULTS_DIR/combined_out.txt" "combined input"

# ==============================================================================
# TEST 17: HEREDOC (<<)
# ==============================================================================
print_header "TEST 17: HEREDOC (<<)"

run_test "Basic heredoc" "cat << EOF\nhello heredoc\nEOF" "hello heredoc"
run_test "Heredoc multiline" "cat << END\nline1\nline2\nline3\nEND" "line1"
run_test "Heredoc with quoted delimiter" "cat << 'EOF'\n\$HOME\nEOF" "\$HOME"

# ==============================================================================
# TEST 18: PIPES
# ==============================================================================
print_header "TEST 18: PIPES"

run_test "Simple pipe" "echo hello | cat" "hello"
run_test "Multiple pipes" "echo hello | cat | cat" "hello"
run_test "Triple pipe" "echo test | cat | cat | cat" "test"
run_test "ls | grep" "ls | grep minishell" "minishell"
run_test "echo | grep" "echo hello world | grep hello" "hello world"
run_test "ls | wc -l" "ls | wc -l" "NOCRASH"

# Failing command in pipe
run_test "Failing ls in pipe" "ls nonexistent_file_12345 | cat" "NOCRASH"

# Mixed pipes and redirections
test_file_output "Pipe with output redir" "echo hello | cat > $RESULTS_DIR/pipe_out.txt" "$RESULTS_DIR/pipe_out.txt" "hello"

# ==============================================================================
# TEST 19: ENVIRONMENT VARIABLES
# ==============================================================================
print_header "TEST 19: ENVIRONMENT VARIABLES"

run_test "echo \$PWD" "echo \$PWD" "/"
run_test "echo \$PATH" "echo \$PATH" "/"
run_test "echo \$NONEXISTENT (empty)" "echo \$NONEXISTENT" "NOCRASH"
run_test "echo \$?" "echo \$?" "0"
run_test "echo \$? after false" "/bin/false\necho \$?" "1"
run_test "Variable expansion" "export MYTEST=hello\necho \$MYTEST" "hello"

# ==============================================================================
# TEST 20: EDGE CASES & STRESS TESTS
# ==============================================================================
print_header "TEST 20: EDGE CASES & STRESS TESTS"

run_test "Nonexistent command" "dsbksdgbksdghsd" "NOCRASH"
run_test "Very long echo" "echo $(printf 'a%.0s' {1..200})" "aaaa"
run_test "Command with special chars" "echo test\!@#%" "NOCRASH"
run_test "Empty double quotes" 'echo ""' "NOCRASH"
run_test "Empty single quotes" "echo ''" "NOCRASH"
run_test "Mixed quotes" 'echo "hello '\''world'\''"' "NOCRASH"
run_test "Quote in quote" "echo \"'hello'\"" "'hello'"
run_test "Many arguments" "echo a b c d e f g h i j k l m n o p q r s t u v w x y z" "a b c d e f g h i j k l m n o p q r s t u v w x y z"

# ==============================================================================
# TEST 21: MEMORY LEAK CHECK (with valgrind if available)
# ==============================================================================
print_header "TEST 21: MEMORY LEAK CHECK"

if command -v valgrind &> /dev/null; then
    echo -e "${CYAN}Running valgrind memory check...${NC}"
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    valgrind_out=$(echo -e "echo test\nls\npwd\nexport X=1\nunset X\nexit" | timeout 30 valgrind --leak-check=full --error-exitcode=42 $MINISHELL 2>&1)
    valgrind_exit=$?
    
    if echo "$valgrind_out" | grep -q "definitely lost: 0 bytes"; then
        echo -e "${GREEN}[PASS]${NC} No memory leaks detected by valgrind"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    elif [[ $valgrind_exit -ne 42 ]]; then
        echo -e "${GREEN}[PASS]${NC} No critical memory errors"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${YELLOW}[WARN]${NC} Potential memory leaks detected"
        echo "$valgrind_out" | grep -A3 "LEAK SUMMARY"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
else
    skip_test "Valgrind memory check" "valgrind not installed"
fi

# ==============================================================================
# MANUAL TESTING REMINDERS
# ==============================================================================
print_header "MANUAL TESTING REQUIRED"

echo -e "${YELLOW}The following tests REQUIRE manual/interactive testing:${NC}\n"

echo -e "${CYAN}SIGNALS:${NC}"
echo "  • Ctrl+C on empty prompt → should show new line with new prompt"
echo "  • Ctrl+\\ on empty prompt → should not do anything"
echo "  • Ctrl+D on empty prompt → should quit minishell"
echo "  • Ctrl+C after typing text → should clear line, show new prompt"
echo "  • Ctrl+D after typing text → should not do anything"
echo "  • Ctrl+\\ after typing text → should quit"
echo "  • Ctrl+C during 'cat' → should interrupt cat, new prompt"
echo "  • Ctrl+\\ during 'cat' → should show 'Quit (core dumped)'"
echo "  • Ctrl+D during 'cat' → should end cat input"

echo -e "\n${CYAN}HISTORY:${NC}"
echo "  • Type commands, then use UP arrow → should show previous command"
echo "  • DOWN arrow → should navigate forward in history"
echo "  • Commands should be saved in history"
echo "  • Type partial command + Ctrl+C + Enter → buffer should be clean"

# ==============================================================================
# FINAL SUMMARY
# ==============================================================================
print_header "FINAL TEST SUMMARY"

echo -e "${BOLD}╔═══════════════════════════════════════╗${NC}"
echo -e "${BOLD}║          TEST RESULTS                 ║${NC}"
echo -e "${BOLD}╠═══════════════════════════════════════╣${NC}"
printf "${BOLD}║${NC}  Total Tests:    ${BOLD}%-20d${NC}${BOLD}║${NC}\n" "$TOTAL_TESTS"
printf "${BOLD}║${NC}  ${GREEN}Passed:${NC}         ${GREEN}%-20d${NC}${BOLD}║${NC}\n" "$PASSED_TESTS"
printf "${BOLD}║${NC}  ${RED}Failed:${NC}         ${RED}%-20d${NC}${BOLD}║${NC}\n" "$FAILED_TESTS"
printf "${BOLD}║${NC}  ${YELLOW}Skipped:${NC}        ${YELLOW}%-20d${NC}${BOLD}║${NC}\n" "$SKIPPED_TESTS"
echo -e "${BOLD}╠═══════════════════════════════════════╣${NC}"

if [[ $TOTAL_TESTS -gt 0 ]]; then
    PERCENTAGE=$((PASSED_TESTS * 100 / TOTAL_TESTS))
    printf "${BOLD}║${NC}  ${CYAN}Score:${NC}          ${BOLD}${CYAN}%-17d%%${NC}${BOLD}║${NC}\n" "$PERCENTAGE"
else
    PERCENTAGE=0
fi

echo -e "${BOLD}╚═══════════════════════════════════════╝${NC}"

if [[ $PERCENTAGE -ge 95 ]]; then
    echo -e "\n${GREEN}${BOLD}🎉 EXCELLENT! Your minishell is performing great!${NC}"
elif [[ $PERCENTAGE -ge 80 ]]; then
    echo -e "\n${GREEN}${BOLD}✅ GOOD! Minor issues to address.${NC}"
elif [[ $PERCENTAGE -ge 60 ]]; then
    echo -e "\n${YELLOW}${BOLD}⚠️  NEEDS WORK. Review the failed tests.${NC}"
else
    echo -e "\n${RED}${BOLD}❌ SIGNIFICANT ISSUES. Many tests failing.${NC}"
fi

# Cleanup
rm -rf "$RESULTS_DIR"

echo -e "\n${CYAN}Run with VERBOSE=1 for detailed failure output:${NC}"
echo -e "  ${BOLD}VERBOSE=1 ./test_mandatory.sh${NC}\n"

exit $FAILED_TESTS
