if defined? RHO_ME
else
	$:.unshift(File.join(File.dirname(__FILE__), '..'))
	require 'sqlite3/database'
end
require 'rhom/rhom'