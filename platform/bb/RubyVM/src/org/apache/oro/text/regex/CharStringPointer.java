/* 
 * $Id: CharStringPointer.java 124053 2005-01-04 01:24:35Z dfs $
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
 * The CharStringPointer class is used to facilitate traversal of a char[]
 * in the manner pointer traversals of strings are performed in C/C++.
 * It is expected that the compiler will inline all the functions.
 *
 * @since 1.0
 * @version @version@
 */
final class CharStringPointer {
  static final char _END_OF_STRING = Character.MAX_VALUE;
  int _offset;
  char[] _array;

  CharStringPointer(char[] charArray, int offset) {
    _array  = charArray;
    _offset = offset;
  }

  CharStringPointer(char[] charArray) {
    this(charArray, 0);
  }

  char _getValue()  {
    return _getValue(_offset);
  }

  char _getValue(int offset) {
    if(offset < _array.length && offset >= 0)
      return _array[offset];
    return _END_OF_STRING;
  }

  char _getValueRelative(int offset) {
    return _getValue(_offset + offset);
  }

  int _getLength() { return _array.length; }

  int _getOffset() { return _offset; }

  void _setOffset(int offset) { _offset = offset; }

  boolean _isAtEnd() {
    return (_offset >= _array.length);
  }

  char _increment(int inc) {
    _offset+=inc;
    if(_isAtEnd()) {
      _offset = _array.length;
      return _END_OF_STRING;
    }

    return _array[_offset];
  }

  char _increment() { return _increment(1); }

  char _decrement(int inc) {
    _offset-=inc; 
    if(_offset < 0)
      _offset = 0;

    return _array[_offset];
  }

  char _decrement() { return _decrement(1); }

  char _postIncrement() {
    char ret;
    ret = _getValue();
    _increment();
    return ret;
  }

  char _postDecrement() {
    char ret;
    ret = _getValue();
    _decrement();
    return ret;
  }


  String _toString(int offset) {
    return new String(_array, offset, _array.length - offset);
  }

  public String toString() {
    return _toString(0);
  }
}
