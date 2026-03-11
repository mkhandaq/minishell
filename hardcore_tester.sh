#!/bin/bash

# ================================================================
#  shellGuys — HARDCORE Minishell Tester
#  Tests: wrong output, wrong exit codes, segfaults, memory leaks
# ================================================================

MINISHELL=${1:-./minishell}
PASS=0
FAIL=0
ERRORS=()

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

# Strip ANSI + prompt from stdout
strip_prompt()
{
	sed 's/\x1b\[[0-9;]*[mK]//g' | \
	sed '/shellGuys/d' | \
	grep -v '^> $' || true
}

# Strip only ANSI from stderr (keep error messages!)
strip_stderr()
{
	sed 's/\x1b\[[0-9;]*[mK]//g'
}

run_test()
{
	local desc="$1" cmd="$2" expected_out="$3" expected_exit="${4:-0}"
	local actual_out actual_exit tmpf
	tmpf=$(mktemp)
	actual_out=$(printf '%s\n' "$cmd" \
		| ( "$MINISHELL" 2>/dev/null; echo $? > "$tmpf" ) \
		| strip_prompt)
	actual_exit=$(cat "$tmpf"); rm -f "$tmpf"
	local out_ok=0 exit_ok=0
	[ "$actual_out" = "$expected_out" ] && out_ok=1
	[ "$actual_exit" = "$expected_exit" ] && exit_ok=1
	if [ $out_ok -eq 1 ] && [ $exit_ok -eq 1 ]; then
		echo -e "  ${GREEN}[PASS]${RESET} $desc"
		PASS=$((PASS + 1))
	else
		echo -e "  ${RED}[FAIL]${RESET} $desc"
		[ $out_ok -eq 0 ] && \
			echo -e "         out  expected: |$expected_out|\n         out  got:      |$actual_out|"
		[ $exit_ok -eq 0 ] && \
			echo -e "         exit expected: $expected_exit  got: $actual_exit"
		FAIL=$((FAIL + 1)); ERRORS+=("$desc")
	fi
}

run_exit_test()
{
	local desc="$1" cmd="$2" expected_exit="$3" actual_exit
	printf '%s\n' "$cmd" | "$MINISHELL" >/dev/null 2>&1
	actual_exit=$?
	if [ "$actual_exit" = "$expected_exit" ]; then
		echo -e "  ${GREEN}[PASS]${RESET} $desc"
		PASS=$((PASS + 1))
	else
		echo -e "  ${RED}[FAIL]${RESET} $desc"
		echo -e "         exit expected: $expected_exit  got: $actual_exit"
		FAIL=$((FAIL + 1)); ERRORS+=("$desc")
	fi
}

run_stderr_test()
{
	local desc="$1" cmd="$2" pattern="$3"
	local actual_err
	actual_err=$(printf '%s\n' "$cmd" \
		| "$MINISHELL" 2>&1 >/dev/null | strip_stderr)
	if echo "$actual_err" | grep -q "$pattern"; then
		echo -e "  ${GREEN}[PASS]${RESET} $desc"
		PASS=$((PASS + 1))
	else
		echo -e "  ${RED}[FAIL]${RESET} $desc"
		echo -e "         stderr expected pattern: |$pattern|\n         stderr got: |$actual_err|"
		FAIL=$((FAIL + 1)); ERRORS+=("$desc")
	fi
}

run_crash_test()
{
	local desc="$1" cmd="$2" actual_exit
	printf '%s\n' "$cmd" | "$MINISHELL" >/dev/null 2>&1
	actual_exit=$?
	if [ "$actual_exit" = "139" ]; then
		echo -e "  ${RED}[SEGFAULT]${RESET} $desc"
		FAIL=$((FAIL + 1)); ERRORS+=("SEGFAULT: $desc")
	else
		echo -e "  ${GREEN}[PASS]${RESET} $desc (no crash)"
		PASS=$((PASS + 1))
	fi
}

