using System;
using System.Collections;
using System.Collections.Generic;
//using System.Data;
using System.Globalization;
using System.Reflection;
//using System.Reflection.Emit;
using System.Text;
using rho;

namespace fastJSON
{
	/// <summary>
	/// This class encodes and decodes JSON strings.
	/// Spec. details, see http://www.json.org/
	/// 
	/// JSON uses Arrays and Objects. These correspond here to the datatypes ArrayList and Hashtable.
	/// All numbers are parsed to doubles.
	/// </summary>
	internal class RJSONTokener
	{
        static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

		private const int TOKEN_NONE = 0;
		private const int TOKEN_CURLY_OPEN = 1;
		private const int TOKEN_CURLY_CLOSE = 2;
		private const int TOKEN_SQUARED_OPEN = 3;
		private const int TOKEN_SQUARED_CLOSE = 4;
		private const int TOKEN_COLON = 5;
		private const int TOKEN_COMMA = 6;
		private const int TOKEN_STRING = 7;
		private const int TOKEN_NUMBER = 8;
		private const int TOKEN_TRUE = 9;
		private const int TOKEN_FALSE = 10;
		private const int TOKEN_NULL = 11;
		
		/// <summary>
		/// Parses the string json into a value
		/// </summary>
		/// <param name="json">A JSON string.</param>
		/// <returns>An ArrayList, a dictionary, a double, a string, null, true, or false</returns>
		internal static object JsonDecode(string json)
		{
			bool success = true;
			
			return JsonDecode(json, ref success);
		}

		/// <summary>
		/// Parses the string json into a value; and fills 'success' with the successfullness of the parse.
		/// </summary>
		/// <param name="json">A JSON string.</param>
		/// <param name="success">Successful parse?</param>
		/// <returns>An ArrayList, a Hashtable, a double, a string, null, true, or false</returns>
		private static object JsonDecode(string json, ref bool success)
		{
			success = true;
			if (json != null) {
				char[] charArray = json.ToCharArray();
				int index = 0;
				object value = ParseValue(charArray, ref index, ref success);
				return value;
			} else {
				return null;
			}
		}


        protected static IronRuby.Builtins.Hash ParseObject(char[] json, ref int index, ref bool success)
		{
            IronRuby.Builtins.Hash table = RhoRuby.createHash();
			int token;

			// {
			NextToken(json, ref index);

			bool done = false;
			while (!done) {
				token = LookAhead(json, index);
				if (token == TOKEN_NONE) {
					success = false;
					return null;
				} else if (token == TOKEN_COMMA) {
					NextToken(json, ref index);
				} else if (token == TOKEN_CURLY_CLOSE) {
					NextToken(json, ref index);
					return table;
				} else {

					// name
					object name = ParseString(json, ref index, ref success);
					if (!success) {
						success = false;
						return null;
					}

					// :
					token = NextToken(json, ref index);
					if (token != TOKEN_COLON) {
						success = false;
						return null;
					}

					// value
					object value = ParseValue(json, ref index, ref success);
					if (!success) {
						success = false;
						return null;
					}

					table[name] = value;
				}
			}

			return table;
		}

        protected static IronRuby.Builtins.RubyArray ParseArray(char[] json, ref int index, ref bool success)
		{
            IronRuby.Builtins.RubyArray array = RhoRuby.createArray();

			NextToken(json, ref index);

			bool done = false;
			while (!done) {
				int token = LookAhead(json, index);
				if (token == TOKEN_NONE) {
					success = false;
					return null;
				} else if (token == TOKEN_COMMA) {
					NextToken(json, ref index);
				} else if (token == TOKEN_SQUARED_CLOSE) {
					NextToken(json, ref index);
					break;
				} else {
					object value = ParseValue(json, ref index, ref success);
					if (!success) {
						return null;
					}

					array.Add(value);
				}
			}

			return array;
		}

		protected static object ParseValue(char[] json, ref int index, ref bool success)
		{
			switch (LookAhead(json, index)) {
				case TOKEN_NUMBER:
					return ParseNumber(json, ref index, ref success);
				case TOKEN_STRING:
					return ParseString(json, ref index, ref success);
				case TOKEN_CURLY_OPEN:
					return ParseObject(json, ref index, ref success);
				case TOKEN_SQUARED_OPEN:
					return ParseArray(json, ref index, ref success);
				case TOKEN_TRUE:
					NextToken(json, ref index);
					return true;
				case TOKEN_FALSE:
					NextToken(json, ref index);
					return false;
				case TOKEN_NULL:
					NextToken(json, ref index);
					return null;
				case TOKEN_NONE:
					break;
			}

			success = false;
			return null;
		}

        protected static object ParseString(char[] json, ref int index, ref bool success)
		{
			StringBuilder s = new StringBuilder();
			char c;

			EatWhitespace(json, ref index);
			
			// "
			c = json[index++];

