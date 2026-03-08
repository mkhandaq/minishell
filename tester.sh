#!/bin/bash
# ============================================================
#  Mini tester for shellGuys (minishell)
#  Usage:  bash tester.sh [./minishell]
# ============================================================

SHELL_BIN="${1:-./minishell}"
PASS=0
FAIL=0
TOTAL=0

RED='\033[0;31m'
GRN='\033[0;32m'
YLW='\033[0;33m'
CYN='\033[0;36m'
RST='\033[0m'

# Strip ANSI escape codes + \001/\002 wrappers, then remove any
# "shellGuys$ …" prompt (even mid-line for echo -n), and the "exit" line.
strip_prompt()
{
    sed 's/\x01//g; s/\x02//g; s/\x1b\[[0-9;]*m//g' \
        | sed '/^shellGuys\$ /d; /^shellGuys\$$/d; s/shellGuys\$ .*//g; s/shellGuys\$//g' \
        | sed '/^exit$/d' \
        | sed '/^$/d'
}

run_test()
{
    local desc="$1"
    local cmd="$2"
    TOTAL=$((TOTAL + 1))

    # Run in real bash
    expected=$(echo "$cmd" | bash 2>/dev/null)
    exp_exit=$?

    # Run in minishell
    raw=$(echo "$cmd" | "$SHELL_BIN" 2>/dev/null)
    got_exit=$?
    got=$(printf '%s\n' "$raw" | strip_prompt)

    if [ "$expected" = "$got" ] && [ "$exp_exit" = "$got_exit" ]; then
        printf "${GRN}[PASS]${RST} %s\n" "$desc"
        PASS=$((PASS + 1))
    else
        printf "${RED}[FAIL]${RST} %s\n" "$desc"
        if [ "$expected" != "$got" ]; then
            printf "       expected output: |%s|\n" "$(echo "$expected" | head -3)"
            printf "       got output:      |%s|\n" "$(echo "$got" | head -3)"
        fi
        if [ "$exp_exit" != "$got_exit" ]; then
            printf "       expected exit: %s  got exit: %s\n" "$exp_exit" "$got_exit"
        fi
        FAIL=$((FAIL + 1))
    fi
}

# Like run_test but only compares exit code (for syntax error tests
# where bash writes to stderr and minishell may write to stdout).
run_test_exit()
{
    local desc="$1"
    local cmd="$2"
    TOTAL=$((TOTAL + 1))

    echo "$cmd" | bash 2>/dev/null >/dev/null
    exp_exit=$?

    echo "$cmd" | "$SHELL_BIN" 2>/dev/null >/dev/null
    got_exit=$?

    if [ "$exp_exit" = "$got_exit" ]; then
        printf "${GRN}[PASS]${RST} %s\n" "$desc"
        PASS=$((PASS + 1))
    else
        printf "${RED}[FAIL]${RST} %s\n" "$desc"
        printf "       expected exit: %s  got exit: %s\n" "$exp_exit" "$got_exit"
        FAIL=$((FAIL + 1))
    fi
}

# ------ Check binary exists ------
if [ ! -x "$SHELL_BIN" ]; then
    echo "Error: $SHELL_BIN not found or not executable. Run 'make' first."
    exit 1
fi

printf "${CYN}===== shellGuys tester =====${RST}\n\n"

# ==================== ECHO ====================
printf "${YLW}--- echo ---${RST}\n"
run_test "echo hello"              "echo hello"
run_test "echo multiple words"     "echo hello world foo bar"
run_test "echo -n flag"            "echo -n hello"
run_test "echo -nnnn flag"         "echo -nnnn hello"
run_test "echo empty"              "echo"
run_test "echo with quotes"        "echo 'hello world'"
run_test "echo double quotes"      'echo "hello world"'
run_test "echo mixed quotes"       "echo \"hello\"'world'"
run_test "echo -n no newline"      "echo -n no newline"

# ==================== PWD ====================
printf "\n${YLW}--- pwd ---${RST}\n"
run_test "pwd"                     "pwd"

# ==================== ENV ====================
printf "\n${YLW}--- env (check PATH exists) ---${RST}\n"
run_test "env | grep PATH"         "env | grep PATH"

# ==================== EXPORT / UNSET ====================
printf "\n${YLW}--- export / unset ---${RST}\n"
run_test "export + echo var"       'export TESTVAR=hello && echo $TESTVAR'
run_test "unset var"               'export TESTVAR=bye && unset TESTVAR && echo $TESTVAR'

# ==================== CD ====================
printf "\n${YLW}--- cd ---${RST}\n"
run_test "cd / && pwd"             "cd / && pwd"
run_test_exit "cd nonexistent"     "cd /no_such_dir_12345"

# ==================== VARIABLE EXPANSION ====================
printf "\n${YLW}--- expansion ---${RST}\n"
run_test '$HOME expansion'         'echo $HOME'
run_test '$USER expansion'         'echo $USER'
run_test '$? after true'           'true && echo $?'
run_test "single quotes no expand" "echo '\$HOME'"
run_test "double quotes expand"    'echo "$HOME"'

# ==================== PIPES ====================
printf "\n${YLW}--- pipes ---${RST}\n"
run_test "ls | wc -l"             "ls | wc -l"
run_test "echo | cat"             "echo hello | cat"
run_test "echo | cat | cat"       "echo hello | cat | cat"
run_test "cat /etc/hostname|head" "cat /etc/hostname | head -1"

# ==================== REDIRECTIONS ====================
printf "\n${YLW}--- redirections ---${RST}\n"
run_test "> outfile"              "echo redir_test > /tmp/ms_test_out && cat /tmp/ms_test_out && rm -f /tmp/ms_test_out"
run_test ">> append"              "echo line1 > /tmp/ms_test_app && echo line2 >> /tmp/ms_test_app && cat /tmp/ms_test_app && rm -f /tmp/ms_test_app"
run_test "< infile"               "echo hello > /tmp/ms_test_in && cat < /tmp/ms_test_in && rm -f /tmp/ms_test_in"

# ==================== EXIT STATUS ====================
printf "\n${YLW}--- exit status ---${RST}\n"
run_test "exit 0"                 "exit 0"
run_test "exit 42"                "exit 42"
run_test_exit "command not found" "nonexistent_cmd_xyz"

# ==================== SYNTAX ERRORS (exit code only) ====================
printf "\n${YLW}--- syntax errors ---${RST}\n"
run_test_exit "lone pipe"         "|"
run_test_exit "double pipe start" "|| ls"
run_test_exit "trailing pipe"     "ls |"
run_test_exit "double redir"      "ls > > file"

# ==================== EDGE CASES ====================
printf "\n${YLW}--- edge cases ---${RST}\n"
run_test "empty input"            ""
run_test "only spaces"            "   "
run_test "echo with tab"          "echo hello"

# ==================== SUMMARY ====================
printf "\n${CYN}========== RESULTS ==========${RST}\n"
printf "Total: %d   ${GRN}Pass: %d${RST}   ${RED}Fail: %d${RST}\n" "$TOTAL" "$PASS" "$FAIL"

if [ "$FAIL" -eq 0 ]; then
    printf "${GRN}All tests passed!${RST}\n"
else
    printf "${RED}Some tests failed.${RST}\n"
fi

exit "$FAIL"