run_leak_test()
{
	local desc="$1" cmd="$2"
	if ! command -v valgrind &>/dev/null; then
		echo -e "  ${YELLOW}[SKIP]${RESET} $desc (no valgrind)"
		return
	fi
	local vg_out
	vg_out=$(printf '%s\n' "$cmd" \
		| valgrind --leak-check=full --error-exitcode=42 \
		  "$MINISHELL" 2>&1 >/dev/null)
	if echo "$vg_out" | grep -q "definitely lost: 0 bytes\|no leaks are possible"; then
		echo -e "  ${GREEN}[PASS]${RESET} $desc (no leaks)"
		PASS=$((PASS + 1))
	elif echo "$vg_out" | grep -q "definitely lost"; then
		local lost
		lost=$(echo "$vg_out" | grep "definitely lost" | tail -1)
		echo -e "  ${RED}[LEAK]${RESET} $desc — $lost"
		FAIL=$((FAIL + 1)); ERRORS+=("LEAK: $desc")
	else
		echo -e "  ${YELLOW}[SKIP]${RESET} $desc (valgrind inconclusive)"
	fi
}

section() { echo -e "\n${CYAN}${BOLD}━━━ $1 ━━━${RESET}"; }

[ ! -x "$MINISHELL" ] && \
	echo -e "${RED}Error: '$MINISHELL' not found or not executable.${RESET}" && exit 1

echo -e "${BOLD}shellGuys HARDCORE Tester${RESET}"
echo "Binary: $MINISHELL"
echo "========================================"

# ════════════════════════════════════════════════════════════
section "ECHO — edge cases"
# ════════════════════════════════════════════════════════════

run_test "echo no args"                    "echo"                     ""
run_test "echo empty string"               "echo ''"                  ""
run_test "echo two empty strings"          "echo '' ''"               " "
run_test "echo -n no newline"              "echo -n hello"            "hello"
run_test "echo -nnn treated as -n"         "echo -nnn hello"          "hello"
run_test "echo -na NOT a flag"             "echo -na hello"           "-na hello"
run_test "echo -e NOT a flag"              "echo -e hello"            "-e hello"
run_test "echo -- NOT a flag"              "echo -- hello"            "-- hello"
run_test "echo -n -n multi flag"           "echo -n -n hello"         "hello"
run_test "echo spaces preserved"           "echo '   hi   '"          "   hi   "
run_test "echo tab literal in quotes"      "echo 'a	b'"              "a	b"
run_test "echo backslash literal"          "echo 'a\\b'"              "a\\b"
run_test "echo dollar not in single"       "echo '\$HOME'"            "\$HOME"
run_test "echo adjacent single quotes"     "echo 'a''b'"              "ab"
run_test "echo quote then word"            "echo 'hi'world"           "hiworld"
run_test "echo word then quote"            "echo hi'world'"           "hiworld"
run_test "echo mixed quote types"          "echo \"a\"'b'"            "ab"
run_test "echo many words"                 "echo a b c d e f"         "a b c d e f"

# ════════════════════════════════════════════════════════════
section "EXIT — edge cases"
# ════════════════════════════════════════════════════════════

run_exit_test "exit no arg = 0"            "exit"                     0
run_exit_test "exit 0"                     "exit 0"                   0
run_exit_test "exit 1"                     "exit 1"                   1
run_exit_test "exit 42"                    "exit 42"                  42
run_exit_test "exit 127"                   "exit 127"                 127
run_exit_test "exit 255"                   "exit 255"                 255
run_exit_test "exit 256 wraps to 0"        "exit 256"                 0
run_exit_test "exit 300 wraps to 44"       "exit 300"                 44
run_exit_test "exit -1 = 255"              "exit -1"                  255
run_exit_test "exit -42 = 214"             "exit -42"                 214
run_exit_test "exit non-numeric = 2"       "exit abc"                 2
run_exit_test "exit too many args = 1"     "exit 1 2"                 1
run_exit_test "exit uses \$? from true"    "true && exit"             0
run_exit_test "exit uses \$? from false"   "false || exit"            1

# ════════════════════════════════════════════════════════════
section "CD — edge cases"
# ════════════════════════════════════════════════════════════

run_test  "cd home no arg"                 "cd && pwd"                "$HOME"
run_test  "cd absolute"                    "cd /tmp && pwd"           "/tmp"
run_test  "cd dotdot"                      "cd /tmp && cd .. && pwd"  "/"
run_test  "cd dot is noop"                 "cd /tmp && cd . && pwd"   "/tmp"
run_test  "cd updates OLDPWD"              "cd /tmp && cd / && echo \$OLDPWD" "/tmp"
run_test  "cd - goes to OLDPWD"            "cd /tmp && cd / && cd - && pwd" "$(printf '/tmp\n/tmp')"
run_exit_test "cd nonexistent = 1"         "cd /no_such_dir_xyz"      1
run_exit_test "cd too many args = 1"       "cd a b c"                 1
run_exit_test "cd unset HOME = 1"          "unset HOME && cd"         1
run_stderr_test "cd nonexistent message"   "cd /no_such_dir_xyz"      "No such file or directory"
run_stderr_test "cd too many args message" "cd a b"                   "too many arguments"