			bool complete = false;
			while (!complete) {

				if (index == json.Length) {
					break;
				}

				c = json[index++];
				if (c == '"') {
					complete = true;
					break;
				} else if (c == '\\') {

					if (index == json.Length) {
						break;
					}
					c = json[index++];
					if (c == '"') {
						s.Append('"');
					} else if (c == '\\') {
						s.Append('\\');
					} else if (c == '/') {
						s.Append('/');
					} else if (c == 'b') {
						s.Append('\b');
					} else if (c == 'f') {
						s.Append('\f');
					} else if (c == 'n') {
						s.Append('\n');
					} else if (c == 'r') {
						s.Append('\r');
					} else if (c == 't') {
						s.Append('\t');
					} else if (c == 'u') {
						int remainingLength = json.Length - index;
						if (remainingLength >= 4) {
							// parse the 32 bit hex into an integer codepoint
							uint codePoint;
							if (!(success = UInt32.TryParse(new string(json, index, 4), NumberStyles.HexNumber, CultureInfo.InvariantCulture, out codePoint))) {
								return "";
							}
							// convert the integer codepoint to a unicode char and add to string
							//s.Append(Char.ConvertFromUtf32((int)codePoint));
                            s.Append(codePoint);
							// skip 4 chars
							index += 4;
						} else {
							break;
						}
					}

				} else {
					s.Append(c);
				}

			}

			if (!complete) {
				success = false;
				return null;
			}

			return RhoRuby.createString(s.ToString());
		}

		protected static object ParseNumber(char[] json, ref int index, ref bool success)
		{
			EatWhitespace(json, ref index);

			int lastIndex = GetLastIndexOfNumber(json, index);
			int charLength = (lastIndex - index) + 1;

			string number = new string(json,index,charLength);
			success = true;
			
			index = lastIndex + 1;
			return RhoRuby.createString(number);
		}

		protected static int GetLastIndexOfNumber(char[] json, int index)
		{
			int lastIndex;

			for (lastIndex = index; lastIndex < json.Length; lastIndex++) {
				if ("0123456789+-.eE".IndexOf(json[lastIndex]) == -1) {
					break;
				}
			}
			return lastIndex - 1;
		}

		protected static void EatWhitespace(char[] json, ref int index)
		{
			for (; index < json.Length; index++) {
				if (" \t\n\r".IndexOf(json[index]) == -1) {
					break;
				}
			}
		}

		protected static int LookAhead(char[] json, int index)
		{
			int saveIndex = index;
			return NextToken(json, ref saveIndex);
		}

		protected static int NextToken(char[] json, ref int index)
		{
			EatWhitespace(json, ref index);

			if (index == json.Length) {
				return TOKEN_NONE;
			}
			
			char c = json[index];
			index++;
			switch (c) {
				case '{':
					return TOKEN_CURLY_OPEN;
				case '}':
					return TOKEN_CURLY_CLOSE;
				case '[':
					return TOKEN_SQUARED_OPEN;
				case ']':
					return TOKEN_SQUARED_CLOSE;
				case ',':
					return TOKEN_COMMA;
				case '"':
					return TOKEN_STRING;
				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':
				case '-':
					return TOKEN_NUMBER;
				case ':':
					return TOKEN_COLON;
			}
			index--;

			int remainingLength = json.Length - index;

			// false
			if (remainingLength >= 5) {
				if (json[index] == 'f' &&
				    json[index + 1] == 'a' &&
				    json[index + 2] == 'l' &&
				    json[index + 3] == 's' &&
				    json[index + 4] == 'e') {
					index += 5;
					return TOKEN_FALSE;
				}
			}

			// true
			if (remainingLength >= 4) {
				if (json[index] == 't' &&
				    json[index + 1] == 'r' &&
				    json[index + 2] == 'u' &&
				    json[index + 3] == 'e') {
					index += 4;
					return TOKEN_TRUE;
				}
			}

			// null
			if (remainingLength >= 4) {
				if (json[index] == 'n' &&
				    json[index + 1] == 'u' &&
				    json[index + 2] == 'l' &&
				    json[index + 3] == 'l') {
					index += 4;
					return TOKEN_NULL;
				}
			}

			return TOKEN_NONE;
		}

		public static object SerializeString(string aString)
		{
            StringBuilder builder = new StringBuilder();
			builder.Append("\"");

			char[] charArray = aString.ToCharArray();
			for (int i = 0; i < charArray.Length; i++) {
				char c = charArray[i];
				if (c == '"') {
					builder.Append("\\\"");
				} else if (c == '\\') {
					builder.Append("\\\\");
				} else if (c == '\b') {
					builder.Append("\\b");
				} else if (c == '\f') {
					builder.Append("\\f");
				} else if (c == '\n') {
					builder.Append("\\n");
				} else if (c == '\r') {
					builder.Append("\\r");
				} else if (c == '\t') {
					builder.Append("\\t");
				} else {
					int codepoint = Convert.ToInt32(c);
					if ((codepoint >= 32) && (codepoint <= 126)) {
						builder.Append(c);
					} else {
						builder.Append("\\u" + Convert.ToString(codepoint, 16).PadLeft(4, '0'));
					}
				}
			}

			builder.Append("\"");
			return RhoRuby.createString(builder.ToString());
		}

		protected static bool IsNumeric(object o)
		{
			double result;

			return (o == null) ? false : Double.TryParse(o.ToString(), out result);
		}
	}
}
