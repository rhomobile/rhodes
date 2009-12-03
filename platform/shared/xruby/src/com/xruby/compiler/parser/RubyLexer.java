/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.parser;

import java.util.*;
import java.io.Reader;
import antlr.*;

class StringDelimiter {
    private char delimiter_;
    private int count_;

    public StringDelimiter(char delimiter, int count) {
        delimiter_ = delimiter;
        count_ = count;
    }

    public char getDelimiter() {
        return delimiter_;
    }

    public int getCount() {
        return count_;
    }

    public void setCount(int value) {
        count_ = value;
    }
}

/**
 * I do not want my lexer be coupled with parser (even through it can not be
 * avoid), so my lexer does a little bit of parsing itself. Even though this
 * makes the lexer become more complicated, it is easier to test.
 */
public class RubyLexer extends RubyLexerBase {
    private SymbolTableManager stm_;
    private Token last_token_ = new Token();// create a new toekn with invalid type
    private boolean allow_asignment_ = true;
    private boolean allow_block_parameter_ = false;
    private boolean allow_for_expression_parameter_ = false;
    private boolean seen_whitespace_ = true;
    private boolean is_in_condition = false;
    private boolean just_finished_parsing_regex_expression_substituation_ = false;
    private boolean just_finished_parsing_string_expression_substituation_ = false;
    private boolean just_finished_parsing_heredoc_expression_substituation_ = false;
    private boolean just_finished_parsing_symbol_ = false;
    private boolean is_in_nested_multiple_assign_ = false;
    private Stack<StringDelimiter> current_special_string_delimiter_ = new Stack<StringDelimiter>();
    private InputBufferWithHereDocSupport input_;
    private String current_heredoc_delimiter_ = null;

    /// @strip boolean strip
    public RubyLexer(Reader in, boolean strip) {
        this(in, new SymbolTableManager(), strip);
    }

    public RubyLexer(Reader in, SymbolTableManager stm, boolean strip) {
        super(new InputBufferWithHereDocSupport(in));
        input_ = (InputBufferWithHereDocSupport)super.inputState.getInput();
        stm_ = stm;

        if (strip) {
            stripOffSomeText();
        }
    }

    private void stripOffSomeText() {
        try {
            while (LA(1) != '#' || LA(2) != '!') {
                mLINE(false);
            }
        } catch (CharStreamException e) {
        } catch (RecognitionException e) {
        } catch (TokenStreamException e) {
        }
    }

    public Token nextToken() throws TokenStreamException {
        try {
            try {
                return _nextToken();
            } catch (RecognitionException e) {
                throw new TokenStreamRecognitionException(e);
            }
        } catch (CharStreamException e) {
            if (e instanceof CharStreamIOException) {
                throw new TokenStreamIOException(((CharStreamIOException) e).io);
            } else {
                throw new TokenStreamException(e.toString());
            }
        }
    }

    SymbolTableManager getSymbolTableManager() {
        return stm_;
    }

