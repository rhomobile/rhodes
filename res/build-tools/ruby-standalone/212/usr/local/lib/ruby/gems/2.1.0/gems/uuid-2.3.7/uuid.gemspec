Gem::Specification.new do |s|
  s.name = 'uuid'
  s.version = '2.3.7'
  s.summary = "UUID generator"
  s.description = <<-EOF
UUID generator for producing universally unique identifiers based on RFC 4122
(http://www.ietf.org/rfc/rfc4122.txt).
EOF

  s.authors << 'Assaf Arkin' << 'Eric Hodel'
  s.email = 'assaf@labnotes.org'
  s.homepage = 'http://github.com/assaf/uuid'

  s.files = Dir['{bin,test,lib,docs}/**/*'] + ['README.rdoc', 'MIT-LICENSE', 'Rakefile', 'CHANGELOG', 'uuid.gemspec']
  s.executables = "uuid"
  
  s.rdoc_options << '--main' << 'README.rdoc' << '--title' <<  'UUID generator' << '--line-numbers'
                       '--webcvs' << 'http://github.com/assaf/uuid'
  s.extra_rdoc_files = ['README.rdoc', 'MIT-LICENSE']

  s.add_dependency 'macaddr', ['~>1.0']
end
