/* 
 * $Id: Perl5MatchResult.java 124053 2005-01-04 01:24:35Z dfs $
 *
 * Copyright 2000-2005 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


package org.apache.oro.text.regex;


/**
 * A class used to store and access the results of a Perl5Pattern match.
 *
 * @version @version@
 * @since 1.0
 * @see PatternMatcher
 * @see Perl5Matcher
 */
public class Perl5MatchResult implements MatchResult {
  /**
   * The character offset into the line or stream where the match
   * begins.  Pattern matching methods that look for matches a line at
   * a time should use this field as the offset into the line
   * of the match.  Methods that look for matches independent of line
   * boundaries should use this field as the offset into the entire
   * text stream.
   */
  protected int _matchBeginOffset_;


  /**
   * Arrays containing the beginning and end offsets of the pattern
   * groups matched within the actual matched pattern contained in the 
   * variable <code>match</code>.
   * Pattern matching methods that do not match subgroups, will only contain
   * entries for group 0, which always refers to the entire pattern.
   * <code>beginGroupOffset</code> contains the start offset of the groups,
   * indexed by group number, which will always be 0 for group 0.
   * <code>endGroupOffset</code> contains the ending offset + 1 of the groups.
   * A group matching the null string will have  <code>beginGroupOffset</code>
   * and <code>endGroupOffset</code> entries of equal value.  Following a
   * convention established by the GNU regular expression library for the
   * C language, groups that are not part of a match contain -1 as their
   * begin and end offsets.
   */
  protected int[] _beginGroupOffset_, _endGroupOffset_;


  /**
   * The entire string that matched the pattern.
   */
  protected String _match_;


  /**
   * Constructs a MatchResult able to store match information for
   * a number of subpattern groups.
   * <p>
   * @param groups  The number of groups this MatchResult can store.
   *        Only postitive values greater than or equal to 1 make any
   *        sense.  At minimum, a MatchResult stores one group which
   *        represents the entire pattern matched including all subparts.
   */
  protected Perl5MatchResult(int groups){
    _beginGroupOffset_ = new int[groups];
    _endGroupOffset_   = new int[groups];
  }


  /**
   * @return The length of the match.
   */
  public int length(){
    int length;

    length = (_endGroupOffset_[0] - _beginGroupOffset_[0]);

    return (length > 0 ? length : 0);
  }


  /**
   * @return The number of groups contained in the result.  This number
   *         includes the 0th group.  In other words, the result refers
   *         to the number of parenthesized subgroups plus the entire match
   *         itself.
   */
  public int groups(){
    return _beginGroupOffset_.length;
  }

  /**
   * @param group The pattern subgroup to return.
   * @return A string containing the indicated pattern subgroup.  Group
   *         0 always refers to the entire match.  If a group was never
   *         matched, it returns null.  This is not to be confused with
   *         a group matching the null string, which will return a String
   *         of length 0.
   */
  public String group(int group){
    int begin, end, length;

    if(group < _beginGroupOffset_.length){
      begin  = _beginGroupOffset_[group];
      end    = _endGroupOffset_[group];
      length = _match_.length();

      if(begin >= 0 && end >= 0) {
	if(begin < length && end <= length && end > begin)
	  return _match_.substring(begin, end);
	else if(begin <= end)
	  return "";
      }
    }

    return null;
  }

  /**
   * @param group The pattern subgroup.
   * @return The offset into group 0 of the first token in the indicated
   *         pattern subgroup.  If a group was never matched or does
   *         not exist, returns -1.
   */
  public int begin(int group){
    int begin, end;//, length;
    if(group < _beginGroupOffset_.length){
      begin  = _beginGroupOffset_[group];
      end    = _endGroupOffset_[group];
      //length = _match_.length();
      if(begin >= 0 && end >= 0)// && begin < length && end <= length)
	//return _beginGroupOffset_[group];
	return begin;
    }

    return -1;
  }

  /**
   * @param group The pattern subgroup.
   * @return Returns one plus the offset into group 0 of the last token in
   *         the indicated pattern subgroup.  If a group was never matched
   *         or does not exist, returns -1.  A group matching the null
   *         string will return its start offset.
   */
  public int end(int group){
    int begin, end; //, length;
    if(group < _beginGroupOffset_.length){
      begin  = _beginGroupOffset_[group];
      end    = _endGroupOffset_[group];
      //length = _match_.length();
      if(begin >= 0 && end >= 0)// && begin < length && end <= length)
	//return _endGroupOffset_[group];
	return end;
    }
    return -1;
  }

  /**
   * Returns an offset marking the beginning of the pattern match
   * relative to the beginning of the input.
   * <p>
   * @param group The pattern subgroup.
   * @return The offset of the first token in the indicated
   *         pattern subgroup.  If a group was never matched or does
   *         not exist, returns -1.
   */
  public int beginOffset(int group){
    int begin, end;//, length;
    if(group < _beginGroupOffset_.length){
      begin  = _beginGroupOffset_[group];
      end    = _endGroupOffset_[group];
      //length = _match_.length();
      if(begin >= 0 && end >= 0)// && begin < length && end <= length)
	//return _matchBeginOffset_ + _beginGroupOffset_[group];
	return _matchBeginOffset_ + begin;
    }
    return -1;
  }

  /**
   * Returns an offset marking the end of the pattern match 
   * relative to the beginning of the input.
   * <p>
   * @param group The pattern subgroup.
   * @return Returns one plus the offset of the last token in
   *         the indicated pattern subgroup.  If a group was never matched
   *         or does not exist, returns -1.  A group matching the null
   *         string will return its start offset.
   */
  public int endOffset(int group){
    int begin, end;//, length;
    if(group < _endGroupOffset_.length){
      begin  = _beginGroupOffset_[group];
      end    = _endGroupOffset_[group];
      //length = _match_.length();
      if(begin >= 0 && end >= 0)// && begin < length && end <= length)
	//return _matchBeginOffset_ + _endGroupOffset_[group];
	return _matchBeginOffset_ + end;
    }
    return -1;
  }


  /**
   * The same as group(0).
   *
   * @return A string containing the entire match.
   */
  public String toString() {
    return group(0);
  }
}