    private Token getNextTokenFromStream() throws TokenStreamException, RecognitionException, CharStreamException {
        if (just_finished_parsing_string_expression_substituation_
                || just_finished_parsing_regex_expression_substituation_) {
            if (current_special_string_delimiter_.empty()) {
                throw new TokenStreamException("Delimiter mismatch!");
            }

            StringDelimiter delimiter = current_special_string_delimiter_.peek();
            mSTRING_BETWEEN_EXPRESSION_SUBSTITUTION(true, delimiter.getDelimiter(), delimiter.getCount());
            Token t = _returnToken;
            // System.out.println(t.getText());
            if (STRING_AFTER_EXPRESSION_SUBSTITUTION == t.getType()) {
                current_special_string_delimiter_.pop();
                if (just_finished_parsing_regex_expression_substituation_) {
                    text.setLength(0);
                    mREGEX_MODIFIER(true);
                    String option = _returnToken.getText();
                    t.setText(t.getText() + "/" + option);
                }
            }
            just_finished_parsing_string_expression_substituation_ = false;
            just_finished_parsing_regex_expression_substituation_ = false;
            return t;
        } else if (just_finished_parsing_heredoc_expression_substituation_) {
            mHERE_DOC_CONTENT(true, current_heredoc_delimiter_,
                                HERE_DOC_BETWEEN_EXPRESSION_SUBSTITUTION,
                                HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION);
            just_finished_parsing_heredoc_expression_substituation_ = false;
            if (HERE_DOC_AFTER_EXPRESSION_SUBSTITUTION == _returnToken.getType()) {
                input_.finishedHereDoc();
            }
            return _returnToken;
        } else if (HERE_DOC_BEGIN == last_token_.getType()) {
            // expect here doc
            // match it but skip the content, so that parser does
            // not have to parse it (but we'd better save it to
            // symbol table).
            current_heredoc_delimiter_ = last_token_.getText();
            mHERE_DOC_CONTENT(true, current_heredoc_delimiter_,
                                HERE_DOC_BEFORE_EXPRESSION_SUBSTITUTION,
                                HERE_DOC_CONTENT);
            if (HERE_DOC_CONTENT == _returnToken.getType()) {
                input_.finishedHereDoc();
            }
            return _returnToken;
        } else {
            return super.nextToken();
        }
    }

    private Token _nextToken() throws TokenStreamException, RecognitionException, CharStreamException {

        Token token = getNextTokenFromStream();

        // Even if there are rules for the following tokens, they should not
        // show up here.
        assert(WHITE_SPACE != token.getType());
        assert(END_OF_FILE != token.getType());
        assert(COMMENT != token.getType());
        assert(RDOC != token.getType());
        assert(RPAREN_IN_METHOD_DEFINATION != token.getType());
        assert(Token.SKIP != token.getType());

        // Do not do anything if convertion is done.
        if (!keywordOrOperatorToMethodName(token)) {
            changeLexerStateIfNecessary(token);
        }

        last_token_ = token;
        seen_whitespace_ = false;
        just_finished_parsing_symbol_ = false;
        return token;
    }

    private void changeLexerStateIfNecessary(Token token) throws TokenStreamException, CharStreamException {
        switch (token.getType()) {
        /*
         * it is OK to do this error checking here, now we leave it to the
         * parser.
         */
        case HERE_DOC_BEGIN:
            input_.skipToLineBreak();
            break;
        case SEMI:
        case LINE_BREAK:
            allow_asignment_ = true;
            is_in_condition = false;
            break;
        case RPAREN:
            allow_asignment_ = true;// def asctime() strftime('%c') end
            break;
        case IDENTIFIER:
        case FUNCTION:
            if (DOT != last_token_.getType() &&
                    COLON2 != last_token_.getType() &&
                    COLON_WITH_NO_FOLLOWING_SPACE != last_token_.getType()) {
                updateSymbolTable(token);
            }
            break;
        case BOR:
            if (allow_block_parameter_) {
                allow_block_parameter_ = false;
            } else if ((last_token_.getType() == LCURLY_BLOCK) ||
                    (last_token_.getType() == LITERAL_do) ||
                    (last_token_.getType() == LINE_BREAK)//better check the token before line break is 'do'
                    ) {
                allow_block_parameter_ = true;
            }
            break;
        case LITERAL_if:
        case LITERAL_unless:
        case LITERAL_rescue:
            convertToModifierIfNeeded(token);
            break;
        case LITERAL_while:
        case LITERAL_until:
            if (!convertToModifierIfNeeded(token)) {
                is_in_condition = true;
            }
            break;
        case LITERAL_for:
            is_in_condition = true;
            allow_for_expression_parameter_ = true;
            break;
        case LITERAL_do:
            if (is_in_condition) {
                token.setType(DO_IN_CONDITION);
                is_in_condition = false;
            }
            break;
        case LITERAL_end:
            allow_asignment_ = true;
            break;
        case LITERAL_in:
            allow_for_expression_parameter_ = false;
            break;
        }
    }