# ════════════════════════════════════════════════════════════
section "EXPORT — edge cases"
# ════════════════════════════════════════════════════════════

run_test  "export basic"                   "export X=hello && echo \$X"       "hello"
run_test  "export overwrite"               "export X=a && export X=b && echo \$X" "b"
run_test  "export empty value"             "export X= && echo \$X"            ""
run_test  "export with underscore"         "export _X=ok && echo \$_X"        "ok"
run_test  "export multi vars"              "export A=1 B=2 && echo \$A \$B"   "1 2"
run_test  "export shows declare -x"        "export Z=test && export | grep '^declare -x Z'" \
          "declare -x Z=\"test\""
run_test  "export empty val shows =\"\""   "export Z= && export | grep '^declare -x Z'" \
          "declare -x Z=\"\""
run_test  "export no val shows no ="       \
          "unset TESTNOVAL42 && export TESTNOVAL42 && export | grep '^declare -x TESTNOVAL42$'" \
          "declare -x TESTNOVAL42"
run_exit_test "export no args = 0"         "export"                   0
run_exit_test "export var no value = 0"    "export MYVAR42"           0
run_exit_test "export invalid =val = 1"    "export =val"              1
run_exit_test "export invalid 1var = 1"    "export 1var=x"            1
run_exit_test "export invalid -var = 1"    "export -var=x"            1
run_stderr_test "export invalid message"   "export 1var=x"            "not a valid identifier"

# ════════════════════════════════════════════════════════════
section "UNSET — edge cases"
# ════════════════════════════════════════════════════════════

run_test  "unset removes var"              "export X=hi && unset X && echo \$X"  ""
run_test  "unset nonexistent is ok"        "unset TOTALLY_UNDEFINED_VAR && echo ok" "ok"
run_test  "unset multi vars"               "export A=1 && export B=2 && unset A B && echo \$A\$B" ""
run_exit_test "unset invalid id = 1"       "unset 1invalid"           1
run_stderr_test "unset invalid message"    "unset 1invalid"           "not a valid identifier"

# ════════════════════════════════════════════════════════════
section "EXPANSION — dollar signs"
# ════════════════════════════════════════════════════════════

run_test  "expand \$HOME"                  "echo \$HOME"              "$HOME"
run_test  "expand \$PATH"                  "echo \$PATH"              "$PATH"
run_test  "expand undefined = empty"       "echo \$UNDEFINED_XYZ_VAR" ""
run_test  "expand \$? = 0 after true"      "true && echo \$?"         "0"
run_test  "expand \$? = 1 after false"     "false || echo \$?"        "1"
run_test  "expand \$? updates each cmd"    "true && echo \$? && false || echo \$?" \
          "$(printf '0\n1')"
run_test  "expand \$? = 0 initial"         "echo \$?"                 "0"
run_test  "expand \$?\$? concatenated"     "true && echo \$?\$?"      "00"
run_test  "expand \$ alone = \$"           "echo \$"                  "\$"
run_test  "expand \$9 = empty"             "echo \$9"                 ""
run_test  "expand \$1 = empty"             "echo \$1"                 ""
run_test  "expand in dbl quotes"           "export X=hi && echo \"\$X\"" "hi"
run_test  "no expand in single quotes"     "export X=hi && echo '\$X'" "\$X"
run_test  "expand \$HOME with slash"       "echo \$HOME/"             "$HOME/"
run_test  "export then expand"             "export MYVAR=test123 && echo \$MYVAR" "test123"
run_test  "unset then expand = empty"      "export A=1 && unset A && echo \$A" ""

# ════════════════════════════════════════════════════════════
section "QUOTES — parsing edge cases"
# ════════════════════════════════════════════════════════════

