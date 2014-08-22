# Netrc

This library reads and writes
[`.netrc` files](http://www.gnu.org/software/inetutils/manual/html_node/The-_002enetrc-File.html).

## API

Read a netrc file:

    n = Netrc.read("sample.netrc")

If the file doesn't exist, Netrc.read will return an empty object. If
the filename ends in ".gpg", it will be decrypted using
[GPG](http://www.gnupg.org/).

Read the user's default netrc file (`$HOME/.netrc` on Unix;
`%HOME%\_netrc` on Windows):

    n = Netrc.read

Look up a username and password:

    user, pass = n["example.com"]

Write a username and password:

    n["example.com"] = user, newpass
    n.save

If you make an entry that wasn't there before, it will be appended
to the end of the file. Sometimes people want to include a comment
explaining that the entry was added automatically. You can do it
like this:

    n.new_item_prefix = "# This entry was added automatically\n"
    n["example.com"] = user, newpass
    n.save

Have fun!

## Running Tests

    $ bundle install
    $ bundle exec turn test