    protected void setCurrentSpecialStringDelimiter(char delimiter,
            int delimiter_count) {
        current_special_string_delimiter_.push(new StringDelimiter(delimiter,
                delimiter_count));
    }

    protected void updateCurrentSpecialStringDelimiterCount(
            int delimiter_count) {
        StringDelimiter delimiter = current_special_string_delimiter_.peek();
        delimiter.setCount(delimiter_count);
    }

    void setIsInNestedMultipleAssign(boolean v) {
        is_in_nested_multiple_assign_ = v;
    }

    void setJustFinishedParsingStringExpressionSubstituation() {
        just_finished_parsing_string_expression_substituation_ = true;
        just_finished_parsing_regex_expression_substituation_ = false;
    }

    void setJustFinishedParsingRegexExpressionSubstituation() {
        just_finished_parsing_regex_expression_substituation_ = true;
        just_finished_parsing_string_expression_substituation_ = false;
    }

    void setJustFinishedParsingHeredocExpressionSubstituation() {
        just_finished_parsing_heredoc_expression_substituation_ = true;
    }

    // Parser will call this method to help lexer understand things like "def
    // f() /regex/ end"
    void setLastTokenToBe_RPAREN_IN_METHOD_DEFINATION() {
        assert (RPAREN == last_token_.getType());
        last_token_.setType(RPAREN_IN_METHOD_DEFINATION);
    }

    void setJustFinishedParsingSymbol() {
        just_finished_parsing_symbol_ = true;
    }

    protected boolean justSeenWhitespace() {
        return seen_whitespace_;
    }

    private boolean followedByAssign() throws CharStreamException {
        int i = 0;
        while (true) {
            ++i;
            char c = LA(i);

            switch (c) {
            case ' ':
            case '\t':
                continue;
            case ',':
                return last_token_.getType() != ASSIGN && last_token_.getType() != ASSOC;//return true;
            case ')'://a, (b, c), d = ...
                return is_in_nested_multiple_assign_;
            case '=':
                return (LA(i + 1) != '=');
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '%':
                if (LA(i + 1) == '=') {
                    return true;
                } else {
                    return false;
                }
            case '&':
            case '|':
                if (LA(i + 1) == '=') {
                    return true;
                } else if ('&' == c && '&' == LA(i + 1) && '=' == LA(i + 2)) {
                    return true;
                } else if ('|' == c && '|' == LA(i + 1) && '=' == LA(i + 2)) {
                    return true;
                } else {
                    //block {|x, y|}
                    return is_in_nested_multiple_assign_;
                }
            case '<':
            case '>':
                if ('<' == c && '<' == LA(i + 1) && '=' == LA(i + 2)) {
                    return true;
                } else if ('>' == c && '>' == LA(i + 1) && '=' == LA(i + 2)) {
                    return true;
                } else {
                    return false;
                }
            default:
                return false;
            }
        }
    }

    private void updateSymbolTable(Token token) throws TokenStreamException, CharStreamException {
        if (LITERAL_module == last_token_.getType()
                || LITERAL_class == last_token_.getType()) {
            stm_.addVariable(token.getText());
            return;
        }

        if (allow_for_expression_parameter_) {
            stm_.addVariable(token.getText());
            return;
        }

        if (allow_block_parameter_) {
            stm_.addVariable(token.getText());
            return;
        }

        // If not in symbol table yet (not assigned or being a parameter), set
        // the type to FUNCTION
        if (!stm_.isDefinedInCurrentScope(token.getText())) {
            if (allow_asignment_ && followedByAssign()) {
                stm_.addVariable(token.getText());
            } else {
                token.setType(FUNCTION);
            }
        }

    }