run_test  "single quotes literal pipe"     "echo 'a|b'"               "a|b"
run_test  "single quotes literal and"      "echo 'a&&b'"              "a&&b"
run_test  "single quotes literal redir"    "echo 'a>b'"               "a>b"
run_test  "single quotes literal dollar"   "echo '\$HOME'"            "\$HOME"
run_test  "single quotes literal star"     "echo '*.c'"               "*.c"
run_test  "double quotes expand dollar"    "echo \"\$HOME\""          "$HOME"
run_test  "double quotes literal star"     "echo \"*.c\""             "*.c"
run_test  "double quotes spaces preserved" "echo \"  hi  \""          "  hi  "
run_test  "double quotes literal pipe"     "echo \"a|b\""             "a|b"
run_test  "empty single quotes"            "echo ''"                  ""
run_test  "empty double quotes"            "echo \"\""                ""
run_test  "empty quotes between words"     "echo a''b"                "ab"
run_test  "empty dbl quotes between words" "echo a\"\"b"              "ab"
run_test  "quote touching operator"        "echo 'hi' | cat"          "hi"
run_test  "quoted whitespace as arg"       "echo \"   \""             "   "

# ════════════════════════════════════════════════════════════
section "PIPES — correctness & exit codes"
# ════════════════════════════════════════════════════════════

run_test  "simple pipe"                    "echo hello | cat"               "hello"
run_test  "pipe chain 3"                   "echo hello | cat | cat"         "hello"
run_test  "pipe chain 4"                   "echo hello | cat | cat | cat"   "hello"
run_test  "pipe to grep match"             "echo b | grep b"                "b"
run_test  "pipe to grep no match = empty"  "echo hello | grep NOMATCH"      "" 1
run_test  "pipe multiple words"            "echo one two | cat"             "one two"
run_test  "pipe builtin to cat"            "echo test | cat"                "test"
run_test  "pipe left ignored when right has own input" "true | echo right"  "right"
run_test  "many pipes"                     "echo x | cat | cat | cat | cat" "x"
run_exit_test "pipe exit = rightmost"      "true | false"                   1
run_exit_test "pipe exit = rightmost 2"    "false | true"                   0
run_exit_test "pipe all false = 1"         "false | false | false"          1
run_exit_test "pipe last true = 0"         "false | false | true"           0

# ════════════════════════════════════════════════════════════
section "REDIRECTIONS — edge cases"
# ════════════════════════════════════════════════════════════

T=/tmp/ms_hc_$$
run_test  "> creates file"              "echo hi > ${T}_1 && cat ${T}_1"  "hi"
run_test  ">> appends"                  "echo a > ${T}_2 && echo b >> ${T}_2 && cat ${T}_2" \
          "$(printf 'a\nb')"
run_test  "< reads file"                "echo world > ${T}_3 && cat < ${T}_3" "world"
run_test  "> truncates"                 "echo old > ${T}_4 && echo new > ${T}_4 && cat ${T}_4" "new"
run_test  ">> creates if missing"       "rm -f ${T}_5 && echo only >> ${T}_5 && cat ${T}_5" "only"
run_test  "redir then pipe"             "echo hi > ${T}_6 && cat < ${T}_6 | cat" "hi"
run_test  "multiple redirs same cmd"    "echo hi > ${T}_7 && cat < ${T}_7 > ${T}_8 && cat ${T}_8" "hi"
run_test  "redir out then cat"          "echo x > ${T}_9 && cat ${T}_9" "x"
run_exit_test "< nonexistent = 1"       "cat < /no_such_file_xyz"          1
run_exit_test "> no permission = 1"     "echo hi > /no/such/path/file"     1
run_exit_test "< fail doesnt kill pipe" "cat < /no_such_xyz | echo right"  0
run_stderr_test "< nonexistent message" "cat < /no_such_file_xyz" "No such file or directory"
rm -f ${T}_[1-9]

# ════════════════════════════════════════════════════════════
section "HEREDOC — edge cases"
# ════════════════════════════════════════════════════════════

run_test "heredoc basic" \
	"$(printf 'cat << EOF\nhello\nEOF')" "hello"

run_test "heredoc multiline" \
	"$(printf 'cat << EOF\nline1\nline2\nEOF')" "$(printf 'line1\nline2')"

run_test "heredoc expands dollar (unquoted limiter)" \
	"$(printf 'cat << EOF\n$HOME\nEOF')" "$HOME"