    private boolean expectModifier() {
        switch (last_token_.getType()) {
        case Token.INVALID_TYPE:
        case RPAREN_IN_METHOD_DEFINATION:
        case SEMI:
        case LINE_BREAK:
        case COMMA:
        case LPAREN: // OPTION = (if ...
        case LPAREN_WITH_NO_LEADING_SPACE:
        case LCURLY_BLOCK: // proc { if ...
        case LCURLY_HASH:
        case LBRACK:
        case LBRACK_ARRAY_ACCESS:
        case ASSOC:
        case LITERAL_do:
        case LITERAL_else: // else \n if ...
        case LITERAL_begin:
        case LITERAL_then:
        case LITERAL_ensure:
        case COLON_WITH_NO_FOLLOWING_SPACE:
            return false;
        default:
            return !isOperator(last_token_.getType())
                    || just_finished_parsing_symbol_;
        }
    }

    private boolean convertToModifierIfNeeded(Token token) {
        if (expectModifier()) {
            switch (token.getType()) {
            case LITERAL_if:
                token.setType(IF_MODIFIER);
                break;
            case LITERAL_unless:
                token.setType(UNLESS_MODIFIER);
                break;
            case LITERAL_while:
                token.setType(WHILE_MODIFIER);
                break;
            case LITERAL_until:
                token.setType(UNTIL_MODIFIER);
                break;
            case LITERAL_rescue:
                token.setType(RESCUE_MODIFIER);
                break;
            default:
                assert (false);
            }

            return true;
        } else {
            return false;
        }
    }

    private boolean keywordOrOperatorToMethodName(Token token) {
        switch (token.getType()) {
        case LITERAL___FILE__:
        case LITERAL_and:
        case LITERAL_def:
        case LITERAL_end:
        case LITERAL_in:
        case LITERAL_or:
        case LITERAL_self:
        case LITERAL_unless:
        case LITERAL___LINE__:
        case LITERAL_begin:
        // case LITERAL_defined?:
        case LITERAL_ensure:
        case LITERAL_module:
        case LITERAL_redo:
        case LITERAL_super:
        case LITERAL_until:
        case LITERAL_BEGIN:
        case LITERAL_break:
        case LITERAL_do:
        case LITERAL_false:
        case LITERAL_next:
        case LITERAL_rescue:
        case LITERAL_then:
        case LITERAL_when:
        case LITERAL_END:
        case LITERAL_case:
        case LITERAL_else:
        case LITERAL_for:
        case LITERAL_nil:
        case LITERAL_retry:
        case LITERAL_true:
        case LITERAL_while:
        case LITERAL_alias:
        case LITERAL_class:
        case LITERAL_elsif:
        case LITERAL_if:
        case LITERAL_not:
        case LITERAL_return:
        case LITERAL_undef:
        case LITERAL_yield:
        case BOR:
        case BXOR:
        case BAND:
        case COMPARE:
        case EQUAL:
        case CASE_EQUAL:
        case MATCH:
        case GREATER_THAN:
        case GREATER_OR_EQUAL:
        case LESS_THAN:
        case LESS_OR_EQUAL:
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
        case MOD:
        case POWER:
        case BNOT:
            return doNotExpectKeywordAndOperator(token);
        default:
            return false;
        }
    }

    private boolean doNotExpectKeywordAndOperator(Token token) {
        // e.g.
        // a.do
        // undef do
        // alias do1 do
        // ::new
        // NOTE alias, undef, def has been handled alreay as well.
        switch (last_token_.getType()) {
        case COLON2:
        case DOT:
            token.setType(IDENTIFIER);
            return true;
        default:
            return false;
        }
    }

    protected void setSeenWhitespace() {
        seen_whitespace_ = true;
    }