run_test "heredoc no expand (single-quoted limiter)" \
	"$(printf "cat << 'EOF'\n\$HOME\nEOF")" '$HOME'

run_test "heredoc no expand (double-quoted limiter)" \
	"$(printf 'cat << "EOF"\n$HOME\nEOF')" '$HOME'

run_test "heredoc expands \$?" \
	"$(printf 'true\ncat << EOF\n$?\nEOF')" "0"

run_test "heredoc piped" \
	"$(printf 'cat << EOF | cat\nhello\nEOF')" "hello"

run_test "heredoc tab not stripped" \
	"$(printf 'cat << EOF\n\thello\nEOF')" "	hello"

run_test "heredoc with redir out" \
	"$(printf "cat << EOF > ${T}_hd\nhello\nEOF\ncat ${T}_hd && rm -f ${T}_hd")" "hello"

# ════════════════════════════════════════════════════════════
section "&& and || — operator precedence"
# ════════════════════════════════════════════════════════════

run_test  "&& both succeed"              "echo a && echo b"            "$(printf 'a\nb')"
run_test  "&& first fails skips right"   "false && echo b"             "" 1
run_test  "|| first fails runs right"    "false || echo b"             "b"
run_test  "|| first succeeds skips right" "echo a || echo b"           "a"
run_test  "false && echo || echo"        "false && echo a || echo b"   "b"
run_test  "true || echo && echo"         "true || echo a && echo b"    "b"
run_test  "chained &&"                   "echo a && echo b && echo c"  "$(printf 'a\nb\nc')"
run_test  "chained ||"                   "false || false || echo last" "last"
run_test  "mixed chain"                  "(false || true) && echo yes" "yes"
run_exit_test "&& exit 0"                "true && true"                0
run_exit_test "&& exit 1 right"          "true && false"               1
run_exit_test "|| exit 0 right"          "false || true"               0
run_exit_test "|| both fail = 1"         "false || false"              1

# ════════════════════════════════════════════════════════════
section "SUBSHELL — isolation & edge cases"
# ════════════════════════════════════════════════════════════

run_test  "subshell basic"              "(echo hello)"                "hello"
run_test  "subshell pipe"               "(echo hello | cat)"          "hello"
run_test  "subshell && inside"          "(echo a && echo b)"          "$(printf 'a\nb')"
run_test  "subshell env isolated"       "export X=outer && (export X=inner) && echo \$X" "outer"
run_test  "subshell cd isolated"        "cd /tmp && (cd /) && pwd"    "/tmp"
run_test  "nested subshells"            "((echo deep))"               "deep"
run_test  "subshell output piped"       "(echo a && echo b) | cat"    "$(printf 'a\nb')"
run_test  "subshell unset isolated"     "export A=1 && (unset A) && echo \$A" "1"
run_test  "subshell with redir"         "(echo hi) > ${T}_sub && cat ${T}_sub" "hi"
run_exit_test "subshell exit code"      "(exit 42)"                   42
run_exit_test "subshell false"          "(false)"                     1
run_exit_test "subshell exit 0"         "(true)"                      0
rm -f ${T}_sub

# ════════════════════════════════════════════════════════════
section "WILDCARDS — tricky cases"
# ════════════════════════════════════════════════════════════

WD=/tmp/ms_wc_hc_$$
mkdir -p "$WD"
touch "$WD/a.c" "$WD/b.c" "$WD/test.h" "$WD/.hidden"

run_test "wildcard *.c sorted"          "cd $WD && echo *.c"          "a.c b.c"
run_test "wildcard all non-hidden"       "cd $WD && echo *"            "a.c b.c test.h"
run_test "wildcard hidden not matched"   "cd $WD && echo *"            "a.c b.c test.h"
run_test "wildcard no match = literal"  "cd $WD && echo *.xyz"        "*.xyz"
run_test "wildcard in dbl quotes = lit" "cd $WD && echo \"*.c\""      "*.c"
run_test "wildcard in sgl quotes = lit" "cd $WD && echo '*.c'"        "*.c"
run_test "wildcard middle"              "cd $WD && echo a*c"          "a.c"
run_test "wildcard prefix"              "cd $WD && echo *h"           "test.h"
run_test "wildcard suffix"              "cd $WD && echo t*"           "test.h"
run_test "wildcard piped"               "cd $WD && ls *.c | cat"      "$(printf 'a.c\nb.c')"
rm -rf "$WD"