    protected boolean expectArrayAccess() {
        switch (last_token_.getType()) {
        case IDENTIFIER:
        case CONSTANT:
        case INSTANCE_VARIABLE:
        case CLASS_VARIABLE:
        case GLOBAL_VARIABLE:
        case LITERAL_self:
        case LITERAL_nil:
        case LITERAL_false:
        case LITERAL_true:
        case RPAREN:
        case SINGLE_QUOTE_STRING:
        case DOUBLE_QUOTE_STRING:
        case STRING_AFTER_EXPRESSION_SUBSTITUTION:
        case RBRACK:
        case EMPTY_ARRAY:
        case EMPTY_ARRAY_ACCESS:
        case RCURLY:
        case W_ARRAY:
        case LITERAL_end:
            return true;
        case FUNCTION:
        case LITERAL_yield:
            return !seen_whitespace_;
        default:
            return false;
        }
    }

    protected boolean expectHeredoc() {
        // "<<" can be left shift operator or start of heredoc
        switch (last_token_.getType()) {
        case Token.INVALID_TYPE:
        case FUNCTION: // defines? <<EOF
        case LPAREN: // puts(<<EOF
        case LPAREN_WITH_NO_LEADING_SPACE:
        case ASSOC: // :text=><<EOL).pack(:side=>:top)
        case COMMA: // insert('0.0', <<EOT)
        case SEMI:
        case RPAREN_IN_METHOD_DEFINATION: // def key() :controller end
        case LINE_BREAK: // def success\n <<-EOM ... <<EOM\n end
        case LBRACK:
        case LBRACK_ARRAY_ACCESS:
        case LCURLY_BLOCK:
        case LCURLY_HASH:

        case LITERAL_do: // c.def_method("foo") do <<-EOD
        case LITERAL_if:
        case LITERAL_while:
        case LITERAL_unless:
        case LITERAL_until:
        case LITERAL_then:
        case LITERAL_when:
        case LITERAL_else:
        case LITERAL_elsif:
        case LITERAL_return:
        case LITERAL_super: // super :RubyGems
        case LITERAL_yield: // yield :TOKEN, s
        case IF_MODIFIER:
        case WHILE_MODIFIER:
        case UNLESS_MODIFIER:
        case UNTIL_MODIFIER:
            return true;
        default:
            return isOperator(last_token_.getType());
        }
    }

    protected boolean expectHash() {
        // @hash = @p['hash'] || {}
        // @SYM ||= {}
        // files << { "html_file_name" => f.path }
        // if slot != {}
        // methods.collect {|m| {'methodName' => m[0], 'params' => m[1..-1]} }
        // optobj.assign(self) + {:text=>'DDD'}
        switch (last_token_.getType()) {
        case Token.INVALID_TYPE:
        case ASSIGN:
        case ASSIGN_WITH_NO_LEADING_SPACE:
        case COMMA:
        case LINE_BREAK:
        case SEMI:
        case LCURLY_HASH: // {{}=>""}
        case ASSOC: // {section=>{}}
        case LPAREN: // waitfor({"Prompt" => match, "Timeout" => time_out})
        case LPAREN_WITH_NO_LEADING_SPACE:
        case LBRACK: // @ns_stack = [{}]
        case LBRACK_ARRAY_ACCESS:
        case LITERAL_return: // return {'*'=>'id'}
        case LITERAL_when:
        case LITERAL_in:
            return true;
        default:
            return isOperator(last_token_.getType());
        }
    }

    protected boolean spaceIsNext() throws CharStreamException {
        switch (LA(1)) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            return true;
        default:
            return false;
        }
    }

    protected boolean expressionSubstitutionIsNext()
            throws CharStreamException {
        if (LA(1) != '#') {
            return false;
        }

        switch (LA(2)) {
        case '{':
        case '$':
        case '@':
            return true;
        default:
            return false;
        }
    }

    private boolean isOperator(int type) {
        switch (type) {
        case LITERAL_and:
        case LITERAL_or:
        case LITERAL_not:
        case QUESTION:
        case COLON:
        case ASSIGN:
        case ASSIGN_WITH_NO_LEADING_SPACE:
        case PLUS_ASSIGN:
        case MINUS_ASSIGN:
        case STAR_ASSIGN:
        case DIV_ASSIGN:
        case MOD_ASSIGN:
        case POWER_ASSIGN:
        case BAND_ASSIGN:
        case BXOR_ASSIGN:
        case BOR_ASSIGN:
        case LEFT_SHIFT_ASSIGN:
        case RIGHT_SHIFT_ASSIGN:
        case LOGICAL_AND_ASSIGN:
        case LOGICAL_OR_ASSIGN:
        case INCLUSIVE_RANGE:
        case EXCLUSIVE_RANGE:
        case LOGICAL_OR:
        case LOGICAL_AND:
        case COMPARE:
        case EQUAL:
        case CASE_EQUAL:
        case NOT_EQUAL:
        case MATCH:
        case NOT_MATCH:
        case LESS_THAN:
        case GREATER_THAN:
        case LESS_OR_EQUAL:
        case GREATER_OR_EQUAL:
        case BXOR:
        case BOR:
        case BAND:
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
        case MOD:
        case POWER:
        case UNARY_PLUS:
        case UNARY_MINUS:
        case BNOT:
        case NOT:
            return true;
        default:
            return false;
        }
    }

    protected boolean expectLeadingColon2() {
        switch (last_token_.getType()) {
        case LINE_BREAK:
        case SEMI:
        case COMMA:
        case LPAREN:
        case LPAREN_WITH_NO_LEADING_SPACE:
        case LBRACK:
        case LBRACK_ARRAY_ACCESS:
        case LCURLY_BLOCK:
        case LCURLY_HASH:
            return true;
        default:
            return seen_whitespace_ || isOperator(last_token_.getType());
        }
    }

    protected boolean expectOperator(int k) throws CharStreamException {
        // We consider '/' is divide operator in the following contexts:
        // varible/2 varible<<2
        // array[1]/2 array[1]<<2
        // 1/2 1<<2
        // 1.2/3.4 1.2<<3.4
        // function()/2 function()<<2
        // self / Rational.new!(a, 1)
        // f.one/f.ten
        switch (last_token_.getType()) {
        case FUNCTION:
            if (seen_whitespace_ && ((' ' != LA(k)) && ('\t' != LA(k)))) {
                return false;
            }
        case CONSTANT:
        case IDENTIFIER:
        case SINGLE_QUOTE_STRING:
        case DOUBLE_QUOTE_STRING:
        case STRING_AFTER_EXPRESSION_SUBSTITUTION:
        case REGEX:
        case INSTANCE_VARIABLE:
        case CLASS_VARIABLE:
        case INTEGER:
        case FLOAT:
        case HEX:
        case OCTAL:
        case BINARY:
        case ASCII_VALUE:
        case RBRACK:
        case EMPTY_ARRAY:
        case EMPTY_ARRAY_ACCESS:
        case RPAREN:
        case RCURLY:
        case DOT:
        case GLOBAL_VARIABLE:
        case LITERAL_def:
        case LITERAL_alias:// TODO this only work for the first parameter
        case LITERAL_undef:// TODO this only work for the first parameter
        case LITERAL_self:
        case LITERAL_end:
        case COLON_WITH_NO_FOLLOWING_SPACE:
            return true;
        default:
            return false;
        }
    }

    protected boolean expectUnary() throws CharStreamException {
        // unary plus/minus can only appear as in the following examples:
        // a = -1;
        // 2 + (-1)
        // -1
        // ;+2
        // ,-1
        // 2+ -7
        // 1*-2
        // 4/+2
        // 2..-1
        // 2...-1
        // :from=>-0.1
        // args[-1]
        // {-1}
        // a > -1
        // a ? -1 : -2
        switch (last_token_.getType()) {
        case LINE_BREAK:
        case SEMI:
        case COMMA:
        case Token.INVALID_TYPE:
        case ASSOC:
        case LBRACK:
        case LBRACK_ARRAY_ACCESS:
        case LCURLY_BLOCK:
        case LCURLY_HASH:
        case LPAREN:
        case LPAREN_WITH_NO_LEADING_SPACE:

        // TODO add more keywords
        case LITERAL_when: // when -1, Range, Array
        case LITERAL_then:
        case LITERAL_if:
        case LITERAL_else:
        case LITERAL_unless:
        case LITERAL_until:
        case LITERAL_while:
        case LITERAL_return:
        case LITERAL_rescue:

        case IF_MODIFIER:
        case UNLESS_MODIFIER:
        case WHILE_MODIFIER:
        case UNTIL_MODIFIER:
        case RESCUE_MODIFIER:
            return true;
        case FUNCTION:
            if (seen_whitespace_ && ((' ' != LA(1)) && ('\t' != LA(1)))) {
                return true;
            } else {
                return false;
            }
        default:
            return isOperator(last_token_.getType());
        }
    }

    protected boolean lastTokenIsDotOrColon2() {
        return ((DOT == last_token_.getType())
                || (COLON2 == last_token_.getType()) || (LEADING_COLON2 == last_token_
                .getType()));
    }

    protected boolean lastTokenIsSemi() {
        return (DOT == last_token_.getType());
    }

    protected boolean lastTokenIsKeywordDefOrColonWithNoFollowingSpace() {
        return (LITERAL_def == last_token_.getType() || COLON_WITH_NO_FOLLOWING_SPACE == last_token_
                .getType());
    }

    protected boolean lastTokenIsColonWithNoFollowingSpace() {
        return (COLON_WITH_NO_FOLLOWING_SPACE == last_token_.getType());
    }

    /*
     * lexer should keep linebreaks after operator. Even though most linebreak
     * after operators can be ignored, but operators in ruby can be methodname
     * sometime for example: "alias equal? =="
     */
    protected boolean shouldIgnoreLinebreak() {
        switch (last_token_.getType()) {
        case COMMA:
        case DOT:
        case LBRACK:
        case LBRACK_ARRAY_ACCESS:
        case LCURLY_BLOCK:
        case LCURLY_HASH:
        case LPAREN:
        case LPAREN_WITH_NO_LEADING_SPACE:
        case SEMI:
        case LINE_BREAK: // \n #comment \n
        case IF_MODIFIER:
        case UNLESS_MODIFIER:
        case WHILE_MODIFIER:
        case UNTIL_MODIFIER:
        case RESCUE_MODIFIER:
        case ASSOC:
            return true;
        default:
            return false;
        }
    }

    protected boolean isDelimiter(String next_line, String delimiter) {
        boolean r;
        if ('-' == delimiter.charAt(0)) {
            // ignore whitespace
            String trimmed_next_line = "-" + next_line.trim();
            r = delimiter.equals(trimmed_next_line);
        } else {
            r = delimiter.equals(next_line);
        }

        return r;
    }

    protected boolean isAsciiValueTerminator(char value) {
        switch (value) {
        case '\t':
        case ' ':
        case '\f':
        case '\13':
        case '\r':
        case '\n':
        case EOF_CHAR:
        case ')': // while(ARGV[0] && ARGV[0][0] == ?-)
        case ',': // [?d, ?s, :date]
        case ']': // [?a]
        case ';': // $state[n*2] = ?|;
        case '.': // unless (?A..?Z) === klass[0]
        case ':': // when ?':
            return true;
        default:
            return false;
        }
    }

    protected int trackDelimiterCount(char next_char, char delimeter, int delimeter_count) {
        if (delimeter == translateDelimiter(delimeter)) {
            if (delimeter == next_char) {
                --delimeter_count;
                assert (delimeter_count >= 0);
            }
        } else {
            if (delimeter == next_char) {
                ++delimeter_count;
            } else if (next_char == translateDelimiter(delimeter)) {
                --delimeter_count;
                assert (delimeter_count >= 0);
            }
        }

        return delimeter_count;
    }

    private char translateDelimiter(char in) {
        switch (in) {
        case '{':
            return '}';
        case '[':
            return ']';
        case '(':
            return ')';
        case '<':
            return '>';
        default:
            return in;
        }
    }
}