# ════════════════════════════════════════════════════════════
section "SYNTAX ERRORS — must exit 2"
# ════════════════════════════════════════════════════════════

run_exit_test "bare pipe"               "|"                           2
run_exit_test "bare &&"                 "&&"                          2
run_exit_test "bare ||"                 "||"                          2
run_exit_test "trailing pipe"           "echo hi |"                   2
run_exit_test "trailing &&"             "echo hi &&"                  2
run_exit_test "trailing ||"             "echo hi ||"                  2
run_exit_test "double operator"         "echo hi || &&"               2
run_exit_test "pipe then &&"            "echo | &&"                   2
run_exit_test "unclosed paren"          "(echo hi"                    2
run_exit_test "empty parens"            "()"                          2
run_exit_test "close without open"      ") echo hi"                   2
run_exit_test "> with no file"          "echo hi >"                   2
run_exit_test "< with no file"          "echo hi <"                   2
run_exit_test ">> with no file"         "echo hi >>"                  2
run_exit_test "< then |"               "echo hi < |"                  2

# ════════════════════════════════════════════════════════════
section "COMMAND NOT FOUND / PERMISSIONS"
# ════════════════════════════════════════════════════════════

run_exit_test "nonexistent cmd = 127"   "nonexistent_cmd_xyz"         127
run_exit_test "nonexistent after &&"    "true && nonexistent_cmd_xyz" 127
run_stderr_test "not found message"     "nonexistent_cmd_xyz"         "command not found"
run_test  "cmd with full path"          "/bin/echo hello"             "hello"
run_exit_test "dir as cmd = 126"        "/tmp"                        126

NXFILE=/tmp/ms_noperm_$$
touch "$NXFILE" && chmod 000 "$NXFILE"
run_exit_test "no-execute-perm = 126"   "$NXFILE"                     126
run_stderr_test "permission denied msg" "$NXFILE"                     "Permission denied"
rm -f "$NXFILE"

# ════════════════════════════════════════════════════════════
section "SEGFAULT / CRASH TESTS"
# ════════════════════════════════════════════════════════════

run_crash_test "empty input"                     ""
run_crash_test "spaces only"                     "   "
run_crash_test "tabs only"                       "			"
run_crash_test "single quote unclosed"           "echo 'hello"
run_crash_test "double quote unclosed"           "echo \"hello"
run_crash_test "just dollar sign"                "echo \$"
run_crash_test "dollar digit"                    "echo \$1"
run_crash_test "dollar special char"             "echo \$-"
run_crash_test "very long argument"              "echo $(python3 -c 'print("a"*4096)')"
run_crash_test "many arguments"                  "echo $(seq 1 200 | tr '\n' ' ')"
run_crash_test "deeply nested subshells"         "((((echo hi))))"
run_crash_test "pipe to nothing"                 "echo hi | true"
run_crash_test "redir stdin stdout same file"    \
    "echo test > /tmp/ms_same_$$ && cat < /tmp/ms_same_$$ > /tmp/ms_same_$$; rm -f /tmp/ms_same_$$"
run_crash_test "export with spaces in value"     "export 'A B'=x"
run_crash_test "unset with no args"              "unset"
run_crash_test "cd with empty string"            "cd ''"
run_crash_test "echo then immediate pipe"        "echo|cat"
run_crash_test "consecutive redirects"           \
    "echo hi > /tmp/ms_cr1_$$ > /tmp/ms_cr2_$$; rm -f /tmp/ms_cr1_$$ /tmp/ms_cr2_$$"
run_crash_test "pipe to builtin cd"              "echo /tmp | cd"
run_crash_test "subshell redir and pipe"         \
    "(echo a | cat) > /tmp/ms_cr3_$$; rm -f /tmp/ms_cr3_$$"
run_crash_test "and-or long chain"               "true && true && true && false || echo end"
run_crash_test "empty export value"              "export EMPTYVAR= && echo \$EMPTYVAR"
run_crash_test "double unset same var"           "export X=1 && unset X && unset X && echo ok"
run_crash_test "heredoc eof mid-input"           "$(printf 'cat << EOF\nhello')"
run_crash_test "expand undefined in redir"       "cat < \$UNDEFINED_REDIR_VAR"
run_crash_test "redir to expanded empty"         "echo hi > \$UNDEFINED_TARGET"
run_crash_test "expr in subshell then exit"      "(export X=1 && echo \$X) && echo \$X"
run_crash_test "pwd after unset PWD"             "unset PWD && pwd"
run_crash_test "env after unsetting all"         "unset HOME PATH USER && env | head -1"
run_crash_test "cd - without OLDPWD"             "unset OLDPWD && cd -"
run_crash_test "multiple heredocs sequentially"  "$(printf 'cat << A\nfoo\nA')"
run_crash_test "heredoc then another command"    \
    "$(printf 'cat << EOF\nhello\nEOF\necho world')"
run_crash_test "nested subshell with operators"  "((true && echo ok) || echo bad)"
run_crash_test "pipe then redir"                 \
    "echo hi | cat > /tmp/ms_pr_$$ && cat /tmp/ms_pr_$$; rm -f /tmp/ms_pr_$$"
run_crash_test "export then immediate use"       "export A=hello && echo \$A | cat"
run_crash_test "cd - after one cd"               "cd /tmp && cd -"
run_crash_test "unset PATH then use absolute"    "unset PATH && /bin/echo ok"
run_crash_test "empty string as command"         "''"

# ════════════════════════════════════════════════════════════
section "BEHAVIOR DIFFERENCES vs BASH"
# ════════════════════════════════════════════════════════════

run_test  "pipe exit is rightmost"          "false | false | true"        "" 0
run_test  "subshell cd isolation"           "cd /tmp && (cd /) && pwd"    "/tmp"
run_test  "subshell export isolation"       "export A=1 && (unset A) && echo \$A" "1"
run_test  "unset PATH absolute still works" "unset PATH && /bin/echo hi"  "hi"
run_test  "cd - prints dest"                \
    "cd /tmp && cd / && cd -" "/tmp"
run_test  "heredoc tab not stripped"        \
    "$(printf 'cat << EOF\n\thello\nEOF')" "	hello"
run_test  "empty quoted arg passed to echo" "echo '' | cat"               ""
run_test  "false then \$? = 1"             "false || echo \$?"            "1"
run_test  "true then \$? = 0"              "true && echo \$?"             "0"

# ════════════════════════════════════════════════════════════
section "MEMORY LEAK TESTS (valgrind)"
# ════════════════════════════════════════════════════════════

run_leak_test "simple echo"           "echo hello"
run_leak_test "pipe"                  "echo hello | cat"
run_leak_test "export then unset"     "export X=1 && unset X"
run_leak_test "failed redirect"       "cat < /no_such_file_xyz"
run_leak_test "heredoc"               "$(printf 'cat << EOF\nhello\nEOF')"
run_leak_test "subshell"              "(echo hello)"
run_leak_test "syntax error"          "echo |"
run_leak_test "and-or chain"          "true && false || echo end"
run_leak_test "wildcard no match"     "echo /no_match_xyz_*"
run_leak_test "cd and back"           "cd /tmp && cd -"
run_leak_test "nested subshell"       "((echo deep))"
run_leak_test "many pipes"            "echo x | cat | cat | cat | cat"
run_leak_test "expand undefined"      "echo \$TOTALLY_UNDEFINED_VAR_XYZ"
run_leak_test "exit after export"     "export X=1 && exit 0"
run_leak_test "failed cmd 127"        "nonexistent_cmd_xyz"
run_leak_test "heredoc with expand"   "$(printf 'export X=hi\ncat << EOF\n$X\nEOF')"

# ════════════════════════════════════════════════════════════
# Summary
# ════════════════════════════════════════════════════════════

TOTAL=$((PASS + FAIL))
echo ""
echo "========================================"
echo -e "${BOLD}Results: ${GREEN}$PASS${RESET}${BOLD} passed, ${RED}$FAIL${RESET}${BOLD} failed, $TOTAL total${RESET}"
echo "========================================"

if [ ${#ERRORS[@]} -gt 0 ]; then
	echo -e "\n${YELLOW}${BOLD}Failed tests:${RESET}"
	for err in "${ERRORS[@]}"; do
		echo -e "  ${RED}✗${RESET} $err"
	done
fi
echo ""
[ $FAIL -eq 0 ] && exit 0 || exit 1
